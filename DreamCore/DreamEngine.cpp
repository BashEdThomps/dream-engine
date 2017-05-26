/*
 * DreamEngine
 *
 * Created: 14 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "DreamEngine.h"

namespace Dream {

    DreamEngine::DreamEngine
    (IAudioComponent* audioComponent, IWindowComponent* windowComponent)
    {
        dreamSetVerbose(true);
        if (DEBUG)
        {
            cout << "DreamEngine: Creating new Instance" << endl;
        }
        setAnimationComponent(nullptr);
        setAudioComponent(audioComponent);
        setPhysicsComponent(nullptr);
        setGraphicsComponent(nullptr);
        setWindowComponent(windowComponent);
        setCamera(nullptr);
        setTime(nullptr);
        setDone(false);
        setActiveScene(nullptr);
        setProject(nullptr);
        setGameController(new GameController());
    }



    DreamEngine::~DreamEngine
    ()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Destroying Object" << endl;
        }
        destroyComponents();
        if (mGameController)
        {
            delete mGameController;
        }
    }

    void
    DreamEngine::setWindowComponent
    (IWindowComponent* wc)
    {
        mWindowComponent = wc;
    }

    IWindowComponent*
    DreamEngine::getWindowComponent
    ()
    {
        return mWindowComponent;
    }


    Project*
    DreamEngine::
    getProject()
    {
        return mProject;
    }


    void
    DreamEngine::
    setCamera(Camera* camera)
    {
        mCamera = camera;
    }


    void
    DreamEngine::
    setAudioComponent(IAudioComponent* audioComp)
    {
        mAudioComponent = audioComp;
    }


    void
    DreamEngine::
    setAnimationComponent(AnimationComponent* animComp)
    {
        mAnimationComponent = animComp;
    }


    void
    DreamEngine::
    setPhysicsComponent(PhysicsComponent* physicsComp)
    {
        mPhysicsComponent = physicsComp;
    }


    void
    DreamEngine::
    setGraphicsComponent(GraphicsComponent* graphicsComp)
    {
        mGraphicsComponent = graphicsComp;
    }


    void
    DreamEngine::
    setDone(bool done)
    {
        mDone = done;
    }


    void
    DreamEngine::
    setTime(Time* time)
    {
        mTime = time;
    }


    void
    DreamEngine::
    setProject(Project* project)
    {
        mProject = project;
    }


    bool
    DreamEngine::
    isProjectLoaded()
    {
        return mProject != nullptr;
    }

    bool
    DreamEngine::
    loadProjectFromFileReader(string projectPath, FileReader* reader)
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
            cout << "DreamEngine: Read Project..." << endl
                 << projectJson.dump(2) << endl;
        }
        */
        setProject(new Project(projectPath, projectJson, mAudioComponent));
        return isProjectLoaded();
    }

    bool
    DreamEngine::
    loadFromDirectory(string directory)
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
                    cout << "DreamEngine: loadFromDirectory - Found uuid " << uuid << endl;
                }
            }
            else if (filename.compare(ASSET_DIR) == 0)
            {
                if (DEBUG)
                {
                    cout << "DreamEngine: loadFromDirectory - Found asset directory " << endl;
                }
                hasAssetDirectory = true;
            }
        }

        if (uuid.size() != PROJECT_UUID_LENGTH  || !hasAssetDirectory)
        {
            cerr << "DreamEngine: Error " << directory << " is not a valid project directory!" << endl;
            return false;
        }

        if (VERBOSE)
        {
            cout << "DreamEngine: Loading " << uuid << PROJECT_EXTENSION << " from Directory" << directory << endl;
        }

        string projectFilePath = directory + PROJECT_PATH_SEP + uuid + PROJECT_EXTENSION;

        FileReader *projectFileReader = new FileReader(projectFilePath);
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadProjectFromFileReader(directory, projectFileReader);
        delete projectFileReader;
        return loadSuccess;
    }


    bool
    DreamEngine::
    loadFromArgumentParser(ArgumentParser *parser)
    {
        if (VERBOSE)
        {
            cout << "Dream: Loading from ArgumentParser" << endl;
        }
        FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
        delete projectFileReader;
        delete parser;
        return loadSuccess;
    }


    bool
    DreamEngine::
    loadSceneByUuid(string uuid)
    {
        Scene* scene = mProject->getSceneByUuid(uuid);
        return loadScene(scene);
    }


    bool
    DreamEngine::
    loadScene(Scene* scene)
    {
        // Check valid
        if (scene == nullptr)
        {
            cerr << "Dream: Cannot load scene, null!" << endl;
            return false;
        }
        // Load the new scene
        if (DEBUG)
        {
            cout << "Dream: Loading Scene " << scene->getName() << endl;
        }
        mActiveScene = scene;
        mGraphicsComponent->setClearColour(mActiveScene->getClearColour());
        mGraphicsComponent->setAmbientLightColour(mActiveScene->getAmbientLightColour());
        mPhysicsComponent->setGravity(mActiveScene->getGravity());
        mPhysicsComponent->setDebug(mActiveScene->getPhysicsDebug());
        mCamera->setTranslation(mActiveScene->getDefaultCameraTranslation());
        mCamera->setRotation(mActiveScene->getDefaultCameraRotation());
        mCamera->setMovementSpeed(mActiveScene->getCameraMovementSpeed());
        return true;
    }

    Scene*
    DreamEngine::
    getActiveScene()
    {
        return mActiveScene;
    }


    bool
    DreamEngine::
    initEngine()
    {
        // Init Components
        if(!initComponents())
        {
            cerr << "Dream:Error:Unable to create components." << endl;
            return false;
        }

        // Init Startup Scene
        if (!loadScene(mProject->getStartupScene()))
        {
            cerr << "Dream:Error:Unable to load startup scene." << endl;
            return false;
        }

        if (DEBUG)
        {
            cout << "Dream:Info:Starting Startup Scene " << getActiveScene()->getNameAndUuidString() << endl;
        }

        return true;
    }


    bool DreamEngine::updateLogic()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateLogic Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }

        // Update Time
        mTime->update();

        // Sync the script map from the current scene
        mLuaEngine->setLuaScriptMap(mActiveScene->getLuaScriptMap());

        // Create all script instances
        if (!mLuaEngine->createAllScripts())
        {
            cerr << "DreamEngine: While loading lua scripts" << endl;
            return 1;
        }

        // Call onUpdate on all lua scripts
        if (!mLuaEngine->update())
        {
            cerr << "DreamEngine: LuaComponentInstance update error!" << endl;
            return 1;
        }

        // Update Window
        mWindowComponent->updateComponent(mActiveScene);
        // Create new Assets
        mActiveScene->createAllAssetInstances();

        mPhysicsComponent->updateComponent(mActiveScene);

        return !mDone;
    }

    bool DreamEngine::updateGraphics()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateGraphics Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }
        // Update Graphics/Physics Components
        mGraphicsComponent->updateComponent(mActiveScene);
        mPhysicsComponent->setViewProjectionMatrix(
                    mGraphicsComponent->getViewMatrix(),
                    mGraphicsComponent->getProjectionMatrix()
                    );
        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->draw3DQueue();
        mPhysicsComponent->drawDebug();
        mGraphicsComponent->draw2DQueue();

        mWindowComponent->swapBuffers();
        // Update state
        mDone = mWindowComponent->shouldClose();
        return !mDone;
    }

    bool DreamEngine::updateCleanup()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateCleanup Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }
        // Cleanup Old
        mActiveScene->findDeletedSceneObjects();
        mActiveScene->findDeletedScripts();
        mActiveScene->destroyDeleteQueue();
        mActiveScene->clearDeleteQueue();
        mActiveScene->generateScenegraphVector();
        // Chill
        return !mDone;
    }

    Time*
    DreamEngine::
    getTime()
    {
        return mTime;
    }


    void
    DreamEngine::
    destroyComponents()
    {
        if (mAnimationComponent != nullptr)
        {
            delete mAnimationComponent;
            mAnimationComponent = nullptr;
        }

        if (mAudioComponent != nullptr)
        {
            delete mAudioComponent;
            mAudioComponent = nullptr;
        }

        if (mPhysicsComponent != nullptr)
        {
            delete mPhysicsComponent;
            mPhysicsComponent = nullptr;
        }

        if (mGraphicsComponent != nullptr)
        {
            delete mGraphicsComponent;
            mGraphicsComponent = nullptr;
        }
        if (mWindowComponent != nullptr)
        {
            delete mWindowComponent;
            mWindowComponent = nullptr;
        }
    }


    bool
    DreamEngine::
    initComponents()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Creating Components..." << endl;
        }

        setTime(new Time());

        if (!initWindowComponent())
        {
            return false;
        }

        if(!initGraphicsComponent())
        {
            return false;
        }

        if(!initPhysicsComponent())
        {
            return false;
        }

        if(!initAudioComponent())
        {
            return false;
        }

        if(!initAnimationComponent())
        {
            return false;
        }

        if (!initLuaEngine())
        {
            return false;
        }

        if (DEBUG)
        {
            cout << "Dream: Successfuly created Components." << endl;
        }

        return true;
    }


    bool
    DreamEngine::
    initWindowComponent()
    {
        mWindowComponent->setWidth(mProject->getWindowWidth());
        mWindowComponent->setHeight(mProject->getWindowHeight());
        mWindowComponent->setName(mProject->getName());

        if (!mWindowComponent->init())
        {
            cerr << "DreamEngine: Unable to initialise WindowComponent" << endl;
            return false;
        }

        return mWindowComponent != nullptr;
    }


    bool
    DreamEngine::
    initAudioComponent()
    {
        if (!mAudioComponent->init())
        {
            cerr << "Dream: Unable to initialise AudioComponent." << endl;
            return false;
        }
        return mAudioComponent != nullptr;
    }


    bool
    DreamEngine::
    initPhysicsComponent()
    {
        mPhysicsComponent = new PhysicsComponent();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            cerr << "ComponentManager: Unable to initialise PhysicsComponent." << endl;
            return false;
        }
        return mPhysicsComponent != nullptr;
    }


    bool
    DreamEngine::
    initGraphicsComponent()
    {
        setCamera(new Camera());

        mGraphicsComponent = new GraphicsComponent(mCamera,mWindowComponent);
        mGraphicsComponent->setTime(mTime);

        if (mGraphicsComponent->init())
        {
            mGraphicsComponent->setGameController(mGameController);
            return true;
        }
        else
        {
            cerr << "DreamEngine: Unable to initialise Graphics Component." << endl;
            return false;
        }
    }


    bool
    DreamEngine::
    initAnimationComponent()
    {
        mAnimationComponent = new AnimationComponent();
        mAnimationComponent->setTime(mTime);
        if (mAnimationComponent->init())
        {
            return true;
        }
        else
        {
            cerr << "DreamEngine: Unable to initialise Animation Component." << endl;
            return false;
        }
    }


    bool
    DreamEngine::
    isDone()
    {
        return mDone;
    }

    AnimationComponent*
    DreamEngine::
    getAnimationComponent()
    {
        return mAnimationComponent;
    }


    IAudioComponent*
    DreamEngine::
    getAudioComponent()
    {
        return mAudioComponent;
    }


    PhysicsComponent*
    DreamEngine::
    getPhysicsComponent()
    {
        return mPhysicsComponent;
    }


    GraphicsComponent*
    DreamEngine::
    getGraphicsComponent()
    {
        return mGraphicsComponent;
    }


    Camera*
    DreamEngine::
    getCamera()
    {
        return mCamera;
    }


    GameController*
    DreamEngine::
    getGameController()
    {
        return mGameController;
    }


    void
    DreamEngine::
    setGameController(GameController *gc)
    {
        mGameController = gc;
    }


    void
    DreamEngine::
    setActiveScene(Scene *scene)
    {
        mActiveScene = scene;
    }


    int DreamEngine::heartbeat()
    {
        updateLogic();
        updateGraphics();
        updateCleanup();
        return 0;
    }

    bool DreamEngine::initLuaEngine()
    {
        mLuaEngine = new LuaEngine(this);
        return mLuaEngine->init();
    }

}
