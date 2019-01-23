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
#include "../Components/Path/PathComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Audio/AudioCache.h"
#include "../Components/Input/InputComponent.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Scroller/ScrollerComponent.h"

#include "../Components/Lifetime/LifetimeComponent.h"
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

namespace Dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, WindowComponent* windowComponent)
        : Runtime(project->getDefinition()),
          mDone(false),
          mTime(nullptr),
          mProject(project),
          mAnimationComponent(nullptr),
          mAudioComponent(nullptr),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mNanoVGComponent(nullptr),
          mPhysicsComponent(nullptr),
          mPathComponent(nullptr),
          mScriptComponent(nullptr),
          mLifetimeComponent(nullptr),
          mWindowComponent(windowComponent),
          mAudioCache(nullptr),
          mTextureCache(nullptr),
          mMaterialCache(nullptr),
          mModelCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr),
          mScriptingEnabled(true)
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

        if(!initPathComponent())
        {
            return false;
        }

        if(!initAnimationComponent())
        {
            return false;
        }

        if (!initScrollerComponent())
        {
            return false;
        }

        if (!initScriptComponent())
        {
            return false;
        }

        if (!initLifetimeComponent())
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
        mWindowComponent->setName(projDef->getName());
        return true;
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        mAudioComponent = new AudioComponent();
        if (!mAudioComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise AudioComponent." );
            #endif
            return false;
        }
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
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
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise InputComponent." );
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        mPhysicsComponent = new PhysicsComponent();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise PhysicsComponent." );
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        mGraphicsComponent = new GraphicsComponent(mWindowComponent);
        mGraphicsComponent->setTime(mTime);
        mGraphicsComponent->setShaderCache(mShaderCache);
        if (!mGraphicsComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Graphics Component." );
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initNanoVGComponent
    ()
    {
        mNanoVGComponent = new NanoVGComponent(mWindowComponent);
        mNanoVGComponent->setTime(mTime);
        if (!mNanoVGComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Graphics Component." );
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initPathComponent
    ()
    {
        mPathComponent = new PathComponent();
        mPathComponent->setTime(mTime);
        if (!mPathComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Path Component." );
            #endif
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initAnimationComponent
    ()
    {
        mAnimationComponent = new AnimationComponent();
        mAnimationComponent->setTime(mTime);
        if (!mAnimationComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Animation Component." );
            #endif
            return false;
        }
        return true;
    }


    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        mScriptComponent = new ScriptComponent(this,mScriptCache);
        if(!mScriptComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error( "Unable to initialise Script Engine." );
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initScrollerComponent
    ()
    {
        mScrollerComponent = new ScrollerComponent();
        if (!mScrollerComponent->init())
        {
            #if DREAM_LOG
            getLog()->error("Unable to initialise scroller component");
            #endif
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initLifetimeComponent
    ()
    {
        mLifetimeComponent = new LifetimeComponent();
        if (!mLifetimeComponent->init())
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to init lifetime component");
            #endif
            return false;
        }
        mLifetimeComponent->setTime(mTime);
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
        if (mLifetimeComponent != nullptr)
        {
            delete mLifetimeComponent;
            mLifetimeComponent = nullptr;
        }

        if(mScrollerComponent != nullptr)
        {
            delete mScrollerComponent;
            mScrollerComponent = nullptr;
        }

        if (mAnimationComponent != nullptr)
        {
            delete mAnimationComponent;
            mAnimationComponent = nullptr;
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

    AnimationComponent*
    ProjectRuntime::getAnimationComponent
    ()
    {
        return mAnimationComponent;
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

    ScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    {
        return mScriptComponent;
    }

    LifetimeComponent*
    ProjectRuntime::getLifetimeComponent
    ()
    {
        return mLifetimeComponent;
    }

    bool
    ProjectRuntime::updateLogic
    (SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->debug("\n====================\nUpdate Logic called @ {}\n====================",mTime->getAbsoluteTime());
        #endif
        // Get Inputs
        mTime->updateFrameTime();
        mInputComponent->updateComponent(sr);
        // Do Processing
        mLifetimeComponent->updateComponent(sr);
        mPathComponent->updateComponent(sr);
        mScrollerComponent->updateComponent(sr);
        mAnimationComponent->updateComponent(sr);
        mPhysicsComponent->setCamera(sr->getCamera());
        mPhysicsComponent->updateComponent(sr);
        if (mScriptingEnabled)
        {
            mScriptComponent->updateComponent(sr);
        }
        // Producec Outputs
        mAudioComponent->updateComponent(sr);
        sr->getCamera()->update();
        mGraphicsComponent->updateComponent(sr);
        return true;
    }

    void
    ProjectRuntime::updateGraphics
    (SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->debug("\n====================\nUpdate Graphics called @ {}\n====================",mTime->getAbsoluteTime());
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
        #ifdef DREAM_LOG
        getLog()->trace("{} Runtimes in {} Draw Calls", ModelMesh::RuntimesDrawn, ModelMesh::DrawCalls);
        #endif
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
        #ifdef DREAM_LOG
        getLog()->debug("CollectGarbage Called @ {}", mTime->getAbsoluteTime());
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
            #ifdef DREAM_LOG
            getLog()->trace("UpdateAll on {}",rt->getNameAndUuidString());
            #endif
            switch (rt->getState())
            {
                case SceneState::SCENE_STATE_TO_LOAD:
                    constructSceneRuntime(rt);
                    break;
                case SceneState::SCENE_STATE_LOADED:
                    break;
                case SceneState::SCENE_STATE_ACTIVE:
                    mWindowComponent->updateComponent(rt);
                    updateLogic(rt);
                    updateGraphics(rt);
                    collectGarbage(rt);
                    break;
                case SceneState::SCENE_STATE_TO_DESTROY:
                    destructSceneRuntime(rt);
                    break;
                case SceneState::SCENE_STATE_DESTROYED:
                    mSceneRuntimesToRemove.push_back(rt);
                    break;
            }
        }

        collectGarbage();
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
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
    {
        for (auto* sr : mSceneRuntimeVector)
        {
            if (sr->getUuid() == uuid)
            {
                return sr;
            }
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
        }
    }

    vector<SceneRuntime*>
    ProjectRuntime::getSceneRuntimeVector
    ()
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
    {
        return mProject;
    }

    InputComponent*
    ProjectRuntime::getInputComponent
    ()
    {
        return mInputComponent;
    }

    ScrollerComponent*
    ProjectRuntime::getScrollerComponent
    ()
    {
       return mScrollerComponent;
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
    {
        return mAudioCache;
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

    ScriptCache*
    ProjectRuntime::getScriptCache
    ()
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
        if (mAudioCache    != nullptr) mAudioCache->clear();
        if (mModelCache    != nullptr) mModelCache->clear();
        if (mShaderCache   != nullptr) mShaderCache->clear();
        if (mMaterialCache != nullptr) mMaterialCache->clear();
        if (mTextureCache  != nullptr) mTextureCache->clear();
        if (mScriptCache   != nullptr) mScriptCache->clear();
    }

    bool
    ProjectRuntime::getScriptingEnabled
    ()
    const
    {
        return mScriptingEnabled;
    }

    void
    ProjectRuntime::setScriptingEnabled
    (bool en)
    {
        mScriptingEnabled = en;
    }

    AssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (uint32_t uuid)
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
    {
        for (auto srt : mSceneRuntimeVector)
        {
            if (srt->getState() == SceneState::SCENE_STATE_ACTIVE) return true;
        }
        return false;
    }

    bool
    ProjectRuntime::hasLoadedScenes
    ()
    {
        for (auto srt : mSceneRuntimeVector)
        {
            if (
                srt->getState() >= SceneState::SCENE_STATE_LOADED
                &&
                srt->getState() < SceneState::SCENE_STATE_DESTROYED
                )
            {
                return true;
            }
        }
        return false;
    }


    bool
    ProjectRuntime::hasSceneRuntime
    (uint32_t uuid)
    {
        for (auto srt : mSceneRuntimeVector)
        {
            if (srt->getUuid() == uuid) return true;
        }
        return false;
    }
}
