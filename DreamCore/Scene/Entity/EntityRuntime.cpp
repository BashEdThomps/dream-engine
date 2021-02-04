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
#include "EntityRuntime.h"

#include "EntityDefinition.h"
#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Components/Event.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Audio/AudioCache.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Light/LightRuntime.h"
#include "Components/Graphics/Font/FontRuntime.h"
#include "Components/Graphics/Sprite/SpriteDefinition.h"
#include "Components/Graphics/Sprite/SpriteRuntime.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Font/FontCache.h"
#include "Components/Graphics/Light/LightDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptComponent.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Project/ProjectDefinition.h"
#include "Common/Uuid.h"
#include "Components/Task/TaskManager.h"
#include "Components/Script/ScriptTasks.h"
#include "Components/Time.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

using std::vector;
using std::make_shared;


namespace octronic::dream
{
    EntityRuntime::EntityRuntime(EntityDefinition* sd,SceneRuntime* sr,bool randomUuid)
        : Runtime("EntityRuntime",sd),
          mAnimationRuntime(nullptr),
          mAudioRuntime(nullptr),
          mLightRuntime(nullptr),
          mPathRuntime(nullptr),
          mPhysicsObjectRuntime(nullptr),
          mScriptRuntime(nullptr),
          mScriptInitialised(false),
          mScriptError(false),
          mModelRuntime(nullptr),
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
          mScriptConstructionTask(this),
          mScriptOnInitTask(this),
          mScriptOnUpdateTask(this),
          mScriptOnEventTask(this),
          mScriptDestructionTask(nullptr),
          mSpriteRuntime(nullptr),
          mFontRuntime(nullptr),
          mFontText(""),
          mFontColor(1.f),
          mFontScale(1.f)
    {
        LOG_TRACE( "EntityRuntime: Constructing Object" );

        mEventQueueMutex.lock();
        mEventQueue.reserve(10);
        mEventQueueMutex.unlock();

        if (mRandomUuid)
        {
            mUuid = Uuid::generateUuid();
        }

        if (static_cast<SceneDefinition*>(mSceneRuntime->getDefinition())->getPlayerObject() == mUuid)
        {
            mSceneRuntime->setPlayerEntity(this);
        }

        setAttribute("uuid",getUuidString());
    }

    EntityRuntime::~EntityRuntime
    ()
    {
        LOG_TRACE( "EntityRuntime: Destroying Object" );

        for (auto child : mChildRuntimes)
        {
            delete child;
        }
        mChildRuntimes.clear();

        removeAnimationRuntime();
        removeAudioRuntime();
        removeFontRuntime();
        removeLightRuntime();
        removeModelRuntime();
        removePathRuntime();
        removePhysicsObjectRuntime();
        removeScriptRuntime();
        removeSpriteRuntime();
    }

