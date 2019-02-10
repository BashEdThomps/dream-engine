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
#include "SceneObjectRuntime.h"
#include "SceneObjectDefinition.h"
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
#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"
#include "../../Utilities/Uuid.h"
#include "../../TaskManager/TaskManager.h"
#include "../../Components/Script/ScriptTasks.h"
#include "SceneObjectTasks.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

using std::vector;
using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(
        SceneObjectDefinition* sd,
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
        mBoundingBox(),
        mHasCameraFocus(false),
        mDeleted(false),
        mHidden(false),
        mAlwaysDraw(false),
        mRandomUuid(randomUuid),
        mDeferredFor(0),
        mObjectLifetime(0),
        mDieAfter(0),
        mLifetimeUpdateTask(this),
        mScriptOnInitTask(this),
        mScriptOnUpdateTask(this),
        mScriptOnEventTask(this)
    {
        #ifdef DREAM_LOG
        setLogClassName("SceneObjectRuntime");
        getLog()->trace( "Constructing Object" );
        #endif
        if (mRandomUuid)
        {
            mUuid = Uuid::generateUuid();
        }
    }

    SceneObjectRuntime::~SceneObjectRuntime
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
    }

    void
    SceneObjectRuntime::removeAnimationRuntime
    ()
    {
        if (mAnimationRuntime != nullptr)
        {
            delete mAnimationRuntime;
            mAnimationRuntime = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeAudioRuntime
    ()
    {
        mAudioRuntime = nullptr;
    }

    void
    SceneObjectRuntime::removePathRuntime
    ()
    {
        if (mPathRuntime != nullptr)
        {
            delete mPathRuntime;
            mPathRuntime = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeModelRuntime
    ()
    {
        if (mModelRuntime != nullptr)
        {
            mModelRuntime->removeRuntime(this);
        }
    }

    void
    SceneObjectRuntime::removeLightRuntime
    ()
    {
        if (mLightRuntime != nullptr)
        {
            delete mLightRuntime;
            mLightRuntime = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeScriptRuntime
    ()
    {
        //delete mScriptRuntime;
        mScriptRuntime = nullptr;
    }

    void
    SceneObjectRuntime::removePhysicsObjectRuntime
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

    void SceneObjectRuntime::removeParticleEmitterRuntime()
    {
        if (mParticleEmitterRuntime != nullptr)
        {
            delete mParticleEmitterRuntime;
            mParticleEmitterRuntime = nullptr;
        }
    }

    void SceneObjectRuntime::removeScrollerRuntime()
    {
        if (mScrollerRuntime != nullptr)
        {
            delete mScrollerRuntime;
            mScrollerRuntime = nullptr;
        }
    }

    AnimationRuntime*
    SceneObjectRuntime::getAnimationRuntime
    ()
    {
        return mAnimationRuntime;
    }

    PathRuntime*
    SceneObjectRuntime::getPathRuntime
    ()
    {
        return mPathRuntime;
    }

    AudioRuntime*
    SceneObjectRuntime::getAudioRuntime
    ()
    {
        return mAudioRuntime;
    }

    ModelRuntime*
    SceneObjectRuntime::getModelRuntime
    ()
    {
        return mModelRuntime;
    }

    ScriptRuntime*
    SceneObjectRuntime::getScriptRuntime
    ()
    {
        return mScriptRuntime;
    }

    LightRuntime*
    SceneObjectRuntime::getLightRuntime
    ()
    {
        return mLightRuntime;
    }

    ParticleEmitterRuntime*
    SceneObjectRuntime::getParticleEmitterRuntime
    ()
    {
       return mParticleEmitterRuntime;
    }

    ScrollerRuntime*
    SceneObjectRuntime::getScrollerRuntime
    ()
    {
        return mScrollerRuntime;
    }

    AssetRuntime*
    SceneObjectRuntime::getAssetRuntime
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
           default:
               break;
       }
       return nullptr;
    }

    bool
    SceneObjectRuntime::hasAnimationRuntime
    ()
    {
        return mAnimationRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::hasLightRuntime
    ()
    {
        return mLightRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::hasModelRuntime
    ()
    {
        return mModelRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::hasScriptRuntime
    ()
    {
        return mScriptRuntime != nullptr;
    }

    void
    SceneObjectRuntime::setAssetDefinitionsMap
    (const map<AssetType,uint32_t>& assetMap)
    {
        mAssetDefinitions = assetMap;
    }

    map<AssetType,uint32_t>
    SceneObjectRuntime::getAssetDefinitionsMap
    ()
    {
        return mAssetDefinitions;
    }

    bool
    SceneObjectRuntime::getAlwaysDraw
    () const
    {
        return mAlwaysDraw;
    }

    void
    SceneObjectRuntime::setAlwaysDraw
    (bool alwaysDraw)
    {
        mAlwaysDraw = alwaysDraw;
    }

    PhysicsObjectRuntime*
    SceneObjectRuntime::getPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime;
    }

    Transform*
    SceneObjectRuntime::getTransform
    ()
    {
        return &mTransform;
    }

    void
    SceneObjectRuntime::initTransform
    ()
    {
        if (mDefinition)
        {
            auto definedTransform = static_cast<SceneObjectDefinition*>(mDefinition)->getTransform();
            mInitialTransform.setMatrix(definedTransform.getMatrix());
            mTransform.setMatrix(definedTransform.getMatrix());
        }
    }

    bool
    SceneObjectRuntime::hasEvents
    ()
    const
    {
        return !mEventQueue.empty();
    }

    void
    SceneObjectRuntime::addEvent
    (const Event& event)
    {
        #ifdef DREAM_LOG
        getLog()->trace
        (
            "Event posted from {} to {}",
            event.getSender()->getNameAndUuidString(),
            getNameAndUuidString()
        );
        #endif
        mEventQueue.push_back(event);
    }

    vector<Event>&
    SceneObjectRuntime::getEventQueue
    ()
    {
        return mEventQueue;
    }

    void
    SceneObjectRuntime::clearEventQueue
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Clearing event queue");
        #endif
        mEventQueue.clear();
    }

    void
    SceneObjectRuntime::collectGarbage
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Collecting Garbage {}" ,getNameAndUuidString());
        #endif

        vector<SceneObjectRuntime*> toDelete;

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
    }

    bool
    SceneObjectRuntime::hasPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::hasPathRuntime
    ()
    {
        return mPathRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::hasAudioRuntime
    ()
    {
        return mAudioRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::createAssetRuntimes
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
    SceneObjectRuntime::getAssetDefinitionByUuid
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
    SceneObjectRuntime::replaceAssetUuid
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
            default:
                return false;
        }
    }

    bool
    SceneObjectRuntime::createPhysicsObjectRuntime
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
    SceneObjectRuntime::createParticleEmitterRuntime
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
    SceneObjectRuntime::createScrollerRuntime
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
    SceneObjectRuntime::createAnimationRuntime
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
    SceneObjectRuntime::createPathRuntime
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
    SceneObjectRuntime::createAudioRuntime
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
    SceneObjectRuntime::createModelRuntime
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
    SceneObjectRuntime::createScriptRuntime
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
    SceneObjectRuntime::createLightRuntime
    (LightDefinition* definition)
    {
        removeLightRuntime();
        #ifdef DREAM_LOG
        getLog()->trace( "Creating Light Asset Runtime." );
        #endif
        mLightRuntime = new LightRuntime(definition,this);
        return mLightRuntime->useDefinition();
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getChildRuntimeByUuid
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

    vector<SceneObjectRuntime*>
    SceneObjectRuntime::getChildRuntimes
    ()
    {
        return mChildRuntimes;
    }

    bool
    SceneObjectRuntime::isParentOf
    (SceneObjectRuntime*  child)
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
    SceneObjectRuntime::setParentRuntime
    (SceneObjectRuntime* parent)
    {
        mParentRuntime = parent;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getParentRuntime
    ()
    {
        return mParentRuntime;
    }

    SceneRuntime*
    SceneObjectRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntime;
    }

    SceneObjectDefinition*
    SceneObjectRuntime::getSceneObjectDefinition()
    {
       return static_cast<SceneObjectDefinition*>(getDefinition());
    }


    bool
    SceneObjectRuntime::loadChildrenFromDefinition
    (SceneObjectDefinition* definition)
    {
        vector<SceneObjectDefinition*> definitions = definition->getChildDefinitionsList();
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
    SceneObjectRuntime::setTransform
    (Transform* transform)
    {
        mTransform.setMatrix(transform->getMatrix());
    }

    void
    SceneObjectRuntime::translateWithChildren
    (const vec3& translation)
    {
        applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
                [&](SceneObjectRuntime* rt)
                {
                    rt->getTransform()->translate(translation);
                    return static_cast<SceneObjectRuntime*>(nullptr);
                }
            )
        );
    }

    void
    SceneObjectRuntime::preTranslateWithChildren
    (const vec3& translation)
    {
        applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime* rt)
            {
                rt->getTransform()->preTranslate(translation);
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    Transform*
    SceneObjectRuntime::getInitialTransform
    ()
    {
       return &mInitialTransform;
    }

    void
    SceneObjectRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        mTransform.setMatrix(matrix * mInitialTransform.getMatrix());
    }

    void
    SceneObjectRuntime::translateOffsetInitial
    (const vec3& tx)
    {
        mTransform.setMatrix(glm::translate(mat4(1.0),tx)* mInitialTransform.getMatrix());
    }

    bool
    SceneObjectRuntime::getHasCameraFocus
    () const
    {
        return mHasCameraFocus;
    }

    void
    SceneObjectRuntime::setHasCameraFocus
    (bool cf)
    {
        mHasCameraFocus = cf;
    }

    bool
    SceneObjectRuntime::getDeleted
    () const
    {
        return mDeleted;
    }

    void
    SceneObjectRuntime::setDeleted
    (bool deleted)
    {
        mDeleted = deleted;
    }

    bool
    SceneObjectRuntime::getHidden
    () const
    {
        return mHidden;
    }

    void
    SceneObjectRuntime::setHidden
    (bool hidden)
    {
        mHidden = hidden;
    }

    void
    SceneObjectRuntime::removeChildRuntime
    (SceneObjectRuntime* child)
    {
        child->setDeleted(true);
    }

    void
    SceneObjectRuntime::addChildRuntime
    (SceneObjectRuntime* rt)
    {
        mChildRuntimes.push_back(rt);
    }

    SceneObjectRuntime*
    SceneObjectRuntime::createAndAddChildRuntime
    (SceneObjectDefinition* def)
    {
        auto* child = new SceneObjectRuntime(def, mSceneRuntime, mRandomUuid);
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

    SceneObjectRuntime*
    SceneObjectRuntime::addChildFromTemplateUuid
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
            auto* child = new SceneObjectRuntime(def, mSceneRuntime, true);
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
    SceneObjectRuntime::useDefinition
    ()
    {
        if (mDefinition)
        {
            auto def = static_cast<SceneObjectDefinition*>(mDefinition);
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
    SceneObjectRuntime::loadDeferred
    ()
    {
        if (!createAssetRuntimes())
        {
            return false;
        }
        if(!loadChildrenFromDefinition(static_cast<SceneObjectDefinition*>(mDefinition)))
        {
            return false;
        }
        return true;
    }

    BoundingBox&
    SceneObjectRuntime::getBoundingBox
    ()
    {
        if (mModelRuntime != nullptr)
        {
            return mModelRuntime->getBoundingBox();
        }
        return mBoundingBox;
    }

    void
    SceneObjectRuntime::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        mBoundingBox = boundingBox;
    }

    float
    SceneObjectRuntime::distanceFrom
    (SceneObjectRuntime* other)
    {
        return mTransform.distanceFrom(*other->getTransform());
    }

    float
    SceneObjectRuntime::distanceFrom
    (const vec3& other)
    {
        return glm::distance(vec3(mTransform.getMatrix()[3]),other);
    }

    bool
    SceneObjectRuntime::visibleInFrustum
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
    SceneObjectRuntime::containedInFrustum
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
    SceneObjectRuntime::containedInFrustumAfterTransform
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
    SceneObjectRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const vec3& tx)
    {
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->exceedsFrustumPlaneAtTranslation(plane,this,tx);
        }
        return false;
    }

    bool
    SceneObjectRuntime::hasScrollerRuntime
    ()
    {
        return mScrollerRuntime != nullptr;
    }

    bool
    SceneObjectRuntime::applyToAll
    (const function<bool(SceneObjectRuntime*)>& fn)
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

    SceneObjectRuntime*
    SceneObjectRuntime::applyToAll
    (const function<SceneObjectRuntime*(SceneObjectRuntime*)>& fn)
    {
        SceneObjectRuntime* retval = fn(this);

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
    SceneObjectRuntime::translateOffsetInitialWithChildren
    (const vec3& translation)
    {
        static mat4 ident(1.0f);
        applyToAll
        (function<SceneObjectRuntime*(SceneObjectRuntime*)>([&](SceneObjectRuntime* rt)
        {
            auto initial = rt->getInitialTransform()->getMatrix();
            rt->getTransform()->setMatrix(glm::translate(ident,translation)* initial);
            return static_cast<SceneObjectRuntime*>(nullptr);
        }
        ));
    }

    long
    SceneObjectRuntime::getDeferredFor
    ()
    const
    {
        return mDeferredFor;
    }

    void
    SceneObjectRuntime::setDeferredFor
    (long deferred)
    {
        mDeferredFor = deferred;
    }

    long
    SceneObjectRuntime::getObjectLifetime
    ()
    const
    {
       return mObjectLifetime;
    }

    void
    SceneObjectRuntime::setObjectLifetime
    (long d)
    {
        mObjectLifetime = d;
    }

    void
    SceneObjectRuntime::increaseLifetime
    (long l)
    {
        mObjectLifetime += l;
        if (mDieAfter > 0 && mObjectLifetime > mDieAfter)
        {
            setDeleted(true);
        }
    }

    void
    SceneObjectRuntime::setDieAfter
    (long d)
    {
        mDieAfter = d;
    }

    long
    SceneObjectRuntime::getDieAfter
    ()
    const
    {
        return mDieAfter;
    }

    LifetimeUpdateTask*
    SceneObjectRuntime::getLifetimeUpdateTask
    ()
    {
        return &mLifetimeUpdateTask;
    }

    ScriptOnInitTask*
    SceneObjectRuntime::getScriptOnInitTask
    ()
    {
       return &mScriptOnInitTask;
    }

    ScriptOnEventTask*
    SceneObjectRuntime::getScriptOnEventTask
    ()
    {
       return &mScriptOnEventTask;
    }

    ScriptOnUpdateTask*
    SceneObjectRuntime::getScriptOnUpdateTask
    ()
    {
        return &mScriptOnUpdateTask;
    }
}
