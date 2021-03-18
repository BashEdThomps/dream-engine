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
 *
 */

#include "ProjectDefinition.h"
#include "Project.h"
#include "Scene/SceneDefinition.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Common/Uuid.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

using std::pair;
using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  ProjectDefinition::ProjectDefinition
  (const json& data)
    : Definition("ProjectDefinition",data)

  {
    LOG_TRACE("ProjectDefinition: Constructing {}", getNameAndUuidString());
  }

  ProjectDefinition::~ProjectDefinition
  ()
  {
    LOG_TRACE("ProjectDefinition: Destructing {}", getNameAndUuidString());
    deleteSceneDefinitions();
    deleteAssetDefinitions();
  }

  void
  ProjectDefinition::setStartupSceneUuid
  (UuidType sceneUuid)
  {
    mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
  }

  UuidType
  ProjectDefinition::getStartupSceneUuid
  ()
  const
  {
    if (mJson.find(Constants::PROJECT_STARTUP_SCENE) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::PROJECT_STARTUP_SCENE];
  }

  void
  ProjectDefinition::loadChildDefinitions
  ()
  {
    loadAssetDefinitions();
    loadSceneDefinitions();
  }

  map<AssetType, vector<string>>
  ProjectDefinition::getAssetDefinitionGroups
  ()
  const
  {
    return mAssetDefinitionGroups;
  }

  void
  ProjectDefinition::removeAssetDefinition
  (const weak_ptr<AssetDefinition>& assetDefinition)
  {
    auto adLock = assetDefinition.lock();
    LOG_DEBUG("ProjectDefinition: Removing AssetDefinition {} from {}",
              adLock->getNameAndUuidString(),getNameAndUuidString());

    for (auto itr = begin(mAssetDefinitions); itr != end(mAssetDefinitions); ++itr)
    {
      if ((*itr) == adLock)
      {
        LOG_DEBUG("ProjectDefinition: Found AssetDefinition to {} remove from {}",
                  adLock->getNameAndUuidString(), getNameAndUuidString());
        mAssetDefinitions.erase(itr);
        return;
      }
    }
  }

  size_t
  ProjectDefinition::countAssetDefinitions
  ()
  const
  {
    return mAssetDefinitions.size();
  }

  weak_ptr<AssetDefinition>
  ProjectDefinition::getAssetDefinitionByUuid
  (UuidType uuid)
  const
  {
    for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
    {
      if ((*it)->hasUuid(uuid))
      {
        return (*it);
      }
    }
    return weak_ptr<AssetDefinition>();
  }

  weak_ptr<AssetDefinition>
  ProjectDefinition::getAssetDefinitionByName
  (const string& name)
  const
  {
    for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
    {
      if ((*it)->hasName(name))
      {
        return (*it);
      }
    }
    return weak_ptr<AssetDefinition>();
  }

  void
  ProjectDefinition::addAssetDefinition
  (const shared_ptr<AssetDefinition>& def)
  {
    mAssetDefinitions.push_back(def);
    regroupAssetDefinitions();
  }

  size_t
  ProjectDefinition::countScenesDefinitions
  ()
  const
  {
    return mSceneDefinitions.size();
  }

  weak_ptr<SceneDefinition>
  ProjectDefinition::getSceneDefinitionByName
  (const string &name)
  const
  {
    for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
    {
      if ((*it)->hasName(name))
      {
        return (*it);
      }
    }
    return weak_ptr<SceneDefinition>();
  }

  weak_ptr<SceneDefinition>
  ProjectDefinition::getSceneDefinitionByUuid
  (UuidType uuid)
  const
  {
    for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
    {
      if ((*it)->hasUuid(uuid))
      {
        return (*it);
      }
    }
    return weak_ptr<SceneDefinition>();
  }

  vector<weak_ptr<SceneDefinition>>
  ProjectDefinition::getSceneDefinitionsVector
  ()
  const
  {
    vector<weak_ptr<SceneDefinition>> ret;
    for (auto sd : mSceneDefinitions)
    {
      ret.push_back(sd);
    }
    return ret;
  }

  void
  ProjectDefinition::removeSceneDefinition
  (const weak_ptr<SceneDefinition>& sceneDef)
  {
    if (auto sdLock = sceneDef.lock())
    {
      LOG_DEBUG("ProjectDefinition: Removing SceneDefinition {} from {}",
                sdLock->getNameAndUuidString(), getNameAndUuidString());

      auto iter = std::find(mSceneDefinitions.begin(), mSceneDefinitions.end(), sdLock);
      auto endPos = end(mSceneDefinitions);

      if (iter != endPos)
      {
        LOG_DEBUG("ProjectDefinition: Found scene to {} remove from {}",
                  sdLock->getNameAndUuidString(), getNameAndUuidString());
        mSceneDefinitions.erase(iter);
        return;
      }
    }
  }

  vector<weak_ptr<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionsVectorForType
  ()
  const
  {
    vector<weak_ptr<AssetDefinition>> ret;
    for (auto ad : mAssetDefinitions)
    {
      ret.push_back(ad);
    }
    return ret;
  }

  vector<weak_ptr<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionsVectorForType
  (AssetType type)
  const
  {
    vector<weak_ptr<AssetDefinition>> definitionsList;
    for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
    {
      auto thisAssetType = Constants::getAssetTypeEnumFromString((*it)->getType());
      if (thisAssetType == type)
      {
        definitionsList.push_back((*it));
      }
    }
    return definitionsList;
  }


  weak_ptr<SceneDefinition>
  ProjectDefinition::createNewSceneDefinition
  ()
  {
    json scene;
    scene[Constants::UUID] = Uuid::generateUuid();
    scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
    auto sd = make_shared<SceneDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), scene);
    sd->createNewRootEntityDefinition();
    mSceneDefinitions.push_back(sd);
    return sd;
  }

  weak_ptr<AssetDefinition>
  ProjectDefinition::createNewAssetDefinition
  (AssetType type)
  {
    json assetDefinitionJson;

    string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
    LOG_DEBUG("ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);

    assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
    assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
    assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
    assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
    auto ad = createAssetDefinition(assetDefinitionJson);
    mAssetDefinitions.push_back(ad);
    return ad;
  }

  weak_ptr<SceneDefinition>
  ProjectDefinition::getStartupSceneDefinition
  ()
  const
  {
    UuidType startupScene = getStartupSceneUuid();
    LOG_DEBUG("ProjectDefinition: Finding startup scene {}", startupScene);
    return getSceneDefinitionByUuid(startupScene);
  }

  json
  ProjectDefinition::toJson
  ()
  {
    mJson[Constants::PROJECT_ASSET_ARRAY] = json::array();
    for (auto ad : getAssetDefinitionsVectorForType())
    {
      mJson[Constants::PROJECT_ASSET_ARRAY].push_back(ad.lock()->toJson());
    }

    mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
    for (auto sd : getSceneDefinitionsVector())
    {
      mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd.lock()->toJson());
    }

    return mJson;
  }

  map<AssetType, vector<weak_ptr<AssetDefinition>>>
  ProjectDefinition::getAssetDefinitionsMap
  ()
  const
  {
    map<AssetType, vector<weak_ptr<AssetDefinition>>> handlesMap;
    auto endMap = end(handlesMap);

    // Iterate over AD s
    for (auto itr = begin(mAssetDefinitions); itr != end(mAssetDefinitions); ++itr)
    {
      // Current AD type
      AssetType currentType = Constants::getAssetTypeEnumFromString((*itr)->getType());
      // Find type vector in map
      auto typeVector = handlesMap.find(currentType);
      // Vector does not exist
      if (typeVector == endMap)
      {
        // Create it
        vector<weak_ptr<AssetDefinition>> typeVector;
        handlesMap.insert(
              std::pair<AssetType, vector<weak_ptr<AssetDefinition>>>
              (currentType, typeVector));
      }
      handlesMap.at(currentType).push_back((*itr));
    }
    return handlesMap;
  }

  void
  ProjectDefinition::deleteAssetDefinitions
  ()
  {
    mAssetDefinitions.clear();
  }

  void
  ProjectDefinition::deleteSceneDefinitions
  ()
  {
    mSceneDefinitions.clear();
  }

  long
  ProjectDefinition::getSceneDefinitionIndex
  (const weak_ptr<SceneDefinition>& sDef)
  const
  {
    auto it = std::find(mSceneDefinitions.begin(), mSceneDefinitions.end(), sDef.lock());
    if (it == mSceneDefinitions.end())
    {
      return -1;
    }
    else
    {
      return std::distance(mSceneDefinitions.begin(), it);
    }
  }

  weak_ptr<SceneDefinition>
  ProjectDefinition::getSceneDefinitionAtIndex
  (int index)
  const
  {
    return mSceneDefinitions.at(index);
  }

  long
  ProjectDefinition::getAssetDefinitionIndex
  (AssetType type, const weak_ptr<AssetDefinition>& sDef)
  const
  {
    vector<weak_ptr<AssetDefinition>> defs = getAssetDefinitionsVectorForType(type);
    auto it = std::find_if(defs.begin(), defs.end(), [&](const weak_ptr<AssetDefinition>& nextDef){ return nextDef.lock() == sDef.lock();});
    if (it == defs.end())
    {
      return -1;
    }
    else
    {
      return std::distance(defs.begin(), it);
    }
  }

  long
  ProjectDefinition::getAssetDefinitionIndex
  (AssetType type, UuidType uuid)
  const
  {
    vector<weak_ptr<AssetDefinition>> defs = getAssetDefinitionsVectorForType(type);

    auto it = std::find_if(defs.begin(), defs.end(),
                           [&](const weak_ptr<AssetDefinition>& ad)
    { return ad.lock()->getUuid() == uuid; } );

    if (it == defs.end())
    {
      return -1;
    }
    else
    {
      return std::distance(defs.begin(), it);
    }
  }

  weak_ptr<AssetDefinition>
  ProjectDefinition::getAssetDefinitionAtIndex
  (AssetType type, int idx)
  const
  {
    return getAssetDefinitionsVectorForType(type).at(idx);
  }

  vector<string>
  ProjectDefinition::getAssetNamesVector
  (AssetType type)
  const
  {
    vector<string> retval;
    auto assets = getAssetDefinitionsVectorForType(type);
    for (auto asset : assets)
    {
      retval.push_back(asset.lock()->getName());
    }
    return retval;
  }

  // Private =================================================================

  void
  ProjectDefinition::loadSceneDefinitions
  ()
  {
    LOG_DEBUG("ProjectDefinition: Loading ScenesDefinitions from JSON");

    for (const json& it : mJson[Constants::PROJECT_SCENE_ARRAY])
    {
      loadSingleSceneDefinition(it);
    }
  }

  void
  ProjectDefinition::loadAssetDefinitions
  ()
  {
    LOG_DEBUG("ProjectDefinition: Loading AssetDefinitions from JSON");

    for (const json& it : mJson[Constants::PROJECT_ASSET_ARRAY])
    {
      loadSingleAssetDefinition(it);
    }
    regroupAssetDefinitions();
  }

  void
  ProjectDefinition::loadSingleAssetDefinition
  (const json &assetDefinitionJs)
  {

    shared_ptr<AssetDefinition> newDef = createAssetDefinition(assetDefinitionJs);
    if (newDef != nullptr)
    {
      mAssetDefinitions.push_back(newDef);
    }
  }

  void
  ProjectDefinition::loadSingleSceneDefinition
  (const json &scene)
  {
    auto so = make_shared<SceneDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), scene);
    so->loadRootEntityDefinition();
    mSceneDefinitions.push_back(so);
  }

  void
  ProjectDefinition::regroupAssetDefinitions
  ()
  {
    mAssetDefinitionGroups.clear();
    for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
    {
      mAssetDefinitionGroups.insert(
            pair<AssetType,vector<string> >(typePair.first,vector<string>()));
    }

    for (auto ad : mAssetDefinitions)
    {
      auto group = ad->getGroup();
      auto& groups = mAssetDefinitionGroups[Constants::getAssetTypeEnumFromString(ad->getType())];
      if (find(groups.begin(),groups.end(), group) == groups.end())
      {
        groups.push_back(group);
      }
    }
  }

  shared_ptr<AssetDefinition>
  ProjectDefinition::createAssetDefinition
  (const json &assetDefinitionJs)
  {

    AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

    switch (type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        return make_shared<AnimationDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_AUDIO:
        return make_shared<AudioDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_FONT:
        return make_shared<FontDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_MATERIAL:
        return make_shared<MaterialDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_MODEL:
        return make_shared<ModelDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_PATH:
        return make_shared<PathDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        return make_shared<PhysicsObjectDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_SCRIPT:
        return make_shared<ScriptDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_SHADER:
        return make_shared<ShaderDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_TEXTURE:
        return make_shared<TextureDefinition>(static_pointer_cast<ProjectDefinition>(shared_from_this()), assetDefinitionJs);
      case ASSET_TYPE_ENUM_NONE:
        LOG_ERROR("ProjectDefinition: Unable to create Asset Definition. Unknown Type");
        break;
    }
    return nullptr;
  }

  // Static ==================================================================

  shared_ptr<ProjectDefinition>
  ProjectDefinition::createNewProjectDefinition
  (const string &name)
  {
    json j = json::object();
    j[Constants::NAME] = name;
    j[Constants::UUID] = Uuid::generateUuid();
    j[Constants::PROJECT_STARTUP_SCENE] = Uuid::INVALID;
    j[Constants::PROJECT_ASSET_ARRAY] = json::array();
    j[Constants::PROJECT_SCENE_ARRAY] = json::array();
    return make_shared<ProjectDefinition>(j);
  }
}
