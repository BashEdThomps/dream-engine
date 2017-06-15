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

namespace Dream
{
    Project::Project
    (IWindowComponent* windowComponent)
        : mActiveSceneHandle(nullptr)
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
        destroyAllScenes();
        destroyAllAssetDefinitions();
    }

    void
    Project::loadMetadataFromJson
    (nlohmann::json mJson)
    {
        loadAssetDefinitionsFromJson(mJson[Constants::PROJECT_ASSET_ARRAY]);
        loadScenesFromJson(mJson[Constants::PROJECT_SCENE_ARRAY]);
        showStatus();
    }

    void
    Project::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Project: " << mJson.dump(1) << endl;
        }
    }

    string
    Project::getName
    ()
    {
        return mJson[Constants::PROJECT_NAME];
    }

    void
    Project::setName
    (string name)
    {
        mJson[Constants::PROJECT_NAME] = name;
    }

    void
    Project::setUuid
    (string uuid)
    {
        mJson[Constants::PROJECT_UUID] = uuid;
    }

    string
    Project::getUuid
    ()
    {
        return mJson[Constants::PROJECT_UUID];
    }

    void
    Project::loadScenesFromJson
    (nlohmann::json jsonSceneArray)
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Loading Scenes from JSON Array" << endl;
        }
        for (nlohmann::json it : jsonSceneArray)
        {
            if (Constants::DEBUG)
            {
                cout << "Project: Creating Scene using project " << mProjectPath << endl;
            }
            Scene *nextScene = new Scene(this,it);
            nextScene->showStatus();
            addScene(nextScene);
        }
    }

    void
    Project::setStartupSceneUuid
    (string sceneUuid)
    {
        mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
    }

    string
    Project::getStartupSceneUuid
    ()
    {
        return mJson[Constants::PROJECT_STARTUP_SCENE];
    }

    Scene*
    Project::getStartupScene
    ()
    {
        return getSceneByUuid(getStartupSceneUuid());
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
        mJson[Constants::PROJECT_DESCRIPTION] = description;
    }

    string
    Project::getDescription
    (void)
    {
        return mJson[Constants::PROJECT_DESCRIPTION];
    }

    void
    Project::setAuthor
    (string author)
    {
        mJson[Constants::PROJECT_AUTHOR] = author;
    }

    string
    Project::getAuthor
    ()
    {
        return mJson[Constants::PROJECT_AUTHOR];
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
        mActiveSceneHandle = scene;
    }

    Scene*
    Project::getActiveScene
    ()
    {
        return mActiveSceneHandle;
    }

    bool
    Project::hasActiveScene
    ()
    {
        return mActiveSceneHandle != nullptr;
    }

    int
    Project::getWindowWidth
    ()
    {
        return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH];
    }

    void
    Project::setWindowWidth
    (int width)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WIDTH] = width;
    }

    int
    Project::getWindowHeight
    ()
    {
       return mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT];
    }

    void
    Project::setWindowHeight
    (int height)
    {
        mJson[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_HEIGHT] = height;
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
        if (Constants::DEBUG)
        {
            cout << "Project: Removing AssetDefinition "
                 << assetDef->getNameAndUuidString() << endl;
        }

        remove_if(begin(mAssetDefinitions),end(mAssetDefinitions),
            [&](AssetDefinition* thisDefinition)
            {
                return thisDefinition == assetDef;
            }
        );
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
        if (Constants::DEBUG)
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
        for (Scene* scene : mScenes)
        {
            delete scene;
        }

        mScenes.clear();
    }

    void
    Project::destroyAllAssetDefinitions
    ()
    {
        for (AssetDefinition* assetDef : mAssetDefinitions)
        {
            delete assetDef;
        }
        mAssetDefinitions.clear();
    }

    nlohmann::json Project::getJson()
    {
        return mJson;
    }

    bool
    Project::loadActiveScene
    ()
    {
        // Check valid
        if (!hasActiveScene())
        {
            cerr << "Project: Cannot load scene, null!" << endl;
            return false;
        }

        // Load the new scene
        if (Constants::DEBUG)
        {
            cout << "Project: Loading Scene " << mActiveSceneHandle->getName() << endl;
        }

        mRuntime->getGraphicsComponent()->setActiveScene(mActiveSceneHandle);
        mRuntime->getPhysicsComponent()->setGravity(mActiveSceneHandle->getGravity());
        mRuntime->getPhysicsComponent()->setDebug(mActiveSceneHandle->getPhysicsDebug());
        mRuntime->getCamera()->setTranslation(mActiveSceneHandle->getDefaultCameraTransform().getTranslation());
        mRuntime->getCamera()->setRotation(mActiveSceneHandle->getDefaultCameraTransform().getRotation());
        mRuntime->getCamera()->setMovementSpeed(mActiveSceneHandle->getCameraMovementSpeed());

        return true;
    }

    void
    Project::cleanUpActiveScene
    ()
    {
        mActiveSceneHandle->cleanUp();
        mRuntime->cleanupComponents(mActiveSceneHandle);
        mActiveSceneHandle = nullptr;
    }


    bool
    Project::loadFromFileReader
    (string projectPath, FileReader* reader)
    {
        if (Constants::DEBUG)
        {
            cout << "Project: Loading project from FileReader " << reader->getPath() << endl;
        }

        string projectJsonStr = reader->getContentsAsString();

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
        mProjectPath = projectPath;
        mJson = projectJson;
        loadMetadataFromJson(mJson);

        return true;
    }

    bool
    Project::loadFromDirectory
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
                    cout << "Project: loadFromDirectory - Found uuid " << uuid << endl;
                }
            }
            else if (filename.compare(Constants::ASSET_DIR) == 0)
            {
                if (Constants::DEBUG)
                {
                    cout << "Project: loadFromDirectory - Found asset directory " << endl;
                }
                hasAssetDirectory = true;
            }
        }

        if (uuid.size() != Constants::PROJECT_UUID_LENGTH  || !hasAssetDirectory)
        {
            cerr << "Project: Error " << directory << " is not a valid project directory!" << endl;
            return false;
        }

        if (Constants::VERBOSE)
        {
            cout << "Project: Loading " << uuid << Constants::PROJECT_EXTENSION << " from Directory" << directory << endl;
        }

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + uuid + Constants::PROJECT_EXTENSION;

        FileReader *projectFileReader = new FileReader(projectFilePath);
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadFromFileReader(directory, projectFileReader);
        delete projectFileReader;
        return loadSuccess;
    }

    bool
    Project::loadFromArgumentParser
    (ArgumentParser *parser)
    {
        if (Constants::VERBOSE)
        {
            cout << "Project: Loading from ArgumentParser" << endl;
        }
        FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadFromFileReader(parser->getProjectPath(), projectFileReader);
        delete projectFileReader;
        delete parser;
        return loadSuccess;
    }

    void
    Project::setStartupSceneActive
    ()
    {
        setActiveScene(getStartupScene());
    }

    void
    Project::updateLogic
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== Project: UpdateLogic Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }

        mActiveSceneHandle->createAllAssetInstances();
        mActiveSceneHandle->loadAllAssetInstances();

        mRuntime->getTime()->update();

        mRuntime->getLuaEngine()->createAllScripts();
        mRuntime->getLuaEngine()->update();

        mRuntime->getAnimationComponent()->updateComponent(mActiveSceneHandle);
        mRuntime->getAudioComponent()->updateComponent(mActiveSceneHandle);
        mRuntime->getWindowComponentHandle()->updateComponent(mActiveSceneHandle);
        mRuntime->getPhysicsComponent()->updateComponent(mActiveSceneHandle);

        // Update Graphics/Physics Components

        mRuntime->getGraphicsComponent()->updateComponent(mActiveSceneHandle);
        mRuntime->getPhysicsComponent()->setViewProjectionMatrix(
            mRuntime->getGraphicsComponent()->getViewMatrix(),
            mRuntime->getGraphicsComponent()->getProjectionMatrix()
        );
    }

    void
    Project::updateGraphics
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== Project: UpdateGraphics Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }

        // Draw 3D/PhysicsDebug/2D

        mRuntime->getGraphicsComponent()->preRender();

        mRuntime->getGraphicsComponent()->draw3DQueue();
        mRuntime->getPhysicsComponent()->drawDebug();
        mRuntime->getGraphicsComponent()->draw2DQueue();
        mRuntime->getWindowComponentHandle()->swapBuffers();

        mRuntime->getGraphicsComponent()->postRender();

    }

    void
    Project::updateFlush
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== Project: updateFlush Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }

        // Cleanup Old
        mActiveSceneHandle->flush();

    }

    void
    Project::updateAll
    ()
    {
        if (mActiveSceneHandle)
        {
            updateLogic();
            updateGraphics();
            updateFlush();
        }
    }

    bool
    Project::initRuntime
    ()
    {
        return mRuntime->initComponents();
    }

    ProjectRuntime*
    Project::getRuntime
    ()
    {
        return mRuntime.get();
    }
} // End of Dream
