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

#include "Common/Uuid.h"
#include "Components/Time.h"
#include "Components/Cache.h"
#include "Components/Event.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptComponent.h"
#include "TemplateEntityDefinition.h"
#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Project/ProjectRuntime.h"
#include "Project/ProjectDefinition.h"

#include <iostream>

using std::vector;
using std::make_shared;

namespace octronic::dream
{
  EntityRuntime::EntityRuntime
  (ProjectRuntime& projRunt,
   SceneRuntime& sceneRunt,
   SceneEntityDefinition& sceneEntDef,
   TemplateEntityDefinition& templDef)
    : Runtime(sceneEntDef),
      mProjectRuntime(projRunt),
      mSceneRuntime(sceneRunt),
      mTemplateEntityDefinition(templDef),
      mFontText(""),
      mFontColor(1.f),
      mFontScale(1.f),
      mScriptError(false),
      mScriptInitialised(false),
      mDeleted(false)
  {
    LOG_TRACE("EntityRuntime: {}", __FUNCTION__);
  }

  void
  EntityRuntime::removeAnimationRuntime
  ()
  {
    mAnimationRuntime.reset();
  }

  void
  EntityRuntime::removeAudioRuntime
  ()
  {
    if (mAudioRuntime)
    {
      mAudioRuntime.value().get().removeInstance(*this);
      mAudioRuntime.reset();
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
    if (mModelRuntime)
    {
      mModelRuntime.value().get().removeInstance(*this);
      mModelRuntime.reset();
    }
  }

  void
  EntityRuntime::removeFontRuntime
  ()
  {
    if (mFontRuntime)
    {
      mFontRuntime.value().get().removeInstance(*this);
      mFontRuntime.reset();
    }
  }

  void
  EntityRuntime::removeTextureRuntime
  ()
  {
    if (mTextureRuntime)
    {
      mTextureRuntime.value().get().removeInstance(*this);
      mTextureRuntime.reset();
    }
  }

  void
  EntityRuntime::removeScriptRuntime
  ()
  {
    if (mScriptRuntime)
    {
      auto& dq = getProjectRuntime().getDestructionTaskQueue();
      dq.pushTask(mScriptRemoveStateTask);
      mScriptRuntime.value().get().removeInstance(*this);
      mScriptRuntime.reset();
    }
  }

  void
  EntityRuntime::removePhysicsObjectRuntime
  ()
  {
    if (mPhysicsObjectRuntime)
    {
      auto& pr = getSceneRuntime().getProjectRuntime();
      auto& physicsComp = pr.getPhysicsComponent();
      physicsComp.removePhysicsObjectRuntime(getPhysicsObjectRuntime());
      mPhysicsObjectRuntime.reset();
    }
  }

  AnimationRuntime&
  EntityRuntime::getAnimationRuntime
  ()
  {
    return mAnimationRuntime.value();
  }

  AudioRuntime&
  EntityRuntime::getAudioRuntime
  ()
  const
  {
    return mAudioRuntime.value();
  }

  FontRuntime&
  EntityRuntime::getFontRuntime
  ()
  const
  {
    return mFontRuntime.value();
  }

  ModelRuntime&
  EntityRuntime::getModelRuntime
  ()
  const
  {
    return mModelRuntime.value();
  }

  PathRuntime&
  EntityRuntime::getPathRuntime
  ()
  {
    return mPathRuntime.value();
  }

  PhysicsObjectRuntime&
  EntityRuntime::getPhysicsObjectRuntime
  ()
  {
    return mPhysicsObjectRuntime.value();
  }

  ScriptRuntime&
  EntityRuntime::getScriptRuntime
  ()
  const
  {
    return mScriptRuntime.value();
  }

  TextureRuntime&
  EntityRuntime::getTextureRuntime
  ()
  const
  {
    return mTextureRuntime.value();
  }

  bool
  EntityRuntime::hasFontRuntime()
  const
  {
    return mFontRuntime.has_value();
  }

  bool
  EntityRuntime::hasAnimationRuntime
  ()
  const
  {
    return mAnimationRuntime.has_value();
  }

  bool
  EntityRuntime::hasModelRuntime
  ()
  const
  {
    return mModelRuntime.has_value();
  }

  bool
  EntityRuntime::hasScriptRuntime
  ()
  const
  {
    return mScriptRuntime.has_value();
  }

  bool
  EntityRuntime::hasTextureRuntime()
  const
  {
    return mTextureRuntime.has_value();
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
    mInitialTransform = getSceneEntityDefinition().getTransform();
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
      mEventQueue.push_back(event);
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

    vector<UuidType> toDelete;

    for (auto& child : mChildRuntimes)
    {
      if (child.getDeleted())
      {
        toDelete.push_back(child.getUuid());
      }
    }

    for (auto child : toDelete)
    {
      LOG_TRACE("EntityRuntime: Deleting child {}",child);

      std::remove_if(mChildRuntimes.begin(), mChildRuntimes.end(),
                     [=](EntityRuntime& next)
      { return next.getUuid() == child;} );
    }
    clearProcessedEvents();
  }

  bool
  EntityRuntime::hasPhysicsObjectRuntime
  ()
  const
  {
    return mPhysicsObjectRuntime.has_value();
  }

  bool
  EntityRuntime::hasPathRuntime
  ()
  const
  {
    return mPathRuntime.has_value();
  }

  bool
  EntityRuntime::hasAudioRuntime
  ()
  const
  {
    return mAudioRuntime.has_value();
  }

  bool
  EntityRuntime::createAssetRuntimes
  (map<AssetType,UuidType> assetDefinitions)
  {
    bool result = true;

    auto& pd = static_cast<ProjectDefinition&>(getProjectRuntime().getDefinition());
    for (auto assetPair : assetDefinitions)
    {
      auto defOpt = pd.getAssetDefinitionByUuid(assetPair.first, assetPair.second);
      if (defOpt)
      {
        auto& def = defOpt.value().get();
        LOG_TRACE("EntityRuntime: Creating {}",def.getNameAndUuidString());
        switch (assetPair.first)
        {
          case AssetType::ASSET_TYPE_ENUM_ANIMATION:
            result &= createAnimationRuntime(static_cast<AnimationDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_AUDIO:
            result &= createAudioRuntime(static_cast<AudioDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_FONT:
            result &= createFontRuntime(static_cast<FontDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_MODEL:
            result &= createModelRuntime(static_cast<ModelDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_PATH:
            result &= createPathRuntime(static_cast<PathDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT:
            result &= createPhysicsObjectRuntime(static_cast<PhysicsObjectDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_SCRIPT:
            result &= createScriptRuntime(static_cast<ScriptDefinition&>(def));
            break;
          case AssetType::ASSET_TYPE_ENUM_TEXTURE:
            result &= createTextureRuntime(static_cast<TextureDefinition&>(def));
            break;
            // Not owned by Entities
          case AssetType::ASSET_TYPE_ENUM_NONE:
          case AssetType::ASSET_TYPE_ENUM_MATERIAL:
          case AssetType::ASSET_TYPE_ENUM_SHADER:
            break;
        }
        if (!result) break;
      }
    }
    return result;
  }

  // DiscreteAssetRuntimes ===================================================

  bool
  EntityRuntime::createPhysicsObjectRuntime
  (PhysicsObjectDefinition& definition)
  {
    removePhysicsObjectRuntime();
    LOG_TRACE( "EntityRuntime: Creating Physics Object Asset Runtime." );
    mPhysicsObjectRuntime.emplace(mProjectRuntime,definition,*this);
    return mPhysicsObjectRuntime.value().init();
  }

  bool
  EntityRuntime::createAnimationRuntime
  (AnimationDefinition& definition)
  {
    LOG_TRACE( "EntityRuntime: Creating Animation asset Runtime." );
    removeAnimationRuntime();
    mAnimationRuntime.emplace(mProjectRuntime,definition,*this);
    return mAnimationRuntime.value().init();
  }

  bool
  EntityRuntime::createPathRuntime
  (PathDefinition& definition)
  {
    LOG_TRACE( "EntityRuntime: Creating Path asset Runtime." );
    removePathRuntime();
    mPathRuntime.emplace(getProjectRuntime(),definition,*this);
    return mPathRuntime.value().init();
  }

  // SharedAssetRuntimes =====================================================

  bool
  EntityRuntime::createAudioRuntime
  (AudioDefinition& definition)
  {
    // AudioRuntime must be taken from AudioComponent rather than AudioCache
    auto& audioComp = getProjectRuntime().getAudioComponent();
    mAudioRuntime = audioComp.getAudioRuntime(definition);
    mAudioRuntime.value().get().addInstance(*this);
    return mAudioRuntime.has_value();
  }

  bool
  EntityRuntime::createModelRuntime
  (ModelDefinition& definition)
  {
    removeModelRuntime();
    LOG_INFO("EntityRuntime: Creating Model asset Runtime.");
    auto& cache = getProjectRuntime().getModelCache();
    mModelRuntime = cache.getRuntime(definition);
    getModelRuntime().addInstance(*this);
    return mModelRuntime.has_value();
  }

  bool
  EntityRuntime::createScriptRuntime
  (ScriptDefinition& definition)
  {
    removeScriptRuntime();
    auto& scriptCache = getProjectRuntime().getScriptCache();
    LOG_TRACE("EntityRuntime: Creating Script asset Runtime.");
    mScriptRuntime = scriptCache.getRuntime(definition);
    mScriptCreateStateTask = make_shared<EntityScriptCreateStateTask>(getProjectRuntime(), *this);
    mScriptOnInitTask      = make_shared<EntityScriptOnInitTask>(getProjectRuntime(), *this);
    mScriptOnUpdateTask    = make_shared<EntityScriptOnUpdateTask>(getProjectRuntime(),*this);
    mScriptOnEventTask     = make_shared<EntityScriptOnEventTask>(getProjectRuntime(),*this);
    mScriptRemoveStateTask = make_shared<EntityScriptRemoveStateTask>(getProjectRuntime(),getUuid(), getScriptRuntime());
    getScriptRuntime().addInstance(*this);
    return true;
  }

  bool
  EntityRuntime::createFontRuntime
  (FontDefinition& definition)
  {
    removeFontRuntime();
    auto& fontCache = getProjectRuntime().getFontCache();
    LOG_TRACE( "EntityRuntime: Creating Font Asset Runtime." );
    mFontRuntime = fontCache.getRuntime(definition);
    auto& seDef = static_cast<SceneEntityDefinition&>(getDefinition());
    mFontColor = seDef.getFontColor();
    mFontScale = seDef.getFontScale();
    mFontText = seDef.getFontText();
    getFontRuntime().addInstance(*this);
    return true;
  }

  bool
  EntityRuntime::createTextureRuntime
  (TextureDefinition& texDef)
  {
    removeTextureRuntime();
    auto& textureCache = getProjectRuntime().getTextureCache();
    LOG_TRACE( "EntityRuntime: Creating Texture Asset Runtime." );
    mTextureRuntime = textureCache.getRuntime(texDef);
    getTextureRuntime().addInstance(*this);
    return true;
  }


  // Accessors ===============================================================

  EntityRuntime&
  EntityRuntime::getChildRuntimeByUuid
  (UuidType uuid)
  {
    for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
    {
      auto& next = (*it);
      if (next.hasUuid(uuid)) return next;
    }
    throw std::exception();
  }

  vector<reference_wrapper<EntityRuntime>>
  EntityRuntime::generateFlatVector
  ()
  {
    vector<reference_wrapper<EntityRuntime>> retval;
    if (!mParentEntityRuntime)
    {
      retval.push_back(*this);
    }

    for (auto& er : mChildRuntimes)
    {
      retval.push_back(er);
      auto fv = er.generateFlatVector();
      fv.insert(fv.end(), fv.begin(), fv.end());
    }
    return retval;
  }

  vector<EntityRuntime>&
  EntityRuntime::getChildRuntimes
  ()
  {
    return mChildRuntimes;
  }

  bool
  EntityRuntime::isParentOf
  (EntityRuntime&  child)
  const
  {
    return std::find_if(mChildRuntimes.begin(), mChildRuntimes.end(),
                        [&](const EntityRuntime& next)
    { return next.getUuid() == child.getUuid();}) != mChildRuntimes.end();
  }

  void
  EntityRuntime::setParentEntityRuntime
  (EntityRuntime& parent)
  {
    mParentEntityRuntime = parent;
    setAttribute("parent",parent.getUuidString());
  }

  EntityRuntime&
  EntityRuntime::getParentEntityRuntime
  ()
  {
    return mParentEntityRuntime.value();
  }

  SceneRuntime&
  EntityRuntime::getSceneRuntime
  ()
  {
    return mSceneRuntime.get();
  }

  TemplateEntityDefinition&
  EntityRuntime::getTemplateEntityDefinition()
  {
    return mTemplateEntityDefinition;
  }

  SceneEntityDefinition&
  EntityRuntime::getSceneEntityDefinition()
  {
    return static_cast<SceneEntityDefinition&>(getDefinition());
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
  (EntityRuntime& child)
  {
    std::remove_if(mChildRuntimes.begin(), mChildRuntimes.end(),
                   [&](const EntityRuntime& next){ return next.getUuid() == child.getUuid(); });
  }

  EntityRuntime&
  EntityRuntime::createChildRuntime
  (SceneEntityDefinition& sceneEntDef)
  {
    auto& sceneDef = sceneEntDef.getSceneDefinition();
    auto& pDef = sceneDef.getProjectDefinition();
    auto templateDef = pDef.getTemplateEntityDefinitionByUuid(sceneEntDef.getTemplateUuid());
    auto& child = mChildRuntimes.emplace_back(
          mProjectRuntime,  mSceneRuntime,
          sceneEntDef, templateDef.value().get());

    child.setParentEntityRuntime(*this);

    if (!child.loadFromDefinition())
    {
      LOG_ERROR("EntityRuntime: Error creating child runtime");
      removeChildRuntime(child);
      throw std::exception();
    }
    return child;
  }


  bool
  EntityRuntime::loadFromDefinition
  ()
  {
    auto& sceneEntityDef = static_cast<SceneEntityDefinition&>(getDefinition());
    auto& templateDef = mTemplateEntityDefinition.get();

    LOG_TRACE("EntityRuntime: Using SceneEntity Definition {} with Template {}", sceneEntityDef.getNameAndUuidString(), templateDef.getNameAndUuidString());
    setName(templateDef.getName());
    setUuid(sceneEntityDef.getUuid());
    setTransform(sceneEntityDef.getTransform());
    createAssetRuntimes(templateDef.getAssetDefinitionsMap());
    for (auto& entityDef : sceneEntityDef.getChildDefinitionsVector())
    {
      createChildRuntime(entityDef);
    }
    return true;
  }

  BoundingBox
  EntityRuntime::getBoundingBox
  ()
  const
  {
    if (mModelRuntime)
    {
      return getModelRuntime().getBoundingBox();
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
  (EntityRuntime& other)
  const
  {
    return mCurrentTransform.distanceFrom(other.getTransform());
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
    auto& cam = getSceneRuntime().getCameraRuntime();
    return cam.visibleInFrustum(*this);
  }

  bool
  EntityRuntime::containedInFrustum
  ()
  {
    auto& cam = getSceneRuntime().getCameraRuntime();
    return cam.containedInFrustum(*this);
  }

  shared_ptr<EntityScriptOnInitTask>
  EntityRuntime::getScriptOnInitTask
  ()
  const
  {
    return mScriptOnInitTask;
  }

  shared_ptr<EntityScriptOnEventTask>
  EntityRuntime::getScriptOnEventTask
  ()
  const
  {
    return mScriptOnEventTask;
  }

  shared_ptr<EntityScriptOnUpdateTask>
  EntityRuntime::getScriptOnUpdateTask
  ()
  const
  {
    return mScriptOnUpdateTask;
  }

  shared_ptr<EntityScriptCreateStateTask>
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
    if (mAnimationRuntime) getAnimationRuntime().pushTasks();
    // Physics
    if (mPhysicsObjectRuntime) getPhysicsObjectRuntime().pushTasks();
    // Path
    if (mPathRuntime) getPathRuntime().pushTasks();
  }

  bool EntityRuntime::allRuntimesLoaded()
  const
  {
    bool all_loaded = true;
    // Shared
    if (mAudioRuntime)   all_loaded &= getAudioRuntime().getLoaded();
    if (mFontRuntime)    all_loaded &= getFontRuntime().getLoaded();
    if (mModelRuntime)   all_loaded &= getModelRuntime().getLoaded();
    if (mScriptRuntime)  all_loaded &= getScriptRuntime().getLoaded();
    if (mTextureRuntime) all_loaded &= getTextureRuntime().getLoaded();
    // Discrete
    if (mAnimationRuntime)     all_loaded &= mAnimationRuntime.value().getLoaded();
    if (mPathRuntime)          all_loaded &= mPathRuntime.value().getLoaded();
    if (mPhysicsObjectRuntime) all_loaded &= mPhysicsObjectRuntime.value().getLoaded();
    return  all_loaded;
  }

  AssetRuntime&
  EntityRuntime::getAssetRuntime
  (AssetType type)
  {
    switch (type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        return getAnimationRuntime();
      case ASSET_TYPE_ENUM_AUDIO:
        return getAudioRuntime();
      case ASSET_TYPE_ENUM_FONT:
        return getFontRuntime();
      case ASSET_TYPE_ENUM_MODEL:
        return getModelRuntime();
      case ASSET_TYPE_ENUM_PATH:
        return getPathRuntime();
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        return getPhysicsObjectRuntime();
      case ASSET_TYPE_ENUM_SCRIPT:
        return getScriptRuntime();
      case ASSET_TYPE_ENUM_TEXTURE:
        return getTextureRuntime();
      case ASSET_TYPE_ENUM_MATERIAL:
      case ASSET_TYPE_ENUM_SHADER:
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    throw std::exception();
  }

  ProjectRuntime&
  EntityRuntime::getProjectRuntime
  ()
  const
  {
    return mProjectRuntime.get();
  }
}
