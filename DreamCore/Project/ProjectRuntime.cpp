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
#include "../Components/Path/PathComponent.h"
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
#include "../Components/Graphics/Model/MaterialCache.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Font/FontCache.h"

using std::endl;

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (shared_ptr<Project> project, shared_ptr<IWindowComponent> windowComponent)
        : Runtime(project->getProjectDefinition()),
          ILoggable ("ProjectRuntime"),
          mDone(false),
          mParallel(false),
          mWindowComponent(windowComponent),
          mProject(project),
          mGraphicsUpdating(false),
          mLogicUpdating(false)

    {
        auto log = getLog();
        log->info( "Constructing" );
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        auto log = getLog();
        log->info( "Destructing" );
    }

    shared_ptr<IWindowComponent>
    ProjectRuntime::getWindowComponent
    ()
    {
        return mWindowComponent;
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



    shared_ptr<Time>
    ProjectRuntime::getTime
    ()
    {
        return mTime;
    }

    bool
    ProjectRuntime::initComponents
    ()
    {
        auto log = getLog();
        log->info( "Initialising Components..." );

        mTime = make_shared<Time>();

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

        if(!initPathComponent())
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
        if (!mWindowComponent->init())
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
        mAudioComponent = make_shared<AudioComponent>();
        if (!mAudioComponent->init())
        {
            log->error( "Unable to initialise AudioComponent." );
            return false;
        }
        mAudioComponent->setRunning(true);
        if (mParallel)
            mAudioComponentThread = make_shared<ComponentThread>(mAudioComponent);
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        auto log = getLog();
        mInputComponent = make_shared<InputComponent>();
        if (!mInputComponent->init())
        {
            log->error( "Unable to initialise InputComponent." );
            return false;
        }
        mInputComponent->setRunning(true);
        if (mParallel)
            mInputComponentThread = make_shared<ComponentThread>(mInputComponent);
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        auto log = getLog();
        mPhysicsComponent = make_shared<PhysicsComponent>();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            log->error( "Unable to initialise PhysicsComponent." );
            return false;
        }
        mPhysicsComponent->setRunning(true);
        if (mParallel)
            mPhysicsComponentThread = make_shared<ComponentThread>(mPhysicsComponent);
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        auto log = getLog();
        mCamera = make_shared<Camera>();
        mGraphicsComponent = make_shared<GraphicsComponent>(mCamera,mWindowComponent);
        mGraphicsComponent->setTime(mTime);

        if (!mGraphicsComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
            return false;
        }
        mGraphicsComponent->setRunning(true);

        if (mParallel)
            mGraphicsComponentThread = make_shared<ComponentThread>(mGraphicsComponent);

        mNanoVGComponent = make_shared<NanoVGComponent>(mWindowComponent);
        if (!mNanoVGComponent->init())
        {
            log->error( "Unable to initialise NanoVG Component.");
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initPathComponent
    ()
    {
        auto log = getLog();
        mPathComponent = make_shared<PathComponent>();
        mPathComponent->setTime(mTime);
        if (!mPathComponent->init())
        {
            log->error( "Unable to initialise Path Component." );
            return false;
        }
        mPathComponent->setRunning(true);

        if (mParallel)
            mPathComponentThread = make_shared<ComponentThread>(mPathComponent);
        return true;
    }

    bool
    ProjectRuntime::initLuaComponent
    ()
    {
        auto log = getLog();
        mLuaComponent = make_shared<LuaComponent>(dynamic_pointer_cast<ProjectRuntime>(shared_from_this()),mScriptCache);

        if(!mLuaComponent->init())
        {
            log->error( "Unable to initialise Lua Engine." );
            return false;
        }
        mLuaComponent->setRunning(true);

        if (mParallel)
            mLuaComponentThread = make_shared<ComponentThread>(mLuaComponent);

        if (mInputComponent != nullptr)
        {
            mInputComponent->setLuaComponent(mLuaComponent);
        }
        return true;
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        mFontCache = make_shared<FontCache>();
        mModelCache = make_shared<AssimpCache>();
        mShaderCache = make_shared<ShaderCache>();
        mTextureCache = make_shared<MaterialCache>();
        mScriptCache = make_shared<LuaScriptCache>();
        return true;
    }

    bool
    ProjectRuntime::isDone
    ()
    {
        return mDone;
    }

    shared_ptr<PathComponent>
    ProjectRuntime::getPathComponent
    ()
    {
        return mPathComponent;
    }

    shared_ptr<AudioComponent>
    ProjectRuntime::getAudioComponent
    ()
    {
        return mAudioComponent;
    }

    shared_ptr<PhysicsComponent>
    ProjectRuntime::getPhysicsComponent
    ()
    {
        return mPhysicsComponent;
    }

    shared_ptr<GraphicsComponent>
    ProjectRuntime::getGraphicsComponent
    ()
    {
        return mGraphicsComponent;
    }

    shared_ptr<NanoVGComponent>
    ProjectRuntime::getNanoVGComponent
    ()
    {
       return mNanoVGComponent;
    }

    shared_ptr<Camera>
    ProjectRuntime::getCamera
    ()
    {
        return mCamera;
    }

    shared_ptr<LuaComponent>
    ProjectRuntime::getLuaComponent
    ()
    {
        return mLuaComponent;
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

                mInputComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mInputComponent->setShouldUpdate(true);

                mLuaComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mLuaComponent->setShouldUpdate(true);

                mPathComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mPathComponent->setShouldUpdate(true);

                mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mAudioComponent->setShouldUpdate(true);

                mWindowComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mWindowComponent->updateComponent();

                mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mPhysicsComponent->setShouldUpdate(true);

                mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mGraphicsComponent->setShouldUpdate(true);
                return true;
            }
            else
            {
                mLogicUpdating = true;

                log->info("==== UpdateLogic (SingleThreaded) Called @ {}  ====",  mTime->now());

                mTime->updateFrameTime();

                mInputComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mInputComponent->setShouldUpdate(true);
                mInputComponent->updateComponent();

                mLuaComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mLuaComponent->setShouldUpdate(true);
                mLuaComponent->updateComponent();

                mPathComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mPathComponent->setShouldUpdate(true);
                mPathComponent->updateComponent();

                mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mAudioComponent->setShouldUpdate(true);
                mAudioComponent->updateComponent();

                mWindowComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mWindowComponent->updateComponent();
                mWindowComponent->updateComponent();

                mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
                mPhysicsComponent->setShouldUpdate(true);
                mPhysicsComponent->updateComponent();

                mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
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
        bool path = mPathComponent->getUpdateComplete();
        bool audio = mAudioComponent->getUpdateComplete();
        bool input = mInputComponent->getUpdateComplete();
        bool graphics = mGraphicsComponent->getUpdateComplete();
        bool physics = mPhysicsComponent->getUpdateComplete();
        bool lua = mLuaComponent->getUpdateComplete();
        log->info(
            "\n========================================\n"
            "Path..........[{}]\n"
            "Audio..............[{}]\n"
            "Input..............[{}]\n"
            "Graphics...........[{}]\n"
            "Physics............[{}]\n"
            "Lua................[{}]\n"
            "========================================",
            path ? "√" : " ",
            audio     ? "√" : " ",
            input     ? "√" : " ",
            graphics  ? "√" : " ",
            physics   ? "√" : " ",
            lua       ? "√" : " "
        );

        mLogicUpdating = !path || !audio || !input || !graphics || !physics || !lua;
        return path && audio && input && graphics && physics && lua;
    }

    void
    ProjectRuntime::updateGraphics
    ()
    {
        auto log = getLog();
        log->info(
            "\n====================\nReady to draw @ {}\n====================",
            getTime()->now()
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
        mWindowComponent->swapBuffers();
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

    shared_ptr<SceneRuntime>
    ProjectRuntime::getActiveSceneRuntime
    ()
    {
        return mActiveSceneRuntime;
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

    shared_ptr<SceneRuntime>
    ProjectRuntime::constructActiveSceneRuntime
    (shared_ptr<SceneDefinition> sceneDefinition)
    {
        auto log = getLog();
        if (sceneDefinition == nullptr)
        {
            log->error( "Cannot load SceneRuntime. SceneDefinition is nullptr!" );
            return nullptr;
        }

        // Load the new scene
        log->info( "Loading SceneRuntime" );

        mActiveSceneRuntime = make_shared<SceneRuntime>(sceneDefinition , dynamic_pointer_cast<ProjectRuntime>(shared_from_this()));
        mActiveSceneRuntime->useDefinition(sceneDefinition);

        if (mGraphicsComponent != nullptr)
        {
            mGraphicsComponent->setMeshCullDistance(sceneDefinition->getMeshCullDistance());
            mGraphicsComponent->setMinimumDraw(sceneDefinition->getMinDrawDistance());
            mGraphicsComponent->setMaximumDraw(sceneDefinition->getMaxDrawDistance());
        }
        else
        {
            log->error("Unable to set mesh cull distance, graphics component is still null");
        }

        return mActiveSceneRuntime;
    }

    shared_ptr<Project>
    ProjectRuntime::getProject
    ()
    {
        return mProject;
    }

    void
    ProjectRuntime::useDefinition
    (shared_ptr<IDefinition>)
    {
        initCaches();
        initComponents();
    }

    shared_ptr<FontCache>
    ProjectRuntime::getFontCache
    ()
    {
        return mFontCache;
    }

    shared_ptr<ShaderCache>
    ProjectRuntime::getShaderCache
    ()
    {
        return mShaderCache;
    }

    shared_ptr<MaterialCache>
    ProjectRuntime::getTextureCache
    ()
    {
        return mTextureCache;
    }

    shared_ptr<AssimpCache>
    ProjectRuntime::getModelCache
    ()
    {
        return mModelCache;
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
        cleanUpPathComponentThread();
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

    void ProjectRuntime::cleanUpPathComponentThread()
    {
        auto log = getLog();
        log->info("Cleaning up PhysicsComponentThread");
        mPathComponent->setRunning(false);
        mPathComponent->setShouldUpdate(false);
        if (mPathComponentThread->joinable())
        {
            mPathComponentThread->join();
        }
    }

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        mActiveSceneRuntime.reset();
    }
}
