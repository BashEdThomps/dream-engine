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

#include <iostream>
#include "ActorRuntime.h"
#include "ActorDefinition.h"
#include "../SceneRuntime.h"
#include "../SceneDefinition.h"
#include "../../Components/Event.h"
#include "../../Components/Path/PathRuntime.h"
#include "../../Components/Animation/AnimationDefinition.h"
#include "../../Components/Animation/AnimationRuntime.h"
#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Scroller/ScrollerDefinition.h"
#include "../../Components/Scroller/ScrollerRuntime.h"
#include "../../Components/Audio/AudioCache.h"
#include "../../Components/Audio/AudioRuntime.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Model/ModelRuntime.h"
#include "../../Components/Graphics/Model/ModelCache.h"
#include "../../Components/Graphics/Light/LightRuntime.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterRuntime.h"
#include "../../Components/Physics/PhysicsObjectRuntime.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/AssetDefinition.h"
#include "../../Components/Audio/AudioDefinition.h"
#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"
#include "../../Components/Script/ScriptDefinition.h"
#include "../../Components/Script/ScriptComponent.h"
#include "../../Components/Script/ScriptRuntime.h"
#include "../../Components/ObjectEmitter/ObjectEmitterDefinition.h"
#include "../../Components/ObjectEmitter/ObjectEmitterRuntime.h"
#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"
#include "../../Common/Uuid.h"
#include "../../TaskManager/TaskManager.h"
#include "../../Components/Script/ScriptTasks.h"
#include "../../Components/Time.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

using std::vector;

namespace Dream
{
    ActorRuntime::ActorRuntime(
        ActorDefinition* sd,
        SceneRuntime* sr,
        bool randomUuid
    ):  Runtime(sd),
        mAnimationRuntime(nullptr),
        mAudioRuntime(nullptr),
        mLightRuntime(nullptr),
        mParticleEmitterRuntime(nullptr),
        mPathRuntime(nullptr),
        mPhysicsObjectRuntime(nullptr),
        mScriptRuntime(nullptr),
        mModelRuntime(nullptr),
        mScrollerRuntime(nullptr),
        mSceneRuntime(sr),
        mParentRuntime(nullptr),
        mObjectEmitterRuntime(nullptr),
        mBoundingBox(),
        mHasCameraFocus(false),
        mDeleted(false),
        mHidden(false),
        mAlwaysDraw(false),
        mRandomUuid(randomUuid),
        mDeferredFor(0),
        mObjectLifetime(0),
        mDieAfter(0),
        mScriptOnInitTask(this),
        mScriptOnUpdateTask(this),
        mScriptOnEventTask(this),
        mScriptOnDestroyTask(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("ActorRuntime");
        getLog()->trace( "Constructing Object" );
        #endif

        mEventQueueMutex.lock();
        mEventQueue.reserve(10);
        mEventQueueMutex.unlock();

        if (mRandomUuid)
        {
            mUuid = Uuid::generateUuid();
        }

        if (static_cast<SceneDefinition*>(mSceneRuntime->getDefinition())->getPlayerObject() == mUuid)
        {
            mSceneRuntime->setPlayerObject(this);
        }

        setAttribute("uuid",getUuidString());
    }

    ActorRuntime::~ActorRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace( "Destroying Object" );
        #endif

        for (auto child : mChildRuntimes)
        {
            delete child;
        }
        mChildRuntimes.clear();

