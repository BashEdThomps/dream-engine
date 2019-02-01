/*
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
#include "ProjectDirectory.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneDefinition.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"

#include "../Components/AssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Audio/AudioCache.h"
#include "../Components/Input/InputComponent.h"

#include "../Components/Logic/LogicComponent.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/NanoVGComponent.h"
#include "../Components/Graphics/Model/ModelMesh.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Window/WindowComponent.h"
#include "../Components/Scripting/ScriptComponent.h"

#include "../Components/Graphics/Model/ModelCache.h"
#include "../Components/Graphics/Material/MaterialCache.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Shader/ShaderRuntime.h"
#include "../Components/Graphics/Texture/TextureCache.h"

#include <thread>

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, WindowComponent* windowComponent)
        : Runtime(project->getDefinition()),
          mDone(false),
          mTime(nullptr),
          mProject(project),
          mAudioComponent(nullptr),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mNanoVGComponent(nullptr),
          mPhysicsComponent(nullptr),
          mScriptComponent(nullptr),
          mLogicComponent(nullptr),
          mWindowComponent(windowComponent),
          mAudioCache(nullptr),
          mTextureCache(nullptr),
          mMaterialCache(nullptr),
          mModelCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("ProjectRuntime");
        getLog()->debug( "Constructing" );
        #endif
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Destructing" );
        #endif
        deleteCaches();
        deleteComponents();

        if (mTime != nullptr)
        {
            delete mTime;
            mTime = nullptr;
        }

    }

    WindowComponent*
    ProjectRuntime::getWindowComponent
    ()
    const
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
    const
    {
        return mTime;
    }

    bool
    ProjectRuntime::initComponents
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Initialising Components..." );
        #endif

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

        if (!initScriptComponent())
        {
            return false;
        }

        if (!initLogicComponent())
        {
            return false;
        }

        #ifdef DREAM_LOG
        getLog()->debug( "Successfuly created Components." );
        #endif

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        if (!mWindowComponent)
        {
            #ifdef DREAM_LOG
            getLog()->critical("Window component is null");
            #endif
            return false;
        }
        auto projDef = dynamic_cast<ProjectDefinition*>(mDefinition);
        projDef->lock();
        mWindowComponent->setName(projDef->getName());
        projDef->unlock();
        return true;
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        mAudioComponent = new AudioComponent();
        mAudioComponent->lock();
        if (!mAudioComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise AudioComponent." );
            #endif
            mAudioComponent->unlock();
            return false;
        }
        mAudioComponent->unlock();
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        auto projectDef = dynamic_cast<ProjectDefinition*>(mDefinition);
        projectDef->lock();
        mInputComponent = new InputComponent(
            projectDef->getCaptureKeyboard(),
            projectDef->getCaptureMouse(),
            projectDef->getCaptureJoystick()
        );
        projectDef->unlock();
        mInputComponent->lock();

        if (!mInputComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise InputComponent." );
            #endif
            mInputComponent->unlock();
            return false;
        }
        mInputComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        mPhysicsComponent = new PhysicsComponent();
        mPhysicsComponent->lock();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise PhysicsComponent." );
            #endif
            mPhysicsComponent->unlock();
            return false;
        }
        mPhysicsComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        mGraphicsComponent = new GraphicsComponent(mWindowComponent);
        mGraphicsComponent->lock();
        mGraphicsComponent->setTime(mTime);
        mGraphicsComponent->setShaderCache(mShaderCache);
        if (!mGraphicsComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Graphics Component." );
            #endif
            mGraphicsComponent->unlock();
            return false;
        }
        mGraphicsComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initNanoVGComponent
    ()
    {
        mNanoVGComponent = new NanoVGComponent(mWindowComponent);
        mNanoVGComponent->lock();
        mNanoVGComponent->setTime(mTime);
        if (!mNanoVGComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Graphics Component." );
            #endif
            mNanoVGComponent->unlock();
            return false;
        }
        mNanoVGComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        mScriptComponent = new ScriptComponent(this,mScriptCache);
        mScriptComponent->lock();
        if(!mScriptComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Script Engine." );
            #endif
            mScriptComponent->unlock();
            return false;
        }
        mScriptComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initLogicComponent
    ()
    {
        mLogicComponent = new LogicComponent();
        mLogicComponent->lock();
        mLogicComponent->setTime(mTime);
        if (!mLogicComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to init lifetime component");
            #endif
            mLogicComponent->unlock();
            return false;
        }
        mLogicComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("initialising caches");
        #endif
        mAudioCache = new AudioCache(this);
        mTextureCache = new TextureCache(this);
        mShaderCache  = new ShaderCache(this);
        mMaterialCache = new MaterialCache(this,mShaderCache,mTextureCache);
        mModelCache   = new ModelCache(this,mShaderCache, mMaterialCache);
        mScriptCache  = new ScriptCache(this);
        return true;
    }

    void
    ProjectRuntime::deleteCaches
    ()
    {
        if (mAudioCache != nullptr)
        {
            delete mAudioCache;
            mAudioCache = nullptr;
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

    void
    ProjectRuntime::deleteComponents
    ()
    {
        if (mLogicComponent != nullptr)
        {
            delete mLogicComponent;
            mLogicComponent = nullptr;
        }

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

        if (mScriptComponent != nullptr)
        {
            delete mScriptComponent;
            mScriptComponent = nullptr;
        }
    }

    bool
    ProjectRuntime::isDone
    ()
    const
    {
        return mDone;
    }

    AudioComponent*
    ProjectRuntime::getAudioComponent
    ()
    const
    {
        return mAudioComponent;
    }

    PhysicsComponent*
    ProjectRuntime::getPhysicsComponent
    ()
    const
    {
        return mPhysicsComponent;
    }

    GraphicsComponent*
    ProjectRuntime::getGraphicsComponent
    ()
    const
    {
        return mGraphicsComponent;
    }

    NanoVGComponent*
    ProjectRuntime::getNanoVGComponent
    ()
    const
    {
        return mNanoVGComponent;
    }

    ScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    const
    {
        return mScriptComponent;
    }

    LogicComponent*
    ProjectRuntime::getLogicComponent
    ()
    const
    {
        return mLogicComponent;
    }

    bool
    ProjectRuntime::updateLogic
    (SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->debug("\n"
        "=======================================================================\n"
        "Update Logic called @ {}\n"
        "=======================================================================\n",
        mTime->getAbsoluteTime());
        #endif

        // Get Inputs
        mTime->updateFrameTime();
        sr->createSceneObjectUpdateQueues();

        mInputComponent->updateComponent(sr);

        // Do Processing
        mLogicComponent->updateComponent(sr);
        mPhysicsComponent->setCamera(sr->getCamera());
        mPhysicsComponent->updateComponent(sr);
        mScriptComponent->updateComponent(sr);
        mAudioComponent->updateComponent(sr);

        // Produce Outputs
        sr->getCamera()->update();
        mGraphicsComponent->updateComponent(sr);
        return true;
    }

    void
    ProjectRuntime::updateGraphics
    (SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->debug("\n"
        "=======================================================================\n"
        "Update Graphics called @ {}\n"
        "=======================================================================\n",
        mTime->getAbsoluteTime());
        #endif
        // Draw 3D/PhysicsDebug/2D
        ModelMesh::ClearCounters();
        mGraphicsComponent->executeTaskQueue();
        mGraphicsComponent->handleResize();
        mGraphicsComponent->renderGeometryPass(sr);
        mGraphicsComponent->renderShadowPass(sr);
        mGraphicsComponent->renderLightingPass(sr);
        mNanoVGComponent->render(sr);
        ShaderRuntime::InvalidateState();
        mPhysicsComponent->drawDebug();
    }

    int
    ProjectRuntime::getWindowWidth
    ()
    const
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
    const
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
        #ifdef DREAM_LOG
        getLog()->debug("\n"
        "=======================================================================\n"
        "CollectGarbage Called @ {}\n"
        "=======================================================================",
        mTime->getAbsoluteTime());
        #endif
        rt->collectGarbage();
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        for (auto rt : mSceneRuntimesToRemove)
        {
            auto itr = find(mSceneRuntimeVector.begin(),mSceneRuntimeVector.end(),rt);
            if (itr != mSceneRuntimeVector.end())
            {
                mSceneRuntimeVector.erase(itr);
            }
        }
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        for (auto rt : mSceneRuntimeVector)
        {
            rt->lock();
            switch (rt->getState())
            {
                case SceneState::SCENE_STATE_TO_LOAD:
                    constructSceneRuntime(rt);
                    break;
                case SceneState::SCENE_STATE_LOADED:
                    break;
                case SceneState::SCENE_STATE_ACTIVE:
                    updateLogic(rt);
                    updateGraphics(rt);
                    mWindowComponent->updateComponent(rt);
                    collectGarbage(rt);
                    break;
                case SceneState::SCENE_STATE_TO_DESTROY:
                    destructSceneRuntime(rt);
                    break;
                case SceneState::SCENE_STATE_DESTROYED:
                    mSceneRuntimesToRemove.push_back(rt);
                    break;
            }
            rt->unlock();
        }

        collectGarbage();
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
    const
    {
        int nRuntimes = mSceneRuntimeVector.size();
        for (int i=0;i<nRuntimes;i++)
        {
            if (mSceneRuntimeVector.at(i)->getState() == SceneState::SCENE_STATE_ACTIVE)
            {
                return mSceneRuntimeVector.at(i);
            }
        }
        return nullptr;
    }

    SceneRuntime*
    ProjectRuntime::getSceneRuntimeByUuid
    (uint32_t uuid)
    const
    {
        for (auto* sr : mSceneRuntimeVector)
        {
            sr->lock();
            if (sr->getUuid() == uuid)
            {
                sr->unlock();
                return sr;
            }
            sr->unlock();
        }
        return nullptr;
    }

    void
    ProjectRuntime::setSceneRuntimeAsActive
    (uint32_t uuid)
    {
        int nRuntimes = mSceneRuntimeVector.size();
        for (int i=0;i<nRuntimes;i++)
        {
            auto srt = mSceneRuntimeVector.at(i);
            srt->lock();
            if (srt->getUuid() == uuid)
            {
                srt->setState(SceneState::SCENE_STATE_ACTIVE);
            }
            else
            {
                if (srt->getState() == SceneState::SCENE_STATE_ACTIVE)
                {
                    srt->setState(SceneState::SCENE_STATE_LOADED);
                }
            }
            srt->unlock();
        }
    }

    vector<SceneRuntime*>
    ProjectRuntime::getSceneRuntimeVector
    ()
    const
    {
        return mSceneRuntimeVector;
    }

    void
    ProjectRuntime::addSceneRuntime
    (SceneRuntime* rt)
    {
        auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
        if (itr == mSceneRuntimeVector.end())
        {
            mSceneRuntimeVector.push_back(rt);
        }
    }

    void
    ProjectRuntime::removeSceneRuntime
    (SceneRuntime* rt)
    {
        auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
        if (itr != mSceneRuntimeVector.end())
        {
            mSceneRuntimeVector.erase(itr);
        }
    }

    bool
    ProjectRuntime::constructSceneRuntime
    (SceneRuntime* rt)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Constructing Scene Runtime");
        #endif
        return rt->useDefinition();
    }

    Project*
    ProjectRuntime::getProject
    ()
    const
    {
        return mProject;
    }

    InputComponent*
    ProjectRuntime::getInputComponent
    ()
    const
    {
        return mInputComponent;
    }

    bool
    ProjectRuntime::useDefinition
    ()
    {
        if (!initCaches())
        {
            return false;
        }

        if (!initComponents())
        {
            return false;
        }

        return true;
    }

    AudioCache*
    ProjectRuntime::getAudioCache
    ()
    const
    {
        return mAudioCache;
    }

    ShaderCache*
    ProjectRuntime::getShaderCache
    ()
    const
    {
        return mShaderCache;
    }

    MaterialCache*
    ProjectRuntime::getMaterialCache
    ()
    const
    {
        return mMaterialCache;
    }

    ModelCache*
    ProjectRuntime::getModelCache
    ()
    const
    {
        return mModelCache;
    }

    TextureCache*
    ProjectRuntime::getTextureCache
    ()
    const
    {
        return mTextureCache;
    }

    ScriptCache*
    ProjectRuntime::getScriptCache
    ()
    const
    {
        return mScriptCache;
    }

    void
    ProjectRuntime::destructSceneRuntime
    (SceneRuntime* rt, bool clearCaches)
    {
        rt->destroyRuntime();
        if (clearCaches)
        {
            clearAllCaches();
        }
    }

    void
    ProjectRuntime::clearAllCaches
    ()
    {
        if (mAudioCache != nullptr)
        {
            mAudioCache->lock();
            mAudioCache->clear();
            mAudioCache->unlock();
        }

        if (mModelCache != nullptr)
        {
            mModelCache->lock();
            mModelCache->clear();
            mModelCache->unlock();
        }

        if (mShaderCache != nullptr)
        {
            mShaderCache->lock();
            mShaderCache->clear();
            mShaderCache->unlock();
        }

        if (mMaterialCache != nullptr)
        {
            mMaterialCache->lock();
            mMaterialCache->clear();
            mMaterialCache->unlock();
        }

        if (mTextureCache != nullptr)
        {
            mTextureCache->lock();
            mTextureCache->clear();
            mTextureCache->unlock();
        }

        if (mScriptCache != nullptr)
        {
            mScriptCache->lock();
            mScriptCache->clear();
            mScriptCache->unlock();
        }
    }

    AssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (uint32_t uuid)
    const
    {
        if (mProject != nullptr)
        {
            return mProject->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    SceneObjectRuntime*
    ProjectRuntime::getSceneObjectRuntimeByUuid
    (SceneRuntime* rt, uint32_t uuid)
    const
    {
        if (rt == nullptr)
        {
            return nullptr;
        }

        return rt->getSceneObjectRuntimeByUuid(uuid);
    }

    bool
    ProjectRuntime::hasActiveScene
    ()
    const
    {
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getState() == SceneState::SCENE_STATE_ACTIVE) {
                srt->unlock();
                return true;
            }
            srt->unlock();
        }
        return false;
    }

    bool
    ProjectRuntime::hasLoadedScenes
    ()
    const
    {
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getState() >= SceneState::SCENE_STATE_LOADED &&
                srt->getState() < SceneState::SCENE_STATE_DESTROYED)
            {
                srt->unlock();
                return true;
            }
            srt->unlock();
        }
        return false;
    }


    bool
    ProjectRuntime::hasSceneRuntime
    (uint32_t uuid)
    const
    {
        bool result = false;
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getUuid() == uuid)
            {
                result = true;
            }
            srt->unlock();

            if (result) break;
        }
        return result;
    }
}
