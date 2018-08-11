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
#include "../Scene/SceneDefinition.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/ComponentThread.h"

#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/NanoVGComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Lua/LuaEngine.h"

#include "../Components/Graphics/Model/AssimpCache.h"
#include "../Components/Graphics/Model/TextureCache.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Font/FontCache.h"

using std::endl;

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* projectHandle,IWindowComponent* windowComponentHandle)
        : Runtime(projectHandle->getProjectDefinitionHandle()),
          ILoggable ("ProjectRuntime"),
          mDone(false),
          mWindowComponentHandle(windowComponentHandle),
          mProjectHandle(projectHandle)
    {
        auto log = getLog();
        log->info( "Constructing" );
        initCaches();
        initComponents();
        useDefinition(mDefinitionHandle);
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        auto log = getLog();
        log->info( "Destructing" );
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
        auto log = getLog();
        log->info( "Initialising Components..." );

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
        log->info( "Successfuly created Components." );

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        auto log = getLog();
        if (!mWindowComponentHandle->init())
        {
            log->error( "Unable to initialise WindowComponent" );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        auto log = getLog();
        mAudioComponent.reset(new AudioComponent());
        if (!mAudioComponent->init())
        {
            log->error( "Unable to initialise AudioComponent." );
            return false;
        }
        mAudioComponent->setRunning(true);
        mAudioComponentThread.reset(new ComponentThread(mAudioComponent.get()));
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        auto log = getLog();
        mPhysicsComponent.reset(new PhysicsComponent());
        mPhysicsComponent->setTime(mTime.get());
        if (!mPhysicsComponent->init())
        {
            log->error( "Unable to initialise PhysicsComponent." );
            return false;
        }
        mPhysicsComponent->setRunning(true);
        mPhysicsComponentThread.reset(new ComponentThread(mPhysicsComponent.get()));
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        auto log = getLog();
        mCamera.reset(new Camera());
        mGraphicsComponent.reset(new GraphicsComponent(mCamera.get(),mWindowComponentHandle));
        mGraphicsComponent->setTime(mTime.get());

        if (!mGraphicsComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
            return false;
        }
        mGraphicsComponent->setRunning(true);
        mGraphicsComponentThread.reset(new ComponentThread(mGraphicsComponent.get()));

        mNanoVGComponent.reset(new NanoVGComponent(mWindowComponentHandle));
        if (!mNanoVGComponent->init())
        {
            log->error( "Unable to initialise NanoVG Component.");
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initAnimationComponent
    ()
    {
        auto log = getLog();
        mAnimationComponent.reset(new AnimationComponent());
        mAnimationComponent->setTime(mTime.get());
        if (!mAnimationComponent->init())
        {
            log->error( "Unable to initialise Animation Component." );
            return false;
        }
        mAnimationComponent->setRunning(true);
        mAnimationComponentThread.reset(new ComponentThread(mAnimationComponent.get()));
        return true;
    }

    bool
    ProjectRuntime::initLuaEngine
    ()
    {
        auto log = getLog();
        mLuaEngine.reset(new LuaEngine(this,mScriptCache.get()));

        if(!mLuaEngine->init())
        {
            log->error( "Unable to initialise Lua Engine." );
            return false;
        }
        mLuaEngine->setRunning(true);
        mLuaEngineThread.reset(new ComponentThread(mLuaEngine.get()));
        return true;
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        mFontCache.reset(new FontCache());
        mModelCache.reset(new AssimpCache());
        mShaderCache.reset(new ShaderCache());
        mTextureCache.reset(new TextureCache());
        mScriptCache.reset(new LuaScriptCache());
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

    NanoVGComponent*
    ProjectRuntime::getNanoVGComponentHandle
    ()
    {
       return mNanoVGComponent.get();
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
        auto log = getLog();
        log->info("==== UpdateLogic Called @ {}  ====",  mTime->getFrameTimeDelta());

        mTime->updateFrameTime();

        //mLuaEngine->updateComponent();
        mLuaEngine->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mLuaEngine->setShouldUpdate(true);

        //mAnimationComponent->updateComponent(mActiveSceneRuntime.get());
        mAnimationComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mAnimationComponent->setShouldUpdate(true);

        //mAudioComponent->updateComponent(mActiveSceneRuntime.get());
        mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mAudioComponent->setShouldUpdate(true);

        mWindowComponentHandle->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mWindowComponentHandle->updateComponent();

        //mPhysicsComponent->updateComponent(mActiveSceneRuntime.get());
        mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mPhysicsComponent->setShouldUpdate(true);

        //mGraphicsComponent->updateComponent(mActiveSceneRuntime.get());
        mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
        mGraphicsComponent->setShouldUpdate(true);

        mPhysicsComponent->setViewProjectionMatrix(
            mGraphicsComponent->getViewMatrix(), mGraphicsComponent->getProjectionMatrix()
        );
    }

    bool ProjectRuntime::allThreadsHaveUpdated
    ()
    {
        auto log = getLog();
        bool animation = mAnimationComponent->getUpdateComplete();
        bool audio = mAudioComponent->getUpdateComplete();
        bool graphics = mGraphicsComponent->getUpdateComplete();
        bool physics = mPhysicsComponent->getUpdateComplete();
        bool lua = mLuaEngine->getUpdateComplete();
        log->info(
            "\n========================================\n"
            "Animation..........[{}]\n"
            "Audio..............[{}]\n"
            "Graphics...........[{}]\n"
            "Physics............[{}]\n"
            "Lua................[{}]\n"
            "========================================",
            animation ? "√" : " ",
            audio     ? "√" : " ",
            graphics  ? "√" : " ",
            physics   ? "√" : " ",
            lua       ? "√" : " "
        );

       return animation && audio && graphics && physics && lua;
    }

    void
    ProjectRuntime::updateGraphics
    ()
    {
        auto log = getLog();
        log->info("==== UpdateGraphics Called @ {}  ====" , mTime->getFrameTimeDelta());

        while (!allThreadsHaveUpdated())
        {
            log->info("UpdateGraphics Waiting... {}" , mTime->getFrameTimeDelta());
            std::this_thread::yield();
        }
        log->info("==== UpdateGraphics Ready to draw @ {}  ====" , mTime->now());

        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->handleResize();
        mGraphicsComponent->drawModelQueue();
        mGraphicsComponent->drawFontQueue();
        mGraphicsComponent->drawSpriteQueue();
        mLuaEngine->updateNanoVG();
        mPhysicsComponent->drawDebug();
        mWindowComponentHandle->swapBuffers();
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        auto log = getLog();
        log->info("==== CollectGarbage Called @ {}  ====", mTime->getFrameTimeDelta());
        // Cleanup Old
        mActiveSceneRuntime.get()->collectGarbage();
    }

    bool
    ProjectRuntime::hasActiveSceneRuntime
    ()
    {
        return mActiveSceneRuntime.get() != nullptr;
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntimeHandle
    ()
    {
        return mActiveSceneRuntime.get();
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        if (mActiveSceneRuntime)
        {
            updateLogic();
            updateGraphics();
            collectGarbage();
        }
    }

    SceneRuntime*
    ProjectRuntime::constructActiveSceneRuntime
    (SceneDefinition* sceneDefinitionHandle)
    {
        auto log = getLog();
        if (!sceneDefinitionHandle)
        {
            log->error( "Cannot load SceneRuntime. SceneDefinition is nullptr!" );
            return nullptr;
        }

        // Load the new scene
        log->info( "Loading SceneRuntime" );

        mActiveSceneRuntime.reset(new SceneRuntime(sceneDefinitionHandle , this));

        return mActiveSceneRuntime.get();
    }

    Project*
    ProjectRuntime::getProjectHandle
    ()
    {
        return mProjectHandle;
    }

    void
    ProjectRuntime::useDefinition
    (IDefinition*)
    {
        /*
            ProjectDefinition *pdHandle = dynamic_cast<ProjectDefinition*>(definitionHandle);
        */
    }

    FontCache*
    ProjectRuntime::getFontCacheHandle
    ()
    {
        return mFontCache.get();
    }

    ShaderCache*
    ProjectRuntime::getShaderCacheHandle
    ()
    {
        return mShaderCache.get();
    }

    TextureCache*
    ProjectRuntime::getTextureCacheHandle
    ()
    {
        return mTextureCache.get();
    }

    AssimpCache*
    ProjectRuntime::getModelCacheHandle
    ()
    {
        return mModelCache.get();
    }

    void ProjectRuntime::cleanUpThreads()
    {
        auto log = getLog();
        log->info("Cleaning up ComponentThreads");
        cleanUpLuaEngineThread();
        cleanUpPhysicsComponentThread();
        cleanUpAnimationComponentThread();
        cleanUpAudioComponentThread();
        cleanUpGraphicsComponentThread();
    }

    void ProjectRuntime::cleanUpLuaEngineThread()
    {
        auto log = getLog();
        log->info("Cleaning up LuaEngineThread");
        mLuaEngine->setRunning(false);
        mLuaEngine->setShouldUpdate(false);
        mLuaEngineThread->join();
    }

    void ProjectRuntime::cleanUpAudioComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up AudioComponentThread");
        mAudioComponent->setRunning(false);
        mAudioComponent->setShouldUpdate(false);
        mAudioComponentThread->join();
    }

    void ProjectRuntime::cleanUpPhysicsComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mPhysicsComponent->setRunning(false);
        mPhysicsComponent->setShouldUpdate(false);
        mPhysicsComponentThread->join();
    }

    void ProjectRuntime::cleanUpGraphicsComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mGraphicsComponent->setRunning(false);
        mGraphicsComponent->setShouldUpdate(false);
        mGraphicsComponentThread->join();
    }

    void ProjectRuntime::cleanUpAnimationComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mAnimationComponent->setRunning(false);
        mAnimationComponent->setShouldUpdate(false);
        mAnimationComponentThread->join();
    }

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        mActiveSceneRuntime.reset(nullptr);
    }
}