        removeScrollerRuntime();
        removeAnimationRuntime();
        removeAudioRuntime();
        removeLightRuntime();
        removeModelRuntime();
        removeParticleEmitterRuntime();
        removePathRuntime();
        removePhysicsObjectRuntime();
        removeScriptRuntime();
        removeObjectEmitterRuntime();
    }

    void
    ActorRuntime::removeAnimationRuntime
    ()
    {
        if (mAnimationRuntime != nullptr)
        {
            delete mAnimationRuntime;
            mAnimationRuntime = nullptr;
        }
    }

    void
    ActorRuntime::removeAudioRuntime
    ()
    {
        mAudioRuntime = nullptr;
    }

    void
    ActorRuntime::removePathRuntime
    ()
    {
        if (mPathRuntime != nullptr)
        {
            delete mPathRuntime;
            mPathRuntime = nullptr;
        }
    }

    void
    ActorRuntime::removeModelRuntime
    ()
    {
        if (mModelRuntime != nullptr)
        {
            mModelRuntime->removeRuntime(this);
        }
    }

    void
    ActorRuntime::removeLightRuntime
    ()
    {
        if (mLightRuntime != nullptr)
        {
            delete mLightRuntime;
            mLightRuntime = nullptr;
        }
    }

    void
    ActorRuntime::removeScriptRuntime
    ()
    {
        if (mScriptRuntime)
        {
            mSceneRuntime
                ->getProjectRuntime()
                ->getTaskManager()
                ->pushDestructionTask(mScriptOnDestroyTask);
        }
        mScriptRuntime = nullptr;
    }

    void
    ActorRuntime::removePhysicsObjectRuntime
    ()
    {
        if (hasPhysicsObjectRuntime())
        {
            auto physicsComp = mSceneRuntime
                ->getProjectRuntime()
                ->getPhysicsComponent();
           if (physicsComp != nullptr)
           {
                physicsComp->removePhysicsObjectRuntime(getPhysicsObjectRuntime());
           }
        }

        if (mPhysicsObjectRuntime != nullptr)
        {
            delete mPhysicsObjectRuntime;
            mPhysicsObjectRuntime = nullptr;
        }
    }

    void
    ActorRuntime::removeParticleEmitterRuntime
    ()
    {
        if (mParticleEmitterRuntime != nullptr)
        {
            delete mParticleEmitterRuntime;
            mParticleEmitterRuntime = nullptr;
        }
    }

    void
    ActorRuntime::removeObjectEmitterRuntime
    ()
    {
        if (mObjectEmitterRuntime != nullptr)
        {
            delete mObjectEmitterRuntime;
            mObjectEmitterRuntime = nullptr;
        }
    }


    void
    ActorRuntime::removeScrollerRuntime
    ()
    {
        if (mScrollerRuntime != nullptr)
        {
            delete mScrollerRuntime;
            mScrollerRuntime = nullptr;
        }
    }

    AnimationRuntime*
    ActorRuntime::getAnimationRuntime
    ()
    {
        return mAnimationRuntime;
    }

    PathRuntime*
    ActorRuntime::getPathRuntime
    ()
    {
        return mPathRuntime;
    }

    AudioRuntime*
    ActorRuntime::getAudioRuntime
    ()
    {
        return mAudioRuntime;
    }

    ModelRuntime*
    ActorRuntime::getModelRuntime
    ()
    {
        return mModelRuntime;
    }

    ScriptRuntime*
    ActorRuntime::getScriptRuntime
    ()
    {
        return mScriptRuntime;
    }

    LightRuntime*
    ActorRuntime::getLightRuntime
    ()
    {
        return mLightRuntime;
    }

    ParticleEmitterRuntime*
    ActorRuntime::getParticleEmitterRuntime
    ()
    {
       return mParticleEmitterRuntime;
    }

    ObjectEmitterRuntime*
    ActorRuntime::getObjectEmitterRuntime
    ()
    {
        return mObjectEmitterRuntime;
    }

    ScrollerRuntime*
    ActorRuntime::getScrollerRuntime
    ()
    {
        return mScrollerRuntime;
    }

    AssetRuntime*
    ActorRuntime::getAssetRuntime
    (AssetType type)
    {
       switch (type)
       {
           case Dream::ANIMATION:
               return getAnimationRuntime();
           case Dream::PATH:
               return getPathRuntime();
           case Dream::AUDIO:
               return getAudioRuntime();
           case Dream::LIGHT:
               return getLightRuntime();
           case Dream::MODEL:
               return getModelRuntime();
           case Dream::PHYSICS_OBJECT:
               return getPhysicsObjectRuntime();
           case Dream::SCRIPT:
               return getScriptRuntime();
           case Dream::SCROLLER:
               return getScrollerRuntime();
           case Dream::PARTICLE_EMITTER:
               return getParticleEmitterRuntime();
           case Dream::OBJECT_EMITTER:
               return getObjectEmitterRuntime();
           default:
               break;
       }
       return nullptr;
    }

    bool
    ActorRuntime::hasAnimationRuntime
    ()
    {
        return mAnimationRuntime != nullptr;
    }

    bool
    ActorRuntime::hasLightRuntime
    ()
    {
        return mLightRuntime != nullptr;
    }

    bool
    ActorRuntime::hasModelRuntime
    ()
    {
        return mModelRuntime != nullptr;
    }

    bool
    ActorRuntime::hasScriptRuntime
    ()
    {
        return mScriptRuntime != nullptr;
    }

    bool
    ActorRuntime::hasObjectEmitterRuntime
    ()
    {
        return mObjectEmitterRuntime != nullptr;
    }

    void
    ActorRuntime::setAssetDefinitionsMap
    (const map<AssetType,uint32_t>& assetMap)
    {
        mAssetDefinitions = assetMap;
    }

    map<AssetType,uint32_t>
    ActorRuntime::getAssetDefinitionsMap
    ()
    {
        return mAssetDefinitions;
    }

    bool
    ActorRuntime::getAlwaysDraw
    () const
    {
        return mAlwaysDraw;
    }

    void
    ActorRuntime::setAlwaysDraw
    (bool alwaysDraw)
    {
        mAlwaysDraw = alwaysDraw;
    }

    PhysicsObjectRuntime*
    ActorRuntime::getPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime;
    }

    Transform&
    ActorRuntime::getTransform
    ()
    {
        return mTransform;
    }

    void
    ActorRuntime::initTransform
    ()
    {
        if (mDefinition)
        {
            auto definedTransform = static_cast<ActorDefinition*>(mDefinition)->getTransform();
            mInitialTransform.setMatrix(definedTransform.getMatrix());
            mTransform.setMatrix(definedTransform.getMatrix());
        }
    }

    bool
    ActorRuntime::hasEvents
    ()
    {
        bool retval = false;
        mEventQueueMutex.lock();
        retval = !mEventQueue.empty();
        mEventQueueMutex.unlock();
        return retval;
    }

    void
    ActorRuntime::addEvent
    (const Event& event)
    {
        if (!mDeleted)
        {
            #ifdef DREAM_LOG
            getLog()->trace("Event posted from {} to {}",
                event.getAttribute("uuid"), getNameAndUuidString());
            #endif
            mEventQueueMutex.lock();
            mEventQueue.push_back(std::move(event));
            mEventQueueMutex.unlock();
        }
    }

    bool ActorRuntime::tryLockEventQueue()
    {
       return mEventQueueMutex.try_lock();
    }

    void ActorRuntime::unlockEventQueue()
    {
       mEventQueueMutex.unlock();
    }

    vector<Event>*
    ActorRuntime::getEventQueue
    ()
    {
        return &mEventQueue;
    }

    void
    ActorRuntime::clearEventQueue
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Clearing event queue");
        #endif

        if(tryLockEventQueue())
        {
            for (auto itr = mEventQueue.begin(); itr != mEventQueue.end();)
            {
                if ((*itr).getProcessed())
                {
                    mEventQueue.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
        unlockEventQueue();
    }

    void
    ActorRuntime::collectGarbage
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Collecting Garbage {}" ,getNameAndUuidString());
        #endif

        vector<ActorRuntime*> toDelete;

        for (auto child : mChildRuntimes)
        {
           if (child->getDeleted())
           {
                toDelete.push_back(child);
           }
        }

        for (auto child : toDelete)
        {
            #ifdef DREAM_LOG
            getLog()->trace("Deleting child {}",child->getNameAndUuidString());
            #endif
            mChildRuntimes.erase
            (
                find
                (
                    mChildRuntimes.begin(),
                    mChildRuntimes.end(),
                    child
                )
            );
            delete child;
        }

        clearEventQueue();
    }

    bool
    ActorRuntime::hasPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime != nullptr;
    }

    bool
    ActorRuntime::hasPathRuntime
    ()
    {
        return mPathRuntime != nullptr;
    }

    bool
    ActorRuntime::hasAudioRuntime
    ()
    {
        return mAudioRuntime != nullptr;
    }

    bool
    ActorRuntime::createAssetRuntimes
    ()
    {
        for (auto assetPair : mAssetDefinitions)
        {
            AssetDefinition* def = getAssetDefinitionByUuid(assetPair.second);
            bool result = false;
            if (def == nullptr)
            {
                #ifdef DREAM_LOG
                getLog()->error("Could not find asset definition {}", assetPair.second);
                #endif
                continue;
            }
            #ifdef DREAM_LOG
            getLog()->trace("Creating {}",def->getNameAndUuidString());
            #endif
            switch (assetPair.first)
            {
                case AssetType::ANIMATION:
                    result = createAnimationRuntime(static_cast<AnimationDefinition*>(def));
                    break;
                case AssetType::AUDIO:
                    result = createAudioRuntime(static_cast<AudioDefinition*>(def));
                    break;
                case AssetType::LIGHT:
                    result = createLightRuntime(static_cast<LightDefinition*>(def));
                    break;
                case AssetType::MODEL:
                    result = createModelRuntime(static_cast<ModelDefinition*>(def));
                    break;
                case AssetType::PARTICLE_EMITTER:
                    result = createParticleEmitterRuntime(static_cast<ParticleEmitterDefinition*>(def));
                    break;
                case AssetType::PATH:
                    result = createPathRuntime(static_cast<PathDefinition*>(def));
                    break;
                case AssetType::PHYSICS_OBJECT:
                    result = createPhysicsObjectRuntime(static_cast<PhysicsObjectDefinition*>(def));
                    break;
                case AssetType::SCRIPT:
                    result = createScriptRuntime(static_cast<ScriptDefinition*>(def));
                    break;
                case AssetType::SCROLLER:
                    result = createScrollerRuntime(static_cast<ScrollerDefinition*>(def));
                    break;
                case AssetType::OBJECT_EMITTER:
                    result = createObjectEmitterRuntime(static_cast<ObjectEmitterDefinition*>(def));
                    break;
                default:
                    return false;
            }
            if (!result)
            {
                return false;
            }
        }
        return true;
    }

    AssetDefinition*
    ActorRuntime::getAssetDefinitionByUuid
    (uint32_t uuid)
    {
        auto project = mSceneRuntime->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("Project is not found");
            #endif
            return nullptr;
        }
        auto assetDefinition = project->getDefinition()->getAssetDefinitionByUuid(uuid);
        if (assetDefinition == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("AssetDefinition not found");
            #endif
        }
        return assetDefinition;
    }

    bool
    ActorRuntime::replaceAssetUuid
    (AssetType type, uint32_t uuid)
    {
        #ifdef DREAM_LOG
        getLog()->info("REPLACING asset Runtime from uuid {}", uuid);
        #endif
        auto project = mSceneRuntime->getProjectRuntime()->getProject();
        if (project == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("Project is not found");
            #endif
            return false;
        }
        auto def = project->getDefinition()->getAssetDefinitionByUuid(uuid);
        if (def == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("AssetDefinition not found");
            #endif
        }
        switch (type)
        {
            case AssetType::ANIMATION:
                return createAnimationRuntime(static_cast<AnimationDefinition*>(def));
            case AssetType::AUDIO:
                return createAudioRuntime(static_cast<AudioDefinition*>(def));
            case AssetType::LIGHT:
                return createLightRuntime(static_cast<LightDefinition*>(def));
            case AssetType::MODEL:
                return createModelRuntime(static_cast<ModelDefinition*>(def));
            case AssetType::PARTICLE_EMITTER:
                return createParticleEmitterRuntime(static_cast<ParticleEmitterDefinition*>(def));
            case AssetType::PATH:
                return createPathRuntime(static_cast<PathDefinition*>(def));
            case AssetType::PHYSICS_OBJECT:
                return createPhysicsObjectRuntime(static_cast<PhysicsObjectDefinition*>(def));
            case AssetType::SCRIPT:
                return createScriptRuntime(static_cast<ScriptDefinition*>(def));
            case AssetType::SCROLLER:
                return createScrollerRuntime(static_cast<ScrollerDefinition*>(def));
            case AssetType::OBJECT_EMITTER:
                return createObjectEmitterRuntime(static_cast<ObjectEmitterDefinition*>(def));
            default:
                return false;
        }
    }

    bool
    ActorRuntime::createPhysicsObjectRuntime
    (PhysicsObjectDefinition* definition)
    {
        removePhysicsObjectRuntime();
        #ifdef DREAM_LOG
        getLog()->trace( "Creating Physics Object Asset Runtime." );
        #endif
        mPhysicsObjectRuntime = new PhysicsObjectRuntime(
            definition,
            mSceneRuntime->getProjectRuntime()->getPhysicsComponent(),
            mSceneRuntime->getProjectRuntime()->getModelCache(),
            this
        );
        return mPhysicsObjectRuntime->useDefinition();
    }

    bool
    ActorRuntime::createParticleEmitterRuntime
    (ParticleEmitterDefinition* definition)
    {
        #ifdef DREAM_LOG
        getLog()->trace( "Creating ParticleEmitter asset Runtime." );
        #endif
        removeParticleEmitterRuntime();
        mParticleEmitterRuntime = new ParticleEmitterRuntime(definition,this);
        return mParticleEmitterRuntime->useDefinition();
    }

    bool
    ActorRuntime::createScrollerRuntime
    (ScrollerDefinition* def)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Creating Scroller asset Runtime.");
        #endif
        removeScrollerRuntime();
        mScrollerRuntime = new ScrollerRuntime(def,this);
        return mScrollerRuntime->useDefinition();
    }

    bool
    ActorRuntime::createAnimationRuntime
    (AnimationDefinition* definition)
    {
        #ifdef DREAM_LOG
        getLog()->trace( "Creating Animation asset Runtime." );
        #endif
        removeAnimationRuntime();
        mAnimationRuntime = new AnimationRuntime(definition,this);
        return mAnimationRuntime->useDefinition();
    }

    bool
    ActorRuntime::createPathRuntime
    (PathDefinition* definition)
    {
        #ifdef DREAM_LOG
        getLog()->trace( "Creating Path asset Runtime." );
        #endif
        removePathRuntime();
        mPathRuntime = new PathRuntime(definition,this);
        return mPathRuntime->useDefinition();
    }

    bool
    ActorRuntime::createAudioRuntime
    (AudioDefinition* definition)
    {
        auto cache = mSceneRuntime->getProjectRuntime()->getAudioCache();
        if (cache != nullptr)
        {
            mAudioRuntime = static_cast<AudioRuntime*>(cache->getRuntime(definition));
            return mAudioRuntime != nullptr;
        }
        else
        {
            #ifdef DREAM_LOG
            getLog()->error("Cannot create AudioRuntime. AudioComponent is nullptr");
            #endif
        }
        return false;
    }

    bool
    ActorRuntime::createModelRuntime
    (ModelDefinition* definition)
    {
        removeModelRuntime();
        #ifdef DREAM_LOG
        getLog()->info("Creating Model asset Runtime.");
        #endif
        auto cache = mSceneRuntime->getProjectRuntime()->getModelCache();
        if (cache != nullptr)
        {
            mModelRuntime = static_cast<ModelRuntime*>(cache->getRuntime(definition));
            if (mModelRuntime != nullptr)
            {
                mModelRuntime->addRuntime(this);
            }
            else
            {
                #ifdef DREAM_LOG
                getLog()->error("Error getting model Runtime, cache returned nullptr");
                #endif
                return false;
            }
        }
        return mModelRuntime != nullptr;
    }

    bool
    ActorRuntime::createScriptRuntime
    (ScriptDefinition* definition)
    {
        removeScriptRuntime();
        #ifdef DREAM_LOG
        getLog()->trace("Creating Script asset Runtime.");
        #endif
        auto scriptCache = mSceneRuntime->getProjectRuntime()->getScriptCache();
        if (scriptCache)
        {
            mScriptRuntime = static_cast<ScriptRuntime*>(scriptCache->getRuntime(definition));
            if (mScriptRuntime)
            {
                mScriptOnInitTask.setScript(mScriptRuntime);
                mScriptOnUpdateTask.setScript(mScriptRuntime);
                mScriptOnEventTask.setScript(mScriptRuntime);
                mScriptOnDestroyTask = make_shared<ScriptOnDestroyTask>(mUuid,mParentRuntime);
                mScriptOnDestroyTask->setScript(mScriptRuntime);
                return true;
            }
            else
            {
                #ifdef DREAM_LOG
                getLog()->error("Error getting script Runtime, cache returned nullptr");
                #endif
                return false;
            }
        }
        #ifdef DREAM_LOG
        getLog()->error("Script cache is null");
        #endif
        return false;
    }

    bool
    ActorRuntime::createLightRuntime
    (LightDefinition* definition)
    {
        removeLightRuntime();
        #ifdef DREAM_LOG
        getLog()->trace( "Creating Light Asset Runtime." );
        #endif
        mLightRuntime = new LightRuntime(definition,this);
        return mLightRuntime->useDefinition();
    }

    bool
    ActorRuntime::createObjectEmitterRuntime
    (ObjectEmitterDefinition* definition)
    {
        removeObjectEmitterRuntime();
        #ifdef DREAM_LOG
        getLog()->trace( "Creating ObjectEmitter Asset Runtime." );
        #endif
        mObjectEmitterRuntime = new ObjectEmitterRuntime(definition,this);
        return mObjectEmitterRuntime->useDefinition();
    }

    ActorRuntime*
    ActorRuntime::getChildRuntimeByUuid
    (uint32_t uuid)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it != nullptr)
            {
                if ((*it)->hasUuid(uuid))
                {
                    return *it;
                }
            }
        }
        return nullptr;
    }

    vector<ActorRuntime*>
    ActorRuntime::getChildRuntimes
    ()
    {
        return mChildRuntimes;
    }

    bool
    ActorRuntime::isParentOf
    (ActorRuntime*  child)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) == child)
            {
                return true;
            }
        }
        return false;
    }

    void
    ActorRuntime::setParentRuntime
    (ActorRuntime* parent)
    {
        mParentRuntime = parent;
        setAttribute("parent",mParentRuntime->getUuidString());
    }

    ActorRuntime*
    ActorRuntime::getParentRuntime
    ()
    {
        return mParentRuntime;
    }

    SceneRuntime*
    ActorRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntime;
    }

    ActorDefinition*
    ActorRuntime::getActorDefinition()
    {
       return static_cast<ActorDefinition*>(getDefinition());
    }

    bool
    ActorRuntime::loadChildrenFromDefinition
    (ActorDefinition* definition)
    {
        vector<ActorDefinition*> definitions = definition->getChildDefinitionsList();
        for (auto it = begin(definitions); it != end(definitions); it++)
        {
            auto sod = (*it);
            if (sod->getIsTemplate())
            {
               static_cast<SceneDefinition*>(mSceneRuntime->getDefinition())->addTemplate(sod);
            }
            else
            {
                createAndAddChildRuntime(sod);
            }
        }
        return true;
    }

    void
    ActorRuntime::setTransform
    (Transform* transform)
    {
        mTransform.setMatrix(transform->getMatrix());
    }

    void
    ActorRuntime::translateWithChildren
    (const Vector3& translation)
    {
        applyToAll
        (
            function<ActorRuntime*(ActorRuntime*)>(
                [&](ActorRuntime* rt)
                {
                    rt->getTransform().translate(translation);
                    return static_cast<ActorRuntime*>(nullptr);
                }
            )
        );
    }

    void
    ActorRuntime::preTranslateWithChildren
    (const Vector3& translation)
    {
        applyToAll(
            function<ActorRuntime*(ActorRuntime*)>(
            [&](ActorRuntime* rt)
            {
                rt->getTransform().preTranslate(translation);
                return static_cast<ActorRuntime*>(nullptr);
            }
        ));
    }

    Transform
    ActorRuntime::getInitialTransform
    ()
    {
       return mInitialTransform;
    }

    void
    ActorRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        mTransform.setMatrix(matrix * mInitialTransform.getMatrix());
    }

    void
    ActorRuntime::translateOffsetInitial
    (const Vector3& tx)
    {
        mTransform.setMatrix(glm::translate(mat4(1.0),tx.toGLM())* mInitialTransform.getMatrix());
    }

    bool
    ActorRuntime::getHasCameraFocus
    () const
    {
        return mHasCameraFocus;
    }

    void
    ActorRuntime::setHasCameraFocus
    (bool cf)
    {
        mHasCameraFocus = cf;
    }

    bool
    ActorRuntime::getDeleted
    () const
    {
        return mDeleted;
    }

    void
    ActorRuntime::setDeleted
    (bool deleted)
    {
        mDeleted = deleted;
    }

    bool
    ActorRuntime::getHidden
    () const
    {
        return mHidden;
    }

    void
    ActorRuntime::setHidden
    (bool hidden)
    {
        mHidden = hidden;
    }

    void
    ActorRuntime::removeChildRuntime
    (ActorRuntime* child)
    {
        child->setDeleted(true);
    }

    void
    ActorRuntime::addChildRuntime
    (ActorRuntime* rt)
    {
        mChildRuntimes.push_back(rt);
    }

    ActorRuntime*
    ActorRuntime::createAndAddChildRuntime
    (ActorDefinition* def)
    {
        auto* child = new ActorRuntime(def, mSceneRuntime, mRandomUuid);
        child->setParentRuntime(this);
        if (!child->useDefinition())
        {
            #ifdef DREAM_LOG
            getLog()->error("Error creating child runtime");
            #endif
            delete child;
            return nullptr;
        }
        addChildRuntime(child);
        return child;
    }

    ActorRuntime*
    ActorRuntime::addChildFromTemplateUuid
    (uint32_t uuid)
    {
        auto sceneDef = static_cast<SceneDefinition*>(mSceneRuntime->getDefinition());
        auto def = sceneDef->getTemplateByUuid(uuid);
        if (def)
        {
            if (!def->getIsTemplate())
            {
                #ifdef DREAM_LOG
                getLog()->error("This SO is not a Template, too dangerous");
                #endif
                return nullptr;
            }
            auto* child = new ActorRuntime(def, mSceneRuntime, true);
            child->setParentRuntime(this);
            if (!child->useDefinition())
            {
                #ifdef DREAM_LOG
                getLog()->error("Error creating child runtime");
                #endif
                delete child;
                return nullptr;
            }
            addChildRuntime(child);
            #ifdef DREAM_LOG
            getLog()->error("Successfully added child from template {}",def->getNameAndUuidString());
            #endif
            return child;
        }
        #ifdef DREAM_LOG
        getLog()->error("Cannt create child, definition not found");
        #endif
        return nullptr;
    }

    bool
    ActorRuntime::useDefinition
    ()
    {
        if (mDefinition)
        {
            auto def = static_cast<ActorDefinition*>(mDefinition);
            #ifdef DREAM_LOG
            getLog()->trace( "Using Definition {}", def->getNameAndUuidString());
            #endif
            setName(def->getName());
            setUuid(mRandomUuid ? Uuid::generateUuid() : def->getUuid());
            setHasCameraFocus(def->getHasCameraFocus());
            setHidden(def->getHidden());
            initTransform();
            setAssetDefinitionsMap(def->getAssetDefinitionsMap());
            setDeferredFor(def->getDeferred());
            setDieAfter(def->getDieAfter());
            if (mDeferredFor == 0)
            {
                return loadDeferred();
            }
            return true;
        }
        return false;
    }

    bool
    ActorRuntime::loadDeferred
    ()
    {
        if (!createAssetRuntimes())
        {
            return false;
        }
        if(!loadChildrenFromDefinition(static_cast<ActorDefinition*>(mDefinition)))
        {
            return false;
        }
        return true;
    }

    BoundingBox&
    ActorRuntime::getBoundingBox
    ()
    {
        if (mModelRuntime != nullptr)
        {
            return mModelRuntime->getBoundingBox();
        }
        return mBoundingBox;
    }

    void
    ActorRuntime::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        mBoundingBox = boundingBox;
    }

    float
    ActorRuntime::distanceFrom
    (ActorRuntime* other)
    {
        return mTransform.distanceFrom(other->getTransform());
    }

    float
    ActorRuntime::distanceFrom
    (const Vector3& other)
    {
        return glm::distance(vec3(mTransform.getMatrix()[3]),other.toGLM());
    }

    bool
    ActorRuntime::visibleInFrustum
    ()
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->visibleInFrustum(this);
        }
        return false;
    }

    bool
    ActorRuntime::containedInFrustum
    ()
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->containedInFrustum(this);
        }
        return false;
    }

    bool
    ActorRuntime::containedInFrustumAfterTransform
    (const mat4& tx)
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
           return cam->containedInFrustumAfterTransform(this,tx);
        }
        return false;

    }

    bool
    ActorRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const Vector3& tx)
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->exceedsFrustumPlaneAtTranslation(plane,this,tx);
        }
        return false;
    }

    bool
    ActorRuntime::hasScrollerRuntime
    ()
    {
        return mScrollerRuntime != nullptr;
    }

    bool
    ActorRuntime::applyToAll
    (const function<bool(ActorRuntime*)>& fn)
    {
        bool retval = fn(this);

        size_t n = mChildRuntimes.size();
        for (size_t i=0; i<n; i++ )
        {
            auto rt = mChildRuntimes.at(i);
            if (rt)
            {
                retval = retval || (rt)->applyToAll(fn);
            }
        }
        return retval;
    }

    ActorRuntime*
    ActorRuntime::applyToAll
    (const function<ActorRuntime*(ActorRuntime*)>& fn)
    {
        ActorRuntime* retval = fn(this);

        if (retval != nullptr)
        {
            return retval;
        }

        size_t n = mChildRuntimes.size();
        for (size_t i=0; i<n; i++ )
        {
            auto rt = mChildRuntimes.at(i);
            if (rt)
            {
                retval = (rt)->applyToAll(fn);
                if (retval != nullptr)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    void
    ActorRuntime::translateOffsetInitialWithChildren
    (const Vector3& translation)
    {
        static mat4 ident(1.0f);
        applyToAll
        (function<ActorRuntime*(ActorRuntime*)>([&](ActorRuntime* rt)
        {
            auto initial = rt->getInitialTransform().getMatrix();
            rt->getTransform().setMatrix(glm::translate(ident,translation.toGLM())* initial);
            return static_cast<ActorRuntime*>(nullptr);
        }
        ));
    }

    long
    ActorRuntime::getDeferredFor
    ()
    const
    {
        return mDeferredFor;
    }

    void
    ActorRuntime::setDeferredFor
    (long deferred)
    {
        mDeferredFor = deferred;
    }

    long
    ActorRuntime::getObjectLifetime
    ()
    const
    {
       return mObjectLifetime;
    }

    void
    ActorRuntime::setObjectLifetime
    (long d)
    {
        mObjectLifetime = d;
    }

    void
    ActorRuntime::setDieAfter
    (long d)
    {
        mDieAfter = d;
    }

    long
    ActorRuntime::getDieAfter
    ()
    const
    {
        return mDieAfter;
    }

    ScriptOnInitTask*
    ActorRuntime::getScriptOnInitTask
    ()
    {
       return &mScriptOnInitTask;
    }

    ScriptOnEventTask*
    ActorRuntime::getScriptOnEventTask
    ()
    {
       return &mScriptOnEventTask;
    }

    ScriptOnUpdateTask*
    ActorRuntime::getScriptOnUpdateTask
    ()
    {
        return &mScriptOnUpdateTask;
    }
    void
    ActorRuntime::updateLifetime
    ()
    {
        long timeDelta = getSceneRuntime()->getProjectRuntime()->getTime()->getFrameTimeDelta();
        if (mDeferredFor > 0)
        {
            long deferral = mDeferredFor-timeDelta;
            #ifdef DREAM_LOG
            getLog()->critical("Reducing defferal by {} to {} for {}", timeDelta, deferral, getNameAndUuidString());
            #endif
            setDeferredFor(deferral);
            if (deferral < 0)
            {
                #ifdef DREAM_LOG
                getLog()->critical("Loading Deferred Runtime {}", getNameAndUuidString());
                #endif
                loadDeferred();
            }
        }
        else
        {
            mObjectLifetime += timeDelta;
            if (mDieAfter > 0 && mObjectLifetime > mDieAfter)
            {
                setDeleted(true);
            }
        }
    }

    string
    ActorRuntime::getAttribute
    (const string& key)
    const
    {
       if (mAttributes.count(key) > 0)
       {
           return mAttributes.at(key);
       }
       return "";
    }

    void
    ActorRuntime::setAttribute
    (const string& key, const string& value)
    {
       mAttributes[key] = value;
    }

    const map<string,string>&
    ActorRuntime::getAttributesMap
    ()
    const
    {
        return mAttributes;
    }

    bool
    ActorRuntime::isPlayerObject
    ()
    const
    {
        return mSceneRuntime->getPlayerObject() == this;
    }
}
