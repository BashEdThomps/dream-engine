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

#include "../Components/AssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/IAssetInstance.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Lua/LuaEngine.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneDefinition.h"


#include "../Utilities/ArgumentParser.h"
#include "../Utilities/FileReader.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    Project::Project
    (IWindowComponent* windowComponent)
        : mWindowComponentHandle(windowComponent)
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Constructing " << endl;
        }
    }

    Project::~Project()
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Destructing ";
            if (mDefinition)
            {
                 cout << mDefinition->getNameAndUuidString();
            }

            cout << endl;
        }
    }

    bool
    Project::openFromFileReader
    (string projectPath, FileReader &reader)
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Loading project from FileReader " << reader.getPath() << endl;
        }

        string projectJsonStr = reader.getContentsAsString();

        if (projectJsonStr.empty())
        {
            cerr << "Project: Loading Failed. Project Content is Empty" << endl;
            return false;
        }

        json projectJson = json::parse(projectJsonStr);

        if (Constants::DEBUG)
        {
            cout << "Project: Project path " << projectPath << endl;
        }
        mProjectPath = projectPath;
        mDefinition.reset(new ProjectDefinition(projectJson));
        return true;
    }

    bool
    Project::openFromDirectory
    (string directory)
    {
        DIR *dir;
        struct dirent *ent;
        vector<string> directoryContents;
        if ((dir = opendir(directory.c_str())) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                directoryContents.push_back(string(ent->d_name));
            }
            closedir (dir);
        }
        else {
            cerr << "Unable to open directory " << directory << endl;
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
                if (Constants::DEBUG)
                {
                    cout << "Project: openFromDirectory - Found project file "
                         << projectFileName
                         << endl;
                }
            }
            else if (filename.compare(Constants::ASSET_DIR) == 0)
            {
                if (Constants::DEBUG)
                {
                    cout << "Project: openFromDirectory - Found asset directory " << endl;
                }
                hasAssetDirectory = true;
            }
        }

        if (projectFileName.size() == 0 || !hasAssetDirectory)
        {
            cerr << "Project: Error " << directory << " is not a valid project directory!" << endl;
            return false;
        }

        if (Constants::VERBOSE)
        {
            cout << "Project: Loading " << projectFileName << Constants::PROJECT_EXTENSION << " from Directory" << directory << endl;
        }

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + projectFileName + Constants::PROJECT_EXTENSION;

        FileReader projectFileReader(projectFilePath);

        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    ProjectRuntime*
    Project::createProjectRuntime
    ()
    {
        mRuntime.reset(new ProjectRuntime(this, mWindowComponentHandle));
        return mRuntime.get();
    }

    ProjectDefinition*
    Project::createNewProjectDefinition
    (string name)
    {
        json j = json::object();

        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";

        j[Constants::PROJECT_WINDOW_SIZE] = json::object();
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH] = Constants::PROJECT_DEFAULT_WINDOW_WIDTH;
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT] = Constants::PROJECT_DEFAULT_WINDOW_HEIGHT;

        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();

        ProjectDefinition* newProjectDefinitionHandle = new ProjectDefinition(j);
        return newProjectDefinitionHandle;
    }

    bool
    Project::hasProjectRuntime
    ()
    {
        return mRuntime != nullptr;
    }

    void Project::resetProjectRuntime()
    {
        mRuntime.reset(nullptr);
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
        if (Constants::VERBOSE)
        {
            cout << "Project: Loading from ArgumentParser" << endl;
        }
        FileReader projectFileReader(parser.getProjectFilePath());
        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(parser.getProjectPath(), projectFileReader);
        return loadSuccess;
    }

    ProjectRuntime*
    Project::getProjectRuntimeHandle
    ()
    {
        return mRuntime.get();
    }

    ProjectDefinition*
    Project::getProjectDefinitionHandle
    ()
    {
        return mDefinition.get();
    }

    string
    Project::getProjectPath
    ()
    {
        return mProjectPath;
    }


} // End of Dream
