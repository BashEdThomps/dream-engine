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

using std::static_pointer_cast;

namespace octronic::dream
{
  ScriptRuntime::ScriptRuntime
  (const weak_ptr<ProjectRuntime>& rt,
   const weak_ptr<ScriptDefinition>& definition)
    : SharedAssetRuntime(rt, definition),
      mSource("")
  {
    LOG_TRACE( "ScriptRuntime: {} {}",__FUNCTION__, getNameAndUuidString());
    return;
  }

  ScriptRuntime::~ScriptRuntime
  ()
  {
    if (auto defLock = mDefinition.lock())
    {
      LOG_TRACE("ScriptRuntime: {} {}",__FUNCTION__, defLock->getNameAndUuidString());
    }
  }

  bool
  ScriptRuntime::loadFromDefinition
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      auto path = getAssetFilePath();
      LOG_DEBUG( "ScriptRuntime: Script at {}" , path);

      if (auto sm = prLock->getStorageManager().lock())
      {
        if (auto scriptFile = sm->openFile(path).lock())
        {

          if (!scriptFile->exists())
          {
            LOG_ERROR("ScriptCache: Script file does not exist");
            setSource("");
            sm->closeFile(scriptFile);
            mLoadError = true;
            return false;
          }
          else
          {
            setSource(scriptFile->readString());
            mLoaded = true;
          }

          sm->closeFile(scriptFile);
          return true;
        }
      }
    }
    return false;
  }

  bool
  ScriptRuntime::createEntityState
  (const weak_ptr<EntityRuntime>& entity)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {
        return scriptComponent->createEntityState(static_pointer_cast<ScriptRuntime>(shared_from_this()), entity);
      }
    }
    return false;
  }

  bool
  ScriptRuntime::removeEntityState
  (UuidType uuid)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {
        return scriptComponent->removeEntityState(uuid);
      }
    }
    return false;
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
  (const weak_ptr<EntityRuntime>& entity)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {
        return scriptComponent->executeScriptOnUpdate(static_pointer_cast<ScriptRuntime>(shared_from_this()), entity);
      }
    }
    return false;
  }

  bool
  ScriptRuntime::executeOnInit
  (const weak_ptr<EntityRuntime>& entity)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {

        return scriptComponent->executeScriptOnInit(static_pointer_cast<ScriptRuntime>(shared_from_this()), entity);
      }
    }
    return false;
  }

  bool
  ScriptRuntime::executeOnEvent
  (const weak_ptr<EntityRuntime>& entity)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {

        return scriptComponent->executeScriptOnEvent(static_pointer_cast<ScriptRuntime>(shared_from_this()), entity);
      }
    }
    return false;

  }

  bool
  ScriptRuntime::executeOnInput
  (const weak_ptr<SceneRuntime>& sr)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {
        return scriptComponent->executeScriptOnInput(static_pointer_cast<ScriptRuntime>(shared_from_this()), sr);
      }
    }
    return false;
  }


  bool
  ScriptRuntime::registerInputScript
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {

        return scriptComponent->registerInputScript(static_pointer_cast<ScriptRuntime>(shared_from_this()));
      }
    }
    return false;
  }

  bool
  ScriptRuntime::removeInputScript
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scriptComponent = prLock->getScriptComponent().lock())
      {

        return scriptComponent->removeInputScript(getUuid());
      }
    }
    return false;
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
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto activeScene = prLock->getActiveSceneRuntime().lock())
      {
        if (auto inputComp = prLock->getInputComponent().lock())
        {
          if (auto taskQueue = prLock->getTaskQueue().lock())
          {
            if (auto dTaskQueue = prLock->getDestructionTaskQueue().lock())
            {
              if (mReloadFlag)
              {
                // Unload for Reaload
                mLoaded = false;
                mLoadError = false;
                mSource = "";
                mReloadFlag = false;
                mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);

                if (activeScene && activeScene->getInputScript().lock() == shared_from_this())
                {
                  dTaskQueue->pushTask(inputComp->getRemoveScriptTask());
                  inputComp->getRegisterScriptTask()->setState(TASK_STATE_QUEUED);
                }
                else
                {
                  for(auto entityWeak : mInstances)
                  {
                    if (auto entity = entityWeak.lock())
                    {
                      removeEntityState(entity->getUuid());

                      if (auto srLock = entity->getSceneRuntime().lock())
                      {
                        if (srLock->hasState(SCENE_STATE_ACTIVE))
                        {
                          entity->setScriptInitialised(false);
                        }
                      }
                    }
                  }
                }
              }
              else if (!mLoaded && !mLoadError)
              {
                if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
                {
                  taskQueue->pushTask(mLoadFromDefinitionTask);
                }
              }
              else
              {
                if (mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED))
                {
                  for(auto entityWeak : mInstances)
                  {
                    if (auto entity = entityWeak.lock())
                    {
                      // Do entity specific tasks
                      if (auto srLock = entity->getSceneRuntime().lock())
                      {
                        if (srLock->hasState(SCENE_STATE_ACTIVE))
                        {
                          // Not yet Initialised
                          if (!entity->getScriptInitialised())
                          {
                            taskQueue->pushTask(entity->getScriptCreateStateTask());
                          }
                          // Has been initialised
                          else if (entity->allRuntimesLoaded())
                          {
                            if (auto onInitTask = entity->getScriptOnInitTask().lock())
                            {
                              if (onInitTask->hasState(TASK_STATE_QUEUED))
                              {
                                taskQueue->pushTask(entity->getScriptOnInitTask());
                              }
                              else
                              {
                                // Always push update task
                                taskQueue->pushTask(entity->getScriptOnUpdateTask());

                                // If there are events to process, push on event task
                                if (entity->hasEvents())
                                {
                                  taskQueue->pushTask(entity->getScriptOnEventTask());
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
            }
          }
        }
      }
    }
  }
}
