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
#include <dirent.h>
#include <thread>
#include <unistd.h>

#include "ProjectRuntime.h"
#include "ProjectDefinition.h"

#include "../Common/Constants.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/IAssetInstance.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Components/Window/IWindowComponent.h"
#include "../Components/Scripting/IScriptComponent.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneDefinition.h"


#include "../Utilities/ArgumentParser.h"
#include "../Utilities/FileReader.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    Project::Project
    (shared_ptr<IWindowComponent> windowComponent)
        : DreamObject("Project"),
          mWindowComponent(windowComponent)
    {
        getLog()->trace("Constructing");
    }

    Project::~Project()
    {
        auto log = getLog();
        log->trace("Destructing");
        if (mDefinition != nullptr)
        {
            log->info(mDefinition->getNameAndUuidString());
        }
    }

    bool
    Project::openFromFileReader
    (string projectPath, FileReader &reader)
    {
        auto log = getLog();
        log->info("Loading project from FileReader", reader.getPath());

        string projectJsonStr = reader.getContentsAsString();

        if (projectJsonStr.empty())
        {
            log->error("Loading Failed. Project Content is Empty");
            return false;
        }

        json projectJson = json::parse(projectJsonStr);

        log->info("Project path", projectPath);

        mProjectPath = projectPath;
        mDefinition = make_shared<ProjectDefinition>(projectJson);
        mDefinition->loadChildDefinitions();
        return true;
    }

    bool
    Project::openFromDirectory
    (string directory)
    {
        auto log = getLog();
        DIR *dir;
        struct dirent *ent;
        vector<string> directoryContents;
        if ((dir = opendir(directory.c_str())) != nullptr)
        {
            while ((ent = readdir (dir)) != nullptr)
            {
                directoryContents.push_back(string(ent->d_name));
            }
            closedir (dir);
        }
        else
        {
            log->error( "Unable to open directory {}", directory );
            return false;
        }

        string projectFileName;
        bool hasAssetDirectory = false;

        for (string filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                projectFileName = filename.substr(0,dotJsonIndex);
                log->info( "Project: openFromDirectory - Found project file ",
                           projectFileName
                           );
            }
            else if (filename.compare(Constants::ASSET_DIR) == 0)
            {
                log->info( "Project: openFromDirectory - Found asset directory " );
                hasAssetDirectory = true;
            }
        }

        if (projectFileName.size() == 0 || !hasAssetDirectory)
        {
            log->error( "Project: Error {} is not a valid project directory!", directory  );
            return false;
        }

        {
            log->info( "Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , directory );
        }

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + projectFileName + Constants::PROJECT_EXTENSION;

        FileReader projectFileReader(projectFilePath);

        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    shared_ptr<ProjectRuntime>
    Project::createProjectRuntime
    ()
    {
        auto log = getLog();
        mRuntime = make_shared<ProjectRuntime>(dynamic_pointer_cast<Project>(shared_from_this()), mWindowComponent);
        mRuntime->useDefinition(nullptr);
        return mRuntime;
    }

    shared_ptr<ProjectDefinition>
    Project::createNewProjectDefinition
    (string name)
    {
        json j = json::object();

        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";
        j[Constants::PROJECT_CAPTURE_JOYSTICK] = false;
        j[Constants::PROJECT_CAPTURE_MOUSE] = false;
        j[Constants::PROJECT_CAPTURE_KEYBOARD] = false;

        j[Constants::PROJECT_WINDOW_SIZE] = json::object();
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH] = Constants::PROJECT_DEFAULT_WINDOW_WIDTH;
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT] = Constants::PROJECT_DEFAULT_WINDOW_HEIGHT;

        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();

        return make_shared<ProjectDefinition>(j);
    }

    bool
    Project::hasProjectRuntime
    ()
    {
        return mRuntime != nullptr;
    }

    void Project::resetProjectRuntime()
    {
        auto log = getLog();
        log->debug("Resetting project runtime");
        mRuntime.reset();
        log->debug("Runtime now has {} references", mRuntime.use_count());

    }

    bool
    Project::hasProjectDefinition
    ()
    {
        return mDefinition != nullptr;
    }

    bool
    Project::openFromArgumentParser
    (ArgumentParser &parser)
    {
        auto log = getLog();
        log->info( "Project: Loading from ArgumentParser" );
        FileReader projectFileReader(parser.getProjectFilePath());
        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(parser.getProjectPath(), projectFileReader);
        return loadSuccess;
    }

    shared_ptr<ProjectRuntime>
    Project::getProjectRuntime
    ()
    {
        return mRuntime;
    }

    shared_ptr<ProjectDefinition>
    Project::getProjectDefinition
    ()
    {
        return mDefinition;
    }

    string
    Project::getProjectPath
    ()
    {
        return mProjectPath;
    }


} // End of Dream
