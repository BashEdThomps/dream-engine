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
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Light/LightRuntime.h"
#include "Components/Graphics/Font/FontRuntime.h"
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
#include "Components/Script/ScriptTasks.h"
#include "Components/Time.h"
#include "Components/Cache.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

using std::vector;
using std::make_shared;


namespace octronic::dream
{
    EntityRuntime::EntityRuntime(ProjectRuntime* pr, SceneRuntime* sr,EntityDefinition* sd,bool randomUuid)
        : Runtime(sd),
          mProjectRuntimeHandle(pr),
          mSceneRuntime(sr),
          mParentRuntime(nullptr),

          mAnimationRuntime(nullptr),
          mAudioRuntime(nullptr),
          mFontRuntime(nullptr),
          mLightRuntime(nullptr),
          mModelRuntime(nullptr),
          mPathRuntime(nullptr),
          mPhysicsObjectRuntime(nullptr),

          mScriptRuntime(nullptr),
          mScriptInitialised(false),
          mScriptError(false),

          mBoundingBox(),
          mHasCameraFocus(false),
          mDeleted(false),
          mHidden(false),
          mAlwaysDraw(false),
          mRandomUuid(randomUuid),
          mObjectLifetime(0),

          mScriptCreateStateTask(nullptr),
          mScriptOnInitTask(nullptr),
          mScriptOnUpdateTask(nullptr),
          mScriptOnEventTask(nullptr),
          mScriptRemoveStateTask(make_shared<EntityScriptRemoveStateTask>(sr->getProjectRuntime(), mUuid)),

          mFontText(""),
          mFontColor(1.f),
          mFontScale(1.f)
    {
        LOG_TRACE("EntityRuntime: {}", __FUNCTION__);

        mEventQueue.reserve(10);

        if (mRandomUuid)
        {
            mUuid = Uuid::generateUuid();
        }

        if (static_cast<SceneDefinition*>(mSceneRuntime->getDefinitionHandle())->getPlayerObject() == mUuid)
        {
            mSceneRuntime->setPlayerEntity(this);
        }

        setAttribute("uuid",getUuidString());
    }

    EntityRuntime::~EntityRuntime
    ()
    {
        LOG_TRACE("EntityRuntime: {}", __FUNCTION__);

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
        mAudioRuntime = nullptr;
    }

    void
    EntityRuntime::removePathRuntime
    ()
    {
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
        if (mModelRuntime != nullptr)
        {
            mModelRuntime->removeInstance(this);
        }
    }

    void
    EntityRuntime::removeLightRuntime
    ()
    {
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
        if (mFontRuntime != nullptr)
        {
            mFontRuntime->removeInstance(this);
            mFontRuntime = nullptr;
        }
    }

    void
    EntityRuntime::removeScriptRuntime
    ()
    {
        if (mScriptRuntime)
        {
            mSceneRuntime->getProjectRuntime()->getDestructionTaskQueue()->pushTask(mScriptRemoveStateTask);
        }
        mScriptRuntime = nullptr;
    }

    void
    EntityRuntime::removePhysicsObjectRuntime
    ()
    {
        if (hasPhysicsObjectRuntime())
        {
            auto physicsComp = mSceneRuntime->getProjectRuntime()->getPhysicsComponent();
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
        return mFontRuntime;
    }

    AnimationRuntime*
    EntityRuntime::getAnimationRuntime
    ()
    {
        return mAnimationRuntime;
    }

    PathRuntime*
    EntityRuntime::getPathRuntime
    ()
    {
        return mPathRuntime;
    }

    AudioRuntime*
    EntityRuntime::getAudioRuntime
    ()
    {
        return mAudioRuntime;
    }

    ModelRuntime*
    EntityRuntime::getModelRuntime
    ()
    {
        return mModelRuntime;
    }

    ScriptRuntime*
    EntityRuntime::getScriptRuntime
    ()
    {
        return mScriptRuntime;
    }

    LightRuntime*
    EntityRuntime::getLightRuntime
    ()
    {
        return mLightRuntime;
    }

    AssetRuntime*
    EntityRuntime::getAssetRuntime
    (AssetType type)
    {
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
            default:
                break;
        }
        return nullptr;
    }

