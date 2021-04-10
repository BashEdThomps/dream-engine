#include "ProjectDefinition.h"

#include "Common/Uuid.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

using std::make_unique;

namespace octronic::dream
{

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
  ProjectDefinition::getJson
  ()
  {
    mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY] = json::array();
    for (auto& ed : mTemplateEntityDefinitions)
    {
      mJson[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY].push_back(ed->getJson());
    }

    mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY] = json::array();
    for (auto& ad : mAnimationDefinitions)
    {
      mJson[Constants::PROJECT_ANIMATION_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY] = json::array();
    for (auto& ad : mAudioDefinitions)
    {
      mJson[Constants::PROJECT_AUDIO_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_FONT_ASSET_ARRAY] = json::array();
    for (auto& ad : mFontDefinitions)
    {
      mJson[Constants::PROJECT_FONT_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY] = json::array();
    for (auto& ad : mMaterialDefinitions)
    {
      mJson[Constants::PROJECT_MATERIAL_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_MODEL_ASSET_ARRAY] = json::array();
    for (auto& ad : mModelDefinitions)
    {
      mJson[Constants::PROJECT_MODEL_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_PATH_ASSET_ARRAY] = json::array();
    for (auto& ad : mPathDefinitions)
    {
      mJson[Constants::PROJECT_PATH_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_PHYSICS_ASSET_ARRAY] = json::array();
    for (auto& ad : mPhysicsDefinitions)
    {
      mJson[Constants::PROJECT_PHYSICS_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY] = json::array();
    for (auto& ad : mScriptDefinitions)
    {
      mJson[Constants::PROJECT_SCRIPT_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_SHADER_ASSET_ARRAY] = json::array();
    for (auto& ad : mShaderDefinitions)
    {
      mJson[Constants::PROJECT_SHADER_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY] = json::array();
    for (auto& ad : mTextureDefinitions)
    {
      mJson[Constants::PROJECT_TEXTURE_ASSET_ARRAY].push_back(ad->getJson());
    }

    mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
    for (auto& sd : mSceneDefinitions)
    {
      mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd->getJson());
    }

    return mJson;
  }

  void
  ProjectDefinition::removeAssetDefinitionByUuid
  (AssetType type, UuidType id)
  {
    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
      {
        auto itr = find_if(mAnimationDefinitions.begin(), mAnimationDefinitions.end(),
                  [&](unique_ptr<AnimationDefinition>& next) {return next->getUuid() == id;});
        if (itr != mAnimationDefinitions.end()) mAnimationDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_AUDIO:
      {
        auto itr = find_if(mAudioDefinitions.begin(), mAudioDefinitions.end(),
                  [&](unique_ptr<AudioDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mAudioDefinitions.end()) mAudioDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_FONT:
      {
        auto itr = find_if(mFontDefinitions.begin(), mFontDefinitions.end(),
                  [&](unique_ptr<FontDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mFontDefinitions.end()) mFontDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_MATERIAL:
      {
        auto itr = find_if(mMaterialDefinitions.begin(), mMaterialDefinitions.end(),
                  [&](unique_ptr<MaterialDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mMaterialDefinitions.end()) mMaterialDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_MODEL:
      {
        auto itr = find_if(mModelDefinitions.begin(), mModelDefinitions.end(),
                  [&](unique_ptr<ModelDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mModelDefinitions.end()) mModelDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_PATH:
      {
        auto itr = find_if(mPathDefinitions.begin(), mPathDefinitions.end(),
                  [&](unique_ptr<PathDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mPathDefinitions.end()) mPathDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_PHYSICS:
      {
        auto itr = find_if(mPhysicsDefinitions.begin(), mPhysicsDefinitions.end(),
                  [&](unique_ptr<PhysicsDefinition>& next) {return next->getUuid() == id;});
        if (itr != mPhysicsDefinitions.end()) mPhysicsDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_SCRIPT:
      {
        auto itr = find_if(mScriptDefinitions.begin(), mScriptDefinitions.end(),
                  [&](unique_ptr<ScriptDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mScriptDefinitions.end()) mScriptDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_SHADER:
      {
        auto itr = find_if(mShaderDefinitions.begin(), mShaderDefinitions.end(),
                  [&](unique_ptr<ShaderDefinition>& next) {return next->getUuid() == id;} );
        if (itr != mShaderDefinitions.end()) mShaderDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_TEXTURE:
      {
        auto itr = find_if(mTextureDefinitions.begin(), mTextureDefinitions.end(),
                  [&](unique_ptr<TextureDefinition>& next) {return next->getUuid() == id;});
        if (itr != mTextureDefinitions.end()) mTextureDefinitions.erase(itr);
        break;
      }
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
  }

  // Assets ====================================================================

  void
  ProjectDefinition::removeAssetDefinition
  (AssetDefinition& ad)
  {
    removeAssetDefinitionByUuid(ad.getAssetType(), ad.getUuid());
  }

  size_t
  ProjectDefinition::countAssetDefinitions
  (AssetType t)
  {
    return getAssetDefinitionsVector(t).size();
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionByName
  (AssetType type, const string& name)
  {
    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
      {
        auto itr = find_if(
              mAnimationDefinitions.begin(), mAnimationDefinitions.end(),
              [&](unique_ptr<AnimationDefinition>& next) {return next->getName().compare(name) == 0;});
        if (itr!=mAnimationDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_AUDIO:
      {
        auto itr = find_if(
              mAudioDefinitions.begin(), mAudioDefinitions.end(),
              [&](unique_ptr<AudioDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mAudioDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_FONT:
      {
        auto itr = find_if(
              mFontDefinitions.begin(), mFontDefinitions.end(),
              [&](unique_ptr<FontDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mFontDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_MATERIAL:
      {
        auto itr = find_if
            (mMaterialDefinitions.begin(), mMaterialDefinitions.end(),
             [&](unique_ptr<MaterialDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mMaterialDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_MODEL:
      {
        auto itr = find_if(
              mModelDefinitions.begin(), mModelDefinitions.end(),
              [&](unique_ptr<ModelDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mModelDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_PATH:
      {
        auto itr = find_if(
              mPathDefinitions.begin(), mPathDefinitions.end(),
              [&](unique_ptr<PathDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mPathDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_PHYSICS:
      {
        auto itr = find_if(
              mPhysicsDefinitions.begin(), mPhysicsDefinitions.end(),
              [&](unique_ptr<PhysicsDefinition>& next) {return next->getName().compare(name) == 0;});
        if (itr!=mPhysicsDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_SCRIPT:
      {
        auto itr = find_if(
              mScriptDefinitions.begin(), mScriptDefinitions.end(),
              [&](unique_ptr<ScriptDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mScriptDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_SHADER:
      {
        auto itr = find_if(
              mShaderDefinitions.begin(), mShaderDefinitions.end(),
              [&](unique_ptr<ShaderDefinition>& next) {return next->getName().compare(name) == 0;} );
        if (itr!=mShaderDefinitions.end())return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_TEXTURE:
      {
        auto itr = find_if(
              mTextureDefinitions.begin(), mTextureDefinitions.end(),
              [&](unique_ptr<TextureDefinition>& next) {return next->getName().compare(name) == 0;});
        if (itr!=mTextureDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    throw std::exception();
  }

  optional<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionByUuid
  (AssetType type, UuidType id)
  {
    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
      {
        auto itr = find_if(
              mAnimationDefinitions.begin(), mAnimationDefinitions.end(),
              [&](unique_ptr<AnimationDefinition>& next) {return next->getUuid() == id;});
        if (itr!=mAnimationDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_AUDIO:
      {
        auto itr = find_if(
              mAudioDefinitions.begin(), mAudioDefinitions.end(),
              [&](unique_ptr<AudioDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mAudioDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_FONT:
      {
        auto itr = find_if(
              mFontDefinitions.begin(), mFontDefinitions.end(),
              [&](unique_ptr<FontDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mFontDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_MATERIAL:
      {
        auto itr = find_if
            (mMaterialDefinitions.begin(), mMaterialDefinitions.end(),
             [&](unique_ptr<MaterialDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mMaterialDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_MODEL:
      {
        auto itr = find_if(
              mModelDefinitions.begin(), mModelDefinitions.end(),
              [&](unique_ptr<ModelDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mModelDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_PATH:
      {
        auto itr = find_if(
              mPathDefinitions.begin(), mPathDefinitions.end(),
              [&](unique_ptr<PathDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mPathDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_PHYSICS:
      {
        auto itr = find_if(
              mPhysicsDefinitions.begin(), mPhysicsDefinitions.end(),
              [&](unique_ptr<PhysicsDefinition>& next) {return next->getUuid() == id;});
        if (itr!=mPhysicsDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_SCRIPT:
      {
        auto itr = find_if(
              mScriptDefinitions.begin(), mScriptDefinitions.end(),
              [&](unique_ptr<ScriptDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mScriptDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_SHADER:
      {
        auto itr = find_if(
              mShaderDefinitions.begin(), mShaderDefinitions.end(),
              [&](unique_ptr<ShaderDefinition>& next) {return next->getUuid() == id;} );
        if (itr!=mShaderDefinitions.end())return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_TEXTURE:
      {
        auto itr = find_if(
              mTextureDefinitions.begin(), mTextureDefinitions.end(),
              [&](unique_ptr<TextureDefinition>& next) {return next->getUuid() == id;});
        if (itr!=mTextureDefinitions.end()) return *(*itr);
        break;
      }
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    return std::nullopt;
  }

  vector<reference_wrapper<AssetDefinition>>
  ProjectDefinition::getAssetDefinitionsVector
  (AssetType type)
  {
    vector<reference_wrapper<AssetDefinition>> defs;

    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        for (auto& def : mAnimationDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*(def.get())));
        break;
      case ASSET_TYPE_ENUM_AUDIO:
        for (auto& def : mAudioDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_FONT:
        for (auto& def : mFontDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_MATERIAL:
        for (auto& def : mMaterialDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_MODEL:
        for (auto& def : mModelDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_PATH:
        for (auto& def : mPathDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_PHYSICS:
        for (auto& def : mPhysicsDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_SCRIPT:
        for (auto& def : mScriptDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_SHADER:
        for (auto& def : mShaderDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_TEXTURE:
        for (auto& def : mTextureDefinitions) defs.push_back(reference_wrapper<AssetDefinition>(*def.get()));
        break;
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    return defs;
  }

  AssetDefinition&
  ProjectDefinition::createAssetDefinition
  (AssetType type)
  {
    json assetDefinitionJs;

    string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
    LOG_DEBUG("ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);
    assetDefinitionJs[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
    assetDefinitionJs[Constants::UUID] = Uuid::RandomUuid();
    assetDefinitionJs[Constants::ASSET_TYPE] = AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);
    assetDefinitionJs[Constants::ASSET_FORMAT] = defaultFormat;

    switch(type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:
        return *mAnimationDefinitions.emplace_back(make_unique<AnimationDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_AUDIO:
        return *mAudioDefinitions.emplace_back(make_unique<AudioDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_FONT:
        return *mFontDefinitions.emplace_back(make_unique<FontDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_MATERIAL:
        return *mMaterialDefinitions.emplace_back(make_unique<MaterialDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_MODEL:
        return *mModelDefinitions.emplace_back(make_unique<ModelDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_PATH:
        return *mPathDefinitions.emplace_back(make_unique<PathDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_PHYSICS:
        return *mPhysicsDefinitions.emplace_back(make_unique<PhysicsDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_SCRIPT:
        return *mScriptDefinitions.emplace_back(make_unique<ScriptDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_SHADER:
        return *mShaderDefinitions.emplace_back(make_unique<ShaderDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_TEXTURE:
        return *mTextureDefinitions.emplace_back(make_unique<TextureDefinition>(*this, assetDefinitionJs));
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    throw std::exception();
  }

  void
  ProjectDefinition::removeAllAssetDefinitions
  ()
  {
    mAnimationDefinitions.clear();
    mAudioDefinitions.clear();
    mFontDefinitions.clear();
    mMaterialDefinitions.clear();
    mModelDefinitions.clear();
    mPathDefinitions.clear();
    mPhysicsDefinitions.clear();
    mScriptDefinitions.clear();
    mShaderDefinitions.clear();
    mTextureDefinitions.clear();
  }

  int
  ProjectDefinition::getAssetDefinitionIndex
  (AssetDefinition& def)
  {
    auto id = def.getUuid();

    switch(def.getAssetType())
    {
      case ASSET_TYPE_ENUM_ANIMATION:
      {
        auto itr = find_if(
              mAnimationDefinitions.begin(), mAnimationDefinitions.end(),
              [&](unique_ptr<AnimationDefinition>& next) {return next->getUuid() == id;});
        if (itr == mAnimationDefinitions.end()) return -1;
        else return std::distance(mAnimationDefinitions.begin(), itr);
        break;
      }
      case ASSET_TYPE_ENUM_AUDIO:
      {
        auto itr = find_if(
              mAudioDefinitions.begin(), mAudioDefinitions.end(),
              [&](unique_ptr<AudioDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mAudioDefinitions.end()) return -1;
        else return std::distance(mAudioDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_FONT:
      {
        auto itr = find_if(
              mFontDefinitions.begin(), mFontDefinitions.end(),
              [&](unique_ptr<FontDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mFontDefinitions.end()) return -1;
        else return std::distance(mFontDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_MATERIAL:
      {
        auto itr = find_if
            (mMaterialDefinitions.begin(), mMaterialDefinitions.end(),
             [&](unique_ptr<MaterialDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mMaterialDefinitions.end()) return -1;
        else return std::distance(mMaterialDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_MODEL:
      {
        auto itr = find_if(
              mModelDefinitions.begin(), mModelDefinitions.end(),
              [&](unique_ptr<ModelDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mModelDefinitions.end()) return -1;
        else return std::distance(mModelDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_PATH:
      {
        auto itr = find_if(
              mPathDefinitions.begin(), mPathDefinitions.end(),
              [&](unique_ptr<PathDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mPathDefinitions.end()) return -1;
        else return std::distance(mPathDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_PHYSICS:
      {
        auto itr = find_if(
              mPhysicsDefinitions.begin(), mPhysicsDefinitions.end(),
              [&](unique_ptr<PhysicsDefinition>& next) {return next->getUuid() == id;});
        if (itr == mPhysicsDefinitions.end()) return -1;
        else return std::distance(mPhysicsDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_SCRIPT:
      {
        auto itr = find_if(
              mScriptDefinitions.begin(), mScriptDefinitions.end(),
              [&](unique_ptr<ScriptDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mScriptDefinitions.end()) return -1;
        else return std::distance(mScriptDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_SHADER:
      {
        auto itr = find_if(
              mShaderDefinitions.begin(), mShaderDefinitions.end(),
              [&](unique_ptr<ShaderDefinition>& next) {return next->getUuid() == id;} );
        if (itr == mShaderDefinitions.end()) return -1;
        else return std::distance(mShaderDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_TEXTURE:
      {
        auto itr = find_if(
              mTextureDefinitions.begin(), mTextureDefinitions.end(),
              [&](unique_ptr<TextureDefinition>& next) {return next->getUuid() == id;});
        if (itr == mTextureDefinitions.end()) return -1;
        else return std::distance(mTextureDefinitions.begin(), itr);

        break;
      }
      case ASSET_TYPE_ENUM_NONE:
        break;
    }
    return -1;
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
    for (auto assetWrap : assets)
    {
      auto& asset = assetWrap.get();
      stringstream ss;
      ss << asset.getGroup() << "::" << asset.getName();
      retval.push_back(ss.str());
    }
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

    if (mJson.find(Constants::PROJECT_PHYSICS_ASSET_ARRAY) != mJson.end())
    {
      for (const json& it : mJson[Constants::PROJECT_PHYSICS_ASSET_ARRAY])
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
        mAnimationDefinitions.emplace_back(make_unique<AnimationDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_AUDIO:
        mAudioDefinitions.emplace_back(make_unique<AudioDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_FONT:
        mFontDefinitions.emplace_back(make_unique<FontDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_MATERIAL:
        mMaterialDefinitions.emplace_back(make_unique<MaterialDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_MODEL:
        mModelDefinitions.emplace_back(make_unique<ModelDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_PATH:
        mPathDefinitions.emplace_back(make_unique<PathDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_PHYSICS:
        mPhysicsDefinitions.emplace_back(make_unique<PhysicsDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_SCRIPT:
        mScriptDefinitions.emplace_back(make_unique<ScriptDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_SHADER:
        mShaderDefinitions.emplace_back(make_unique<ShaderDefinition>(*this, assetDefinitionJs));
        break;
      case ASSET_TYPE_ENUM_TEXTURE:
        mTextureDefinitions.emplace_back(make_unique<TextureDefinition>(*this, assetDefinitionJs));
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
        mSceneDefinitions.emplace_back(make_unique<SceneDefinition>(*this, js));
      }
    }
  }


  vector<string>
  ProjectDefinition::getSceneDefinitionNamesVector
  ()
  const
  {
    vector<string> ret;
    for (auto& scene : mSceneDefinitions)
    {
      stringstream ss;
      ss << scene->getGroup() << "::" << scene->getName();
      ret.push_back(ss.str());
    }
    return  ret;
  }

  void
  ProjectDefinition::removeAllSceneDefinitions
  ()
  {
    mSceneDefinitions.clear();
  }

  int
  ProjectDefinition::getSceneDefinitionIndex
  (SceneDefinition& sDef)
  const
  {
    auto it = std::find_if(mSceneDefinitions.begin(), mSceneDefinitions.end(),
                           [&](const unique_ptr<SceneDefinition>& next){return *next == sDef;});
    if (it != mSceneDefinitions.end())
    {
      return std::distance(mSceneDefinitions.begin(), it);
    }
    return -1;
  }

  SceneDefinition&
  ProjectDefinition::getSceneDefinitionAtIndex
  (int index)
  {
    return *mSceneDefinitions.at(index);
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
      if ((*it)->hasName(name))
      {
        return *(*it);
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
      if ((*it)->hasUuid(uuid))
      {
        return *(*it);
      }
    }
    return std::nullopt;
  }

  vector<reference_wrapper<SceneDefinition>>
  ProjectDefinition::getSceneDefinitionsVector
  ()
  const
  {
    vector<reference_wrapper<SceneDefinition>> ret;
    for (auto& scene : mSceneDefinitions)
    {
      ret.push_back(*scene);
    }
    return ret;
  }

  void
  ProjectDefinition::removeSceneDefinition
  (SceneDefinition& sceneDef)
  {
    LOG_DEBUG("ProjectDefinition: Removing SceneDefinition {} from {}",
              sceneDef.getNameAndUuidString(), getNameAndUuidString());

    auto itr = find_if(mSceneDefinitions.begin(), mSceneDefinitions.end(),
              [&](unique_ptr<SceneDefinition>& next)
    { return next->getUuid() == sceneDef.getUuid(); });
    if(itr != mSceneDefinitions.end()) mSceneDefinitions.erase(itr);
  }

  SceneDefinition&
  ProjectDefinition::createSceneDefinition
  ()
  {
    json scene;
    scene[Constants::UUID] = Uuid::RandomUuid();
    scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
    scene[Constants::SCENE_ROOT_SCENE_ENTITY] = json::object();
    return *mSceneDefinitions.emplace_back(make_unique<SceneDefinition>(*this, scene));
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
    defJson[Constants::UUID] = Uuid::RandomUuid();
    return *mTemplateEntityDefinitions.emplace_back(make_unique<TemplateEntityDefinition>(*this,defJson));
  }

  // Template Entity ===========================================================

  vector<string>
  ProjectDefinition::getTemplateEntityDefinitionNamesVector
  ()
  {
    vector<string> names;
    for (auto& entity : mTemplateEntityDefinitions)
    {
      stringstream ss;
      ss << entity->getGroup() << "::" << entity->getName();
      names.push_back(ss.str());
    }
    return names;
  }


  vector<reference_wrapper<TemplateEntityDefinition>>
  ProjectDefinition::getTemplateEntityDefinitionsVector
  ()
  const
  {
    vector<reference_wrapper<TemplateEntityDefinition>> ret;
    for (auto& tmpl : mTemplateEntityDefinitions)
    {
      ret.push_back(*tmpl);
    }
    return ret;
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
      mTemplateEntityDefinitions.emplace_back(make_unique<TemplateEntityDefinition>(*this, entityJs));
    }
  }

  optional<reference_wrapper<TemplateEntityDefinition>>
  ProjectDefinition::getTemplateEntityDefinitionByUuid
  (UuidType uuid)
  {
    auto itr = find_if(mTemplateEntityDefinitions.begin(),
                       mTemplateEntityDefinitions.end(),
                       [&](unique_ptr<TemplateEntityDefinition>& next)
    { return next->getUuid() == uuid; });

    if (itr != mTemplateEntityDefinitions.end()) return *(*itr);

    return std::nullopt;
  }

  void
  ProjectDefinition::removeTemplateEntityDefinitionByUuid
  (UuidType id)
  {
    auto itr = find_if(mTemplateEntityDefinitions.begin(),
              mTemplateEntityDefinitions.end(),
              [&](unique_ptr<TemplateEntityDefinition>& next)
    { return next->getUuid() == id; });

    if (itr != mTemplateEntityDefinitions.end()) mTemplateEntityDefinitions.erase(itr);

  }

  void
  ProjectDefinition::removeTemplateEntityDefinition
  (TemplateEntityDefinition& def)
  {
    removeTemplateEntityDefinitionByUuid(def.getUuid());
  }

  int
  ProjectDefinition::getTemplateEntityDefinitionIndex
  (TemplateEntityDefinition& def)
  {
    auto itr = find_if(
          mTemplateEntityDefinitions.begin(), mTemplateEntityDefinitions.end(),
          [&](unique_ptr<TemplateEntityDefinition>& next) {return next->getUuid() == def.getUuid();});
    if (itr == mTemplateEntityDefinitions.end()) return -1;
    else return std::distance(mTemplateEntityDefinitions.begin(), itr);
  }


  TemplateEntityDefinition&
  ProjectDefinition::getTemplateEntityDefinitionAtIndex
  (int index)
  {
    if (index >= 0 && index < (int)mTemplateEntityDefinitions.size())
    {
      return *mTemplateEntityDefinitions.at(index);
    }
    throw std::exception();
  }
}

