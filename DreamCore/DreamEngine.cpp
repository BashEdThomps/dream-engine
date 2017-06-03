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

namespace Dream
{
    DreamEngine::DreamEngine
    (shared_ptr<IWindowComponent> windowComponent)
    {
        dreamSetVerbose(true);
        if (DEBUG)
        {
            cout << "DreamEngine: Creating new Instance" << endl;
        }
        setComponentsInitialised(false);
        setWindowComponent(windowComponent);
        setAnimationComponent(nullptr);
        setAudioComponent(nullptr);
        setPhysicsComponent(nullptr);
        setGraphicsComponent(nullptr);
        setDone(false);
    }

    DreamEngine::~DreamEngine
    ()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Destroying Object" << endl;
        }
        // TODO - Think
        //destroyComponents();
    }

    void
    DreamEngine::setWindowComponent
    (shared_ptr<IWindowComponent> wc)
    {
        mWindowComponent = wc;
    }

    shared_ptr<IWindowComponent>
    DreamEngine::getWindowComponent
    ()
    {
        return mWindowComponent;
    }

    Project
    DreamEngine::getProject
    ()
    {
        return mProject;
    }

    void
    DreamEngine::setCamera
    (Camera& camera)
    {
        mCamera = camera;
    }

    void
    DreamEngine::setAudioComponent
    (shared_ptr<AudioComponent> audioComp)
    {
        mAudioComponent = audioComp;
    }

    void
    DreamEngine::setAnimationComponent
    (shared_ptr<AnimationComponent> animComp)
    {
        mAnimationComponent = animComp;
    }

    void
    DreamEngine::setPhysicsComponent
    (shared_ptr<PhysicsComponent> physicsComp)
    {
        mPhysicsComponent = physicsComp;
    }

    void
    DreamEngine::setGraphicsComponent
    (shared_ptr<GraphicsComponent> graphicsComp)
    {
        mGraphicsComponent = graphicsComp;
    }

    void
    DreamEngine::setDone
    (bool done)
    {
        mDone = done;
    }

    void
    DreamEngine::setTime
    (Time& time)
    {
        mTime = time;
    }

    void
    DreamEngine::setProject
    (Project& project)
    {
        mProject = project;
    }

    bool
    DreamEngine::isProjectLoaded
    ()
    {
        return mProject.isLoaded();
    }

    bool
    DreamEngine::loadProjectFromFileReader
    (string projectPath, FileReader& reader)
    {
        if (DEBUG)
        {
            cout << "Dream: Loading project from FileReader " << reader.getPath() << endl;
        }
        string projectJsonStr = reader.getContentsAsString();
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
        Project proj(projectPath, projectJson, mAudioComponent,mLuaEngine);
        setProject(proj);
        return isProjectLoaded();
    }

    bool
    DreamEngine::
    loadFromDirectory
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

        FileReader projectFileReader(projectFilePath);
        projectFileReader.readIntoStringStream();
        bool loadSuccess = loadProjectFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    bool
    DreamEngine::loadFromArgumentParser
    (ArgumentParser parser)
    {
        if (VERBOSE)
        {
            cout << "Dream: Loading from ArgumentParser" << endl;
        }
        FileReader projectFileReader(parser.getProjectFilePath());
        projectFileReader.readIntoStringStream();
        bool loadSuccess = loadProjectFromFileReader(parser.getProjectPath(), projectFileReader);
        return loadSuccess;
    }

    bool
    DreamEngine::loadSceneByUuid
    (string uuid)
    {
        Scene scene = mProject.getSceneByUuid(uuid);
        return loadScene(scene);
    }

    bool
    DreamEngine::loadScene
    (Scene scene)
    {
        // Load the new scene
        if (DEBUG)
        {
            cout << "Dream: Loading Scene " << scene.getName() << endl;
        }
        mActiveScene = scene;
        mGraphicsComponent->setClearColour(mActiveScene.getClearColour());
        mGraphicsComponent->setAmbientLightColour(mActiveScene.getAmbientLightColour());
        mPhysicsComponent->setGravity(mActiveScene.getGravity());
        mPhysicsComponent->setDebug(mActiveScene.getPhysicsDebug());
        mCamera.setTranslation(mActiveScene.getDefaultCameraTranslation());
        mCamera.setRotation(mActiveScene.getDefaultCameraRotation());
        mCamera.setMovementSpeed(mActiveScene.getCameraMovementSpeed());
        return true;
    }

    void
    DreamEngine::stopActiveScene
    ()
    {
        if (DEBUG)
        {
            cout << "********************************************************************************" << endl
                 << "Stopping Scene" << mActiveScene.getNameAndUuidString() << endl
                 << "********************************************************************************" << endl;
        }
       mActiveScene.getRootSceneObject().setDeleteFlag(true);
       updateLogic();
       updateCleanup();
       cleanupComponents();
    }

    void
    DreamEngine::cleanupComponents
    ()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Cleaning up Components..." << endl;
        }

        if (mWindowComponent)
        {
            mWindowComponent->cleanUp();
        }

        if(mGraphicsComponent)
        {
            mGraphicsComponent->cleanUp();
        }

        if(mPhysicsComponent)
        {
            mPhysicsComponent->cleanUp();
        }

        if(mAudioComponent)
        {
            mAudioComponent->cleanUp();
        }

        if(mAnimationComponent)
        {
            mAnimationComponent->cleanUp();
        }

        if (mLuaEngine)
        {
            mLuaEngine->cleanUp();
        }

        if (DEBUG)
        {
            cout << "Dream: Finished Cleaning Up Components." << endl;
        }
    }

    Scene
    DreamEngine::getActiveScene
    ()
    {
        return mActiveScene;
    }

    bool
    DreamEngine::initEngine
    ()
    {
        // Init Components
        if(!initComponents())
        {
            cerr << "Dream:Error:Unable to create components." << endl;
            return false;
        }

        // Init Startup Scene
        if (!loadScene(mProject.getStartupScene()))
        {
            cerr << "Dream:Error:Unable to load startup scene." << endl;
            return false;
        }

        if (DEBUG)
        {
            cout << "Dream:Info:Starting Startup Scene " << getActiveScene().getNameAndUuidString() << endl;
        }

        return true;
    }

    bool
    DreamEngine::updateLogic
    ()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateLogic Called @ " << mTime.getTimeDelta() << " ====" << endl;
        }

        // Update Time
        mTime.update();

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

        // Create new Assets
        mActiveScene.createAllAssetInstances();
        // Update Audio
        mAudioComponent->updateComponent(mActiveScene);
        // Update Window
        mWindowComponent->updateComponent(mActiveScene);
        // Update Physics
        mPhysicsComponent->updateComponent(mActiveScene);

        return !mDone;
    }

    bool
    DreamEngine::updateGraphics
    ()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateGraphics Called @ " << mTime.getTimeDelta() << " ====" << endl;
        }
        // Update Graphics/Physics Components
        mGraphicsComponent->updateComponent(mActiveScene);
        mPhysicsComponent->setViewProjectionMatrix(
                    mGraphicsComponent->getViewMatrix(),
                    mGraphicsComponent->getProjectionMatrix()
                    );
        // Draw 3D/PhysicsDebug/2D

        mGraphicsComponent->preRender();

        mGraphicsComponent->draw3DQueue();
        mPhysicsComponent->drawDebug();
        mGraphicsComponent->draw2DQueue();
        mWindowComponent->swapBuffers();

        mGraphicsComponent->postRender();
        // Update state
        mDone = mWindowComponent->shouldClose();
        return !mDone;
    }

    bool
    DreamEngine::updateCleanup
    ()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: UpdateCleanup Called @ " << mTime.getTimeDelta() << " ====" << endl;
        }
        // Cleanup Old
        mActiveScene.findDeletedSceneObjects();
        mActiveScene.findDeletedScripts();
        mActiveScene.clearDeleteQueue();
        // Chill
        return !mDone;
    }

    Time
    DreamEngine::getTime
    ()
    {
        return mTime;
    }

    bool
    DreamEngine::initComponents
    ()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Initialising Components..." << endl;
        }

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

        setComponentsInitialised(true);
        return true;
    }


    bool
    DreamEngine::initWindowComponent
    ()
    {
        mWindowComponent->setWidth(mProject.getWindowWidth());
        mWindowComponent->setHeight(mProject.getWindowHeight());
        mWindowComponent->setName(mProject.getName());

        if (!mWindowComponent->init())
        {
            cerr << "DreamEngine: Unable to initialise WindowComponent" << endl;
            return false;
        }

        return mWindowComponent != nullptr;
    }


    bool
    DreamEngine::initAudioComponent
    ()
    {
        mAudioComponent.make_shared();
        if (!mAudioComponent->init())
        {
            cerr << "Dream: Unable to initialise AudioComponent." << endl;
            return false;
        }
        return true;
    }


    bool
    DreamEngine::initPhysicsComponent
    ()
    {
        mPhysicsComponent.make_shared();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            cerr << "ComponentManager: Unable to initialise PhysicsComponent." << endl;
            return false;
        }
        return mPhysicsComponent != nullptr;
    }


    bool
    DreamEngine::initGraphicsComponent
    ()
    {
        mGraphicsComponent.make_shared(mCamera,mWindowComponent);
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
    DreamEngine::initAnimationComponent
    ()
    {
        mAnimationComponent.make_shared();
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
    DreamEngine::isDone
    ()
    {
        return mDone;
    }

    shared_ptr<AnimationComponent>
    DreamEngine::getAnimationComponent
    ()
    {
        return mAnimationComponent;
    }


    shared_ptr<AudioComponent>
    DreamEngine::getAudioComponent
    ()
    {
        return mAudioComponent;
    }


    shared_ptr<PhysicsComponent>
    DreamEngine::getPhysicsComponent
    ()
    {
        return mPhysicsComponent;
    }


    shared_ptr<GraphicsComponent>
    DreamEngine::getGraphicsComponent
    ()
    {
        return mGraphicsComponent;
    }


    Camera
    DreamEngine::getCamera
    ()
    {
        return mCamera;
    }


    GameController
    DreamEngine::getGameController
    ()
    {
        return mGameController;
    }


    void
    DreamEngine::setGameController
    (GameController& gc)
    {
        mGameController = gc;
    }


    void
    DreamEngine::setActiveScene
    (Scene& scene)
    {
        mActiveScene = scene;
    }


    bool
    DreamEngine::heartbeat
    ()
    {
        updateLogic();
        updateGraphics();
        updateCleanup();
        return mDone;
    }

    bool
    DreamEngine::initLuaEngine
    ()
    {
        mLuaEngine.make_shared(shared_ptr<DreamEngine>(this));
        return mLuaEngine->init();
    }

    bool DreamEngine::getComponentsInitialised()
    {
        return mComponentsInitialised;
    }

    void DreamEngine::setComponentsInitialised(bool initd)
    {
        mComponentsInitialised = initd;
    }

}
