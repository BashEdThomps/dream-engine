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

#include "Project.h"

#include "ProjectRuntime.h"
#include "ProjectDefinition.h"

#include "Common/Logger.h"
#include "Common/Uuid.h"
#include "Storage/Directory.h"

#include "Components/AssetDefinition.h"
#include "Components/Time.h"
#include "Math/Transform.h"
#include "Components/AssetRuntime.h"
#include "Components/Window/WindowComponent.h"

#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"

#include <algorithm>
#include <thread>



namespace octronic::dream
{
    Project::Project
    (ProjectDirectory* dir, StorageManager* fm)
        : mDirectory(dir),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mWindowComponent(nullptr),
          mAudioComponent(nullptr),
          mStorageManager(fm)
    {
        LOG_TRACE("Project: Constructing");
    }

    Project::~Project
    ()
    {
        LOG_TRACE("Project: Destructing");

        if (mRuntime != nullptr)
        {
            delete mRuntime;
            mRuntime = nullptr;
        }

        if (mDefinition != nullptr)
        {
            delete mDefinition;
            mDefinition = nullptr;
        }
    }

    ProjectRuntime* // public
    Project::createProjectRuntime
    ()
    {
        LOG_DEBUG("Project: Creating project runtime for {}", mDefinition->getNameAndUuidString());
        mRuntime = new ProjectRuntime(this, mWindowComponent,mAudioComponent,mStorageManager);
        if (!mRuntime->loadFromDefinition())
        {
            LOG_CRITICAL("Project: Failed to create project runtime");
            delete mRuntime;
            mRuntime = nullptr;
        }
        return mRuntime;
    }

    bool
    Project::hasProjectRuntime
    ()
    const
    {
        return mRuntime != nullptr;
    }

    void Project::resetProjectRuntime()
    {
        LOG_DEBUG("Project: Resetting project runtime");
        delete mRuntime;
        mRuntime = nullptr;
    }

    bool // public
    Project::hasProjectDefinition
    ()
    const
    {
        return mDefinition != nullptr;
    }

    ProjectRuntime* // public
    Project::getRuntime
    ()
    const
    {
        return mRuntime;
    }

    ProjectDefinition* // public
    Project::getDefinition
    () const
    {
        return mDefinition;
    }

    AssetDefinition* // public
    Project::getAssetDefinitionByUuid
    (UuidType uuid)
    const
    {
        if (mDefinition != nullptr)
        {
            return dynamic_cast<ProjectDefinition*>(mDefinition)->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    void // public
    Project::setDefinition
    (ProjectDefinition* definition)
    {
        mDefinition = definition;
    }

    void // public
    Project::setWindowComponent
    (WindowComponent* windowComponent)
    {
        mWindowComponent = windowComponent;
    }

    void  // public
    Project::setAudioComponent(AudioComponent* audioComponent)
    {
        mAudioComponent = audioComponent;
    }

    ProjectDirectory* // public
    Project::getDirectory
    ()
    const
    {
        return mDirectory;
    }
}
