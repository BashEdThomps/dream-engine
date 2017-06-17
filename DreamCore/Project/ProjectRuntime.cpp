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
#include "AssetDefinition.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneDefinition.h"
#include "../Scene/SceneObject/SceneObject.h"

#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Lua/LuaEngine.h"

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, IWindowComponent* windowComponentHandle)
        : Runtime(),
          mDone(false),
          mWindowComponentHandle(windowComponentHandle),
          mProjectHandle(project),
          mActiveSceneHandle(nullptr)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Creating new Instance" << endl;
        }
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Destroying Object" << endl;
        }
    }

    void
    ProjectRuntime::setProjectHandle
    (Project* proj)
    {
        mProjectHandle = proj;
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
    (Scene* sceneHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "ProjectRuntime: Cleaning up Components..." << endl;
        }

        if (mWindowComponentHandle)
        {
            mWindowComponentHandle->cleanUp(sceneHandle);
        }

        if(mGraphicsComponent)
        {
            mGraphicsComponent->cleanUp(sceneHandle);
        }

        if(mPhysicsComponent)
        {
            mPhysicsComponent->cleanUp(sceneHandle);
        }

        if(mAudioComponent)
        {
            mAudioComponent->cleanUp(sceneHandle);
        }

        if(mAnimationComponent)
        {
            mAnimationComponent->cleanUp(sceneHandle);
        }

        if (mLuaEngine)
        {
            mLuaEngine->cleanUp(sceneHandle);
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
        mLuaEngine.reset(new LuaEngine(mProjectHandle));
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

        mActiveSceneHandle->createAllAssetInstances();
        mActiveSceneHandle->loadAllAssetInstances();

        mTime->update();

        mLuaEngine->createAllScripts();
        mLuaEngine->update();

        mAnimationComponent->updateComponent(mActiveSceneHandle);
        mAudioComponent->updateComponent(mActiveSceneHandle);
        mWindowComponentHandle->updateComponent(mActiveSceneHandle);
        mPhysicsComponent->updateComponent(mActiveSceneHandle);

        // Update Graphics/Physics Components

        mGraphicsComponent->updateComponent(mActiveSceneHandle);
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
        mActiveSceneHandle->flush();
    }

    void
    ProjectRuntime::setActiveSceneHandle
    (Scene *sceneHandle)
    {
       mActiveSceneHandle = sceneHandle;
    }

    bool
    ProjectRuntime::hasActiveSceneHandle
    ()
    {
        return mActiveSceneHandle != nullptr;
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        if (mActiveSceneHandle)
        {
            updateLogic();
            updateGraphics();
            updateFlush();
        }
    }

    string
    ProjectRuntime::getProjectPath
    ()
    {
        return mProjectPath;
    }

    void
    ProjectRuntime::setProjectPath
    (string dir)
    {
        mProjectPath = dir;
    }

    bool
    ProjectRuntime::loadActiveScene
    ()
    {
        // Check valid
        if (!hasActiveSceneHandle())
        {
            cerr << "ProjectDefinition: Cannot load scene, null!" << endl;
            return false;
        }

        // Load the new scene
        if (Constants::DEBUG)
        {
            cout << "ProjectDefinition: Loading Scene " << mActiveSceneHandle->getDefinitionHandle()->getNameAndUuidString() << endl;
        }

        SceneDefinition* sceneDefinitionHandle = mActiveSceneHandle->getDefinitionHandle();
        mGraphicsComponent->setActiveScene(mActiveSceneHandle);
        mPhysicsComponent->setGravity(sceneDefinitionHandle->getGravity());
        mPhysicsComponent->setDebug(sceneDefinitionHandle->getPhysicsDebug());
        mCamera->setTranslation(sceneDefinitionHandle->getDefaultCameraTransform().getTranslation());
        mCamera->setRotation(sceneDefinitionHandle->getDefaultCameraTransform().getRotation());
        mCamera->setMovementSpeed(sceneDefinitionHandle->getCameraMovementSpeed());

        return true;
    }

    void
    ProjectRuntime::cleanUpActiveScene
    ()
    {
        mActiveSceneHandle->cleanUpRuntime();
        cleanupComponents(mActiveSceneHandle);
        mActiveSceneHandle = nullptr;
    }

}
