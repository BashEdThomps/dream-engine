/*
 * Project
 *
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

#include "ProjectContext.h"

#include "ProjectRuntime.h"
#include "ProjectDefinition.h"
#include "ProjectDirectory.h"

#include "Common/Logger.h"
#include "Storage/StorageManager.h"
#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"

using std::exception;

namespace octronic::dream
{
  ProjectContext::ProjectContext
  (WindowComponent& wc, AudioComponent& ac, StorageManager& sm, const string& base_dir)
    : mWindowComponent(wc),
      mAudioComponent(ac),
      mStorageManager(sm),
      mProjectDirectory(std::in_place, sm, base_dir)
  {
    LOG_TRACE("Project: Constructing");
  }


  ProjectRuntime&
  ProjectContext::createProjectRuntime
  ()
  {
    // Create a new definition
    if (mProjectDefinition && mProjectDirectory)
    {
      auto& pDef = mProjectDefinition.value();
      auto& pDir = mProjectDirectory.value();

      LOG_DEBUG("Project: Creating project runtime for {}", pDef.getNameAndUuidString());

      mProjectRuntime.emplace(
            pDef,
            pDir,
            getStorageManager(),
            getWindowComponent(),
            getAudioComponent());

      if (mProjectRuntime.value().loadFromDefinition())
      {
        return mProjectRuntime.value();
      }
      LOG_CRITICAL("Project: Failed to create project runtime");
    }
    throw std::exception();
  }

  optional<ProjectRuntime>&
  ProjectContext::getProjectRuntime
  ()
  {
    return mProjectRuntime;
  }

  optional<ProjectDefinition>&
  ProjectContext::getProjectDefinition
  ()
  {
    return mProjectDefinition;
  }

  WindowComponent&
  ProjectContext::getWindowComponent
  ()
  const
  {
    return  mWindowComponent;
  }

  void
  ProjectContext::step()
  {
    if (mProjectRuntime)
    {
      mProjectRuntime.value().step();

      if (!mProjectRuntime.value().hasActiveSceneRuntime())
      {
        auto& pDef = mProjectDefinition.value();
        auto& pRunt = mProjectRuntime.value();

        auto startupSceneDef = pDef.getStartupSceneDefinition();
        if (startupSceneDef)
        {
          auto& startupSceneRuntime = pRunt.getSceneRuntimeByUuid(startupSceneDef.value().get().getUuid());

          if (startupSceneRuntime.hasState(SCENE_STATE_LOADED))
          {
            pRunt.setActiveSceneRuntime(startupSceneRuntime.getUuid());
          }
        }
      }
    }
  }

  StorageManager&
  ProjectContext::getStorageManager()
  const
  {
    return mStorageManager.get();
  }

  optional<ProjectDirectory>&
  ProjectContext::getProjectDirectory
  ()
  {
    return mProjectDirectory;
  }

  AudioComponent&
  ProjectContext::getAudioComponent
  ()
  const
  {
    return mAudioComponent.get();
  }

  void
  ProjectContext::addPrintListener
  (ScriptPrintListener& pl)
  {
    ScriptComponent::AddPrintListener(pl);
  }

  bool
  ProjectContext::openFromPath
  ()
  {
    if (mProjectDirectory)
    {
      auto& pDir = mProjectDirectory.value();
      mProjectDefinition.emplace(pDir.readProjectDefinition());

      if (mProjectDefinition)
      {
        createProjectRuntime();
        return true;
      }
      else
      {
        LOG_ERROR("ProjectContext: No Project Definition");
      }
    }
    else
    {
      LOG_ERROR("ProjectContext: No Project Directory");
    }
    return false;
  }

  bool
  ProjectContext::newProject
  (const string& baseDir)
  {
    mProjectDirectory.emplace(mStorageManager, baseDir);
    mProjectDefinition.emplace(mProjectDirectory.value().createProjectDefinition());
    try
    {
      createProjectRuntime();
      return true;
    }
    catch (exception& ex)
    {
      LOG_ERROR("ProjectContext: Error creating new project {}", ex.what());
      return false;
    }
  }
}
