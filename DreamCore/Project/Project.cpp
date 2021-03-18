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
#include <memory>

using std::make_shared;

namespace octronic::dream
{
    Project::Project
    (const weak_ptr<ProjectDirectory>& dir,
     const weak_ptr<StorageManager>& fm)
        : mDirectory(dir),
          mStorageManager(fm)
    {
        LOG_TRACE("Project: Constructing");
    }

    Project::~Project
    ()
    {
        LOG_TRACE("Project: Destructing");
    }

    weak_ptr<ProjectRuntime>
    Project::createProjectRuntime
    ()
    {
        LOG_DEBUG("Project: Creating project runtime for {}", mDefinition->getNameAndUuidString());
        mRuntime = make_shared<ProjectRuntime>(mDefinition, mDirectory, mWindowComponent,mAudioComponent,mStorageManager);
        if (!mRuntime->loadFromDefinition())
        {
            LOG_CRITICAL("Project: Failed to create project runtime");
            mRuntime.reset();
        	return mRuntime;
        }
        return weak_ptr<ProjectRuntime>();
    }

    bool
    Project::hasProjectRuntime
    ()
    const
    {
        return mRuntime != nullptr;
    }

    void
    Project::resetProjectRuntime
    ()
    {
        LOG_DEBUG("Project: Resetting project runtime");
        mRuntime.reset();
    }

    bool
    Project::hasProjectDefinition
    ()
    const
    {
        return mDefinition != nullptr;
    }

    weak_ptr<ProjectRuntime>
    Project::getRuntime
    ()
    const
    {
        return mRuntime;
    }

    weak_ptr<ProjectDefinition>
    Project::getDefinition
    () const
    {
        return mDefinition;
    }

    weak_ptr<AssetDefinition>
    Project::getAssetDefinitionByUuid
    (UuidType uuid)
    const
    {
        if (mDefinition != nullptr)
        {
            return mDefinition->getAssetDefinitionByUuid(uuid);
        }
        return weak_ptr<AssetDefinition>();
    }

    void
    Project::setDefinition
    (const shared_ptr<ProjectDefinition>& definition)
    {
        mDefinition = definition;
    }

    void
    Project::setWindowComponent
    (const shared_ptr<WindowComponent>& windowComponent)
    {
        mWindowComponent = windowComponent;
    }

    void
    Project::setAudioComponent
    (const shared_ptr<AudioComponent>& audioComponent)
    {
        mAudioComponent = audioComponent;
    }

    weak_ptr<ProjectDirectory>
    Project::getDirectory
    ()
    const
    {
        return mDirectory;
    }
}
