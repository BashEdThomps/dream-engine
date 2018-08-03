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

#include "../Components/Graphics/GraphicsComponent.h"
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
        log->info( "ProjectRuntime: Constructing" );
        initCaches();
        initComponents();
        useDefinition(mDefinitionHandle);
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        auto log = getLog();
        log->info( "ProjectRuntime: Destructing" );
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
        log->info( "ProjectRuntime: Initialising Components..." );

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
        log->info( "Dream: Successfuly created Components." );

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        auto log = getLog();
        if (!mWindowComponentHandle->init())
        {
            log->error( "ProjectRuntime: Unable to initialise WindowComponent" );
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
            log->error( "Dream: Unable to initialise AudioComponent." );
            return false;
        }
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
            log->error( "ComponentManager: Unable to initialise PhysicsComponent." );
            return false;
        }
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
            log->error( "ProjectRuntime: Unable to initialise Graphics Component." );
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
            log->error( "ProjectRuntime: Unable to initialise Animation Component." );
            return false;
        }
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
            log->error( "ProjectRuntime: Unable to initialise Lua Engine." );
            return false;
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
        log->info("==== ProjectDefinition: UpdateLogic Called @ {}  ====",  mTime->getTimeDelta());

        mTime->update();

        //mLuaEngine->createAllScripts();
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
        auto log = getLog();
        log->info("==== ProjectDefinition: UpdateGraphics Called @ {}  ====" , mTime->getTimeDelta());
        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->drawModelQueue();
        mGraphicsComponent->drawFontQueue();
        mGraphicsComponent->drawSpriteQueue();
        mGraphicsComponent->drawNanoVG();
        mPhysicsComponent->drawDebug();
        mWindowComponentHandle->swapBuffers();
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        auto log = getLog();
        log->info("==== ProjectDefinition: CollectGarbage Called @ {}  ====", mTime->getTimeDelta());
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
            log->error( "ProjectRuntime: Cannot load SceneRuntime. SceneDefinitoin is nullptr!" );
            return nullptr;
        }

        // Load the new scene
        {
            log->info( "ProjectRuntime: Loading SceneRuntime" );
        }

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

    void
    ProjectRuntime::resetActiveSceneRuntime
    ()
    {
        mActiveSceneRuntime.reset(nullptr);
    }
}
