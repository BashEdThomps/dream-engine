/*
* Project::Project
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

#include "../Scene/Scene.h"
#include "../Scene/SceneDefinition.h"

#include "../Project/AssetDefinition.h"

#include "../Utilities/ArgumentParser.h"
#include "../Utilities/FileReader.h"
#include "../Utilities/String.h"

using std::move;

namespace Dream
{
    Project::Project
    (IWindowComponent* windowComponent)
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Constructing Object" << endl;
        }
        mRuntime.reset(new ProjectRuntime(this, windowComponent));
    }

    Project::~Project()
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Destroying Object" << endl;
        }
    }

    bool
    Project::openFromFileReader
    (string projectPath, FileReader& reader)
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
            cout << "Project: using project path " << projectPath << endl;
        }
        mRuntime->setProjectPath(projectPath);
        mDefinition.reset(new ProjectDefinition(projectJson));
        //mDefinition->applyToRuntime(mRuntime.get());

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
        projectFileReader.readIntoStringStream();
        bool loadSuccess = openFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    bool
    Project::openFromArgumentParser
    (ArgumentParser& parser)
    {
        if (Constants::VERBOSE)
        {
            cout << "Project: Loading from ArgumentParser" << endl;
        }
        FileReader projectFileReader(parser.getProjectFilePath());
        projectFileReader.readIntoStringStream();
        bool loadSuccess = openFromFileReader(parser.getProjectPath(), projectFileReader);
        return loadSuccess;
    }

    ProjectRuntime*
    Project::getRuntimeHandle
    ()
    {
        return mRuntime.get();
    }

    ProjectDefinition*
    Project::getDefinitionHandle
    ()
    {
        return mDefinition.get();
    }

    void
    Project::addScene
    (json scene)
    {
        mScenes.push_back
        (
            unique_ptr<Scene>
            (
                new Scene(this,scene)
            )
        );
    }

    size_t
    Project::countScenes
    ()
    {
        return mScenes.size();
    }

    Scene*
    Project::getSceneHandleByUuid
    (string uuid)
    {
        for(auto it = begin(mScenes); it != end(mScenes); it++)
        {
            if ((*it)->getDefinitionHandle()->hasUuid(uuid))
            {
                return (*it).get();
            }
        }
        return nullptr;
    }

    void
    Project::setStartupSceneAsActive
    ()
    {
        setActiveSceneHandle(getSceneHandleByUuid(getDefinitionHandle()->getStartupSceneUuid()));
    }


    void
    Project::setActiveSceneHandle
    (Scene* scene)
    {
        mActiveSceneHandle = scene;
    }

    Scene*
    Project::getActiveSceneHandle
    ()
    {
        return mActiveSceneHandle;
    }

    bool
    Project::hasActiveSceneHandle
    ()
    {
        return mActiveSceneHandle != nullptr;
    }

    void
    Project::addAssetDefinition
    (json assetDefinition)
    {
        mAssetDefinitions.push_back
        (
            unique_ptr<AssetDefinition>
            (
                new AssetDefinition(this,assetDefinition)
            )
        );
    }

    void
    Project::removeAssetDefinition
    (const unique_ptr<AssetDefinition>& assetDef)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Removing AssetDefinition "
                 << assetDef->getNameAndUuidString() << endl;
        }

        remove_if(begin(mAssetDefinitions),end(mAssetDefinitions),
            [&](const unique_ptr<AssetDefinition>& thisDefinition)
            {
                return thisDefinition == assetDef;
            }
        );
    }

    size_t
    Project::countAssetDefinitions
    ()
    {
        return mAssetDefinitions.size();
    }

    AssetDefinition*
    Project::getAssetDefinitionHandleByUuid
    (string uuid)
    {
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it).get();
            }
        }
        return nullptr;
    }

} // End of Dream
