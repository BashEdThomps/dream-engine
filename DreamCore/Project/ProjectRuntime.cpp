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
#include "Components/Storage/ProjectDirectory.h"

#include "Common/Logger.h"

#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

#include "Components/AssetDefinition.h"
#include "Components/Time.h"
#include "Components/Transform.h"

#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioCache.h"
#include "Components/Input/InputComponent.h"

#include "Components/Task/TaskManager.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Window/WindowComponent.h"
#include "Components/Script/ScriptComponent.h"

#include "Components/Graphics/Font/FontCache.h"
#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Material/MaterialCache.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureCache.h"

#include <thread>

namespace octronic::dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, WindowComponent* windowComponent, AudioComponent* ac, StorageManager* fm)
        : Runtime("ProjectRuntime",project->getDefinition()),
          mDone(false),
          mTime(nullptr),
          mProject(project),
          mAudioComponent(ac),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mPhysicsComponent(nullptr),
          mScriptComponent(nullptr),
          mWindowComponent(windowComponent),
          mTaskManager(nullptr),
          mStorageManager(fm),
          mAudioCache(nullptr),
          mTextureCache(nullptr),
          mMaterialCache(nullptr),
          mModelCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr),
          mFontCache(nullptr)
    {
        LOG_DEBUG( "ProjectRuntime: Constructing" );
        mFrameDurationHistory.resize(MaxFrameCount);
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        LOG_DEBUG( "ProjectRuntime: Destructing" );
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
        if(dreamTryLock()){
            dreamLock();

            mDone = done;

        } dreamElseLockFailed
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
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE( "ProjectRuntime: {}",__FUNCTION__ );

            mTime = new Time();

            if (!initWindowComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init WindowComponent");
                return false;
            }

            if(!initGraphicsComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init GraphicsComponent");
                return false;
            }

            if (!initInputComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init InputComponent");
                return false;
            }

            if(!initPhysicsComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init PhysicsComponent");
                return false;
            }

            if(!initAudioComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init AudioComponent");
                return false;
            }

            if (!initScriptComponent())
            {
                LOG_ERROR("ProjectRuntime: Failed to init ScriptComponent");
                return false;
            }

            if (!initTaskManager())
            {
                LOG_ERROR("ProjectRuntime: Failed to init TaskManager");
                return false;
            }

            LOG_DEBUG( "ProjectRuntime: Successfully created Components." );

            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            if (!mWindowComponent)
            {
                LOG_CRITICAL("ProjectRuntime: Window component is null");
                return false;
            }
            auto projDef = dynamic_cast<ProjectDefinition*>(mDefinition);
            mWindowComponent->setName(projDef->getName());
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            if(mAudioComponent == nullptr)
            {
                LOG_ERROR("ProjectRuntime: AudioComponent is null");
                return false;
            }
            mAudioComponent->setProjectRuntime(this);

            // Audio component must be initialised outside of dream

            /*
        if (!mAudioComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise AudioComponent." );
            mAudioComponent->unlock();
            return false;
        }
        */
            return true;
        } dreamElseLockFailed
    }

    bool ProjectRuntime::initInputComponent()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            auto projectDef = dynamic_cast<ProjectDefinition*>(mDefinition);
            mInputComponent = new InputComponent(this);

            if (!mInputComponent->init())
            {
                LOG_ERROR( "ProjectRuntime: Unable to initialise InputComponent." );
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mPhysicsComponent = new PhysicsComponent(this);
            mPhysicsComponent->setTime(mTime);
            if (!mPhysicsComponent->init())
            {
                LOG_ERROR( "ProjectRuntime: Unable to initialise PhysicsComponent." );
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mGraphicsComponent = new GraphicsComponent(this,mWindowComponent);
            mGraphicsComponent->setTime(mTime);
            mGraphicsComponent->setShaderCache(mShaderCache);
            if (!mGraphicsComponent->init())
            {
                LOG_ERROR( "ProjectRuntime: Unable to initialise Graphics Component." );
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mScriptComponent = new ScriptComponent(this,mScriptCache);
            if(!mScriptComponent->init())
            {
                LOG_ERROR( "ProjectRuntime: Unable to initialise Script Engine." );
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initTaskManager
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mTaskManager = new TaskManager();
            return true;
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mAudioCache = new AudioCache(this);
            mTextureCache = new TextureCache(this);
            mShaderCache  = new ShaderCache(this);
            mMaterialCache = new MaterialCache(this,mShaderCache,mTextureCache);
            mModelCache   = new ModelCache(this,mShaderCache, mMaterialCache);
            mScriptCache  = new ScriptCache(this);
            mFontCache = new FontCache(this);
            return true;
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::deleteCaches
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
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

            if (mFontCache != nullptr)
            {
                delete mFontCache;
                mFontCache = nullptr;
            }
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::deleteComponents
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            if (mTaskManager != nullptr)
            {
                delete mTaskManager;
                mTaskManager = nullptr;
            }

            if (mAudioComponent != nullptr)
            {
                //delete mAudioComponent;
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

            if (mScriptComponent != nullptr)
            {
                delete mScriptComponent;
                mScriptComponent = nullptr;
            }
        } dreamElseLockFailed
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

    ScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    const
    {
        return mScriptComponent;
    }

    TaskManager*
    ProjectRuntime::getTaskManager
    ()
    const
    {
        return mTaskManager;
    }

    StorageManager*
    ProjectRuntime::getStorageManager
    ()
    const
    {
        return mStorageManager;
    }

    bool
    ProjectRuntime::updateLogic
    (SceneRuntime* sr)
    {
        if(dreamTryLock()){
            dreamLock();

            mTaskManager->waitForThreadsToFinish();
            LOG_DEBUG("\n"
        "=======================================================================\n"
        "Update Logic called @ {}\n"
        "=======================================================================\n",
                      mTime->getAbsoluteTime());
            mTime->updateFrameTime();

            mFrameDurationHistory.push_back(1000.0f/mTime->getFrameTimeDelta());

            if (mFrameDurationHistory.size() > MaxFrameCount)
            {
                mFrameDurationHistory.pop_front();
            }

            sr->createSceneTasks();
            mTaskManager->allowThreadsToRun();
            std::this_thread::yield();
            mTaskManager->waitForThreadsToFinish();
            mGraphicsComponent->handleResize();
            sr->getCamera()->update();
            return true;
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::updateGraphics
    (SceneRuntime* sr)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_DEBUG("\n"
        "=======================================================================\n"
        "Update Graphics called @ {}\n"
        "=======================================================================\n",
                      mTime->getAbsoluteTime());
            // Draw 3D/PhysicsDebug/2D
            ModelMesh::ClearCounters();
            mGraphicsComponent->executeTaskQueue();
            mGraphicsComponent->renderGeometryPass(sr);
            mGraphicsComponent->renderShadowPass(sr);
            mGraphicsComponent->renderLightingPass(sr);
            mGraphicsComponent->renderFonts(sr);
            mGraphicsComponent->executeDestructionTaskQueue();
            ShaderRuntime::InvalidateState();
            mPhysicsComponent->setCamera(sr->getCamera());
            mPhysicsComponent->drawDebug();
        } dreamElseLockFailed
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
        if(dreamTryLock()){
            dreamLock();

            if (mWindowComponent != nullptr)
            {
                mWindowComponent->setWidth(w);
            }
        } dreamElseLockFailed
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
        if(dreamTryLock()){
            dreamLock();

            if (mWindowComponent != nullptr)
            {
                mWindowComponent->setHeight(h);
            }
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::collectSceneGarbage
    (SceneRuntime* rt)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_DEBUG("\n"
        "=======================================================================\n"
        "CollectGarbage Called @ {}\n"
        "=======================================================================",
                      mTime->getAbsoluteTime());

            rt->collectGarbage();
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            for (auto rt : mSceneRuntimesToRemove)
            {
                auto itr = find(mSceneRuntimeVector.begin(),mSceneRuntimeVector.end(),rt);
                if (itr != mSceneRuntimeVector.end())
                {
                    mSceneRuntimeVector.erase(itr);
                }
            }
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("\n\n=========================[ Update Started ]=========================\n\n");

            if (mSceneRuntimeVector.empty())
            {
                mTaskManager->allowThreadsToRun();
                std::this_thread::yield();
                mTaskManager->waitForThreadsToFinish();
                mGraphicsComponent->executeTaskQueue();
                mGraphicsComponent->executeDestructionTaskQueue();
            }
            else
            {
                for (auto rt : mSceneRuntimeVector)
                {
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
                            mWindowComponent->updateWindow(rt);
                            collectSceneGarbage(rt);
                            break;
                        case SceneState::SCENE_STATE_TO_DESTROY:
                            destructSceneRuntime(rt);
                            break;
                        case SceneState::SCENE_STATE_DESTROYED:
                            mSceneRuntimesToRemove.push_back(rt);
                            break;
                    }
                }
            }
            collectGarbage();
            LOG_TRACE("\n\n=========================[ Update Complete ]=========================\n\n");
        } dreamElseLockFailed
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
    const
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
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
    (UuidType uuid)
    const
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
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
    (UuidType uuid)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
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
        } dreamElseLockFailed
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
        if(dreamTryLock()){
            dreamLock();

            auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
            if (itr == mSceneRuntimeVector.end())
            {
                mSceneRuntimeVector.push_back(rt);
            }
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::removeSceneRuntime
    (SceneRuntime* rt)
    {
        if(dreamTryLock()){
            dreamLock();

            auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
            if (itr != mSceneRuntimeVector.end())
            {
                mSceneRuntimeVector.erase(itr);
            }
        } dreamElseLockFailed
    }

    bool
    ProjectRuntime::constructSceneRuntime
    (SceneRuntime* rt)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_DEBUG("ProjectRuntime: Constructing Scene Runtime");
            return rt->useDefinition();
        } dreamElseLockFailed
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
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            if (!initCaches())
            {
                return false;
            }

            if (!initComponents())
            {
                return false;
            }

            return true;
        } dreamElseLockFailed
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

    FontCache*
    ProjectRuntime::getFontCache() const
    {
        return mFontCache;
    }

    void
    ProjectRuntime::destructSceneRuntime
    (SceneRuntime* rt, bool clearCaches)
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            rt->destroyRuntime();
            if (clearCaches)
            {
                clearAllCaches();
            }
        } dreamElseLockFailed
    }

    void
    ProjectRuntime::clearAllCaches
    ()
    {
        if(dreamTryLock()){
            dreamLock();

            LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
            mTaskManager->allowThreadsToRun();
            std::this_thread::yield();
            mTaskManager->waitForThreadsToFinish();
            if (mAudioCache != nullptr)
            {
                mAudioCache->clear();
            }

            if (mModelCache != nullptr)
            {
                mModelCache->clear();
            }

            if (mShaderCache != nullptr)
            {
                mShaderCache->clear();
            }

            if (mMaterialCache != nullptr)
            {
                mMaterialCache->clear();
            }

            if (mTextureCache != nullptr)
            {
                mTextureCache->clear();
            }

            if (mScriptCache != nullptr)
            {
                mScriptCache->clear();
            }

            if (mFontCache != nullptr)
            {
                mFontCache->clear();
            }
        } dreamElseLockFailed
    }

    AssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    const
    {
        if (mProject != nullptr)
        {
            return mProject->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    EntityRuntime*
    ProjectRuntime::getEntityRuntimeByUuid
    (SceneRuntime* rt, UuidType uuid)
    const
    {
        if (rt == nullptr)
        {
            return nullptr;
        }

        return rt->getEntityRuntimeByUuid(uuid);
    }

    bool
    ProjectRuntime::hasActiveScene
    ()
    const
    {
        for (auto srt : mSceneRuntimeVector)
        {
            if (srt->getState() == SceneState::SCENE_STATE_ACTIVE) {
                return true;
            }
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
            if (srt->getState() >= SceneState::SCENE_STATE_LOADED &&
                srt->getState() < SceneState::SCENE_STATE_DESTROYED)
            {
                return true;
            }
        }
        return false;
    }


    bool
    ProjectRuntime::hasSceneRuntime
    (UuidType uuid)
    const
    {
        bool result = false;
        for (auto srt : mSceneRuntimeVector)
        {
            if (srt->getUuid() == uuid)
            {
                result = true;
            }

            if (result) break;
        }
        return result;
    }

    deque<float> ProjectRuntime::getFrameDurationHistory() const
    {
        return mFrameDurationHistory;
    }

    float ProjectRuntime::getAverageFramerate()
    {
        if(dreamTryLock()){
            dreamLock();

            float f = 0.0;
            for (const auto& dur : mFrameDurationHistory)
            {
                f += dur;
            }
            return f/MaxFrameCount;
        } dreamElseLockFailed
    }

    int ProjectRuntime::MaxFrameCount = 100;
}
