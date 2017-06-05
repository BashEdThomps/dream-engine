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
    (IWindowComponent* windowComponent)
    {
        mRuntime = new ProjectRuntime(this, windowComponent);
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
    Project::loadMetadataFromJson
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

        loadAssetDefinitionsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
        loadScenesFromJson(jsonProject[PROJECT_SCENE_ARRAY]);
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
            Scene *nextScene = new Scene(it, mProjectPath, mAssetDefinitions, mRuntime);
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

    bool
    Project::loadSceneByUuid
    (string uuid)
    {
        return loadScene(getSceneByUuid(uuid));
    }

    bool
    Project::loadScene
    (Scene* scene)
    {
        // Check valid
        if (scene == nullptr)
        {
            cerr << "Project: Cannot load scene, null!" << endl;
            return false;
        }

        // Load the new scene
        if (DEBUG)
        {
            cout << "Project: Loading Scene " << scene->getName() << endl;
        }
        mActiveScene = scene;
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
    Project::cleanUp
    (Scene* scene)
    {

    }


    bool
    Project::loadFromFileReader
    (string projectPath, FileReader* reader)
    {
        if (DEBUG)
        {
            cout << "Dream: Loading project from FileReader " << reader->getPath() << endl;
        }
        string projectJsonStr = reader->getContentsAsString();
        if (projectJsonStr.empty())
        {
            cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
            return false;
        }
        json projectJson = json::parse(projectJsonStr);
        /*
        if (VERBOSE)
        {
            cout << "Project: Read Project..." << endl
                 << projectJson.dump(2) << endl;
        }
        */
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
            size_t dotJsonIndex = filename.find(PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                uuid = filename.substr(0,dotJsonIndex);
                if (DEBUG)
                {
                    cout << "Project: loadFromDirectory - Found uuid " << uuid << endl;
                }
            }
            else if (filename.compare(ASSET_DIR) == 0)
            {
                if (DEBUG)
                {
                    cout << "Project: loadFromDirectory - Found asset directory " << endl;
                }
                hasAssetDirectory = true;
            }
        }

        if (uuid.size() != PROJECT_UUID_LENGTH  || !hasAssetDirectory)
        {
            cerr << "Project: Error " << directory << " is not a valid project directory!" << endl;
            return false;
        }

        if (VERBOSE)
        {
            cout << "Project: Loading " << uuid << PROJECT_EXTENSION << " from Directory" << directory << endl;
        }

        string projectFilePath = directory + PROJECT_PATH_SEP + uuid + PROJECT_EXTENSION;

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
        if (VERBOSE)
        {
            cout << "Dream: Loading from ArgumentParser" << endl;
        }
        FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadFromFileReader(parser->getProjectPath(), projectFileReader);
        delete projectFileReader;
        delete parser;
        return loadSuccess;
    }



    void
    Project::stopActiveScene
    ()
    {
       mActiveScene->cleanUp();
       mRuntime->cleanupComponents(mActiveScene);
       mActiveScene = nullptr;
    }

    bool
    Project::loadStartupScene
    ()
    {
        // Init Startup Scene
        if (!loadScene(getStartupScene()))
        {
            cerr << "Dream:Error:Unable to load startup scene." << endl;
            return false;
        }
        return true;
    }

    bool
    Project::updateLogic
    ()
    {
        if (VERBOSE)
        {
            cout << "==== Project: UpdateLogic Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }

        // Update Time
        mRuntime->getTime()->update();

        // Create all script instances
        if (!mRuntime->getLuaEngine()->createAllScripts())
        {
            cerr << "Project: While loading lua scripts" << endl;
            return 1;
        }

        // Call onUpdate on all lua scripts
        if (!mRuntime->getLuaEngine()->update())
        {
            cerr << "Project: LuaComponentInstance update error!" << endl;
            return 1;
        }

        // Create new Assets
        mActiveScene->createAllAssetInstances();
        // Update Audio
        mRuntime->getAudioComponent()->updateComponent(mActiveScene);
        // Update Window
        mRuntime->getWindowComponent()->updateComponent(mActiveScene);
        // Update Physics
        mRuntime->getPhysicsComponent()->updateComponent(mActiveScene);

        return !mRuntime->isDone();
    }

    bool
    Project::updateGraphics
    ()
    {
        if (VERBOSE)
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
        // Update state
        return mRuntime->getWindowComponent()->shouldClose();
    }

    bool
    Project::updateCleanup
    ()
    {
        if (VERBOSE)
        {
            cout << "==== Project: UpdateCleanup Called @ " << mRuntime->getTime()->getTimeDelta() << " ====" << endl;
        }
        // Cleanup Old
        mActiveScene->findDeletedSceneObjects();
        mActiveScene->findDeletedScripts();
        mActiveScene->destroyDeleteQueue();
        // Chill
        return !mRuntime->isDone();
    }

    bool
    Project::updateAll
    ()
    {
        updateLogic();
        updateGraphics();
        updateCleanup();
        return mRuntime->isDone();
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
