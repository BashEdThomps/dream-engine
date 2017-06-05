/*
* Dream::Project
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

namespace Dream
{

    Project::Project
    (string projectPath, nlohmann::json jsonProject, AudioComponent *audioComponent)
    {
        mJson = jsonProject;
        mAudioComponent = audioComponent;
        setProjectPath(projectPath);
        setMetadata(jsonProject);
        loadAssetDefinitionsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
        loadScenesFromJson(jsonProject[PROJECT_SCENE_ARRAY]);
        if (DEBUG)
        {
            showStatus();
        }
    }

    Project::~Project()
    {
        if (DEBUG)
        {
            cout << "Project: Destroying Object" << endl;
        }
        destroyAllScenes();
        destroyAllAssetDefinitions();
    }

    void
    Project::setMetadata
    (nlohmann::json jsonProject)
    {
        if (jsonProject [PROJECT_UUID].is_null())
        {
            cerr << "Project: UUID is nullptr." << endl;
        }
        else
        {
            setUuid(jsonProject[PROJECT_UUID]);
        }

        if (jsonProject[PROJECT_NAME].is_null())
        {
            cerr << "Project: Name is nullptr." << endl;
        }
        else
        {
            setName(jsonProject[PROJECT_NAME]);
        }

        if (jsonProject[PROJECT_AUTHOR].is_null())
        {
            cerr << "Project: Author is nullptr." << endl;
        }
        else
        {
            setAuthor(jsonProject[PROJECT_AUTHOR]);
        }

        if (jsonProject[PROJECT_DESCRIPTION].is_null())
        {
            cerr << "Project: Descriptiont is nullptr." << endl;
        }
        else
        {
            setDescription(jsonProject[PROJECT_DESCRIPTION]);
        }

        if (jsonProject[PROJECT_STARTUP_SCENE].is_null())
        {
            cerr << "Project: Startup Scene is nullptr." << endl;
        }
        else
        {
            setStartupSceneUuid(jsonProject[PROJECT_STARTUP_SCENE]);
        }

        if (!jsonProject[PROJECT_WINDOW_SIZE].is_null())
        {
            nlohmann::json windowSizeJson = jsonProject[PROJECT_WINDOW_SIZE];
            setWindowWidth(windowSizeJson[PROJECT_WIDTH]);
            setWindowHeight(windowSizeJson[PROJECT_HEIGHT]);
        }
    }

    void
    Project::showStatus
    ()
    {
        cout << "Project"  << endl;
        cout << "{" << endl;
        cout << "\tUUID: " << getUuid() << endl;
        cout << "\tName: " << getName() << endl;
        cout << "\tAuthor: " << getAuthor() << endl;
        cout << "\tDescription: " << getDescription() << endl;
        cout << "\tStartup Scene: " << getStartupSceneUuid() << endl;
        cout << "\tWindow Width: " << getWindowWidth() << endl;
        cout << "\tWindow Height: " << getWindowHeight() << endl;
        cout << "}" << endl;
    }

    void
    Project::setName
    (string name)
    {
        mName = name;
    }

    void
    Project::setUuid
    (string uuid)
    {
        mUuid = uuid;
    }

    string
    Project::getUuid
    ()
    {
        return mUuid;
    }

    void
    Project::loadScenesFromJson
    (nlohmann::json jsonSceneArray)
    {
        if (DEBUG)
        {
            cout << "Project: Loading Scenes from JSON Array" << endl;
        }
        for (nlohmann::json it : jsonSceneArray)
        {
            if (DEBUG)
            {
                cout << "Project: Creating Scene" << endl;
            }
            Scene *nextScene = new Scene(
                it, mProjectPath, &mAssetDefinitions,mAudioComponent
            );
            nextScene->showStatus();
            addScene(nextScene);
        }
    }

    void
    Project::setStartupSceneUuid
    (string sceneUuid)
    {
        mStartupScene = sceneUuid;
    }

    void
    Project::setStartupSceneName
    (string sceneName)
    {
        mStartupScene = getSceneByName(sceneName)->getUuid();
    }

    string
    Project::getStartupSceneUuid
    ()
    {
        return mStartupScene;
    }

    Scene*
    Project::getStartupScene
    ()
    {
        return getSceneByUuid(getStartupSceneUuid());
    }

    string
    Project::getName
    ()
    {
        return mName;
    }

    vector<Scene*>
    Project::getSceneList
    ()
    {
        return mScenes;
    }

    void
    Project::setDescription
    (string description)
    {
        mDescription = description;
    }

    string
    Project::getDescription
    (void)
    {
        return mDescription;
    }

    void
    Project::setAuthor
    (string author)
    {
        mAuthor = author;
    }

    string
    Project::getAuthor
    ()
    {
        return mAuthor;
    }

    void
    Project::addScene
    (Scene *scene)
    {
        mScenes.push_back(scene);
    }

    size_t
    Project::getNumberOfScenes
    ()
    {
        return mScenes.size();
    }

    Scene*
    Project::getSceneByUuid
    (string uuid)
    {
        for(Scene* it : mScenes)
        {
            if (it->hasUuid(uuid))
            {
                return it;
            }
        }
        return nullptr;
    }

    Scene*
    Project::getSceneByName
    (string name)
    {
        for(Scene* it : mScenes)
        {
            if (it->hasName(name))
            {
                return it;
            }
        }
        return nullptr;
    }

    string
    Project::getProjectPath
    ()
    {
        return mProjectPath;
    }

    void
    Project::setProjectPath
    (string dir)
    {
        mProjectPath = dir;
    }

    void
    Project::setActiveScene
    (Scene* scene)
    {
        mActiveScene = scene;
    }

    Scene*
    Project::getActiveScene
    ()
    {
        return mActiveScene;
    }

    bool
    Project::hasActiveScene
    ()
    {
        return getActiveScene() != nullptr;
    }

    int
    Project::getWindowWidth
    ()
    {
        return mWindowWidth;
    }

    void
    Project::setWindowWidth
    (int width)
    {
        mWindowWidth = width;
    }

    int
    Project::getWindowHeight
    ()
    {
        return mWindowHeight;
    }

    void
    Project::setWindowHeight
    (int height)
    {
        mWindowHeight = height;
    }

    void
    Project::addAssetDefinition
    (AssetDefinition* assetDefinition)
    {
        mAssetDefinitions.push_back(assetDefinition);
    }

    void
    Project::removeAssetDefinition
    (AssetDefinition* assetDef)
    {
        for (vector<AssetDefinition*>::iterator it=mAssetDefinitions.begin(); it!=mAssetDefinitions.end(); it++)
        {
            if ((*it) == assetDef)
            {
                if (DEBUG)
                {
                    cout << "Project: Removing AssetDefinition " << (*it)->getUuid() << endl;
                }
                mAssetDefinitions.erase(it);
                break;
            }
        }
    }

    size_t
    Project::getNumberOfAssetDefinitions
    ()
    {
        return mAssetDefinitions.size();
    }

    void
    Project::loadAssetDefinitionsFromJson
    (nlohmann::json jsonAssetArray)
    {
        if (DEBUG)
        {
            cout << "Project: Loading Assets from JSON Array" << endl;
        }
        for (nlohmann::json it : jsonAssetArray)
        {
            addAssetDefinition(new AssetDefinition(it));
        }
    }

    AssetDefinition*
    Project::getAssetDefinitionByUuid
    (string uuid)
    {
        for (AssetDefinition* it : mAssetDefinitions)
        {
            if (it->hasUuid(uuid))
            {
                return it;
            }
        }
        return nullptr;
    }

    vector<AssetDefinition*>
    Project::getAssetDefinitions
    ()
    {
        return mAssetDefinitions;
    }

    void
    Project::destroyAllScenes
    ()
    {

    }

    void
    Project::destroyAllAssetDefinitions
    ()
    {

    }

    nlohmann::json Project::toJson()
    {
        return mJson;
    }
} // End of Dream
