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

#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/NanoVGComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/IWindowComponent.h"

#include "../Components/Scripting/IScriptComponent.h"
#include "../Components/Scripting/Lua/LuaComponent.h"

#include "../Components/Graphics/Model/AssimpCache.h"
#include "../Components/Graphics/Model/MaterialCache.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Font/FontCache.h"

using std::endl;

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (
        Project* project,
        IWindowComponent* windowComponent)
        : IRuntime(project->getProjectDefinition()),
          mDone(false),
          mTime(nullptr),
          mCamera(nullptr),
          mActiveSceneRuntime(nullptr),
          mProject(project),
          mAudioComponent(nullptr),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mPhysicsComponent(nullptr),
          mPathComponent(nullptr),
          mScriptComponent(nullptr),
          mNanoVGComponent(nullptr),
          mWindowComponent(windowComponent),
          mTextureCache(nullptr),
          mModelCache(nullptr),
          mFontCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr)
    {
        setLogClassName("ProjectRuntime");
        auto log = getLog();
        log->info( "Constructing" );
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        auto log = getLog();
        log->info( "Destructing" );

        if (mActiveSceneRuntime != nullptr)
        {
            delete mActiveSceneRuntime;
            mActiveSceneRuntime = nullptr;
        }

        deleteComponents();

        if (mTime != nullptr)
        {
            delete mTime;
            mTime = nullptr;
        }

        if (mCamera != nullptr)
        {
            delete mCamera;
            mCamera = nullptr;
        }

        deleteCaches();
    }

    IWindowComponent*
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

    Time*
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

        mTime = new Time();

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

        if (!initScriptComponent())
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
        mAudioComponent = new AudioComponent();
        if (!mAudioComponent->init())
        {
            log->error( "Unable to initialise AudioComponent." );
            return false;
        }
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        auto log = getLog();
        auto projectDef = dynamic_cast<ProjectDefinition*>(mDefinition);
        mInputComponent =
        new InputComponent
        (
            projectDef->getCaptureKeyboard(),
            projectDef->getCaptureMouse(),
            projectDef->getCaptureJoystick()
        );

        if (!mInputComponent->init())
        {
            log->error( "Unable to initialise InputComponent." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        auto log = getLog();
        mPhysicsComponent = new PhysicsComponent();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            log->error( "Unable to initialise PhysicsComponent." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        auto log = getLog();
        mCamera = new Camera();
        mGraphicsComponent = new GraphicsComponent(mCamera,mWindowComponent);
        mGraphicsComponent->setTime(mTime);

        if (!mGraphicsComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
            return false;
        }

        mNanoVGComponent = new NanoVGComponent(mWindowComponent);
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
        mPathComponent = new PathComponent();
        mPathComponent->setTime(mTime);
        if (!mPathComponent->init())
        {
            log->error( "Unable to initialise Path Component." );
            return false;
        }

        return true;
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        auto log = getLog();
        mScriptComponent = dynamic_cast<IScriptComponent*>(
            new LuaComponent(
                dynamic_cast<ProjectRuntime*>(this),
                mScriptCache
            )
        );

        if(!mScriptComponent->init())
        {
            log->error( "Unable to initialise Script Engine." );
            return false;
        }

        if (mInputComponent != nullptr)
        {
            mScriptComponent->setInputMap(mInputComponent->getInputMap());
            log->info("Passed InputMap to ScriptComponent");
        }
        else
        {
            log->error("Cannot pass InputMap to ScriptComponent, nullptr");
        }
        return true;
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        mFontCache    = new FontCache();
        mModelCache   = new AssimpCache();
        mShaderCache  = new ShaderCache();
        mTextureCache = new MaterialCache();
        mScriptCache  = new ScriptCache();
        return true;
    }

    void ProjectRuntime::deleteCaches()
    {
        if (mFontCache != nullptr)
        {
            delete mFontCache;
            mFontCache = nullptr;
        }

        if (mModelCache != nullptr)
        {
            delete mModelCache;
            mModelCache = nullptr;
        }

        if (mShaderCache != nullptr)
        {
            delete mShaderCache;
            mShaderCache = nullptr;
        }

        if (mTextureCache != nullptr)
        {
            delete mTextureCache;
            mTextureCache = nullptr;
        }

        if (mScriptCache != nullptr)
        {
            delete mScriptCache;
            mScriptCache = nullptr;
        }

    }

    void ProjectRuntime::deleteComponents()
    {
        if (mAudioComponent != nullptr)
        {
            delete mAudioComponent;
            mAudioComponent = nullptr;
        }
        if (mInputComponent != nullptr)
        {
            delete mInputComponent;
            mInputComponent = nullptr;
        }
        if (mGraphicsComponent != nullptr)
        {
            delete mGraphicsComponent;
            mGraphicsComponent = nullptr;
        }
        if (mPhysicsComponent != nullptr)
        {
            delete mPhysicsComponent;
            mPhysicsComponent = nullptr;
        }
        if (mPathComponent != nullptr)
        {
            delete mPathComponent;
            mPathComponent = nullptr;
        }
        if (mScriptComponent != nullptr)
        {
            delete mScriptComponent;
            mScriptComponent = nullptr;
        }
        if (mNanoVGComponent != nullptr)
        {
            delete mNanoVGComponent;
            mNanoVGComponent = nullptr;
        }
    }

    bool
    ProjectRuntime::isDone
    ()
    {
        return mDone;
    }

    PathComponent*
    ProjectRuntime::getPathComponent
    ()
    {
        return mPathComponent;
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
    ProjectRuntime::getGraphicsComponent
    ()
    {
        return mGraphicsComponent;
    }

    NanoVGComponent*
    ProjectRuntime::getNanoVGComponent
    ()
    {
       return mNanoVGComponent;
    }

    Camera*
    ProjectRuntime::getCamera
    ()
    {
        return mCamera;
    }

    IScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    {
        return mScriptComponent;
    }

    bool
    ProjectRuntime::updateLogic
    ()
    {
        auto log = getLog();

        log->info("UpdateLogic Called @ {}",  mTime->now());

        mTime->updateFrameTime();

        mInputComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mInputComponent->updateComponent();

        mScriptComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mScriptComponent->updateComponent();

        mPathComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mPathComponent->updateComponent();

        mAudioComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mAudioComponent->updateComponent();

        mWindowComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mWindowComponent->updateComponent();

        mPhysicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mPhysicsComponent->updateComponent();

        mGraphicsComponent->setActiveSceneRuntime(mActiveSceneRuntime);
        mGraphicsComponent->updateComponent();

        return true;
    }

    void
    ProjectRuntime::updateGraphics
    ()
    {
        auto log = getLog();
        log->info(
            "\n====================\nReady to draw @ {}\n====================",
            mTime->now()
        );

        log->info("UpdateGraphics Called @" , mTime->getFrameTimeDelta());
        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->handleResize();
        mGraphicsComponent->drawModelQueue();
        mGraphicsComponent->drawFontQueue();
        mGraphicsComponent->drawSpriteQueue();
        // mScriptComponent->updateNanoVG();
        mPhysicsComponent->setViewProjectionMatrix(
            mGraphicsComponent->getViewMatrix(), mGraphicsComponent->getProjectionMatrix()
        );
        mPhysicsComponent->drawDebug();
        mWindowComponent->swapBuffers();
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        auto log = getLog();
        log->info("CollectGarbage Called @ {}", mTime->getFrameTimeDelta());
        // Cleanup Old
        mActiveSceneRuntime->collectGarbage();
    }

    bool
    ProjectRuntime::hasActiveSceneRuntime
    ()
    {
        return mActiveSceneRuntime != nullptr;
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
    {
        return mActiveSceneRuntime;
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        if (mActiveSceneRuntime != nullptr)
        {
            updateLogic();
            updateGraphics();
            collectGarbage();
        }
    }

    SceneRuntime*
    ProjectRuntime::constructActiveSceneRuntime
    (SceneDefinition* sceneDefinition)
    {
        auto log = getLog();
        if (sceneDefinition == nullptr)
        {
            log->error( "Cannot load SceneRuntime. SceneDefinition is nullptr!" );
            return nullptr;
        }


        // Load the new scene
        log->info( "Loading SceneRuntime" );

        mActiveSceneRuntime = new SceneRuntime(
            sceneDefinition,
            this
        );

        mActiveSceneRuntime->useDefinition();

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

    Project*
    ProjectRuntime::getProject
    ()
    {
        return mProject;
    }

    void
    ProjectRuntime::useDefinition
    ()
    {
        initCaches();
        initComponents();
    }

    FontCache*
    ProjectRuntime::getFontCache
    ()
    {
        return mFontCache;
    }

    ShaderCache*
    ProjectRuntime::getShaderCache
    ()
    {
        return mShaderCache;
    }

    MaterialCache*
    ProjectRuntime::getTextureCache
    ()
    {
        return mTextureCache;
    }

    AssimpCache*
    ProjectRuntime::getModelCache
    ()
    {
        return mModelCache;
    }

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        delete mActiveSceneRuntime;
    }
}
