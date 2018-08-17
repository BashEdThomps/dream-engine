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
#include "../Components/Input/InputComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/ComponentThread.h"

#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/NanoVGComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Components/Lua/LuaComponent.h"

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
          mParallel(false),
          mWindowComponentHandle(windowComponentHandle),
          mProjectHandle(projectHandle),
          mGraphicsUpdating(false),
          mLogicUpdating(false)
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

    bool ProjectRuntime::getParallel() const
    {
        return mParallel;
    }

    void ProjectRuntime::setParallel(bool parallel)
    {
        mParallel = parallel;
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

        if (!initInputComponent())
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

        if (!initLuaComponent())
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
        if (mParallel)
            mAudioComponentThread.reset(new ComponentThread(mAudioComponent.get()));
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        auto log = getLog();
        mInputComponent.reset(new InputComponent());
        if (!mInputComponent->init())
        {
            log->error( "Unable to initialise InputComponent." );
            return false;
        }
        mInputComponent->setRunning(true);
        if (mParallel)
            mInputComponentThread.reset(new ComponentThread(mInputComponent.get()));
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
        if (mParallel)
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

        if (mParallel)
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

        if (mParallel)
            mAnimationComponentThread.reset(new ComponentThread(mAnimationComponent.get()));
        return true;
    }

    bool
    ProjectRuntime::initLuaComponent
    ()
    {
        auto log = getLog();
        mLuaComponent.reset(new LuaComponent(this,mScriptCache.get()));

        if(!mLuaComponent->init())
        {
            log->error( "Unable to initialise Lua Engine." );
            return false;
        }
        mLuaComponent->setRunning(true);

        if (mParallel)
            mLuaComponentThread.reset(new ComponentThread(mLuaComponent.get()));

        if (mInputComponent != nullptr)
        {
            mInputComponent->setLuaComponentHandle(mLuaComponent.get());
        }
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

    LuaComponent*
    ProjectRuntime::getLuaComponentHandle
    ()
    {
        return mLuaComponent.get();
    }

    bool
    ProjectRuntime::updateLogic
    ()
    {
        auto log = getLog();

        if (!mGraphicsUpdating && !mLogicUpdating)
        {
            if (mParallel)
            {

                mLogicUpdating = true;

                log->info("==== UpdateLogic (Parallel) Called @ {}  ====",  mTime->now());

                mTime->updateFrameTime();

                mInputComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mInputComponent->setShouldUpdate(true);

                mLuaComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mLuaComponent->setShouldUpdate(true);

                mAnimationComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mAnimationComponent->setShouldUpdate(true);

                mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mAudioComponent->setShouldUpdate(true);

                mWindowComponentHandle->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mWindowComponentHandle->updateComponent();

                mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mPhysicsComponent->setShouldUpdate(true);

                mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mGraphicsComponent->setShouldUpdate(true);
                return true;
            }
            else
            {
                mLogicUpdating = true;

                log->info("==== UpdateLogic (SingleThreaded) Called @ {}  ====",  mTime->now());

                mTime->updateFrameTime();

                mInputComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mInputComponent->setShouldUpdate(true);
                mInputComponent->updateComponent();

                mLuaComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mLuaComponent->setShouldUpdate(true);
                mLuaComponent->updateComponent();

                mAnimationComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mAnimationComponent->setShouldUpdate(true);
                mAnimationComponent->updateComponent();

                mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mAudioComponent->setShouldUpdate(true);
                mAudioComponent->updateComponent();

                mWindowComponentHandle->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mWindowComponentHandle->updateComponent();
                mWindowComponentHandle->updateComponent();

                mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mPhysicsComponent->setShouldUpdate(true);
                mPhysicsComponent->updateComponent();

                mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime.get());
                mGraphicsComponent->setShouldUpdate(true);
                mGraphicsComponent->updateComponent();

                mLogicUpdating = false;

                return true;
            }
        }

         return false;
    }

    bool ProjectRuntime::allThreadsHaveUpdated
    ()
    {
        if (!mParallel)
        {
            return false;
        }

        auto log = getLog();
        bool animation = mAnimationComponent->getUpdateComplete();
        bool audio = mAudioComponent->getUpdateComplete();
        bool input = mInputComponent->getUpdateComplete();
        bool graphics = mGraphicsComponent->getUpdateComplete();
        bool physics = mPhysicsComponent->getUpdateComplete();
        bool lua = mLuaComponent->getUpdateComplete();
        log->info(
            "\n========================================\n"
            "Animation..........[{}]\n"
            "Audio..............[{}]\n"
            "Input..............[{}]\n"
            "Graphics...........[{}]\n"
            "Physics............[{}]\n"
            "Lua................[{}]\n"
            "========================================",
            animation ? "√" : " ",
            audio     ? "√" : " ",
            input     ? "√" : " ",
            graphics  ? "√" : " ",
            physics   ? "√" : " ",
            lua       ? "√" : " "
        );

        mLogicUpdating = !animation || !audio || !input || !graphics || !physics || !lua;
        return animation && audio && input && graphics && physics && lua;
    }

    void
    ProjectRuntime::updateGraphics
    ()
    {
        auto log = getLog();
        log->info(
            "\n====================\nReady to draw @ {}\n====================",
            getTimeHandle()->now()
        );

        log->info("==== UpdateGraphics Called @ {}  ====" , mTime->getFrameTimeDelta());
        mGraphicsUpdating = true;
        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->handleResize();
        mGraphicsComponent->drawModelQueue();
        mGraphicsComponent->drawFontQueue();
        mGraphicsComponent->drawSpriteQueue();
        // mLuaComponent->updateNanoVG();
        mPhysicsComponent->setViewProjectionMatrix(
            mGraphicsComponent->getViewMatrix(), mGraphicsComponent->getProjectionMatrix()
        );
        mPhysicsComponent->drawDebug();
        mWindowComponentHandle->swapBuffers();
        mGraphicsUpdating = false;
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
    (IDefinition*) {}

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

    bool ProjectRuntime::getGraphicsUpdating() const
    {
        return mGraphicsUpdating;
    }

    bool ProjectRuntime::getLogicUpdating() const
    {
        return mGraphicsUpdating;
    }

    void ProjectRuntime::cleanUpThreads()
    {
        auto log = getLog();
        log->info("Cleaning up ComponentThreads");
        cleanUpLuaComponentThread();
        cleanUpPhysicsComponentThread();
        cleanUpAnimationComponentThread();
        cleanUpInputComponentThread();
        cleanUpAudioComponentThread();
        cleanUpGraphicsComponentThread();
    }

    void ProjectRuntime::cleanUpLuaComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up LuaComponentThread");
        mLuaComponent->setRunning(false);
        mLuaComponent->setShouldUpdate(false);
        if (mLuaComponentThread->joinable())
        {
            mLuaComponentThread->join();
        }
    }

    void ProjectRuntime::cleanUpInputComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up InputComponentThread");
        mInputComponent->setRunning(false);
        mInputComponent->setShouldUpdate(false);
        if (mInputComponentThread->joinable())
        {
            mInputComponentThread->join();
        }
    }

    void ProjectRuntime::cleanUpAudioComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up AudioComponentThread");
        mAudioComponent->setRunning(false);
        mAudioComponent->setShouldUpdate(false);
        if (mAudioComponentThread->joinable())
        {
            mAudioComponentThread->join();
        }
    }

    void ProjectRuntime::cleanUpPhysicsComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mPhysicsComponent->setRunning(false);
        mPhysicsComponent->setShouldUpdate(false);
        if (mPhysicsComponentThread->joinable())
        {
            mPhysicsComponentThread->join();
        }
    }

    void ProjectRuntime::cleanUpGraphicsComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mGraphicsComponent->setRunning(false);
        mGraphicsComponent->setShouldUpdate(false);
        if (mGraphicsComponentThread->joinable())
        {
            mGraphicsComponentThread->join();
        }
    }

    void ProjectRuntime::cleanUpAnimationComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mAnimationComponent->setRunning(false);
        mAnimationComponent->setShouldUpdate(false);
        if (mAnimationComponentThread->joinable())
        {
            mAnimationComponentThread->join();
        }
    }

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        mActiveSceneRuntime.reset(nullptr);
    }
}
