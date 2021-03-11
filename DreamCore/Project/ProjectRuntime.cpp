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
#include "Storage/ProjectDirectory.h"

#include "Common/Logger.h"

#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Entity/EntityRuntime.h"

#include "Components/AssetDefinition.h"
#include "Components/Time.h"
#include "Math/Transform.h"
#include "Components/Cache.h"

#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioRuntime.h"

#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Window/WindowComponent.h"

using std::make_shared;
using std::make_unique;

namespace octronic::dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, WindowComponent* windowComponent, AudioComponent* ac, StorageManager* fm)
        : Runtime(project->getDefinition()),
          mDone(false),
          mProjectHandle(project),
          mAudioComponentHandle(ac),
          mWindowComponentHandle(windowComponent),
          mStorageManagerHandle(fm),
          mInputComponent(this),
          mGraphicsComponent(this),
          mPhysicsComponent(this),
          mScriptComponent(this),
          mActiveSceneRuntime(nullptr),
          mTaskQueue("ProjectTaskQueue"),
          mDestructionTaskQueue("ProjectDestructionTaskQueue"),
          mAudioCache(make_shared<Cache<AudioDefinition,AudioRuntime>>(this)),
          mTextureCache(make_shared<Cache<TextureDefinition, TextureRuntime>>(this)),
          mMaterialCache(make_shared<Cache<MaterialDefinition,MaterialRuntime>>(this)),
          mModelCache(make_shared<Cache<ModelDefinition,ModelRuntime>>(this)),
          mShaderCache(make_shared<Cache<ShaderDefinition,ShaderRuntime>>(this)),
          mScriptCache(make_shared<Cache<ScriptDefinition,ScriptRuntime>>(this)),
          mFontCache(make_shared<Cache<FontDefinition,FontRuntime>>(this))
    {
        LOG_DEBUG( "ProjectRuntime: Constructing" );
        mFrameDurationHistory.resize(MaxFrameCount);
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        LOG_DEBUG( "ProjectRuntime: Destructing" );
    }

    // Init ====================================================================

    bool
    ProjectRuntime::initComponents
    ()
    {
        LOG_TRACE( "ProjectRuntime: {}",__FUNCTION__ );

        if (!initWindowComponent())
        {
            LOG_ERROR("ProjectRuntime: Failed to init WindowComponent");
            return false;
        }

        if (!initGraphicsComponent())
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

        LOG_DEBUG( "ProjectRuntime: Successfully created Components." );

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (!mWindowComponentHandle)
        {
            LOG_CRITICAL("ProjectRuntime: Window component is null");
            return false;
        }
        auto projDef = static_cast<ProjectDefinition*>(mDefinitionHandle);
        mWindowComponentHandle->setName(projDef->getName());
        return true;
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if(mAudioComponentHandle == nullptr)
        {
            LOG_ERROR("ProjectRuntime: AudioComponent is null");
            return false;
        }
        mAudioComponentHandle->setProjectRuntime(this);

        // Audio component must be initialised outside of dream
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        auto projectDef = static_cast<ProjectDefinition*>(mDefinitionHandle);

        if (!mInputComponent.init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise InputComponent." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

        if (!mPhysicsComponent.init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise PhysicsComponent." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (!mGraphicsComponent.init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise Graphics Component." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if(!mScriptComponent.init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise Script Engine." );
            return false;
        }
        return true;
    }

    bool
    ProjectRuntime::constructSceneRuntime
    (SceneRuntime* rt)
    {
        LOG_DEBUG("ProjectRuntime: Constructing Scene Runtime");
        auto t = rt->getLoadFromDefinitionTask();
        if (t->hasState(TASK_STATE_QUEUED)) mTaskQueue.pushTask(t);
        return true;
    }

    bool
    ProjectRuntime::loadFromDefinition
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

        if (!initComponents())
        {
            return false;
        }

        return true;
    }

    void
    ProjectRuntime::destructSceneRuntime
    (SceneRuntime* rt)
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        rt->destroyRuntime();
        if (mActiveSceneRuntime == rt)
        {
            mActiveSceneRuntime = nullptr;
        }
    }

    // Running =================================================================

    void
    ProjectRuntime::collectGarbage
    ()
    {
        LOG_DEBUG("\n"
        "=======================================================================\n"
        "CollectGarbage Called @ {}\n"
        "=======================================================================",
                  mTime.getAbsoluteTime());

        mDestructionTaskQueue.executeQueue();
        mGraphicsComponent.getDestructionTaskQueue()->executeQueue();

        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        for (auto rt : mSceneRuntimesToRemove)
        {
        	rt->collectGarbage();

            auto itr = find(mSceneRuntimeVector.begin(),mSceneRuntimeVector.end(),rt);
            if (itr != mSceneRuntimeVector.end())
            {
                mSceneRuntimeVector.erase(itr);
            }

            delete rt;
        }
        mSceneRuntimesToRemove.clear();
    }

    void ProjectRuntime::pushComponentTasks()
    {
        // Maintain this order
        mWindowComponentHandle->pushTasks();
        mInputComponent.pushTasks();
        mAudioComponentHandle->pushTasks();
        mPhysicsComponent.pushTasks();
        mScriptComponent.pushTasks();
        mGraphicsComponent.pushTasks();
    }

    void
    ProjectRuntime::step
    ()
    {
        LOG_TRACE("\n\n=========================[ Update Started ]=========================\n\n");

        pushComponentTasks();

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
                    mTime.updateFrameTime();
                    mFrameDurationHistory.push_back(1000.0f/mTime.getFrameTimeDelta());
                    if (mFrameDurationHistory.size() > MaxFrameCount) mFrameDurationHistory.pop_front();

                    if (rt != nullptr)
                    {
                        rt->getCamera()->update();
                        rt->createSceneTasks();
                    }

                    rt->collectGarbage();
                    break;
                case SceneState::SCENE_STATE_TO_DESTROY:
                    destructSceneRuntime(rt);
                    break;
                case SceneState::SCENE_STATE_DESTROYED:
                    mSceneRuntimesToRemove.push_back(rt);
                    break;
            }
        }

		ModelMesh::ClearCounters();
		ShaderRuntime::InvalidateState();
        mTaskQueue.executeQueue();
        mGraphicsComponent.getTaskQueue()->executeQueue();

        collectGarbage();

        LOG_TRACE("\n\n=========================[ Update Complete ]=========================\n\n");
    }

    void
    ProjectRuntime::clearAllCaches
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

        mDestructionTaskQueue.executeQueue();
        mGraphicsComponent.getDestructionTaskQueue()->executeQueue();

        mAudioCache->clear();
        mModelCache->clear();
        mShaderCache->clear();
        mMaterialCache->clear();
        mTextureCache->clear();
        mScriptCache->clear();
        mFontCache->clear();
    }

    // Accessors  ==============================================================

    shared_ptr<Cache<AudioDefinition,AudioRuntime>>
    ProjectRuntime::getAudioCache
    ()
    {
        return mAudioCache;
    }

    shared_ptr<Cache<ShaderDefinition,ShaderRuntime>>
    ProjectRuntime::getShaderCache
    ()
    {
        return mShaderCache;
    }

    shared_ptr<Cache<MaterialDefinition,MaterialRuntime>>
    ProjectRuntime::getMaterialCache
    ()
    {
        return mMaterialCache;
    }

    shared_ptr<Cache<ModelDefinition,ModelRuntime>>
    ProjectRuntime::getModelCache
    ()
    {
        return mModelCache;
    }

    shared_ptr<Cache<TextureDefinition,TextureRuntime>>
    ProjectRuntime::getTextureCache
    ()
    {
        return mTextureCache;
    }

    shared_ptr<Cache<ScriptDefinition,ScriptRuntime>>
    ProjectRuntime::getScriptCache
    ()
    {
        return mScriptCache;
    }

    shared_ptr<Cache<FontDefinition,FontRuntime>>
    ProjectRuntime::getFontCache()
    {
        return mFontCache;
    }

    AssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    const
    {
        if (mProjectHandle != nullptr)
        {
            return mProjectHandle->getAssetDefinitionByUuid(uuid);
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
        return mActiveSceneRuntime != nullptr;
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
        float f = 0.0;
        for (const auto& dur : mFrameDurationHistory)
        {
            f += dur;
        }
        return f/MaxFrameCount;
    }

    TaskQueue<shared_ptr<Task>>* ProjectRuntime::getTaskQueue()
    {
        return &mTaskQueue;
    }

    TaskQueue<shared_ptr<DestructionTask>>* ProjectRuntime::getDestructionTaskQueue()
    {
        return &mDestructionTaskQueue;
    }

    WindowComponent*
    ProjectRuntime::getWindowComponent
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
    ProjectRuntime::getTime
    ()
    {
        return &mTime;
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
    {
        return mAudioComponentHandle;
    }

    PhysicsComponent*
    ProjectRuntime::getPhysicsComponent
    ()
    {
        return &mPhysicsComponent;
    }

    GraphicsComponent*
    ProjectRuntime::getGraphicsComponent
    ()
    {
        return &mGraphicsComponent;
    }

    ScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    {
        return &mScriptComponent;
    }

    StorageManager*
    ProjectRuntime::getStorageManager
    ()
    {
        return mStorageManagerHandle;
    }



    int
    ProjectRuntime::getWindowWidth
    ()
    const
    {
        if (mWindowComponentHandle != nullptr)
        {
            return mWindowComponentHandle->getWidth();
        }
        return 0;
    }

    void
    ProjectRuntime::setWindowWidth
    (int w)
    {
        if (mWindowComponentHandle != nullptr)
        {
            mWindowComponentHandle->setWidth(w);
        }
    }

    int
    ProjectRuntime::getWindowHeight
    ()
    const
    {
        if (mWindowComponentHandle != nullptr)
        {
            return mWindowComponentHandle->getHeight();
        }
        return 0;
    }

    void
    ProjectRuntime::setWindowHeight
    (int h)
    {
        if (mWindowComponentHandle != nullptr)
        {
            mWindowComponentHandle->setHeight(h);
        }
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
    const
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        return mActiveSceneRuntime;
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
    ProjectRuntime::setActiveSceneRuntime
    (UuidType uuid)
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mActiveSceneRuntime = getSceneRuntimeByUuid(uuid);
        mActiveSceneRuntime->setState(SCENE_STATE_ACTIVE);
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
            LOG_TRACE("ProjectRuntime: Adding SceneRuntime {}",rt->getNameAndUuidString());
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

    Project*
    ProjectRuntime::getProject
    ()
    const
    {
        return mProjectHandle;
    }

    InputComponent*
    ProjectRuntime::getInputComponent
    ()
    {
        return &mInputComponent;
    }

    int ProjectRuntime::MaxFrameCount = 100;
}
