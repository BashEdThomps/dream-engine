/*
 * ProjectRuntime.cpp
 *
 * Created: 05 2017 by Ashley
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
#include "ProjectRuntime.h"
#include "Project.h"

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, IWindowComponent* windowComponent)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Creating new Instance" << endl;
        }
        setProject(project);
        setWindowComponent(windowComponent);
        setAnimationComponent(nullptr);
        setAudioComponent(nullptr);
        setPhysicsComponent(nullptr);
        setGraphicsComponent(nullptr);
        setCamera(nullptr);
        setTime(nullptr);
        setDone(false);
        setGameController(new GameController());
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Destroying Object" << endl;
        }
        destroyComponents();
        if (mGameController)
        {
            delete mGameController;
        }
    }

    void
    ProjectRuntime::setProject
    (Project* proj)
    {
        mProject = proj;
    }

    void
    ProjectRuntime::setWindowComponent
    (IWindowComponent* wc)
    {
        mWindowComponent = wc;
    }

    IWindowComponent*
    ProjectRuntime::getWindowComponent
    ()
    {
        return mWindowComponent;
    }

    void
    ProjectRuntime::setCamera
    (Camera* camera)
    {
        mCamera = camera;
    }

    void
    ProjectRuntime::setAudioComponent
    (AudioComponent* audioComp)
    {
        mAudioComponent = audioComp;
    }

    void
    ProjectRuntime::setAnimationComponent
    (AnimationComponent* animComp)
    {
        mAnimationComponent = animComp;
    }

    void
    ProjectRuntime::setPhysicsComponent
    (PhysicsComponent* physicsComp)
    {
        mPhysicsComponent = physicsComp;
    }

    void
    ProjectRuntime::setGraphicsComponent
    (GraphicsComponent* graphicsComp)
    {
        mGraphicsComponent = graphicsComp;
    }

    void
    ProjectRuntime::setDone
    (bool done)
    {
        mDone = done;
    }

    void
    ProjectRuntime::setTime
    (Time* time)
    {
        mTime = time;
    }

    void
    ProjectRuntime::cleanupComponents
    (Scene* scene)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Cleaning up Components..." << endl;
        }

        if (mWindowComponent)
        {
            mWindowComponent->cleanUp(scene);
        }

        if(mGraphicsComponent)
        {
            mGraphicsComponent->cleanUp(scene);
        }

        if(mPhysicsComponent)
        {
            mPhysicsComponent->cleanUp(scene);
        }

        if(mAudioComponent)
        {
            mAudioComponent->cleanUp(scene);
        }

        if(mAnimationComponent)
        {
            mAnimationComponent->cleanUp(scene);
        }

        if (mLuaEngine)
        {
            mLuaEngine->cleanUp(scene);
        }

        if (Constants::DEBUG)
        {
            cout << "Dream: Finished Cleaning Up Components." << endl;
        }
    }

    Time*
    ProjectRuntime::getTime
    ()
    {
        return mTime;
    }

    void
    ProjectRuntime::destroyComponents
    ()
    {
        if (mTime)
        {
            delete mTime;
            mTime = nullptr;
        }
        if (mAnimationComponent)
        {
            delete mAnimationComponent;
            mAnimationComponent = nullptr;
        }

        if (mAudioComponent)
        {
            delete mAudioComponent;
            mAudioComponent = nullptr;
        }

        if (mPhysicsComponent)
        {
            delete mPhysicsComponent;
            mPhysicsComponent = nullptr;
        }

        if (mGraphicsComponent)
        {
            delete mGraphicsComponent;
            mGraphicsComponent = nullptr;
        }
        if (mWindowComponent)
        {
            delete mWindowComponent;
            mWindowComponent = nullptr;
        }
    }


    bool
    ProjectRuntime::initComponents
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Creating Components..." << endl;
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

        if (Constants::DEBUG)
        {
            cout << "Dream: Successfuly created Components." << endl;
        }

        return true;
    }


    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        //mWindowComponent->setWidth(mProject->getWindowWidth());
        //mWindowComponent->setHeight(mProject->getWindowHeight());
        //mWindowComponent->setName(mProject->getName());

        if (!mWindowComponent->init())
        {
            cerr << "ProjectRuntime: Unable to initialise WindowComponent" << endl;
            return false;
        }

        return mWindowComponent != nullptr;
    }


    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        mAudioComponent = new AudioComponent();
        if (!mAudioComponent->init())
        {
            cerr << "Dream: Unable to initialise AudioComponent." << endl;
            return false;
        }
        return mAudioComponent != nullptr;
    }


    bool
    ProjectRuntime::initPhysicsComponent
    ()
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
    ProjectRuntime::initGraphicsComponent
    ()
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
            cerr << "ProjectRuntime: Unable to initialise Graphics Component." << endl;
            return false;
        }
    }


    bool
    ProjectRuntime::initAnimationComponent
    ()
    {
        mAnimationComponent = new AnimationComponent();
        mAnimationComponent->setTime(mTime);
        if (mAnimationComponent->init())
        {
            return true;
        }
        else
        {
            cerr << "ProjectRuntime: Unable to initialise Animation Component." << endl;
            return false;
        }
    }


    bool
    ProjectRuntime::isDone
    ()
    {
        return mDone;
    }

    AnimationComponent*
    ProjectRuntime::getAnimationComponent
    ()
    {
        return mAnimationComponent;
    }


    AudioComponent*
    ProjectRuntime::getAudioComponent
    ()
    {
        return mAudioComponent;
    }


    PhysicsComponent*
    ProjectRuntime::getPhysicsComponent
    ()
    {
        return mPhysicsComponent;
    }


    GraphicsComponent*
    ProjectRuntime::
    getGraphicsComponent()
    {
        return mGraphicsComponent;
    }


    Camera*
    ProjectRuntime::getCamera
    ()
    {
        return mCamera;
    }


    GameController*
    ProjectRuntime::getGameController
    ()
    {
        return mGameController;
    }


    void
    ProjectRuntime::setGameController
    (GameController *gc)
    {
        mGameController = gc;
    }

    bool
    ProjectRuntime::initLuaEngine
    ()
    {
        mLuaEngine = new LuaEngine(mProject);
        return mLuaEngine->init();
    }

    void
    ProjectRuntime::setGraphicsClearColour
    (vector<float> colour)
    {
       mGraphicsComponent->setClearColour(colour);
    }

    void
    ProjectRuntime::setGraphicsAmbientLightColour
    (vector<float> colour)
    {
        mGraphicsComponent->setAmbientLightColour(colour);
    }

    void
    ProjectRuntime::setPhysicsGravity
    (vector<float> gravity)
    {
        mPhysicsComponent->setGravity(gravity);
    }

    void
    ProjectRuntime::setPhysicsDebug
    (bool debug)
    {
        mPhysicsComponent->setDebug(debug);
    }

    void
    ProjectRuntime::setCameraTranslation
    (glm::vec3 translation)
    {
        mCamera->setTranslation(translation);
    }

    void
    ProjectRuntime::setCameraRotation
    (glm::vec3 rotation)
    {
        mCamera->setRotation(rotation);
    }

    void
    ProjectRuntime::setCameraMovementSpeed
    (float moveSpeed)
    {
        mCamera->setMovementSpeed(moveSpeed);
    }

    LuaEngine*
    ProjectRuntime::getLuaEngine
    ()
    {
        return mLuaEngine;
    }
}
