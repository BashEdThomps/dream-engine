/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptComponent.h"

#include "Components/Input/InputComponent.h"
#include "Storage/StorageManager.h"
#include "Storage/File.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol.h>

#include <memory>

namespace octronic::dream
{
  ScriptRuntime::ScriptRuntime
  (ProjectRuntime& rt,
   ScriptDefinition& definition)
    : SharedAssetRuntime(rt, definition),
      mSource("")
  {
    LOG_TRACE( "ScriptRuntime: {} {}",__FUNCTION__, getNameAndUuidString());
    return;
  }

  bool
  ScriptRuntime::loadFromDefinition
  ()
  {
    auto& projectDir = getProjectRuntime().getProjectDirectory();
    auto& def = static_cast<ScriptDefinition&>(getDefinition());
    auto path = projectDir.getAssetAbsolutePath(def);

    LOG_DEBUG( "ScriptRuntime: Script at {}" , path);

    auto& sm = getProjectRuntime().getStorageManager();
    auto& scriptFile = sm.openFile(path);

    if (!scriptFile.exists())
    {
      LOG_ERROR("ScriptCache: Script file does not exist");
      setSource("");
      sm.closeFile(scriptFile);
      mLoadError = true;
      return false;
    }
    else
    {
      setSource(scriptFile.readString());
      mLoaded = true;
      sm.closeFile(scriptFile);
      return true;
    }
  }

  bool
  ScriptRuntime::createEntityState
  (EntityRuntime& entity)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.createEntityState(*this, entity);
  }

  bool
  ScriptRuntime::removeEntityState
  (UuidType uuid)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.removeEntityState(uuid);
  }

  string
  ScriptRuntime::getSource
  ()
  const
  {
    return mSource;
  }

  void
  ScriptRuntime::setSource
  (const string& source)
  {
    mSource = source;
  }

  // Function Execution =======================================================

  bool
  ScriptRuntime::executeOnUpdate
  (EntityRuntime& entity)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.executeScriptOnUpdate(*this, entity);
  }

  bool
  ScriptRuntime::executeOnInit
  (EntityRuntime& entity)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.executeScriptOnInit(*this, entity);
  }

  bool
  ScriptRuntime::executeOnEvent
  (EntityRuntime& entity)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.executeScriptOnEvent(*this, entity);
  }

  bool
  ScriptRuntime::executeOnInput
  (SceneRuntime& sr)
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.executeScriptOnInput(*this, sr);
  }


  bool
  ScriptRuntime::registerInputScript
  ()
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.registerInputScript(*this);
  }

  bool
  ScriptRuntime::removeInputScript
  ()
  {
    auto& scriptComponent = getProjectRuntime().getScriptComponent();
    return scriptComponent.removeInputScript(getUuid());
  }

  bool
  ScriptRuntime::hasSource
  ()
  const
  {
    return !mSource.empty();
  }

  void
  ScriptRuntime::pushTasks
  ()
  {
    auto activeSceneOpt = getProjectRuntime().getActiveSceneRuntime();

    if (activeSceneOpt)
    {
      auto& activeScene = activeSceneOpt.value().get();
      auto& inputComp = getProjectRuntime().getInputComponent();
      auto& taskQueue = getProjectRuntime().getTaskQueue();
      auto& dTaskQueue = getProjectRuntime().getDestructionTaskQueue();

      if (mReloadFlag)
      {
        // Unload for Reaload
        mLoaded = false;
        mLoadError = false;
        mSource = "";
        mReloadFlag = false;
        mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);

        if (activeScene.getInputScript() == *this)
        {
          dTaskQueue.pushTask(inputComp.getRemoveScriptTask());
          inputComp.getRegisterScriptTask()->setState(TASK_STATE_QUEUED);
        }
        else
        {
          for(auto& entityWrap : mInstances)
          {
            auto& entity = entityWrap.get();
            removeEntityState(entity.getUuid());

            auto& sr = entity.getSceneRuntime();
            if (sr.hasState(SCENE_STATE_ACTIVE))
            {
              entity.setScriptInitialised(false);
            }
          }
        }
      }
      else if (!mLoaded && !mLoadError)
      {
        if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
          taskQueue.pushTask(mLoadFromDefinitionTask);
        }
      }
      else
      {
        if (mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED))
        {
          for(auto entityWrap : mInstances)
          {
            auto& entity = entityWrap.get();
            // Do entity specific tasks
            auto& sr = entity.getSceneRuntime();
            if (sr.hasState(SCENE_STATE_ACTIVE))
            {
              // Not yet Initialised
              if (!entity.getScriptInitialised())
              {
                taskQueue.pushTask(entity.getScriptCreateStateTask());
              }
              // Has been initialised
              else if (entity.allRuntimesLoaded())
              {
                auto onInitTask = entity.getScriptOnInitTask();

                if (onInitTask->hasState(TASK_STATE_QUEUED))
                {
                  taskQueue.pushTask(entity.getScriptOnInitTask());
                }
                else
                {
                  // Always push update task
                  taskQueue.pushTask(entity.getScriptOnUpdateTask());

                  // If there are events to process, push on event task
                  if (entity.hasEvents())
                  {
                    taskQueue.pushTask(entity.getScriptOnEventTask());
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
