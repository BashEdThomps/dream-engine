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
#include "Components/Graphics/Font/FontCache.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
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
using std::unique_lock;

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
          mFontRuntime(nullptr),
          mFontText(""),
          mFontColor(1.f),
          mFontScale(1.f)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
    }

    void
    EntityRuntime::removeAnimationRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mAnimationRuntime != nullptr)
        {
            delete mAnimationRuntime;
            mAnimationRuntime = nullptr;
        }
    }

    void
    EntityRuntime::removeAudioRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mAudioRuntime = nullptr;
    }

    void
    EntityRuntime::removePathRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mPathRuntime != nullptr)
        {
            delete mPathRuntime;
            mPathRuntime = nullptr;
        }
    }

    void
    EntityRuntime::removeModelRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mModelRuntime != nullptr)
        {
            mModelRuntime->removeRuntime(this);
        }
    }

    void
    EntityRuntime::removeLightRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mLightRuntime != nullptr)
        {
            delete mLightRuntime;
            mLightRuntime = nullptr;
        }
    }

    void
    EntityRuntime::removeFontRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mFontRuntime != nullptr)
        {
            mFontRuntime->popInstance(this);
            mFontRuntime = nullptr;
        }
    }


    void
    EntityRuntime::removeScriptRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mScriptRuntime)
        {
            mSceneRuntime
                    ->getProjectRuntime()
                    ->getTaskManager()
                    ->pushDestructionTask(mScriptDestructionTask);
        }
        mScriptRuntime = nullptr;
    }

    void
    EntityRuntime::removePhysicsObjectRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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

    FontRuntime*
    EntityRuntime::getFontRuntime()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mFontRuntime;
    }

    AnimationRuntime*
    EntityRuntime::getAnimationRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mAnimationRuntime;
    }

    PathRuntime*
    EntityRuntime::getPathRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mPathRuntime;
    }

    AudioRuntime*
    EntityRuntime::getAudioRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mAudioRuntime;
    }

    ModelRuntime*
    EntityRuntime::getModelRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mModelRuntime;
    }

    ScriptRuntime*
    EntityRuntime::getScriptRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mScriptRuntime;
    }

    LightRuntime*
    EntityRuntime::getLightRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mLightRuntime;
    }

    AssetRuntime*
    EntityRuntime::getAssetRuntime
    (AssetType type)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        switch (type)
        {
            case ASSET_TYPE_ENUM_ANIMATION:
                return getAnimationRuntime();
            case ASSET_TYPE_ENUM_PATH:
                return getPathRuntime();
            case ASSET_TYPE_ENUM_AUDIO:
                return getAudioRuntime();
            case ASSET_TYPE_ENUM_LIGHT:
                return getLightRuntime();
            case ASSET_TYPE_ENUM_MODEL:
                return getModelRuntime();
            case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                return getPhysicsObjectRuntime();
            case ASSET_TYPE_ENUM_SCRIPT:
                return getScriptRuntime();
            case ASSET_TYPE_ENUM_FONT:
                return getFontRuntime();
            default:
                break;
        }
        return nullptr;
    }

    bool
    EntityRuntime::hasFontRuntime()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mFontRuntime != nullptr;
    }

    bool
    EntityRuntime::hasAnimationRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mAnimationRuntime != nullptr;
    }

    bool
    EntityRuntime::hasLightRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mLightRuntime != nullptr;
    }

    bool
    EntityRuntime::hasModelRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mModelRuntime != nullptr;
    }

    bool
    EntityRuntime::hasScriptRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mScriptRuntime != nullptr;
    }

    void
    EntityRuntime::setAssetDefinitionsMap
    (const map<AssetType,UuidType>& assetMap)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mAssetDefinitions = assetMap;
    }

    map<AssetType,UuidType>
    EntityRuntime::getAssetDefinitionsMap
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mAssetDefinitions;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mAlwaysDraw = alwaysDraw;
    }

    PhysicsObjectRuntime*
    EntityRuntime::getPhysicsObjectRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mPhysicsObjectRuntime;
    }

    Transform&
    EntityRuntime::getTransform
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mTransform;
    }

    void
    EntityRuntime::initTransform
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        if (mDefinition)
        {
            auto definedTransform = static_cast<EntityDefinition*>(mDefinition)->getTransform();
            mInitialTransform.setMatrix(definedTransform.getMatrix());
            mTransform.setMatrix(definedTransform.getMatrix());
        }
    }

    bool
    EntityRuntime::hasEvents
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        bool retval = false;
        mEventQueueMutex.lock();
        retval = !mEventQueue.empty();
        mEventQueueMutex.unlock();
        return retval;
    }

    void
    EntityRuntime::addEvent
    (const Event& event)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        if (!mDeleted)
        {
            LOG_TRACE("EntityRuntime: Event posted from {} to {}",
                      event.getAttribute("uuid"), getNameAndUuidString());
            mEventQueueMutex.lock();
            mEventQueue.push_back(std::move(event));
            mEventQueueMutex.unlock();
        }
    }

    bool EntityRuntime::tryLockEventQueue()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mEventQueueMutex.try_lock();
    }

    void EntityRuntime::unlockEventQueue()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mEventQueueMutex.unlock();
    }

    vector<Event>*
    EntityRuntime::getEventQueue
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return &mEventQueue;
    }

    void
    EntityRuntime::clearEventQueue
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    void
    EntityRuntime::collectGarbage
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::hasPhysicsObjectRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mPhysicsObjectRuntime != nullptr;
    }

    bool
    EntityRuntime::hasPathRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mPathRuntime != nullptr;
    }

    bool
    EntityRuntime::hasAudioRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mAudioRuntime != nullptr;
    }

    bool
    EntityRuntime::createAssetRuntimes
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    EntityRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::replaceAssetUuid
    (AssetType type, UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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

            default:
                return false;
        }
    }

    bool
    EntityRuntime::createPhysicsObjectRuntime
    (PhysicsObjectDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        removePhysicsObjectRuntime();
        LOG_TRACE( "EntityRuntime: Creating Physics Object Asset Runtime." );
        mPhysicsObjectRuntime = new PhysicsObjectRuntime(
                    definition,
                    mSceneRuntime->getProjectRuntime()->getPhysicsComponent(),
                    mSceneRuntime->getProjectRuntime()->getModelCache(),
                    this
                    );
        return mPhysicsObjectRuntime->useDefinition();
    }

    bool
    EntityRuntime::createAnimationRuntime
    (AnimationDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        LOG_TRACE( "EntityRuntime: Creating Animation asset Runtime." );
        removeAnimationRuntime();
        mAnimationRuntime = new AnimationRuntime(definition,this);
        return mAnimationRuntime->useDefinition();
    }

    bool
    EntityRuntime::createPathRuntime
    (PathDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        LOG_TRACE( "EntityRuntime: Creating Path asset Runtime." );
        removePathRuntime();
        mPathRuntime = new PathRuntime(definition,this);
        return mPathRuntime->useDefinition();
    }

    bool
    EntityRuntime::createAudioRuntime
    (AudioDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::createModelRuntime
    (ModelDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::createScriptRuntime
    (ScriptDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::createLightRuntime
    (LightDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        removeLightRuntime();
        LOG_TRACE( "EntityRuntime: Creating Light Asset Runtime." );
        mLightRuntime = new LightRuntime(definition,this);
        return mLightRuntime->useDefinition();
    }

    bool
    EntityRuntime::createFontRuntime
    (FontDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    EntityRuntime*
    EntityRuntime::getChildRuntimeByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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

    vector<EntityRuntime*>
    EntityRuntime::getChildRuntimes
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mChildRuntimes;
    }

    bool
    EntityRuntime::isParentOf
    (EntityRuntime*  child)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    EntityRuntime::setParentRuntime
    (EntityRuntime* parent)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mParentRuntime = parent;
        setAttribute("parent",mParentRuntime->getUuidString());
    }

    EntityRuntime*
    EntityRuntime::getParentRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mParentRuntime;
    }

    SceneRuntime*
    EntityRuntime::getSceneRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mSceneRuntime;
    }

    EntityDefinition*
    EntityRuntime::getEntityDefinition()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return static_cast<EntityDefinition*>(getDefinition());
    }

    bool
    EntityRuntime::loadChildrenFromDefinition
    (EntityDefinition* definition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    void
    EntityRuntime::setTransform
    (Transform* transform)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mTransform.setMatrix(transform->getMatrix());
    }

    void
    EntityRuntime::translateWithChildren
    (const Vector3& translation)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    void
    EntityRuntime::preTranslateWithChildren
    (const Vector3& translation)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        applyToAll(
                    function<EntityRuntime*(EntityRuntime*)>(
                        [&](EntityRuntime* rt)
                    {
                        rt->getTransform().preTranslate(translation);
                        return static_cast<EntityRuntime*>(nullptr);
                    }
                    ));
    }

    Transform
    EntityRuntime::getInitialTransform
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mInitialTransform;
    }

    void
    EntityRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mTransform.setMatrix(matrix * mInitialTransform.getMatrix());
    }

    void
    EntityRuntime::translateOffsetInitial
    (const Vector3& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mTransform.setMatrix(glm::translate(mat4(1.0),tx.toGLM())* mInitialTransform.getMatrix());
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mHasCameraFocus = cf;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mHidden = hidden;
    }

    void
    EntityRuntime::removeChildRuntime
    (EntityRuntime* child)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        child->setDeleted(true);
    }

    void
    EntityRuntime::addChildRuntime
    (EntityRuntime* rt)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mChildRuntimes.push_back(rt);
    }

    EntityRuntime*
    EntityRuntime::createAndAddChildRuntime
    (EntityDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    EntityRuntime*
    EntityRuntime::addChildFromTemplateUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
    }

    bool
    EntityRuntime::useDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    bool
    EntityRuntime::loadDeferred
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        if (!createAssetRuntimes())
        {
            return false;
        }
        if(!loadChildrenFromDefinition(static_cast<EntityDefinition*>(mDefinition)))
        {
            return false;
        }
        return true;
    }

    BoundingBox&
    EntityRuntime::getBoundingBox
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        if (mModelRuntime != nullptr)
        {
            return mModelRuntime->getBoundingBox();
        }
        return mBoundingBox;
    }

    void
    EntityRuntime::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mBoundingBox.from(boundingBox);
    }

    float
    EntityRuntime::distanceFrom
    (EntityRuntime* other)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return mTransform.distanceFrom(other->getTransform());
    }

    float
    EntityRuntime::distanceFrom
    (const Vector3& other)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return glm::distance(vec3(mTransform.getMatrix()[3]),other.toGLM());
    }

    bool
    EntityRuntime::visibleInFrustum
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->visibleInFrustum(this);
        }
        return false;
    }

    bool
    EntityRuntime::containedInFrustum
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->containedInFrustum(this);
        }
        return false;
    }

    bool
    EntityRuntime::containedInFrustumAfterTransform
    (const mat4& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->containedInFrustumAfterTransform(this,tx);
        }
        return false;

    }

    bool
    EntityRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const Vector3& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        auto cam = mSceneRuntime->getCamera();
        if (cam)
        {
            return cam->exceedsFrustumPlaneAtTranslation(plane,this,tx);
        }
        return false;
    }

    bool
    EntityRuntime::applyToAll
    (const function<bool(EntityRuntime*)>& fn)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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

    EntityRuntime*
    EntityRuntime::applyToAll
    (const function<EntityRuntime*(EntityRuntime*)>& fn)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
    }

    void
    EntityRuntime::translateOffsetInitialWithChildren
    (const Vector3& translation)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        static mat4 ident(1.0f);
        applyToAll
                (function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt)
                 {
                     auto initial = rt->getInitialTransform().getMatrix();
                     rt->getTransform().setMatrix(glm::translate(ident,translation.toGLM())* initial);
                     return static_cast<EntityRuntime*>(nullptr);
                 }
                 ));
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mDeferredFor = deferred;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mObjectLifetime = d;
    }

    void
    EntityRuntime::setDieAfter
    (long d)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mDieAfter = d;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return &mScriptOnInitTask;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return &mScriptOnUpdateTask;
    }
    void
    EntityRuntime::updateLifetime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mAttributes[key] = value;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mFontText = fontText;
    }

    Vector3 EntityRuntime::getFontColor() const
    {
        return mFontColor;
    }

    void EntityRuntime::setFontColor(const Vector3& fontColor)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mFontColor = fontColor;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mScriptError = e;
    }

    bool EntityRuntime::getScriptError() const
    {
        return mScriptError;
    }

    void EntityRuntime::setScriptInitialised(bool i)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        mScriptInitialised = i;
    }

    bool EntityRuntime::getScriptInitialised() const
    {
        return mScriptInitialised;
    }

    EntityScriptConstructionTask* EntityRuntime::getScriptConstructionTask()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();
        return &mScriptConstructionTask;
    }

}