    bool
    EntityRuntime::hasFontRuntime()
    {
        return mFontRuntime != nullptr;
    }

    bool
    EntityRuntime::hasAnimationRuntime
    ()
    {
        return mAnimationRuntime != nullptr;
    }

    bool
    EntityRuntime::hasLightRuntime
    ()
    {
        return mLightRuntime != nullptr;
    }

    bool
    EntityRuntime::hasModelRuntime
    ()
    {
        return mModelRuntime != nullptr;
    }

    bool
    EntityRuntime::hasScriptRuntime
    ()
    {
        return mScriptRuntime != nullptr;
    }

    void
    EntityRuntime::setAssetDefinitionsMap
    (const map<AssetType,UuidType>& assetMap)
    {
        mAssetDefinitions = assetMap;
    }

    map<AssetType,UuidType>
    EntityRuntime::getAssetDefinitionsMap
    ()
    {
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
        mAlwaysDraw = alwaysDraw;
    }

    PhysicsObjectRuntime*
    EntityRuntime::getPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime;
    }

    Transform
    EntityRuntime::getTransform
    ()
    const
    {
        return mTransform;
    }

    void
    EntityRuntime::initTransform
    ()
    {
        if (mDefinitionHandle)
        {
            auto definedTransform = static_cast<EntityDefinition*>(mDefinitionHandle)->getTransform();
            mInitialTransform.setMatrix(definedTransform.getMatrix());
            mTransform.setMatrix(definedTransform.getMatrix());
        }
    }

    bool
    EntityRuntime::hasEvents
    ()
    {
        return !mEventQueue.empty();
    }

    void
    EntityRuntime::addEvent
    (const Event& event)
    {
        if (!mDeleted)
        {
            LOG_TRACE("EntityRuntime: Event posted from {} to {}",
                      event.getAttribute("uuid"), getNameAndUuidString());
            mEventQueue.push_back(std::move(event));
        }
    }

    vector<Event>*
    EntityRuntime::getEventQueue
    ()
    {
        return &mEventQueue;
    }

    void
    EntityRuntime::clearEventQueue
    ()
    {
        LOG_TRACE("EntityRuntime: Clearing event queue");

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

    void
    EntityRuntime::collectGarbage
    ()
    {
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
            mChildRuntimes.erase(find(mChildRuntimes.begin(), mChildRuntimes.end(), child));
            delete child;
        }

        clearEventQueue();
    }

    bool
    EntityRuntime::hasPhysicsObjectRuntime
    ()
    {
        return mPhysicsObjectRuntime != nullptr;
    }

    bool
    EntityRuntime::hasPathRuntime
    ()
    {
        return mPathRuntime != nullptr;
    }

    bool
    EntityRuntime::hasAudioRuntime
    ()
    {
        return mAudioRuntime != nullptr;
    }

    bool
    EntityRuntime::createAssetRuntimes
    ()
    {
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
                    break;
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
        removePhysicsObjectRuntime();
        LOG_TRACE( "EntityRuntime: Creating Physics Object Asset Runtime." );
        mPhysicsObjectRuntime = new PhysicsObjectRuntime(mProjectRuntimeHandle,definition, this);
        return true;//mPhysicsObjectRuntime->loadFromDefinition();
    }

    bool
    EntityRuntime::createAnimationRuntime
    (AnimationDefinition* definition)
    {
        LOG_TRACE( "EntityRuntime: Creating Animation asset Runtime." );
        removeAnimationRuntime();
        mAnimationRuntime = new AnimationRuntime(mProjectRuntimeHandle, definition,this);
        return true;//mAnimationRuntime->loadFromDefinition();
    }

    bool
    EntityRuntime::createPathRuntime
    (PathDefinition* definition)
    {
        LOG_TRACE( "EntityRuntime: Creating Path asset Runtime." );
        removePathRuntime();
        mPathRuntime = new PathRuntime(mProjectRuntimeHandle, definition,this);
        return true;//mPathRuntime->loadFromDefinition();
    }

    bool
    EntityRuntime::createAudioRuntime
    (AudioDefinition* definition)
    {
        // AudioRuntime must be taken from AudioComponent rather than AudioCache

        auto audioComp = mProjectRuntimeHandle->getAudioComponent();
        if (audioComp != nullptr)
        {
            mAudioRuntime = audioComp->getAudioRuntime(definition);
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
        removeModelRuntime();
        LOG_INFO("EntityRuntime: Creating Model asset Runtime.");
        auto cache = mSceneRuntime->getProjectRuntime()->getModelCache();
        if (cache != nullptr)
        {
            mModelRuntime = static_cast<ModelRuntime*>(cache->getRuntimeHandle(definition));
            if (mModelRuntime != nullptr)
            {
                mModelRuntime->addInstance(this);
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
        removeScriptRuntime();
        LOG_TRACE("EntityRuntime: Creating Script asset Runtime.");
        auto scriptCache = mSceneRuntime->getProjectRuntime()->getScriptCache();
        if (scriptCache)
        {
            mScriptRuntime = static_cast<ScriptRuntime*>(scriptCache->getRuntimeHandle(definition));
            if (mScriptRuntime)
            {
                mScriptCreateStateTask = make_shared<EntityScriptCreateStateTask>(mProjectRuntimeHandle, this);
                mScriptOnInitTask       = make_shared<EntityScriptOnInitTask>(mProjectRuntimeHandle, this);
                mScriptOnUpdateTask     = make_shared<EntityScriptOnUpdateTask>(mProjectRuntimeHandle,this);
                mScriptOnEventTask      = make_shared<EntityScriptOnEventTask>(mProjectRuntimeHandle,this);

                mScriptCreateStateTask->setScript(mScriptRuntime);
                mScriptOnInitTask->setScript(mScriptRuntime);
                mScriptOnUpdateTask->setScript(mScriptRuntime);
                mScriptOnEventTask->setScript(mScriptRuntime);
                mScriptRemoveStateTask->setScript(mScriptRuntime);
                mScriptRuntime->addInstance(this);

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
        removeLightRuntime();
        LOG_TRACE( "EntityRuntime: Creating Light Asset Runtime." );
        mLightRuntime = new LightRuntime(mProjectRuntimeHandle, definition,this);
        return mLightRuntime->loadFromDefinition();
    }

    bool
    EntityRuntime::createFontRuntime
    (FontDefinition* definition)
    {
        removeFontRuntime();
        LOG_TRACE( "EntityRuntime: Creating Font Asset Runtime." );
        auto fontCache = mSceneRuntime->getProjectRuntime()->getFontCache();
        mFontRuntime = static_cast<FontRuntime*>(fontCache->getRuntimeHandle(definition));
        if (mFontRuntime == nullptr)
        {
            LOG_ERROR("EntityRuntime: Error creating Font runtime");
        }
        EntityDefinition* entityDef = static_cast<EntityDefinition*>(mDefinitionHandle);
        mFontColor = entityDef->getFontColor();
        mFontScale = entityDef->getFontScale();
        mFontText = entityDef->getFontText();
        mFontRuntime->addInstance(this);
        return mFontRuntime != nullptr;
    }

    EntityRuntime*
    EntityRuntime::getChildRuntimeByUuid
    (UuidType uuid)
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

    vector<EntityRuntime*>
    EntityRuntime::getChildRuntimes
    ()
    {
        return mChildRuntimes;
    }

    bool
    EntityRuntime::isParentOf
    (EntityRuntime*  child)
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
    EntityRuntime::setParentRuntime
    (EntityRuntime* parent)
    {
        mParentRuntime = parent;
        setAttribute("parent",mParentRuntime->getUuidString());

    }

    EntityRuntime*
    EntityRuntime::getParentRuntime
    ()
    {
        return mParentRuntime;
    }

    SceneRuntime*
    EntityRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntime;
    }

    EntityDefinition*
    EntityRuntime::getEntityDefinition()
    {
        return static_cast<EntityDefinition*>(getDefinitionHandle());
    }

    bool
    EntityRuntime::loadChildrenFromDefinition
    (EntityDefinition* definition)
    {
        vector<EntityDefinition*> definitions = definition->getChildDefinitionsList();
        for (auto it = begin(definitions); it != end(definitions); it++)
        {
            auto sod = (*it);
            if (sod->getIsTemplate())
            {
                static_cast<SceneDefinition*>(mSceneRuntime->getDefinitionHandle())->addTemplate(sod);
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
    (const Transform& transform)
    {
        mTransform = transform;
    }

    void
    EntityRuntime::translateWithChildren
    (const Vector3& translation)
    {
        applyToAll(function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt){
                       rt->getTransform().translate(translation);
                       return static_cast<EntityRuntime*>(nullptr);}));
    }

    void
    EntityRuntime::preTranslateWithChildren
    (const Vector3& translation)
    {
        applyToAll(function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt){
                       rt->getTransform().preTranslate(translation);
                       return static_cast<EntityRuntime*>(nullptr); }));
    }

    Transform
    EntityRuntime::getInitialTransform
    ()
    {
        return mInitialTransform;
    }

    void
    EntityRuntime::transformOffsetInitial
    (const mat4& matrix)
    {
        mTransform.setMatrix(matrix * mInitialTransform.getMatrix());
    }

    void
    EntityRuntime::translateOffsetInitial
    (const Vector3& tx)
    {
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
        mHidden = hidden;
    }

    void
    EntityRuntime::removeChildRuntime
    (EntityRuntime* child)
    {
        child->setDeleted(true);
    }

    void
    EntityRuntime::addChildRuntime
    (EntityRuntime* rt)
    {
        mChildRuntimes.push_back(rt);
    }

    EntityRuntime*
    EntityRuntime::createAndAddChildRuntime
    (EntityDefinition* def)
    {
        auto* child = new EntityRuntime(mProjectRuntimeHandle,  mSceneRuntime, def, mRandomUuid);
        child->setParentRuntime(this);
        if (!child->loadFromDefinition())
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
        // Get the scene def
        auto sceneDef = static_cast<SceneDefinition*>(mSceneRuntime->getDefinitionHandle());

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
            auto* child = new EntityRuntime(mProjectRuntimeHandle, mSceneRuntime, def,  true);
            child->setParentRuntime(this);
            // Use definitoin
            if (!child->loadFromDefinition())
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
    EntityRuntime::loadFromDefinition
    ()
    {
        if (mDefinitionHandle)
        {
            auto def = static_cast<EntityDefinition*>(mDefinitionHandle);
            LOG_TRACE( "EntityRuntime: Using Definition {}", def->getNameAndUuidString());
            setName(def->getName());
            setUuid(mRandomUuid ? Uuid::generateUuid() : def->getUuid());
            setHasCameraFocus(def->getHasCameraFocus());
            setHidden(def->getHidden());
            initTransform();
            setAssetDefinitionsMap(def->getAssetDefinitionsMap());
            if (!createAssetRuntimes())
            {
                return false;
            }
            if(!loadChildrenFromDefinition(def))
            {
                return false;
            }
            return true;
        }
        return false;
    }

    BoundingBox&
    EntityRuntime::getBoundingBox
    ()
    {
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
        mBoundingBox.from(boundingBox);
    }

    float
    EntityRuntime::distanceFrom
    (EntityRuntime* other)
    {
        return mTransform.distanceFrom(other->getTransform());
    }

    float
    EntityRuntime::distanceFrom
    (const Vector3& other)
    {
        return glm::distance(vec3(mTransform.getMatrix()[3]),other.toGLM());
    }

    bool
    EntityRuntime::visibleInFrustum
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
    EntityRuntime::containedInFrustum
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
    EntityRuntime::containedInFrustumAfterTransform
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
    EntityRuntime::exceedsFrustumPlaneAtTranslation
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
    EntityRuntime::applyToAll
    (const function<bool(EntityRuntime*)>& fn)
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

    EntityRuntime*
    EntityRuntime::applyToAll
    (const function<EntityRuntime*(EntityRuntime*)>& fn)
    {
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
        static mat4 ident(1.0f);
        applyToAll(function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* rt){
                       auto initial = rt->getInitialTransform().getMatrix();
                       rt->getTransform().setMatrix(glm::translate(ident,translation.toGLM())* initial);
                       return static_cast<EntityRuntime*>(nullptr);
                   }));
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
        mObjectLifetime = d;
    }

    shared_ptr<EntityScriptOnInitTask>
    EntityRuntime::getScriptOnInitTask
    ()
    {
        return mScriptOnInitTask;
    }

    shared_ptr<EntityScriptOnEventTask>
    EntityRuntime::getScriptOnEventTask
    ()
    {
        return mScriptOnEventTask;
    }

    shared_ptr<EntityScriptOnUpdateTask>
    EntityRuntime::getScriptOnUpdateTask
    ()
    {
        return mScriptOnUpdateTask;
    }

    shared_ptr<EntityScriptCreateStateTask>
    EntityRuntime::getScriptCreateStateTask()
    {
        return mScriptCreateStateTask;
    }

    void
    EntityRuntime::updateLifetime
    ()
    {
        long timeDelta = getSceneRuntime()->getProjectRuntime()->getTime()->getFrameTimeDelta();
        mObjectLifetime += timeDelta;
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
        mFontText = fontText;
    }

    Vector3 EntityRuntime::getFontColor() const
    {
        return mFontColor;
    }

    void EntityRuntime::setFontColor(const Vector3& fontColor)
    {
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
        mScriptError = e;
    }

    bool EntityRuntime::getScriptError() const
    {
        return mScriptError;
    }

    void EntityRuntime::setScriptInitialised(bool i)
    {
        mScriptInitialised = i;
    }

    bool EntityRuntime::getScriptInitialised() const
    {
        return mScriptInitialised;
    }

    // For DiscreteAssetRuntimes
    void EntityRuntime::pushTasks()
    {
        // For DiscreteAssetRuntimes ONLY, SharedAssetRuntimes handled by project
        LOG_TRACE("EntityRuntime: Pushing tasks of {}", getNameAndUuidString());
		// Animation
		if (hasAnimationRuntime()) getAnimationRuntime()->pushNextTask();
        // Light
		if (hasLightRuntime()) getLightRuntime()->pushNextTask();
		// Physics
		if (hasPhysicsObjectRuntime()) getPhysicsObjectRuntime()->pushNextTask();
		// Path
		if (hasPathRuntime()) getPathRuntime()->pushNextTask();
    }

    bool EntityRuntime::allRuntimesLoaded()
    {
        bool all_loaded = true;
       if (hasAnimationRuntime())     all_loaded &= mAnimationRuntime->getLoaded();
       if (hasAudioRuntime())         all_loaded &= mAudioRuntime->getLoaded();
       if (hasFontRuntime())          all_loaded &= mFontRuntime->getLoaded();
       if (hasLightRuntime())         all_loaded &= mLightRuntime->getLoaded();
       if (hasModelRuntime())         all_loaded &= mModelRuntime->getLoaded();
       if (hasPathRuntime())          all_loaded &= mPathRuntime->getLoaded();
       if (hasPhysicsObjectRuntime()) all_loaded &= mPhysicsObjectRuntime->getLoaded();
       if (hasScriptRuntime())        all_loaded &= mScriptRuntime->getLoaded();
       return  all_loaded;
    }
}
