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

#include "Common/Uuid.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

namespace octronic::dream
{
  // Static ====================================================================

  ProjectDefinition::ProjectDefinition
  (const string &name)
    : Definition(json::object())
  {
    LOG_TRACE("ProjectDefinition: Constructing New ProjectDefinition");
    mJson[Constants::NAME] = name;
    mJson[Constants::UUID] = Uuid::generateUuid();
    mJson[Constants::PROJECT_STARTUP_SCENE] = Uuid::INVALID;
    mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_FONT_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_MODEL_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_PATH_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_PHYSICS_OBJECT_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_SHADER_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY] = json::array();
    mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
    mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY] = json::array();
  }

  ProjectDefinition::ProjectDefinition
  (const json& data)
    : Definition(data)
  {
    LOG_TRACE("ProjectDefinition: Constructing from JSON {}", getNameAndUuidString());
    loadAllAssetDefinitions();
    loadAllSceneDefinitions();
    loadTemplateEntityDefinitions();
  }

  json
  ProjectDefinition::toJson
  ()
  {
    mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY] = json::array();
    for (auto& ed : mTemplateEntityDefinitions)
    {
      mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY].push_back(ed.toJson());
    }

    mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY] = json::array();
    for (auto& ad : mAnimationDefinitions)
    {
      mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY] = json::array();
    for (auto& ad : mAudioDefinitions)
    {
      mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_FONT_ASSET_ARRAY] = json::array();
    for (auto& ad : mFontDefinitions)
    {
      mJson[Constants::PROJECT_FONT_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY] = json::array();
    for (auto& ad : mMaterialDefinitions)
    {
      mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_MODEL_ASSET_ARRAY] = json::array();
    for (auto& ad : mModelDefinitions)
    {
      mJson[Constants::PROJECT_MODEL_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_PATH_ASSET_ARRAY] = json::array();
    for (auto& ad : mPathDefinitions)
    {
      mJson[Constants::PROJECT_PATH_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_PHYSICS_OBJECT_ASSET_ARRAY] = json::array();
    for (auto& ad : mPhysicsObjectDefinitions)
    {
      mJson[Constants::PROJECT_PHYSICS_OBJECT_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY] = json::array();
    for (auto& ad : mScriptDefinitions)
    {
      mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_SHADER_ASSET_ARRAY] = json::array();
    for (auto& ad : mShaderDefinitions)
    {
      mJson[Constants::PROJECT_SHADER_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY] = json::array();
    for (auto& ad : mTextureDefinitions)
    {
      mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY].push_back(ad.toJson());
    }

    mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
    for (auto& sd : mSceneDefinitions)
    {
      mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd.toJson());
    }

    return mJson;
  }

  // Assets ====================================================================

  void
  ProjectDefinition::removeAssetDefinition
  (AssetDefinition& ad)
  {
    static auto byUuid = [&](AssetDefinition& next) {return next.getUuid() == ad.getUuid();};

    switch(ad.getAssetType())
    {
      case ASSET_TYPE_ENUM_ANIMATION:
      {
        auto itr = find_if(mAnimationDefinitions.begin(), mAnimationDefinitions.end(), byUuid);
        if (itr!=mAnimationDefinitions.end()) mAnimationDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_AUDIO:
      {
        auto itr = find_if(mAudioDefinitions.begin(), mAudioDefinitions.end(), byUuid);
        if (itr!=mAudioDefinitions.end()) mAudioDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_FONT:
      {
        auto itr = find_if(mFontDefinitions.begin(), mFontDefinitions.end(), byUuid);
        if (itr!=mFontDefinitions.end()) mFontDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_MATERIAL:
      {
        auto itr = find_if(mMaterialDefinitions.begin(), mMaterialDefinitions.end(), byUuid);
        if (itr!=mMaterialDefinitions.end()) mMaterialDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_MODEL:
      {
        auto itr = find_if(mModelDefinitions.begin(), mModelDefinitions.end(), byUuid);
        if (itr!=mModelDefinitions.end()) mModelDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_PATH:
      {
        auto itr = find_if(mPathDefinitions.begin(), mPathDefinitions.end(), byUuid);
        if (itr!=mPathDefinitions.end()) mPathDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
      {
        auto itr = find_if(mPhysicsObjectDefinitions.begin(), mPhysicsObjectDefinitions.end(), byUuid);
        if (itr!=mPhysicsObjectDefinitions.end()) mPhysicsObjectDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_SCRIPT:
      {
        auto itr = find_if(mScriptDefinitions.begin(), mScriptDefinitions.end(), byUuid);
        if (itr!=mScriptDefinitions.end()) mScriptDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_SHADER:
      {
        auto itr = find_if(mShaderDefinitions.begin(), mShaderDefinitions.end(), byUuid);
        if (itr!=mShaderDefinitions.end()) mShaderDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_TEXTURE:
      {
        auto itr = find_if(mTextureDefinitions.begin(), mTextureDefinitions.end(), byUuid);
        if (itr!=mTextureDefinitions.end()) mTextureDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
  }

  size_t
  ProjectDefinition::countAssetDefinitions
  (AssetType t)
  {
    return getAssetDefinitionsVector(t).size();
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionByPredicate
  (AssetType type, std::function<bool(AssetDefinition&)> fn)
  {
    auto v = getAssetDefinitionsVector(type);
    auto itr = find_if(v.begin(), v.end(), fn);
    if (itr != v.end()) return (*itr).get();
    return std::nullopt;
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionByName
  (AssetType type, const string& name)
  {
    auto predicate = [&](AssetDefinition& ad) { return ad.getName() == name; };
    return getAssetDefinitionByPredicate(type, predicate);
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionByUuid
  (AssetType type, UuidType id)
  {
    auto predicate = [&](AssetDefinition& ad) { return ad.getUuid() == id; };
    return getAssetDefinitionByPredicate(type, predicate);
  }

  vector<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionsVector
  (AssetType type)
  {
    vector<reference_wrapper<AssetDefinition>> defs;

    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        for (auto& def : mAnimationDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_AUDIO:
        for (auto& def : mAudioDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_FONT:
        for (auto& def : mFontDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
      break;
      case ASSET_TYPE_ENUM_MATERIAL:
        for (auto& def : mMaterialDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_MODEL:
        for (auto& def : mModelDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_PATH:
        for (auto& def : mPathDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        for (auto& def : mPhysicsObjectDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_SCRIPT:
        for (auto& def : mScriptDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_SHADER:
        for (auto& def : mShaderDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_TEXTURE:
        for (auto& def : mTextureDefinitions)
        {
          defs.push_back(reference_wrapper<AssetDefinition>(def));
        }
        break;
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    return defs;
  }

  AssetDefinition&
  ProjectDefinition::createNewAssetDefinition
  (AssetType type)
  {
    json assetDefinitionJs;

    string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
    LOG_DEBUG("ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);
    assetDefinitionJs[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
    assetDefinitionJs[Constants::UUID] = Uuid::generateUuid();
    assetDefinitionJs[Constants::ASSET_TYPE] = AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);
    assetDefinitionJs[Constants::ASSET_FORMAT] = defaultFormat;

    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        return mAnimationDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_AUDIO:
        return mAudioDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_FONT:
        return mFontDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_MATERIAL:
        return mMaterialDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_MODEL:
        return mModelDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_PATH:
        return mPathDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        return mPhysicsObjectDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_SCRIPT:
        return mScriptDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_SHADER:
        return mShaderDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_TEXTURE:
        return mTextureDefinitions.emplace_back(*this, assetDefinitionJs);
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    throw std::exception();
  }

  void
  ProjectDefinition::deleteAllAssetDefinitions
  ()
  {
    mAnimationDefinitions.clear();
    mAudioDefinitions.clear();
    mFontDefinitions.clear();
    mMaterialDefinitions.clear();
    mModelDefinitions.clear();
    mPathDefinitions.clear();
    mPhysicsObjectDefinitions.clear();
    mScriptDefinitions.clear();
    mShaderDefinitions.clear();
    mTextureDefinitions.clear();
  }

  unsigned long
  ProjectDefinition::getAssetDefinitionIndex
  (AssetDefinition& def)
  {
    auto defs = getAssetDefinitionsVector(def.getAssetType());
    auto it = std::find_if(defs.begin(), defs.end(),
                           [&](AssetDefinition& ad) { return ad.getUuid() == def.getUuid(); } );

    if (it == defs.end())
    {
      return -1;
    }
    else
    {
      return std::distance(defs.begin(), it);
    }
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionAtIndex
  (AssetType type, int idx)
  {
    return getAssetDefinitionsVector(type).at(idx);
  }

  vector<string>
  ProjectDefinition::getAssetNamesVector
  (AssetType type)
  {
    vector<string> retval;
    auto assets = getAssetDefinitionsVector(type);
    for (auto asset : assets) retval.push_back(asset.get().getName());
    return retval;
  }

  void
  ProjectDefinition::loadAllAssetDefinitions
  ()
  {
    LOG_DEBUG("ProjectDefinition: Loading AssetDefinitions from JSON");

    if (mJson.find(Constants::PROJECT_ANIMATION_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_AUDIO_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_FONT_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_FONT_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_MATERIAL_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_MODEL_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_MODEL_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_PATH_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_PATH_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_PHYSICS_OBJECT_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_PHYSICS_OBJECT_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_SCRIPT_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_SHADER_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_SHADER_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }

    if (mJson.find(Constants::PROJECT_TEXTURE_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY])
      {
        loadSingleAssetDefinition(it);
      }
    }
  }

  void
  ProjectDefinition::loadSingleAssetDefinition
  (const json &assetDefinitionJs)
  {
    auto typeStr = assetDefinitionJs[Constants::ASSET_TYPE];
    auto type = AssetTypeHelper::GetAssetTypeEnumFromString(typeStr);

    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        mAnimationDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_AUDIO:
        mAudioDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_FONT:
        mFontDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_MATERIAL:
        mMaterialDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_MODEL:
        mModelDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_PATH:
        mPathDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
        mPhysicsObjectDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_SCRIPT:
        mScriptDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_SHADER:
        mShaderDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_TEXTURE:
        mTextureDefinitions.emplace_back(*this, assetDefinitionJs);
        break;
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
  }

  // Scenes ====================================================================

  void
  ProjectDefinition::loadAllSceneDefinitions
  ()
  {
    LOG_DEBUG("ProjectDefinition: Loading ScenesDefinitions from JSON");

    if (mJson.find(Constants::PROJECT_SCENE_ARRAY) != mJson.end())
    {
      for (auto& js : mJson[Constants::PROJECT_SCENE_ARRAY])
      {
        mSceneDefinitions.emplace_back(*this, js);
      }
    }
  }

  void
  ProjectDefinition::deleteAllSceneDefinitions
  ()
  {
    mSceneDefinitions.clear();
  }

  long
  ProjectDefinition::getSceneDefinitionIndex
  (SceneDefinition& sDef)
  const
  {
    auto it = std::find_if(mSceneDefinitions.begin(), mSceneDefinitions.end(),
                           [&](const SceneDefinition& next){return next == sDef;});
    if (it == mSceneDefinitions.end())
    {
      return -1;
    }
    else
    {
      return std::distance(mSceneDefinitions.begin(), it);
    }
    return -1;
  }

  SceneDefinition&
  ProjectDefinition::getSceneDefinitionAtIndex
  (int index)
  {
    return mSceneDefinitions.at(index);
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

  size_t
  ProjectDefinition::countScenesDefinitions
  ()
  const
  {
    return mSceneDefinitions.size();
  }

  optional<reference_wrapper<SceneDefinition>>
  ProjectDefinition::getSceneDefinitionByName
  (const string &name)
  {
    for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
    {
      if ((*it).hasName(name))
      {
        return (*it);
      }
    }
    return std::nullopt;
  }

  optional<reference_wrapper<SceneDefinition>>
  ProjectDefinition::getSceneDefinitionByUuid
  (UuidType uuid)
  {
    for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
    {
      if ((*it).hasUuid(uuid))
      {
        return (*it);
      }
    }
    return std::nullopt;
  }

  vector<SceneDefinition>&
  ProjectDefinition::getSceneDefinitionsVector
  ()
  {
    return mSceneDefinitions;
  }

  void
  ProjectDefinition::removeSceneDefinition
  (SceneDefinition& sceneDef)
  {
    LOG_DEBUG("ProjectDefinition: Removing SceneDefinition {} from {}",
              sceneDef.getNameAndUuidString(), getNameAndUuidString());

    auto itr = find(mSceneDefinitions.begin(), mSceneDefinitions.end(), sceneDef);
    if (itr != mSceneDefinitions.end())
    {
      mSceneDefinitions.erase(itr);
    }
  }

  SceneDefinition&
  ProjectDefinition::createNewSceneDefinition
  ()
  {
    json scene;
    scene[Constants::UUID] = Uuid::generateUuid();
    scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
    scene[Constants::SCENE_ROOT_SCENE_ENTITY] = json::object();
    return mSceneDefinitions.emplace_back(*this, scene);
  }

  optional<reference_wrapper<SceneDefinition>>
  ProjectDefinition::getStartupSceneDefinition
  ()
  {
    UuidType startupScene = getStartupSceneUuid();
    LOG_DEBUG("ProjectDefinition: Finding startup scene {}", startupScene);
    return getSceneDefinitionByUuid(startupScene);
  }

  TemplateEntityDefinition&
  ProjectDefinition::createTemplateEntityDefinition
  ()
  {
    json defJson;
    defJson[Constants::NAME] = Constants::TEMPLATE_ENTITY_DEFAULT_NAME;
    defJson[Constants::UUID] = Uuid::generateUuid();
    return mTemplateEntityDefinitions.emplace_back(*this,defJson);
  }

  vector<string>
  ProjectDefinition::getTemplateEntityNamesVector
  ()
  {
    vector<string> names;
    for (auto& entity : mTemplateEntityDefinitions)
    {
      names.push_back(entity.getName());
    }
    return names;
  }

  void
  ProjectDefinition::loadTemplateEntityDefinitions
  ()
  {
    if (mJson.find(Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY) == mJson.end())
    {
      LOG_ERROR( "SceneDefinition: No Entity Definitions Found!!" );
      return;
    }

    for (auto& entityJs : mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY])
    {
      mTemplateEntityDefinitions.emplace_back(*this, entityJs);
    }
  }

  optional<reference_wrapper<TemplateEntityDefinition>>
  ProjectDefinition::getTemplateEntityDefinitionByUuid
  (UuidType uuid)
  {
    auto itr = find_if(mTemplateEntityDefinitions.begin(),
                       mTemplateEntityDefinitions.end(),
                       [&](const TemplateEntityDefinition& next)
    { return next.getUuid() == uuid; });

    if (itr != mTemplateEntityDefinitions.end()) return (*itr);

    return std::nullopt;
  }

  void
  ProjectDefinition::removeTemplateEntityDefinitionByUuid
  (UuidType id)
  {
    remove_if(mTemplateEntityDefinitions.begin(),
                       mTemplateEntityDefinitions.end(),
                       [&](const TemplateEntityDefinition& next)
    { return next.getUuid() == id; });

  }

  void
  ProjectDefinition::removeTemplateEntityDefinition
  (TemplateEntityDefinition& def)
  {
 		removeTemplateEntityDefinitionByUuid(def.getUuid());
  }

}