    void
    EntityRuntime::removeAnimationRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mAnimationRuntime != nullptr)
            {
                delete mAnimationRuntime;
                mAnimationRuntime = nullptr;
            }
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeAudioRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            mAudioRuntime = nullptr;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removePathRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mPathRuntime != nullptr)
            {
                delete mPathRuntime;
                mPathRuntime = nullptr;
            }
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeModelRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mModelRuntime != nullptr)
            {
                mModelRuntime->removeRuntime(this);
            }
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeLightRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mLightRuntime != nullptr)
            {
                delete mLightRuntime;
                mLightRuntime = nullptr;
            }
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeFontRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mFontRuntime != nullptr)
            {
                mFontRuntime->popInstance(this);
                mFontRuntime = nullptr;
            }
        } dreamElseLockFailed
    }


    void
    EntityRuntime::removeScriptRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mScriptRuntime)
            {
                mSceneRuntime
                        ->getProjectRuntime()
                        ->getTaskManager()
                        ->pushDestructionTask(mScriptDestructionTask);
            }
            mScriptRuntime = nullptr;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removePhysicsObjectRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

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
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeSpriteRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mSpriteRuntime != nullptr)
            {
                delete mSpriteRuntime;
                mSpriteRuntime = nullptr;
            }
        } dreamElseLockFailed
    }

    SpriteRuntime*
    EntityRuntime::getSpriteRuntime()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mSpriteRuntime;
        } dreamElseLockFailed
    }

    FontRuntime*
    EntityRuntime::getFontRuntime()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mFontRuntime;
        } dreamElseLockFailed
    }

    AnimationRuntime*
    EntityRuntime::getAnimationRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mAnimationRuntime;
        } dreamElseLockFailed
    }

    PathRuntime*
    EntityRuntime::getPathRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mPathRuntime;
        } dreamElseLockFailed
    }

    AudioRuntime*
    EntityRuntime::getAudioRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mAudioRuntime;
        } dreamElseLockFailed
    }

    ModelRuntime*
    EntityRuntime::getModelRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mModelRuntime;
        } dreamElseLockFailed
    }

    ScriptRuntime*
    EntityRuntime::getScriptRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mScriptRuntime;
        } dreamElseLockFailed
    }

    LightRuntime*
    EntityRuntime::getLightRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mLightRuntime;
        } dreamElseLockFailed
    }

    AssetRuntime*
    EntityRuntime::getAssetRuntime
    (AssetType type)
    {
        if(dreamTryLock()) {
            dreamLock();

            switch (type)
            {
                case ASSET_TYPE_ENUM_ANIMATION:
                    return getAnimationRuntime();
                case ASSET_TYPE_ENUM_PATH:
                    return getPathRuntime();
                case ASSET_TYPE_ENUM_AUDIO:
                    return getAudioRuntime();
                case ASSET_TYPE_ENUM_FONT:
                    return getFontRuntime();
                case ASSET_TYPE_ENUM_LIGHT:
                    return getLightRuntime();
                case ASSET_TYPE_ENUM_MODEL:
                    return getModelRuntime();
                case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                    return getPhysicsObjectRuntime();
                case ASSET_TYPE_ENUM_SCRIPT:
                    return getScriptRuntime();
                case ASSET_TYPE_ENUM_SPRITE:
                    return getSpriteRuntime();

                default:
                    break;
            }
            return nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasSpriteRuntime()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mSpriteRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasFontRuntime()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mFontRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasAnimationRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mAnimationRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasLightRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mLightRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasModelRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mModelRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasScriptRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mScriptRuntime != nullptr;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::setAssetDefinitionsMap
    (const map<AssetType,UuidType>& assetMap)
    {
        if(dreamTryLock()) {
            dreamLock();
            mAssetDefinitions = assetMap;
        } dreamElseLockFailed
    }

    map<AssetType,UuidType>
    EntityRuntime::getAssetDefinitionsMap
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mAssetDefinitions;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::getAlwaysDraw
    () const
    {
        return mAlwaysDraw;
    }

    void
    EntityRuntime::setAlwaysDraw
    (bool alwaysDraw)
    {
        if(dreamTryLock()) {
            dreamLock();
            mAlwaysDraw = alwaysDraw;
        } dreamElseLockFailed
    }

    PhysicsObjectRuntime*
    EntityRuntime::getPhysicsObjectRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mPhysicsObjectRuntime;
        } dreamElseLockFailed
    }

    Transform&
    EntityRuntime::getTransform
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mTransform;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::initTransform
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mDefinition)
            {
                auto definedTransform = static_cast<EntityDefinition*>(mDefinition)->getTransform();
                mInitialTransform.setMatrix(definedTransform.getMatrix());
                mTransform.setMatrix(definedTransform.getMatrix());
            }
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasEvents
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            bool retval = false;
            mEventQueueMutex.lock();
            retval = !mEventQueue.empty();
            mEventQueueMutex.unlock();
            return retval;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::addEvent
    (const Event& event)
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mDeleted)
            {
                LOG_TRACE("EntityRuntime: Event posted from {} to {}",
                          event.getAttribute("uuid"), getNameAndUuidString());
                mEventQueueMutex.lock();
                mEventQueue.push_back(std::move(event));
                mEventQueueMutex.unlock();
            }
        } dreamElseLockFailed
    }

    bool EntityRuntime::tryLockEventQueue()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mEventQueueMutex.try_lock();
        } dreamElseLockFailed
    }

    void EntityRuntime::unlockEventQueue()
    {
        if(dreamTryLock()) {
            dreamLock();
            mEventQueueMutex.unlock();
        } dreamElseLockFailed
    }

    vector<Event>*
    EntityRuntime::getEventQueue
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mEventQueue;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::clearEventQueue
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("EntityRuntime: Clearing event queue");

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
        } dreamElseLockFailed
    }

    void
    EntityRuntime::collectGarbage
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("EntityRuntime: Collecting Garbage {}" ,getNameAndUuidString());

            vector<EntityRuntime*> toDelete;

            for (auto child : mChildRuntimes)
            {
                if (child->getDeleted())
                {
                    toDelete.push_back(child);
                }
            }

            for (auto child : toDelete)
            {
                LOG_TRACE("EntityRuntime: Deleting child {}",child->getNameAndUuidString());
                mChildRuntimes.erase(
                            find(mChildRuntimes.begin(), mChildRuntimes.end(), child)
                            );
                delete child;
            }

            clearEventQueue();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasPhysicsObjectRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mPhysicsObjectRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasPathRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mPathRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::hasAudioRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mAudioRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createAssetRuntimes
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            for (auto assetPair : mAssetDefinitions)
            {
                AssetDefinition* def = getAssetDefinitionByUuid(assetPair.second);
                bool result = false;
                if (def == nullptr)
                {
                    LOG_ERROR("EntityRuntime: Could not find asset definition {}", assetPair.second);
                    continue;
                }
                LOG_TRACE("EntityRuntime: Creating {}",def->getNameAndUuidString());
                switch (assetPair.first)
                {
                    case AssetType::ASSET_TYPE_ENUM_ANIMATION:
                        result = createAnimationRuntime(static_cast<AnimationDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_AUDIO:
                        result = createAudioRuntime(static_cast<AudioDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_FONT:
                        result = createFontRuntime(static_cast<FontDefinition*>(def));
                    case AssetType::ASSET_TYPE_ENUM_LIGHT:
                        result = createLightRuntime(static_cast<LightDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_MODEL:
                        result = createModelRuntime(static_cast<ModelDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_PATH:
                        result = createPathRuntime(static_cast<PathDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                        result = createPhysicsObjectRuntime(static_cast<PhysicsObjectDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_SCRIPT:
                        result = createScriptRuntime(static_cast<ScriptDefinition*>(def));
                        break;
                    case AssetType::ASSET_TYPE_ENUM_SPRITE:
                        result = createSpriteRuntime(static_cast<SpriteDefinition*>(def));
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
        } dreamElseLockFailed
    }

    AssetDefinition*
    EntityRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto project = mSceneRuntime->getProjectRuntime()->getProject();
            if (project == nullptr)
            {
                LOG_ERROR("EntityRuntime: Project is not found");
                return nullptr;
            }
            auto assetDefinition = project->getDefinition()->getAssetDefinitionByUuid(uuid);
            if (assetDefinition == nullptr)
            {
                LOG_ERROR("EntityRuntime: AssetDefinition not found");
            }
            return assetDefinition;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::replaceAssetUuid
    (AssetType type, UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_INFO("EntityRuntime: REPLACING asset Runtime from uuid {}", uuid);
            auto project = mSceneRuntime->getProjectRuntime()->getProject();
            if (project == nullptr)
            {
                LOG_ERROR("EntityRuntime: Project is not found");
                return false;
            }
            auto def = project->getDefinition()->getAssetDefinitionByUuid(uuid);
            if (def == nullptr)
            {
                LOG_ERROR("EntityRuntime: AssetDefinition not found");
            }
            switch (type)
            {
                case AssetType::ASSET_TYPE_ENUM_ANIMATION:
                    return createAnimationRuntime(static_cast<AnimationDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_AUDIO:
                    return createAudioRuntime(static_cast<AudioDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_FONT:
                    return createFontRuntime(static_cast<FontDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_LIGHT:
                    return createLightRuntime(static_cast<LightDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_MODEL:
                    return createModelRuntime(static_cast<ModelDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_PATH:
                    return createPathRuntime(static_cast<PathDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                    return createPhysicsObjectRuntime(static_cast<PhysicsObjectDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_SCRIPT:
                    return createScriptRuntime(static_cast<ScriptDefinition*>(def));
                case AssetType::ASSET_TYPE_ENUM_SPRITE:
                    return createSpriteRuntime(static_cast<SpriteDefinition*>(def));
                default:
                    return false;
            }
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createPhysicsObjectRuntime
    (PhysicsObjectDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            removePhysicsObjectRuntime();
            LOG_TRACE( "EntityRuntime: Creating Physics Object Asset Runtime." );
            mPhysicsObjectRuntime = new PhysicsObjectRuntime(
                        definition,
                        mSceneRuntime->getProjectRuntime()->getPhysicsComponent(),
                        mSceneRuntime->getProjectRuntime()->getModelCache(),
                        this
                        );
            return mPhysicsObjectRuntime->useDefinition();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createAnimationRuntime
    (AnimationDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE( "EntityRuntime: Creating Animation asset Runtime." );
            removeAnimationRuntime();
            mAnimationRuntime = new AnimationRuntime(definition,this);
            return mAnimationRuntime->useDefinition();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createSpriteRuntime
    (SpriteDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();

            LOG_TRACE( "EntityRuntime: Creating Sprite asset Runtime." );
            removeSpriteRuntime();
            mSpriteRuntime = new SpriteRuntime(definition,this);
            return mSpriteRuntime->useDefinition();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createPathRuntime
    (PathDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE( "EntityRuntime: Creating Path asset Runtime." );
            removePathRuntime();
            mPathRuntime = new PathRuntime(definition,this);
            return mPathRuntime->useDefinition();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createAudioRuntime
    (AudioDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto cache = mSceneRuntime->getProjectRuntime()->getAudioCache();
            if (cache != nullptr)
            {
                mAudioRuntime = static_cast<AudioRuntime*>(cache->getRuntime(definition));
                return mAudioRuntime != nullptr;
            }
            else
            {
                LOG_ERROR("EntityRuntime: Cannot create AudioRuntime. AudioComponent is nullptr");
            }
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createModelRuntime
    (ModelDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            removeModelRuntime();
            LOG_INFO("EntityRuntime: Creating Model asset Runtime.");
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
                    LOG_ERROR("EntityRuntime: Error getting model Runtime, cache returned nullptr");
                    return false;
                }
            }
            return mModelRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createScriptRuntime
    (ScriptDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            removeScriptRuntime();
            LOG_TRACE("EntityRuntime: Creating Script asset Runtime.");
            auto scriptCache = mSceneRuntime->getProjectRuntime()->getScriptCache();
            if (scriptCache)
            {
                mScriptRuntime = static_cast<ScriptRuntime*>(scriptCache->getRuntime(definition));
                if (mScriptRuntime)
                {
                    mScriptConstructionTask.setScript(mScriptRuntime);
                    mScriptOnInitTask.setScript(mScriptRuntime);
                    mScriptOnUpdateTask.setScript(mScriptRuntime);
                    mScriptOnEventTask.setScript(mScriptRuntime);
                    mScriptDestructionTask = make_shared<EntityScriptDestructionTask>(mUuid);
                    mScriptDestructionTask->setScript(mScriptRuntime);
                    return true;
                }
                else
                {
                    LOG_ERROR("EntityRuntime: Error getting script Runtime, cache returned nullptr");
                    return false;
                }
            }
            LOG_ERROR("EntityRuntime: Script cache is null");
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createLightRuntime
    (LightDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            removeLightRuntime();
            LOG_TRACE( "EntityRuntime: Creating Light Asset Runtime." );
            mLightRuntime = new LightRuntime(definition,this);
            return mLightRuntime->useDefinition();
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::createFontRuntime
    (FontDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            removeFontRuntime();
            LOG_TRACE( "EntityRuntime: Creating Font Asset Runtime." );
            auto fontCache = mSceneRuntime->getProjectRuntime()->getFontCache();
            mFontRuntime = static_cast<FontRuntime*>(fontCache->getRuntime(definition));
            if (mFontRuntime == nullptr)
            {
                LOG_ERROR("EntityRuntime: Error creating Font runtime");
            }
            EntityDefinition* entityDef = static_cast<EntityDefinition*>(mDefinition);
            mFontColor = entityDef->getFontColor();
            mFontScale = entityDef->getFontScale();
            mFontText = entityDef->getFontText();
            mFontRuntime->pushInstance(this);
            return mFontRuntime != nullptr;
        } dreamElseLockFailed
    }

    EntityRuntime*
    EntityRuntime::getChildRuntimeByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    vector<EntityRuntime*>
    EntityRuntime::getChildRuntimes
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mChildRuntimes;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::isParentOf
    (EntityRuntime*  child)
    {
        if(dreamTryLock()) {
            dreamLock();
            for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
            {
                if ((*it) == child)
                {
                    return true;
                }
            }
            return false;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::setParentRuntime
    (EntityRuntime* parent)
    {
        if(dreamTryLock()) {
            dreamLock();
            mParentRuntime = parent;
            setAttribute("parent",mParentRuntime->getUuidString());
        } dreamElseLockFailed
    }

    EntityRuntime*
    EntityRuntime::getParentRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mParentRuntime;
        } dreamElseLockFailed
    }

    SceneRuntime*
    EntityRuntime::getSceneRuntime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mSceneRuntime;
        } dreamElseLockFailed
    }

    EntityDefinition*
    EntityRuntime::getEntityDefinition()
    {
        if(dreamTryLock()) {
            dreamLock();
            return static_cast<EntityDefinition*>(getDefinition());
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::loadChildrenFromDefinition
    (EntityDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            vector<EntityDefinition*> definitions = definition->getChildDefinitionsList();
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
        } dreamElseLockFailed
    }

    void
    EntityRuntime::setTransform
    (Transform* transform)
    {
        if(dreamTryLock()) {
            dreamLock();
            mTransform.setMatrix(transform->getMatrix());
        } dreamElseLockFailed
    }

    void
    EntityRuntime::translateWithChildren
    (const Vector3& translation)
    {
        if(dreamTryLock()) {
            dreamLock();
            applyToAll
                    (
                        function<EntityRuntime*(EntityRuntime*)>(
                            [&](EntityRuntime* rt)
                        {
                            rt->getTransform().translate(translation);
                            return static_cast<EntityRuntime*>(nullptr);
                        }
                        )
                    );
        } dreamElseLockFailed
    }

    void
    EntityRuntime::preTranslateWithChildren
    (const Vector3& translation)
    {
        if(dreamTryLock()) {
            dreamLock();
            applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>(
                            [&](EntityRuntime* rt)
                        {
                            rt->getTransform().preTranslate(translation);
                            return static_cast<EntityRuntime*>(nullptr);
                        }
                        ));
        } dreamElseLockFailed
    }

    Transform
    EntityRuntime::getInitialTransform
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mInitialTransform;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        if(dreamTryLock()) {
            dreamLock();
            mTransform.setMatrix(matrix * mInitialTransform.getMatrix());
        } dreamElseLockFailed
    }

    void
    EntityRuntime::translateOffsetInitial
    (const Vector3& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            mTransform.setMatrix(glm::translate(mat4(1.0),tx.toGLM())* mInitialTransform.getMatrix());
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::getHasCameraFocus
    () const
    {
        return mHasCameraFocus;
    }

    void
    EntityRuntime::setHasCameraFocus
    (bool cf)
    {
        if(dreamTryLock()) {
            dreamLock();
            mHasCameraFocus = cf;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::getDeleted
    () const
    {
        return mDeleted;
    }

    void
    EntityRuntime::setDeleted
    (bool deleted)
    {
        mDeleted = deleted;
    }

    bool
    EntityRuntime::getHidden
    () const
    {
        return mHidden;
    }

    void
    EntityRuntime::setHidden
    (bool hidden)
    {
        if(dreamTryLock()) {
            dreamLock();
            mHidden = hidden;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::removeChildRuntime
    (EntityRuntime* child)
    {
        if(dreamTryLock()) {
            dreamLock();
            child->setDeleted(true);
        } dreamElseLockFailed
    }

    void
    EntityRuntime::addChildRuntime
    (EntityRuntime* rt)
    {
        if(dreamTryLock()) {
            dreamLock();
            mChildRuntimes.push_back(rt);
        } dreamElseLockFailed
    }

    EntityRuntime*
    EntityRuntime::createAndAddChildRuntime
    (EntityDefinition* def)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto* child = new EntityRuntime(def, mSceneRuntime, mRandomUuid);
            child->setParentRuntime(this);
            if (!child->useDefinition())
            {
                LOG_ERROR("EntityRuntime: Error creating child runtime");
                delete child;
                return nullptr;
            }
            addChildRuntime(child);
            return child;
        } dreamElseLockFailed
    }

    EntityRuntime*
    EntityRuntime::addChildFromTemplateUuid
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();

            // Get the scene def
            auto sceneDef = static_cast<SceneDefinition*>(mSceneRuntime->getDefinition());

            // Get the template def
            auto def = sceneDef->getTemplateByUuid(uuid);

            // Check template is good
            if (def)
            {
                if (!def->getIsTemplate())
                {
                    LOG_ERROR("EntityRuntime: This SO is not a Template, too dangerous");
                    return nullptr;
                }

                // Create child
                auto* child = new EntityRuntime(def, mSceneRuntime, true);
                child->setParentRuntime(this);
                // Use definitoin
                if (!child->useDefinition())
                {
                    LOG_DEBUG("EntityRuntime: Error creating child runtime");
                    delete child;
                    return nullptr;
                }
                // Add runtime
                addChildRuntime(child);
                LOG_DEBUG("EntityRuntime: Successfully added child from template {}",def->getNameAndUuidString());
                return child;
            }
            LOG_DEBUG("EntityRuntime: Cannt create child, definition not found");
            return nullptr;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::useDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mDefinition)
            {
                auto def = static_cast<EntityDefinition*>(mDefinition);
                LOG_TRACE( "EntityRuntime: Using Definition {}", def->getNameAndUuidString());
                setName(def->getName());
                setUuid(mRandomUuid ? Uuid::generateUuid() : def->getUuid());
                setHasCameraFocus(def->getHasCameraFocus());
                setHidden(def->getHidden());
                initTransform();
                setAssetDefinitionsMap(def->getAssetDefinitionsMap());
                setDeferredFor(def->getDeferred());
                setDieAfter(def->getDieAfter());
                // Not sure what purpose this serves
                // (may have been part of a count-down-until-spawn-time system)
                if (mDeferredFor == 0)
                {
                    return loadDeferred();
                }
                return true;
            }
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::loadDeferred
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!createAssetRuntimes())
            {
                return false;
            }
            if(!loadChildrenFromDefinition(static_cast<EntityDefinition*>(mDefinition)))
            {
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    BoundingBox&
    EntityRuntime::getBoundingBox
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mModelRuntime != nullptr)
            {
                return mModelRuntime->getBoundingBox();
            }
            return mBoundingBox;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        if(dreamTryLock()) {
            dreamLock();
            mBoundingBox.from(boundingBox);
        } dreamElseLockFailed
    }

    float
    EntityRuntime::distanceFrom
    (EntityRuntime* other)
    {
        if(dreamTryLock()) {
            dreamLock();
            return mTransform.distanceFrom(other->getTransform());
        } dreamElseLockFailed
    }

    float
    EntityRuntime::distanceFrom
    (const Vector3& other)
    {
        if(dreamTryLock()) {
            dreamLock();
            return glm::distance(vec3(mTransform.getMatrix()[3]),other.toGLM());
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::visibleInFrustum
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            auto cam = mSceneRuntime->getCamera();
            if (cam)
            {
                return cam->visibleInFrustum(this);
            }
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::containedInFrustum
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            auto cam = mSceneRuntime->getCamera();
            if (cam)
            {
                return cam->containedInFrustum(this);
            }
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::containedInFrustumAfterTransform
    (const mat4& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto cam = mSceneRuntime->getCamera();
            if (cam)
            {
                return cam->containedInFrustumAfterTransform(this,tx);
            }
            return false;
        } dreamElseLockFailed

    }

    bool
    EntityRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const Vector3& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto cam = mSceneRuntime->getCamera();
            if (cam)
            {
                return cam->exceedsFrustumPlaneAtTranslation(plane,this,tx);
            }
            return false;
        } dreamElseLockFailed
    }

    bool
    EntityRuntime::applyToAll
    (const function<bool(EntityRuntime*)>& fn)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    EntityRuntime*
    EntityRuntime::applyToAll
    (const function<EntityRuntime*(EntityRuntime*)>& fn)
    {
        if(dreamTryLock()) {
            dreamLock();
            EntityRuntime* retval = fn(this);

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
        } dreamElseLockFailed
    }

    void
    EntityRuntime::translateOffsetInitialWithChildren
    (const Vector3& translation)
    {
        if(dreamTryLock()) {
            dreamLock();
            static mat4 ident(1.0f);
            applyToAll
                    (function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt)
                     {
                         auto initial = rt->getInitialTransform().getMatrix();
                         rt->getTransform().setMatrix(glm::translate(ident,translation.toGLM())* initial);
                         return static_cast<EntityRuntime*>(nullptr);
                     }
                     ));
        } dreamElseLockFailed
    }

    long
    EntityRuntime::getDeferredFor
    ()
    const
    {
        return mDeferredFor;
    }

    void
    EntityRuntime::setDeferredFor
    (long deferred)
    {
        if(dreamTryLock()) {
            dreamLock();
            mDeferredFor = deferred;
        } dreamElseLockFailed
    }

    long
    EntityRuntime::getObjectLifetime
    ()
    const
    {
        return mObjectLifetime;
    }

    void
    EntityRuntime::setObjectLifetime
    (long d)
    {
        if(dreamTryLock()) {
            dreamLock();
            mObjectLifetime = d;
        } dreamElseLockFailed
    }

    void
    EntityRuntime::setDieAfter
    (long d)
    {
        if(dreamTryLock()) {
            dreamLock();
            mDieAfter = d;
        } dreamElseLockFailed
    }

    long
    EntityRuntime::getDieAfter
    ()
    const
    {
        return mDieAfter;
    }

    EntityScriptOnInitTask*
    EntityRuntime::getScriptOnInitTask
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mScriptOnInitTask;
        } dreamElseLockFailed
    }

    EntityScriptOnEventTask*
    EntityRuntime::getScriptOnEventTask
    ()
    {
        return &mScriptOnEventTask;
    }

    EntityScriptOnUpdateTask*
    EntityRuntime::getScriptOnUpdateTask
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mScriptOnUpdateTask;
        } dreamElseLockFailed
    }
    void
    EntityRuntime::updateLifetime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            long timeDelta = getSceneRuntime()->getProjectRuntime()->getTime()->getFrameTimeDelta();
            if (mDeferredFor > 0)
            {
                long deferral = mDeferredFor-timeDelta;
                LOG_CRITICAL("EntityRuntime: Reducing defferal by {} to {} for {}", timeDelta, deferral, getNameAndUuidString());
                setDeferredFor(deferral);
                if (deferral < 0)
                {
                    LOG_CRITICAL("EntityRuntime: Loading Deferred Runtime {}", getNameAndUuidString());
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
        } dreamElseLockFailed
    }

    string
    EntityRuntime::getAttribute
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
    EntityRuntime::setAttribute
    (const string& key, const string& value)
    {
        if(dreamTryLock()) {
            dreamLock();
            mAttributes[key] = value;
        } dreamElseLockFailed
    }

    const map<string,string>&
    EntityRuntime::getAttributesMap
    ()
    const
    {
        return mAttributes;
    }

    string EntityRuntime::getFontText() const
    {
        return mFontText;
    }

    void EntityRuntime::setFontText(const string& fontText)
    {
        if(dreamTryLock()) {
            dreamLock();
            mFontText = fontText;
        } dreamElseLockFailed
    }

    Vector3 EntityRuntime::getFontColor() const
    {
        return mFontColor;
    }

    void EntityRuntime::setFontColor(const Vector3& fontColor)
    {
        if(dreamTryLock()) {
            dreamLock();
            mFontColor = fontColor;
        } dreamElseLockFailed
    }

    float EntityRuntime::getFontScale() const
    {
        return mFontScale;
    }

    void EntityRuntime::setFontScale(float fontScale)
    {
        mFontScale = fontScale;
    }

    bool
    EntityRuntime::isPlayerObject
    ()
    const
    {
        return mSceneRuntime->getPlayerEntity() == this;
    }

    void EntityRuntime::setScriptError(bool e)
    {
        if(dreamTryLock()) {
            dreamLock();
            mScriptError = e;
        } dreamElseLockFailed
    }

    bool EntityRuntime::getScriptError() const
    {
        return mScriptError;
    }

    void EntityRuntime::setScriptInitialised(bool i)
    {
        if(dreamTryLock()) {
            dreamLock();
            mScriptInitialised = i;
        } dreamElseLockFailed
    }

    bool EntityRuntime::getScriptInitialised() const
    {
        return mScriptInitialised;
    }

    EntityScriptConstructionTask* EntityRuntime::getScriptConstructionTask()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mScriptConstructionTask;
        } dreamElseLockFailed
    }
}
