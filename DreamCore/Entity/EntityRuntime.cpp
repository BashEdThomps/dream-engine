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
#include "Components/Graphics/Font/FontRuntime.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptComponent.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Project/ProjectDefinition.h"
#include "Common/Uuid.h"
#include "Components/Time.h"
#include "Components/Cache.h"

#include <iostream>

using std::vector;
using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  EntityRuntime::EntityRuntime(
      const weak_ptr<ProjectRuntime>& pr,
      const weak_ptr<SceneRuntime>& sr,
      const weak_ptr<EntityDefinition>& sd,
      bool randomUuid)
    : Runtime(sd),
      mProjectRuntime(pr),
      mSceneRuntime(sr),
      mScriptInitialised(false),
      mScriptError(false),
      mBoundingBox(),
      mDeleted(false),
      mRandomUuid(randomUuid),
      mFontText(""),
      mFontColor(1.f),
      mFontScale(1.f)
  {
    LOG_TRACE("EntityRuntime: {}", __FUNCTION__);

    if (mRandomUuid)
    {
      mUuid = Uuid::generateUuid();
    }
  }

  EntityRuntime::~EntityRuntime
  ()
  {
    LOG_TRACE("EntityRuntime: {}", __FUNCTION__);

    mChildRuntimes.clear();

    removeAnimationRuntime();
    removeAudioRuntime();
    removeFontRuntime();
    removeModelRuntime();
    removePathRuntime();
    removePhysicsObjectRuntime();
    removeScriptRuntime();
    removeTextureRuntime();
  }

  void
  EntityRuntime::removeAnimationRuntime
  ()
  {
  }

  void
  EntityRuntime::removeAudioRuntime
  ()
  {
    if (auto arLock = mAudioRuntime.lock())
    {
      arLock->removeInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
    }
  }

  void
  EntityRuntime::removePathRuntime
  ()
  {
    mPathRuntime.reset();
  }

  void
  EntityRuntime::removeModelRuntime
  ()
  {
    if (auto mrLock = mModelRuntime.lock())
    {
      mrLock->removeInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
    }
  }

  void
  EntityRuntime::removeFontRuntime
  ()
  {
    if (auto frLock = mFontRuntime.lock())
    {
      frLock->removeInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
    }
  }

  void
  EntityRuntime::removeTextureRuntime
  ()
  {
    if (auto trLock = mTextureRuntime.lock())
    {
      trLock->removeInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
    }
  }

  void
  EntityRuntime::removeScriptRuntime
  ()
  {
    if (auto srLock = mScriptRuntime.lock())
    {
      if (auto sceneLock = mSceneRuntime.lock())
      {
        if (auto prLock = sceneLock->getProjectRuntime().lock())
        {
          if (auto dqLock = prLock->getDestructionTaskQueue().lock())
          {
            dqLock->pushTask(mScriptRemoveStateTask);
            srLock->removeInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
          }
        }
      }
    }
  }

  void
  EntityRuntime::removePhysicsObjectRuntime
  ()
  {
    if (auto srLock = mSceneRuntime.lock())
    {
      if (hasPhysicsObjectRuntime())
      {
        if (auto prLock = srLock->getProjectRuntime().lock())
        {
          if (auto physicsComp = prLock->getPhysicsComponent().lock())
          {
            physicsComp->removePhysicsObjectRuntime(getPhysicsObjectRuntime());
          }
        }
      }
    }
  }

  weak_ptr<FontRuntime>
  EntityRuntime::getFontRuntime
  ()
  const
  {
    return mFontRuntime;
  }

  weak_ptr<AnimationRuntime>
  EntityRuntime::getAnimationRuntime
  ()
  const
  {
    return mAnimationRuntime;
  }

  weak_ptr<PathRuntime>
  EntityRuntime::getPathRuntime()
  const
  {
    return mPathRuntime;
  }

  weak_ptr<AudioRuntime>
  EntityRuntime::getAudioRuntime
  ()
  const
  {
    return mAudioRuntime;
  }

  weak_ptr<ModelRuntime>
  EntityRuntime::getModelRuntime
  ()
  const
  {
    return mModelRuntime;
  }

  weak_ptr<ScriptRuntime>
  EntityRuntime::getScriptRuntime
  ()
  const
  {
    return mScriptRuntime;
  }

  weak_ptr<TextureRuntime>
  EntityRuntime::getTextureRuntime
  ()
  const
  {
    return mTextureRuntime;
  }

  weak_ptr<AssetRuntime>
  EntityRuntime::getAssetRuntime
  (AssetType type)
  const
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
      case ASSET_TYPE_ENUM_MODEL:
        return getModelRuntime();
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        return getPhysicsObjectRuntime();
      case ASSET_TYPE_ENUM_SCRIPT:
        return getScriptRuntime();
      case ASSET_TYPE_ENUM_TEXTURE:
        return getTextureRuntime();
      default:
        break;
    }
    return weak_ptr<AssetRuntime>();
  }

  bool
  EntityRuntime::hasFontRuntime()
  const
  {
    return !mFontRuntime.expired();
  }

  bool
  EntityRuntime::hasAnimationRuntime
  ()
  const
  {
    return mAnimationRuntime != nullptr;
  }

  bool
  EntityRuntime::hasModelRuntime
  ()
  const
  {
    return !mModelRuntime.expired();
  }

  bool
  EntityRuntime::hasScriptRuntime
  ()
  const
  {
    return !mScriptRuntime.expired();
  }

  bool
  EntityRuntime::hasTextureRuntime()
  const
  {
    return !mTextureRuntime.expired();
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
  const
  {
    return mAssetDefinitions;
  }

  weak_ptr<PhysicsObjectRuntime>
  EntityRuntime::getPhysicsObjectRuntime
  ()
  const
  {
    return mPhysicsObjectRuntime;
  }

  Transform
  EntityRuntime::getTransform
  ()
  const
  {
    return mCurrentTransform;
  }

  void
  EntityRuntime::initTransform
  ()
  {
    if (auto defLock = mDefinition.lock())
    {
      mInitialTransform = (static_pointer_cast<EntityDefinition>(defLock))->getTransform();
    }
  }

  bool
  EntityRuntime::hasEvents
  ()
  const
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

  vector<Event>
  EntityRuntime::getEventQueue
  ()
  {
    return mEventQueue;
  }

  void
  EntityRuntime::clearProcessedEvents
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

    vector<shared_ptr<EntityRuntime>> toDelete;

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
    }

    clearProcessedEvents();
  }

  bool
  EntityRuntime::hasPhysicsObjectRuntime
  ()
  const
  {
    return mPhysicsObjectRuntime != nullptr;
  }

  bool
  EntityRuntime::hasPathRuntime
  ()
  const
  {
    return mPathRuntime != nullptr;
  }

  bool
  EntityRuntime::hasAudioRuntime
  ()
  const
  {
    return !mAudioRuntime.expired();
  }

  bool
  EntityRuntime::createAssetRuntimes
  ()
  {
    bool result = false;

    for (auto assetPair : mAssetDefinitions)
    {
      if (auto def = getAssetDefinitionByUuid(assetPair.second).lock())
      {
        LOG_TRACE("EntityRuntime: Creating {}",def->getNameAndUuidString());
        switch (assetPair.first)
        {
          case AssetType::ASSET_TYPE_ENUM_ANIMATION:
            result = createAnimationRuntime(static_pointer_cast<AnimationDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_AUDIO:
            result = createAudioRuntime(static_pointer_cast<AudioDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_FONT:
            result = createFontRuntime(static_pointer_cast<FontDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_MODEL:
            result = createModelRuntime(static_pointer_cast<ModelDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_PATH:
            result = createPathRuntime(static_pointer_cast<PathDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
            result = createPhysicsObjectRuntime(static_pointer_cast<PhysicsObjectDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_SCRIPT:
            result = createScriptRuntime(static_pointer_cast<ScriptDefinition>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_TEXTURE:
            result = createTextureRuntime(static_pointer_cast<TextureDefinition>(def));
            break;
          default:
            return false;
        }
        if (result) break;
      }
    }
    return result;
  }

  weak_ptr<AssetDefinition>
  EntityRuntime::getAssetDefinitionByUuid
  (UuidType uuid)
  {
    if (auto srLock = mSceneRuntime.lock())
    {
      if (auto project = srLock->getProjectRuntime().lock())
      {
        if (auto projectDef = static_pointer_cast<ProjectDefinition>(project->getDefinition().lock()))
        {
          return projectDef->getAssetDefinitionByUuid(uuid);
        }
      }
    }
    return weak_ptr<AssetDefinition>();
  }

  bool
  EntityRuntime::replaceAssetUuid
  (AssetType type, UuidType uuid)
  {
    LOG_INFO("EntityRuntime: REPLACING asset Runtime from uuid {}", uuid);

    if (auto srLock = mSceneRuntime.lock())
    {
      if (auto project = srLock->getProjectRuntime().lock())
      {
        if (auto projectDef = static_pointer_cast<ProjectDefinition>(project->getDefinition().lock()))
        {
          if (auto def = projectDef->getAssetDefinitionByUuid(uuid).lock())
          {
            switch (type)
            {
              case AssetType::ASSET_TYPE_ENUM_ANIMATION:
                return createAnimationRuntime(static_pointer_cast<AnimationDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_AUDIO:
                return createAudioRuntime(static_pointer_cast<AudioDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_FONT:
                return createFontRuntime(static_pointer_cast<FontDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_MODEL:
                return createModelRuntime(static_pointer_cast<ModelDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_PATH:
                return createPathRuntime(static_pointer_cast<PathDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                return createPhysicsObjectRuntime(static_pointer_cast<PhysicsObjectDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_SCRIPT:
                return createScriptRuntime(static_pointer_cast<ScriptDefinition>(def));
              case AssetType::ASSET_TYPE_ENUM_TEXTURE:
                return createTextureRuntime(static_pointer_cast<TextureDefinition>(def));
              default:
                break;
            }
          }
        }
      }
    }
    return false;
  }

  // DiscreteAssetRuntimes ===================================================

  bool
  EntityRuntime::createPhysicsObjectRuntime
  (const weak_ptr<PhysicsObjectDefinition>& definition)
  {
    removePhysicsObjectRuntime();
    LOG_TRACE( "EntityRuntime: Creating Physics Object Asset Runtime." );
    mPhysicsObjectRuntime = make_shared<PhysicsObjectRuntime>(
          mProjectRuntime,definition,
          static_pointer_cast<EntityRuntime>(shared_from_this()));
    return mPhysicsObjectRuntime->init();
  }

  bool
  EntityRuntime::createAnimationRuntime
  (const weak_ptr<AnimationDefinition>& definition)
  {
    LOG_TRACE( "EntityRuntime: Creating Animation asset Runtime." );
    removeAnimationRuntime();
    mAnimationRuntime = make_shared<AnimationRuntime>(
          mProjectRuntime,
          definition,static_pointer_cast<EntityRuntime>(shared_from_this()));
    return mAnimationRuntime->init();
  }

  bool
  EntityRuntime::createPathRuntime
  (const weak_ptr<PathDefinition>& definition)
  {
    LOG_TRACE( "EntityRuntime: Creating Path asset Runtime." );
    removePathRuntime();
    mPathRuntime = make_shared<PathRuntime>(
          mProjectRuntime,
          definition,
          static_pointer_cast<EntityRuntime>(shared_from_this()));
    return mPathRuntime->init();
  }

  // SharedAssetRuntimes =====================================================

  bool
  EntityRuntime::createAudioRuntime
  (const weak_ptr<AudioDefinition>& definition)
  {
    // AudioRuntime must be taken from AudioComponent rather than AudioCache
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto audioComp = prLock->getAudioComponent().lock())
      {
        mAudioRuntime = audioComp->getAudioRuntime(definition);
        return !mAudioRuntime.expired();
      }
    }
    return false;
  }

  bool
  EntityRuntime::createModelRuntime
  (const weak_ptr<ModelDefinition>& definition)
  {
    removeModelRuntime();
    LOG_INFO("EntityRuntime: Creating Model asset Runtime.");
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto cache = prLock->getModelCache().lock())
      {
        mModelRuntime = cache->getRuntime(definition);
        if (auto mr = mModelRuntime.lock())
        {
          mr->addInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
        }
      }
    }
    return !mModelRuntime.expired();
  }

  bool
  EntityRuntime::createScriptRuntime
  (const weak_ptr<ScriptDefinition>& definition)
  {
    removeScriptRuntime();
    if (auto projectRuntime = mProjectRuntime.lock())
    {
      if (auto sceneRuntime = mSceneRuntime.lock())
      {
        if (auto scriptCache = projectRuntime->getScriptCache().lock())
        {
          LOG_TRACE("EntityRuntime: Creating Script asset Runtime.");
          auto shared_this = static_pointer_cast<EntityRuntime>(shared_from_this());
          mScriptRuntime = scriptCache->getRuntime(definition);
          if (auto scriptRuntime = mScriptRuntime.lock())
          {
            mScriptCreateStateTask = make_shared<EntityScriptCreateStateTask>(mProjectRuntime, shared_this);
            mScriptOnInitTask      = make_shared<EntityScriptOnInitTask>(mProjectRuntime, shared_this);
            mScriptOnUpdateTask    = make_shared<EntityScriptOnUpdateTask>(mProjectRuntime,shared_this);
            mScriptOnEventTask     = make_shared<EntityScriptOnEventTask>(mProjectRuntime,shared_this);
            mScriptRemoveStateTask = make_shared<EntityScriptRemoveStateTask>(mProjectRuntime,getUuid(), mScriptRuntime);
            scriptRuntime->addInstance(shared_this);
            return true;
          }
        }
      }
    }
    return false;
  }

  bool
  EntityRuntime::createFontRuntime
  (const weak_ptr<FontDefinition>& definition)
  {
    removeFontRuntime();
    if (auto projectRuntime = mProjectRuntime.lock())
    {
      if (auto fontCache = projectRuntime->getFontCache().lock())
      {
        if (auto defLock = mDefinition.lock())
        {
          LOG_TRACE( "EntityRuntime: Creating Font Asset Runtime." );
          mFontRuntime = fontCache->getRuntime(definition);
          if (auto frLock = mFontRuntime.lock())
          {
            shared_ptr<EntityDefinition> entityDef = static_pointer_cast<EntityDefinition>(defLock);
            mFontColor = entityDef->getFontColor();
            mFontScale = entityDef->getFontScale();
            mFontText = entityDef->getFontText();
            frLock->addInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
            return true;
          }
        }
      }
    }
    return false;
  }

  bool
  EntityRuntime::createTextureRuntime
  (const weak_ptr<TextureDefinition>& definition)
  {
    removeTextureRuntime();
    if (auto projectRuntime = mProjectRuntime.lock())
    {
      if (auto textureCache = projectRuntime->getTextureCache().lock())
      {
        if (auto defLock = mDefinition.lock())
        {
          LOG_TRACE( "EntityRuntime: Creating Texture Asset Runtime." );
          mTextureRuntime = textureCache->getRuntime(definition);
          if (auto trLock = mTextureRuntime.lock())
          {
            auto entityDef = static_pointer_cast<EntityDefinition>(defLock);
            trLock->addInstance(static_pointer_cast<EntityRuntime>(shared_from_this()));
            return true;
          }
        }
      }
    }
    return false;
  }


  // Accessors ===============================================================

  weak_ptr<EntityRuntime>
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
    return weak_ptr<EntityRuntime>();
  }

  vector<weak_ptr<EntityRuntime>>
  EntityRuntime::generateFlatVector
  ()
  {
    vector<weak_ptr<EntityRuntime>> retval;
    if (mParentEntityRuntime.expired())
    {
      retval.push_back(static_pointer_cast<EntityRuntime>(shared_from_this()));
    }
    for (auto er : getChildRuntimes())
    {
      if (auto erLock = er.lock())
      {
        retval.push_back(er);
        auto fv = erLock->generateFlatVector();
        fv.insert(fv.end(), fv.begin(), fv.end());
      }
    }
    return retval;
  }

  vector<weak_ptr<EntityRuntime>>
  EntityRuntime::getChildRuntimes
  ()
  const
  {
    vector<weak_ptr<EntityRuntime>> r;
    r.insert(r.begin(),mChildRuntimes.begin(), mChildRuntimes.end());
    return r;
  }

  bool
  EntityRuntime::isParentOf
  (const weak_ptr<EntityRuntime>&  child)
  const
  {
    auto itr = std::find_if(mChildRuntimes.begin(), mChildRuntimes.end(),
                            [&](const shared_ptr<EntityRuntime>& nextChild)
    { return nextChild == child.lock(); });
    return itr != mChildRuntimes.end();
  }

  void
  EntityRuntime::setParentEntityRuntime
  (const weak_ptr<EntityRuntime>& parent)
  {
    if (auto parentLock = parent.lock())
    {
      mParentEntityRuntime = parent;
      setAttribute("parent",parentLock->getUuidString());
    }
  }

  weak_ptr<EntityRuntime>
  EntityRuntime::getParentEntityRuntime
  ()
  const
  {
    return mParentEntityRuntime;
  }

  weak_ptr<SceneRuntime>
  EntityRuntime::getSceneRuntime
  ()
  const
  {
    return mSceneRuntime;
  }

  weak_ptr<EntityDefinition>
  EntityRuntime::getEntityDefinition()
  const
  {
    return static_pointer_cast<EntityDefinition>(getDefinition().lock());
  }

  bool
  EntityRuntime::loadChildrenFromDefinition
  (const weak_ptr<EntityDefinition>& definition)
  {
    if (auto defLock = definition.lock())
    {
      vector<weak_ptr<EntityDefinition>> definitions = defLock->getChildDefinitionsVector();

      for (auto it = begin(definitions); it != end(definitions); it++)
      {
        if (auto entityDef = (*it).lock())
        {
          if (!entityDef->getIsTemplate())
          {
            createAndAddChildRuntime(entityDef);
          }
        }
      }
    }
    return true;
  }

  void
  EntityRuntime::setTransform
  (const Transform& transform)
  {
    mCurrentTransform = transform;
  }

  Transform
  EntityRuntime::getInitialTransform
  ()
  const
  {
    return mInitialTransform;
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

  void
  EntityRuntime::removeChildRuntime
  (const weak_ptr<EntityRuntime>& child)
  {
    if (auto childLock = child.lock())
    {
      childLock->setDeleted(true);
    }
  }

  void
  EntityRuntime::addChildRuntime
  (const shared_ptr<EntityRuntime>& rt)
  {
    mChildRuntimes.push_back(rt);
  }

  weak_ptr<EntityRuntime>
  EntityRuntime::createAndAddChildRuntime
  (const weak_ptr<EntityDefinition>& def)
  {
    auto child = make_shared<EntityRuntime>(mProjectRuntime,  mSceneRuntime, def, mRandomUuid);
    child->setParentEntityRuntime(static_pointer_cast<EntityRuntime>(shared_from_this()));
    if (!child->loadFromDefinition())
    {
      LOG_ERROR("EntityRuntime: Error creating child runtime");
      return weak_ptr<EntityRuntime>();
    }
    addChildRuntime(child);
    return child;
  }

  weak_ptr<EntityRuntime>
  EntityRuntime::addChildFromTemplateUuid
  (UuidType uuid)
  {
    if (auto srLock = mSceneRuntime.lock())
    {
    	// Get the scene def
      if (auto srDefLock = srLock->getDefinition().lock())
      {
        auto sceneDef = static_pointer_cast<SceneDefinition>(srDefLock);

        // Get the template def
        if (auto def = sceneDef->getTemplateByUuid(uuid).lock())
        {
          if (def->getIsTemplate())
          {
            // Create child
            auto child = make_shared<EntityRuntime>(mProjectRuntime, mSceneRuntime, def,  true);
            child->setParentEntityRuntime(static_pointer_cast<EntityRuntime>(shared_from_this()));
            // Use definitoin
            if (child->loadFromDefinition())
            {
              // Add runtime
              addChildRuntime(child);
              LOG_DEBUG("EntityRuntime: Successfully added child from template {}",def->getNameAndUuidString());
              return child;
            }
          }
        }
      }
    }
    return weak_ptr<EntityRuntime>();
  }


  bool
  EntityRuntime::loadFromDefinition
  ()
  {
    if (auto defLock = mDefinition.lock())
    {
      auto def = static_pointer_cast<EntityDefinition>(defLock);
      LOG_TRACE( "EntityRuntime: Using Definition {}", def->getNameAndUuidString());
      setName(def->getName());
      setUuid(mRandomUuid ? Uuid::generateUuid() : def->getUuid());
      setTransform(def->getTransform());
      setAssetDefinitionsMap(def->getAssetDefinitionsMap());

      if (createAssetRuntimes())
      {
        if(loadChildrenFromDefinition(def))
        {
          return true;
        }
      }
    }
    return false;
  }

  BoundingBox
  EntityRuntime::getBoundingBox
  ()
  const
  {
    if (auto mrLock = mModelRuntime.lock())
    {
      return mrLock->getBoundingBox();
    }
    return mBoundingBox;
  }

  void
  EntityRuntime::setBoundingBox
  (const BoundingBox& boundingBox)
  {
    mBoundingBox = boundingBox;
  }

  float
  EntityRuntime::distanceFrom
  (const weak_ptr<EntityRuntime>& other)
  const
  {
    if (auto otherLock = other.lock())
    {
      return mCurrentTransform.distanceFrom(otherLock->getTransform());
    }
    return 0.f;
  }

  float
  EntityRuntime::distanceFrom
  (const vec3& other)
  const
  {
    return glm::distance(mCurrentTransform.getTranslation(),other);
  }

  bool
  EntityRuntime::visibleInFrustum
  ()
  {
    if (auto srLock = mSceneRuntime.lock())
    {
      if (auto cam = srLock->getCamera().lock())
      {
        return cam->visibleInFrustum(static_pointer_cast<EntityRuntime>(shared_from_this()));
      }
    }
    return false;
  }

  bool
  EntityRuntime::containedInFrustum
  ()
  {
    if (auto srLock = mSceneRuntime.lock())
    {
      if (auto cam = srLock->getCamera().lock())
      {
        return cam->containedInFrustum(static_pointer_cast<EntityRuntime>(shared_from_this()));
      }
    }
    return false;
  }

  weak_ptr<EntityScriptOnInitTask>
  EntityRuntime::getScriptOnInitTask
  ()
  const
  {
    return mScriptOnInitTask;
  }

  weak_ptr<EntityScriptOnEventTask>
  EntityRuntime::getScriptOnEventTask
  ()
  const
  {
    return mScriptOnEventTask;
  }

  weak_ptr<EntityScriptOnUpdateTask>
  EntityRuntime::getScriptOnUpdateTask
  ()
  const
  {
    return mScriptOnUpdateTask;
  }

  weak_ptr<EntityScriptCreateStateTask>
  EntityRuntime::getScriptCreateStateTask()
  const
  {
    return mScriptCreateStateTask;
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

  vec4 EntityRuntime::getFontColor() const
  {
    return mFontColor;
  }

  void EntityRuntime::setFontColor(const vec4& fontColor)
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
    if (mAnimationRuntime) mAnimationRuntime->pushTasks();
    // Physics
    if (mPhysicsObjectRuntime) mPhysicsObjectRuntime->pushTasks();
    // Path
    if (mPathRuntime) mPathRuntime->pushTasks();
  }

  bool EntityRuntime::allRuntimesLoaded()
  const
  {
    bool all_loaded = true;
    // Shared
    if (auto audio = mAudioRuntime.lock()) all_loaded &= audio->getLoaded();
    if (auto font = mFontRuntime.lock()) all_loaded &= font->getLoaded();
    if (auto model = mModelRuntime.lock()) all_loaded &= model->getLoaded();
    if (auto script = mScriptRuntime.lock()) all_loaded &= script->getLoaded();
    if (auto texture = mTextureRuntime.lock()) all_loaded &= texture->getLoaded();
    // Discrete
    if (mAnimationRuntime) all_loaded &= mAnimationRuntime->getLoaded();
    if (mPathRuntime) all_loaded &= mPathRuntime->getLoaded();
    if (mPhysicsObjectRuntime) all_loaded &= mPhysicsObjectRuntime->getLoaded();
    return  all_loaded;
  }
}
