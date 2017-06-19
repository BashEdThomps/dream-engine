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

using std::move;

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
            cout << "Project: Destructing "
                 << mDefinition->getNameAndUuidString()
                 << endl;
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

        string uuid;
        bool hasAssetDirectory = false;

        for (string filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                uuid = filename.substr(0,dotJsonIndex);
                if (Constants::DEBUG)
                {
                    cout << "Project: openFromDirectory - Found uuid " << uuid << endl;
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

        if (uuid.size() != static_cast<size_t>(Constants::PROJECT_UUID_LENGTH)  || !hasAssetDirectory)
        {
            cerr << "Project: Error " << directory << " is not a valid project directory!" << endl;
            return false;
        }

        if (Constants::VERBOSE)
        {
            cout << "Project: Loading " << uuid << Constants::PROJECT_EXTENSION << " from Directory" << directory << endl;
        }

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + uuid + Constants::PROJECT_EXTENSION;

        FileReader projectFileReader(projectFilePath);
        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    void
    Project::createProjectRuntime
    ()
    {
        mRuntime.reset(new ProjectRuntime(this, mWindowComponentHandle));
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