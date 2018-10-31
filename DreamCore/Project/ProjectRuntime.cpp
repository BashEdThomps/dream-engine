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

#include "../Components/Graphics/Model/ModelCache.h"
#include "../Components/Graphics/Material/MaterialCache.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Graphics/Texture/TextureCache.h"

using std::endl;

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (
            Project* project,
            IWindowComponent* windowComponent)
        : IRuntime(
              project->getProjectDefinition(),
              project->getProjectDefinition()->getName(),
              project->getProjectDefinition()->getUuid()
          ),
          mDone(false),
          mTime(nullptr),
          mCamera(nullptr),
          mProject(project),
          mAudioComponent(nullptr),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mNanoVGComponent(nullptr),
          mPhysicsComponent(nullptr),
          mPathComponent(nullptr),
          mScriptComponent(nullptr),
          mWindowComponent(windowComponent),
          mTextureCache(nullptr),
          mMaterialCache(nullptr),
          mModelCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr),
          mScriptingEnabled(true)
    {
        setLogClassName("ProjectRuntime");
        auto log = getLog();
        log->debug( "Constructing" );
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        auto log = getLog();
        log->debug( "Destructing" );

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
        log->debug( "Initialising Components..." );

        mTime = new Time();

        if (!initWindowComponent())
        {
            return false;
        }

        if(!initGraphicsComponent())
        {
            return false;
        }

        if(!initNanoVGComponent())
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
        log->debug( "Successfuly created Components." );

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        auto log = getLog();
        if (!mWindowComponent)
        {
            log->critical("Window component is null");
            return false;
        }
        auto projDef = dynamic_cast<ProjectDefinition*>(mDefinition);

        mWindowComponent->setWidth(projDef->getWindowWidth());
        mWindowComponent->setHeight(projDef->getWindowHeight());
        mWindowComponent->setName(projDef->getName());

        /*if (!mWindowComponent->init())
        {
            log->error( "Unable to initialise WindowComponent" );
            return false;
        }
        */
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
        mGraphicsComponent->setShaderCache(mShaderCache);
        if (!mGraphicsComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
            return false;
        }

        return true;
    }

    bool
    ProjectRuntime::initNanoVGComponent
    ()
    {
        auto log = getLog();
        mNanoVGComponent = new NanoVGComponent(mWindowComponent);
        mNanoVGComponent->setTime(mTime);
        if (!mNanoVGComponent->init())
        {
            log->error( "Unable to initialise Graphics Component." );
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
            //mScriptComponent->setInputMap(mInputComponent->getInputMap());
            log->debug("Passed InputMap to ScriptComponent");
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
        getLog()->trace("initialising caches");
        mTextureCache = new TextureCache(this);
        mShaderCache  = new ShaderCache(this);
        mMaterialCache = new MaterialCache(this,mShaderCache,mTextureCache);
        mModelCache   = new ModelCache(this,mShaderCache, mMaterialCache);
        mScriptCache  = new ScriptCache();
        return true;
    }

    void ProjectRuntime::deleteCaches()
    {
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

        if (mMaterialCache != nullptr)
        {
            delete mMaterialCache;
            mMaterialCache = nullptr;
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
        if (mNanoVGComponent != nullptr)
        {
            delete mNanoVGComponent;
            mNanoVGComponent = nullptr;
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
    (SceneRuntime* sr)
    {
        auto log = getLog();

        log->debug(
            "\n"
            "====================\n"
            "Update Logic called @ {}\n"
            "====================",
             mTime->nowLL()
        );

        mTime->updateFrameTime();

        mInputComponent->updateComponent(sr);

        if (mScriptingEnabled)
        {
            mScriptComponent->updateComponent(sr);
        }

        mCamera->updateCameraVectors();
        mPathComponent->updateComponent(sr);
        mAudioComponent->updateComponent(sr);
        mWindowComponent->updateComponent(sr);
        mPhysicsComponent->updateComponent(sr);
        mGraphicsComponent->updateComponent(sr);
        return true;
    }

    void
    ProjectRuntime::updateGraphics
    (SceneRuntime* sr)
    {
        auto log = getLog();
        log->debug(
            "\n"
            "====================\n"
            "Update Graphics called @ {}\n"
            "====================",
            mTime->nowLL()
        );
        // Draw 3D/PhysicsDebug/2D
        mGraphicsComponent->handleResize();
        mGraphicsComponent->renderGeometryPass(sr);
        mGraphicsComponent->renderLightingPass();
        mScriptComponent->updateNanoVG();
        ShaderInstance::InvalidateState();
        mPhysicsComponent->setViewProjectionMatrix
        (
            mGraphicsComponent->getViewMatrix(),
            mGraphicsComponent->getProjectionMatrix()
        );
        mPhysicsComponent->drawDebug();
    }

    int
    ProjectRuntime::getWindowWidth
    ()
    {
       if (mWindowComponent != nullptr)
       {
           return mWindowComponent->getWidth();
       }
       return 0;
    }

    void
    ProjectRuntime::setWindowWidth
    (int w)
    {
       if (mWindowComponent != nullptr)
       {
           mWindowComponent->setWidth(w);
       }
    }

    int
    ProjectRuntime::getWindowHeight
    ()
    {
       if (mWindowComponent != nullptr)
       {
           return mWindowComponent->getHeight();
       }
       return 0;
    }

    void
    ProjectRuntime::setWindowHeight
    (int h)
    {
       if (mWindowComponent != nullptr)
       {
            mWindowComponent->setHeight(h);
       }
    }

    void
    ProjectRuntime::collectGarbage
    (SceneRuntime* rt)
    {
        auto log = getLog();
        log->debug("CollectGarbage Called @ {}", mTime->nowLL());
        rt->collectGarbage();
    }

    void ProjectRuntime::collectGarbage(){}

    void
    ProjectRuntime::updateAll
    (SceneRuntime* rt)
    {
        if (rt != nullptr)
        {
            updateLogic(rt);
            updateGraphics(rt);
            collectGarbage(rt);
        }
    }

    SceneRuntime*
    ProjectRuntime::constructSceneRuntime
    (SceneDefinition* sceneDefinition)
    {
        auto log = getLog();
        SceneRuntime* rt = nullptr;
        if (sceneDefinition == nullptr)
        {
            log->error( "Cannot load SceneRuntime. SceneDefinition is nullptr!" );
            return nullptr;
        }

        // Load the new scene
        log->debug( "Loading SceneRuntime" );

        rt = new SceneRuntime(sceneDefinition,this);

        if (!rt->useDefinition())
        {
            delete rt;
            return nullptr;
        }

        mTextureCache->flushRawTextureImageData();

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

        return rt;
    }

    Project*
    ProjectRuntime::getProject
    ()
    {
        return mProject;
    }

    bool
    ProjectRuntime::useDefinition
    ()
    {
        if (!initCaches()) return false;
        if (!initComponents()) return false;
        return true;
    }

    ShaderCache*
    ProjectRuntime::getShaderCache
    ()
    {
        return mShaderCache;
    }

    MaterialCache*
    ProjectRuntime::getMaterialCache
    ()
    {
        return mMaterialCache;
    }

    ModelCache*
    ProjectRuntime::getModelCache
    ()
    {
        return mModelCache;
    }

    TextureCache*
    ProjectRuntime::getTextureCache
    ()
    {
       return mTextureCache;
    }

    void
    ProjectRuntime::destructSceneRuntime
    (SceneRuntime* rt, bool clearCaches)
    {
        if (clearCaches)
        {
            clearAllCaches();
        }
        delete rt;
    }

    void
    ProjectRuntime::clearAllCaches
    ()
    {
        if (mModelCache    != nullptr) mModelCache->clear();
        if (mShaderCache   != nullptr) mShaderCache->clear();
        if (mMaterialCache != nullptr) mMaterialCache->clear();
        if (mTextureCache  != nullptr) mTextureCache->clear();
        // TODO if (mScriptCache   != nullptr) mScriptCache->clear();
    }

    bool ProjectRuntime::getScriptingEnabled() const
    {
        return mScriptingEnabled;
    }

    void ProjectRuntime::setScriptingEnabled(bool en)
    {
        mScriptingEnabled = en;
    }

    IAssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (string uuid)
    {
        if (mProject != nullptr)
        {
            return mProject->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    string
    ProjectRuntime::getAssetAbsolutePath
    (string uuid)
    {
        auto def = getAssetDefinitionByUuid(uuid);
        return mProject->getProjectPath()+def->getAssetPath();
    }

    SceneObjectRuntime*
    ProjectRuntime::getSceneObjectRuntimeByUuid
    (SceneRuntime* rt, string uuid)
    {
        if (rt == nullptr)
        {
            return nullptr;
        }

        return rt->getSceneObjectRuntimeByUuid(uuid);
    }

    string
    ProjectRuntime::getProjectPath
    ()
    {
        return mProject->getProjectPath();
    }

    SceneRuntime* ProjectRuntime::CurrentSceneRuntime = nullptr;
}
