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
#include "ProjectDefinition.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"

#include "../Components/AssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Lua/LuaEngine.h"

using std::endl;

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* projectHandle,IWindowComponent* windowComponentHandle)
        : Runtime(),
          mDone(false),
          mWindowComponentHandle(windowComponentHandle),
          mProjectHandle(projectHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Constructing" << endl;
        }
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Destructing" << endl;
        }
    }

    IWindowComponent*
    ProjectRuntime::getWindowComponentHandle
    ()
    {
        return mWindowComponentHandle;
    }

    void
    ProjectRuntime::setDone
    (bool done)
    {
        mDone = done;
    }

    void
    ProjectRuntime::cleanupComponents
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Cleaning up Components..." << endl;
        }

        if (mWindowComponentHandle)
        {
            mWindowComponentHandle->cleanUp(mActiveSceneRuntime.get());
        }

        if(mGraphicsComponent)
        {
            mGraphicsComponent->cleanUp(mActiveSceneRuntime.get());
        }

        if(mPhysicsComponent)
        {
            mPhysicsComponent->cleanUp(mActiveSceneRuntime.get());
        }

        if(mAudioComponent)
        {
            mAudioComponent->cleanUp(mActiveSceneRuntime.get());
        }

        if(mAnimationComponent)
        {
            mAnimationComponent->cleanUp(mActiveSceneRuntime.get());
        }

        if (mLuaEngine)
        {
            mLuaEngine->cleanUp(mActiveSceneRuntime.get());
        }

        if (Constants::VERBOSE)
        {
            cout << "Dream: Finished Cleaning Up Components." << endl;
        }
    }

    Time*
    ProjectRuntime::getTimeHandle
    ()
    {
        return mTime.get();
    }

    bool
    ProjectRuntime::initComponents
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Initialising Components..." << endl;
        }

        mTime.reset(new Time());

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

        if (Constants::VERBOSE)
        {
            cout << "Dream: Successfuly created Components." << endl;
        }

        return true;
    }


    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        if (!mWindowComponentHandle->init())
        {
            cerr << "ProjectRuntime: Unable to initialise WindowComponent" << endl;
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        mAudioComponent.reset(new AudioComponent());
        if (!mAudioComponent->init())
        {
            cerr << "Dream: Unable to initialise AudioComponent." << endl;
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        mPhysicsComponent.reset(new PhysicsComponent());
        mPhysicsComponent->setTime(mTime.get());
        if (!mPhysicsComponent->init())
        {
            cerr << "ComponentManager: Unable to initialise PhysicsComponent." << endl;
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        mCamera.reset(new Camera());
        mGraphicsComponent.reset(new GraphicsComponent(mCamera.get(),mWindowComponentHandle));
        mGraphicsComponent->setTime(mTime.get());

        if (!mGraphicsComponent->init())
        {
            cerr << "ProjectRuntime: Unable to initialise Graphics Component." << endl;
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initAnimationComponent
    ()
    {
        mAnimationComponent.reset(new AnimationComponent());
        mAnimationComponent->setTime(mTime.get());
        if (!mAnimationComponent->init())
        {
            cerr << "ProjectRuntime: Unable to initialise Animation Component." << endl;
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initLuaEngine
    ()
    {
        mLuaEngine.reset(new LuaEngine(this));

        if(!mLuaEngine->init())
        {
            cerr << "ProjectRuntime: Unable to initialise Lua Engine." << endl;
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::isDone
    ()
    {
        return mDone;
    }

    AnimationComponent*
    ProjectRuntime::getAnimationComponentHandle
    ()
    {
        return mAnimationComponent.get();
    }


    AudioComponent*
    ProjectRuntime::getAudioComponentHandle
    ()
    {
        return mAudioComponent.get();
    }


    PhysicsComponent*
    ProjectRuntime::getPhysicsComponentHandle
    ()
    {
        return mPhysicsComponent.get();
    }


    GraphicsComponent*
    ProjectRuntime::getGraphicsComponentHandle
    ()
    {
        return mGraphicsComponent.get();
    }


    Camera*
    ProjectRuntime::getCameraHandle
    ()
    {
        return mCamera.get();
    }

    LuaEngine*
    ProjectRuntime::getLuaEngineHandle
    ()
    {
        return mLuaEngine.get();
    }

    void
    ProjectRuntime::updateLogic
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== ProjectDefinition: UpdateLogic Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }

        mActiveSceneRuntime.get()->createAllAssetInstances();
        mActiveSceneRuntime.get()->loadAllAssetInstances();

        mTime->update();

        mLuaEngine->createAllScripts();
        mLuaEngine->update();

        mAnimationComponent->updateComponent(mActiveSceneRuntime.get());
        mAudioComponent->updateComponent(mActiveSceneRuntime.get());
        mWindowComponentHandle->updateComponent(mActiveSceneRuntime.get());
        mPhysicsComponent->updateComponent(mActiveSceneRuntime.get());

        // Update Graphics/Physics Components

        mGraphicsComponent->updateComponent(mActiveSceneRuntime.get());
        mPhysicsComponent->setViewProjectionMatrix(
            mGraphicsComponent->getViewMatrix(),
            mGraphicsComponent->getProjectionMatrix()
        );
    }

    void
    ProjectRuntime::updateGraphics
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== ProjectDefinition: UpdateGraphics Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }

        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->preRender();
        mGraphicsComponent->draw3DQueue();
        mPhysicsComponent->drawDebug();
        mGraphicsComponent->draw2DQueue();
        mWindowComponentHandle->swapBuffers();
        mGraphicsComponent->postRender();

    }

    void
    ProjectRuntime::updateFlush
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "==== ProjectDefinition: updateFlush Called @ " << mTime->getTimeDelta() << " ====" << endl;
        }
        // Cleanup Old
        mActiveSceneRuntime.get()->flush();
    }

    bool
    ProjectRuntime::hasActiveSceneRuntime
    ()
    {
        return mActiveSceneRuntime.get() != nullptr;
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        if (mActiveSceneRuntime.get())
        {
            updateLogic();
            updateGraphics();
            updateFlush();
        }
    }

    bool
    ProjectRuntime::loadActiveSceneRuntime
    ()
    {
        // Check valid
        if (!hasActiveSceneRuntime())
        {
            cerr << "ProjectDefinition: Cannot load scene, null!" << endl;
            return false;
        }

        // Load the new scene
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Loading SceneRuntime "
                 << mActiveSceneRuntime.get()->getNameAndUuidString()
                 << endl;
        }

        // TODO Fix Me
        /*
        SceneDefinition* sceneDefinitionHandle = nullptr;getSceneDefinitionHandleByUuid(mActiveSceneRuntime->getUuid());

        mGraphicsComponent->setActiveSceneHandle(mActiveSceneRuntime.get());
        mPhysicsComponent->setGravity(sceneDefinitionHandle->getGravity());
        mPhysicsComponent->setDebug(sceneDefinitionHandle->getPhysicsDebug());
        mCamera->setTranslation(sceneDefinitionHandle->getDefaultCameraTransform().getTranslation());
        mCamera->setRotation(sceneDefinitionHandle->getDefaultCameraTransform().getRotation());
        mCamera->setMovementSpeed(sceneDefinitionHandle->getCameraMovementSpeed());
        */

        return true;
    }

    void
    ProjectRuntime::cleanUpActiveSceneRuntime
    ()
    {
       /*
        mActiveSceneRuntime.get()->cleanUpRuntime();
        cleanupComponents(mActiveSceneRuntime.get());
        mActiveSceneRuntime.get() = nullptr;
        */
    }

    Project*
    ProjectRuntime::getProjectHandle
    ()
    {
        return mProjectHandle;
    }
}
