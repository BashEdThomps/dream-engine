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
    {
        mRuntime = new ProjectRuntime(this, windowComponent);
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
    (nlohmann::json jsonProject)
    {
        if (jsonProject [Constants::PROJECT_UUID].is_null())
        {
            cerr << "Project: UUID is nullptr." << endl;
        }
        else
        {
            setUuid(jsonProject[Constants::PROJECT_UUID]);
        }

        if (jsonProject[Constants::PROJECT_NAME].is_null())
        {
            cerr << "Project: Name is nullptr." << endl;
        }
        else
        {
            setName(jsonProject[Constants::PROJECT_NAME]);
        }

        if (jsonProject[Constants::PROJECT_AUTHOR].is_null())
        {
            cerr << "Project: Author is nullptr." << endl;
        }
        else
        {
            setAuthor(jsonProject[Constants::PROJECT_AUTHOR]);
        }

        if (jsonProject[Constants::PROJECT_DESCRIPTION].is_null())
        {
            cerr << "Project: Descriptiont is nullptr." << endl;
        }
        else
        {
            setDescription(jsonProject[Constants::PROJECT_DESCRIPTION]);
        }

        if (jsonProject[Constants::PROJECT_STARTUP_SCENE].is_null())
        {
            cerr << "Project: Startup Scene is nullptr." << endl;
        }
        else
        {
            setStartupSceneUuid(jsonProject[Constants::PROJECT_STARTUP_SCENE]);
        }

        if (!jsonProject[Constants::PROJECT_WINDOW_SIZE].is_null())
        {
            nlohmann::json windowSizeJson = jsonProject[Constants::PROJECT_WINDOW_SIZE];
            setWindowWidth(windowSizeJson[Constants::PROJECT_WIDTH]);
            setWindowHeight(windowSizeJson[Constants::PROJECT_HEIGHT]);
        }

        loadAssetDefinitionsFromJson(jsonProject[Constants::PROJECT_ASSET_ARRAY]);
        loadScenesFromJson(jsonProject[Constants::PROJECT_SCENE_ARRAY]);
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
        return mActiveScene != nullptr;
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
            cout << "Project: Loading Scene " << mActiveScene->getName() << endl;
        }

        mRuntime->setGraphicsClearColour(mActiveScene->getClearColour());
        mRuntime->setGraphicsAmbientLightColour(mActiveScene->getAmbientLightColour());
        mRuntime->setPhysicsGravity(mActiveScene->getGravity());
        mRuntime->setPhysicsDebug(mActiveScene->getPhysicsDebug());
        mRuntime->setCameraTranslation(mActiveScene->getDefaultCameraTranslation());
        mRuntime->setCameraRotation(mActiveScene->getDefaultCameraRotation());
        mRuntime->setCameraMovementSpeed(mActiveScene->getCameraMovementSpeed());

        return true;
    }

    void
    Project::cleanUpActiveScene
    ()
    {
        mActiveScene->cleanUp();
        mRuntime->cleanupComponents(mActiveScene);
        mActiveScene = nullptr;
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

        mActiveScene->createAllAssetInstances();
        mActiveScene->loadAllAssetInstances();

        mRuntime->getTime()->update();

        mRuntime->getLuaEngine()->createAllScripts();
        mRuntime->getLuaEngine()->update();

        mRuntime->getAnimationComponent()->updateComponent(mActiveScene);
        mRuntime->getAudioComponent()->updateComponent(mActiveScene);
        mRuntime->getWindowComponent()->updateComponent(mActiveScene);
        mRuntime->getPhysicsComponent()->updateComponent(mActiveScene);
    }

    void
    Project::updateGraphics
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== Project: UpdateGraphics Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }

        // Update Graphics/Physics Components

        mRuntime->getGraphicsComponent()->updateComponent(mActiveScene);
        mRuntime->getPhysicsComponent()->setViewProjectionMatrix(
            mRuntime->getGraphicsComponent()->getViewMatrix(),
            mRuntime->getGraphicsComponent()->getProjectionMatrix()
        );

        // Draw 3D/PhysicsDebug/2D

        mRuntime->getGraphicsComponent()->preRender();

        mRuntime->getGraphicsComponent()->draw3DQueue();
        mRuntime->getPhysicsComponent()->drawDebug();
        mRuntime->getGraphicsComponent()->draw2DQueue();
        mRuntime->getWindowComponent()->swapBuffers();

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
        mActiveScene->flush();

    }

    void
    Project::updateAll
    ()
    {
        if (mActiveScene)
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
        return mRuntime;
    }
} // End of Dream
