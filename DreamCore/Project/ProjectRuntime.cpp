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
    (shared_ptr<Project> project, shared_ptr<IWindowComponent> windowComponent)
        : IRuntime(project->getProjectDefinition()),
          mDone(false),
          mProject(project),
          mWindowComponent(windowComponent)
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
    }

    weak_ptr<IWindowComponent>
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

    weak_ptr<Time>
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
        mAudioComponent = make_shared<AudioComponent>();
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
        auto projectDef = dynamic_pointer_cast<ProjectDefinition>(mDefinition);
        mInputComponent = make_shared<InputComponent>
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
        mPhysicsComponent = make_shared<PhysicsComponent>();
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
        mCamera = make_shared<Camera>();
        mGraphicsComponent = make_shared<GraphicsComponent>(mCamera,mWindowComponent);
        mGraphicsComponent->setTime(mTime);

        if (!mGraphicsComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
            return false;
        }

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

        return true;
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        auto log = getLog();
        mScriptComponent = dynamic_pointer_cast<IScriptComponent>(
            make_shared<LuaComponent>(
                dynamic_pointer_cast<ProjectRuntime>(shared_from_this()),
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
        mScriptCache = make_shared<ScriptCache>();
        return true;
    }

    bool
    ProjectRuntime::isDone
    ()
    {
        return mDone;
    }

    weak_ptr<PathComponent>
    ProjectRuntime::getPathComponent
    ()
    {
        return mPathComponent;
    }

    weak_ptr<AudioComponent>
    ProjectRuntime::getAudioComponent
    ()
    {
        return mAudioComponent;
    }

    weak_ptr<PhysicsComponent>
    ProjectRuntime::getPhysicsComponent
    ()
    {
        return mPhysicsComponent;
    }

    weak_ptr<GraphicsComponent>
    ProjectRuntime::getGraphicsComponent
    ()
    {
        return mGraphicsComponent;
    }

    weak_ptr<NanoVGComponent>
    ProjectRuntime::getNanoVGComponent
    ()
    {
       return mNanoVGComponent;
    }

    weak_ptr<Camera>
    ProjectRuntime::getCamera
    ()
    {
        return mCamera;
    }

    weak_ptr<IScriptComponent>
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

    weak_ptr<SceneRuntime>
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

    weak_ptr<SceneRuntime>
    ProjectRuntime::constructActiveSceneRuntime
    (shared_ptr<SceneDefinition> sceneDefinition)
    {
        auto log = getLog();
        if (sceneDefinition == nullptr)
        {
            log->error( "Cannot load SceneRuntime. SceneDefinition is nullptr!" );
            return weak_ptr<SceneRuntime>();
        }


        // Load the new scene
        log->info( "Loading SceneRuntime" );

        mActiveSceneRuntime = make_shared<SceneRuntime>(
            sceneDefinition ,
            dynamic_pointer_cast<ProjectRuntime>(shared_from_this())
        );

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

    weak_ptr<Project>
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

    weak_ptr<FontCache>
    ProjectRuntime::getFontCache
    ()
    {
        return mFontCache;
    }

    weak_ptr<ShaderCache>
    ProjectRuntime::getShaderCache
    ()
    {
        return mShaderCache;
    }

    weak_ptr<MaterialCache>
    ProjectRuntime::getTextureCache
    ()
    {
        return mTextureCache;
    }

    weak_ptr<AssimpCache>
    ProjectRuntime::getModelCache
    ()
    {
        return mModelCache;
    }

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        mActiveSceneRuntime.reset();
    }

}
