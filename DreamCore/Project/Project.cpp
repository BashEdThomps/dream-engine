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

#include <algorithm>
#include <thread>

#include "ProjectRuntime.h"
#include "ProjectDefinition.h"

#include "../Components/AssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform.h"
#include "../Components/AssetRuntime.h"
#include "../Components/Window/WindowComponent.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneDefinition.h"

#include "../Utilities/ArgumentParser.h"
#include "../Utilities/File.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"
#include "../Utilities/Directory.h"

namespace Dream
{
    Project::Project
    (ProjectDirectory* dir)
        : DreamObject("Project"),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mWindowComponent(nullptr),
          mDirectory(dir)
    {
        #ifdef DREAM_LOG
        getLog()->trace("Constructing");
        #endif
    }

    Project::~Project
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
        #endif

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

    ProjectRuntime*
    Project::createProjectRuntime
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Creating project runtime for {}", mDefinition->getNameAndUuidString());
        #endif
        mRuntime = new ProjectRuntime(this, mWindowComponent);
        if (!mRuntime->useDefinition())
        {
            #ifdef DREAM_LOG
            log->critical("Failed to create project runtime");
            #endif
            delete mRuntime;
            mRuntime = nullptr;
        }
        return mRuntime;
    }



    bool
    Project::hasProjectRuntime
    ()
    {
        return mRuntime != nullptr;
    }

    void Project::resetProjectRuntime()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Resetting project runtime");
        #endif
        delete mRuntime;
        mRuntime = nullptr;
    }

    bool
    Project::hasProjectDefinition
    ()
    {
        return mDefinition != nullptr;
    }

    ProjectRuntime*
    Project::getRuntime
    ()
    {
        return mRuntime;
    }

    ProjectDefinition*
    Project::getDefinition
    () const
    {
        return mDefinition;
    }

    AssetDefinition*
    Project::getAssetDefinitionByUuid
    (string uuid)
    {
        if (mDefinition != nullptr)
        {
            return dynamic_cast<ProjectDefinition*>(mDefinition)
                ->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    void
    Project::setDefinition
    (ProjectDefinition* definition)
    {
        mDefinition = definition;
    }

    void
    Project::setWindowComponent
    (WindowComponent* windowComponent)
    {
        mWindowComponent = windowComponent;
    }

    ProjectDirectory*
    Project::getDirectory
    ()
    const
    {
        return mDirectory;
    }

} // End of Dream
