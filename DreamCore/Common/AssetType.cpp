#include "AssetType.h"

#include "Common/Constants.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Physics/PhysicsDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Texture/TextureDefinition.h"

namespace octronic::dream
{
  AssetTypeHelper::AssetTypeHelper() {}

  map<AssetType,string> AssetTypeHelper::TypesMap =
  {
    {ASSET_TYPE_ENUM_ANIMATION,        Constants::ASSET_TYPE_ANIMATION},
    {ASSET_TYPE_ENUM_AUDIO,            Constants::ASSET_TYPE_AUDIO},
    {ASSET_TYPE_ENUM_FONT,             Constants::ASSET_TYPE_FONT},
    {ASSET_TYPE_ENUM_MATERIAL,         Constants::ASSET_TYPE_MATERIAL},
    {ASSET_TYPE_ENUM_MODEL,            Constants::ASSET_TYPE_MODEL},
    {ASSET_TYPE_ENUM_PATH,             Constants::ASSET_TYPE_PATH},
    {ASSET_TYPE_ENUM_PHYSICS,   Constants::ASSET_TYPE_PHYSICS},
    {ASSET_TYPE_ENUM_SCRIPT,           Constants::ASSET_TYPE_SCRIPT},
    {ASSET_TYPE_ENUM_SHADER,           Constants::ASSET_TYPE_SHADER},
    {ASSET_TYPE_ENUM_TEXTURE,          Constants::ASSET_TYPE_TEXTURE}
  };

  map<AssetType,string> AssetTypeHelper::ReadableTypesMap =
  {
    {ASSET_TYPE_ENUM_ANIMATION,        Constants::ASSET_TYPE_ANIMATION_READABLE},
    {ASSET_TYPE_ENUM_AUDIO,            Constants::ASSET_TYPE_AUDIO_READABLE},
    {ASSET_TYPE_ENUM_FONT,             Constants::ASSET_TYPE_FONT_READABLE},
    {ASSET_TYPE_ENUM_MATERIAL,         Constants::ASSET_TYPE_MATERIAL_READABLE},
    {ASSET_TYPE_ENUM_MODEL,            Constants::ASSET_TYPE_MODEL_READABLE},
    {ASSET_TYPE_ENUM_PATH,             Constants::ASSET_TYPE_PATH_READABLE},
    {ASSET_TYPE_ENUM_PHYSICS,   Constants::ASSET_TYPE_PHYSICS_READABLE},
    {ASSET_TYPE_ENUM_SCRIPT,           Constants::ASSET_TYPE_SCRIPT_READABLE},
    {ASSET_TYPE_ENUM_SHADER,           Constants::ASSET_TYPE_SHADER_READABLE},
    {ASSET_TYPE_ENUM_TEXTURE,          Constants::ASSET_TYPE_TEXTURE_READABLE}
  };

  string
  AssetTypeHelper::GetAssetTypeStringFromTypeEnum
  (AssetType type)
  {
    switch (type)
    {
      case ASSET_TYPE_ENUM_ANIMATION:      return Constants::ASSET_TYPE_ANIMATION;
      case ASSET_TYPE_ENUM_PATH:           return Constants::ASSET_TYPE_PATH;
      case ASSET_TYPE_ENUM_AUDIO:          return Constants::ASSET_TYPE_AUDIO;
      case ASSET_TYPE_ENUM_FONT:           return Constants::ASSET_TYPE_FONT;
      case ASSET_TYPE_ENUM_MATERIAL:       return Constants::ASSET_TYPE_MATERIAL;
      case ASSET_TYPE_ENUM_MODEL:          return Constants::ASSET_TYPE_MODEL;
      case ASSET_TYPE_ENUM_PHYSICS: return Constants::ASSET_TYPE_PHYSICS;
      case ASSET_TYPE_ENUM_SCRIPT:         return Constants::ASSET_TYPE_SCRIPT;
      case ASSET_TYPE_ENUM_SHADER:         return Constants::ASSET_TYPE_SHADER;
      case ASSET_TYPE_ENUM_TEXTURE:        return Constants::ASSET_TYPE_TEXTURE;
      case ASSET_TYPE_ENUM_NONE:
        return "";
    }
    return "";
  }

  AssetType
  AssetTypeHelper::GetAssetTypeEnumFromString
  (const string& type)
  {
    if (type == Constants::ASSET_TYPE_ANIMATION || type == Constants::ASSET_TYPE_ANIMATION_READABLE)
      return ASSET_TYPE_ENUM_ANIMATION;
    else if (type == Constants::ASSET_TYPE_AUDIO || type == Constants::ASSET_TYPE_AUDIO_READABLE)
      return ASSET_TYPE_ENUM_AUDIO;
    else if (type == Constants::ASSET_TYPE_FONT || type == Constants::ASSET_TYPE_FONT_READABLE)
      return ASSET_TYPE_ENUM_FONT;
    else if (type == Constants::ASSET_TYPE_MATERIAL || type == Constants::ASSET_TYPE_MATERIAL_READABLE)
      return ASSET_TYPE_ENUM_MATERIAL;
    else if (type == Constants::ASSET_TYPE_MODEL || type == Constants::ASSET_TYPE_MODEL_READABLE)
      return ASSET_TYPE_ENUM_MODEL;
    else if (type == Constants::ASSET_TYPE_PATH || type == Constants::ASSET_TYPE_PATH_READABLE)
      return ASSET_TYPE_ENUM_PATH;
    else if (type == Constants::ASSET_TYPE_PHYSICS || type == Constants::ASSET_TYPE_PHYSICS_READABLE)
      return ASSET_TYPE_ENUM_PHYSICS;
    else if (type == Constants::ASSET_TYPE_SCRIPT || type == Constants::ASSET_TYPE_SCRIPT_READABLE)
      return ASSET_TYPE_ENUM_SCRIPT;
    else if (type == Constants::ASSET_TYPE_SHADER || type == Constants::ASSET_TYPE_SHADER_READABLE)
      return ASSET_TYPE_ENUM_SHADER;
    else if (type == Constants::ASSET_TYPE_TEXTURE || type == Constants::ASSET_TYPE_TEXTURE_READABLE)
      return  ASSET_TYPE_ENUM_TEXTURE;

    return ASSET_TYPE_ENUM_NONE;
  }

  string
  AssetTypeHelper::GetAssetTypeReadableNameFromString
  (const string& type)
  {
    if (type == Constants::ASSET_TYPE_ANIMATION)
      return Constants::ASSET_TYPE_ANIMATION_READABLE;
    else if (type == Constants::ASSET_TYPE_AUDIO)
      return Constants::ASSET_TYPE_AUDIO_READABLE;
    else if (type == Constants::ASSET_TYPE_FONT)
      return Constants::ASSET_TYPE_FONT_READABLE;
    else if (type == Constants::ASSET_TYPE_MATERIAL)
      return Constants::ASSET_TYPE_MATERIAL_READABLE;
    else if (type == Constants::ASSET_TYPE_MODEL)
      return Constants::ASSET_TYPE_MODEL_READABLE;
    else if (type == Constants::ASSET_TYPE_PATH)
      return Constants::ASSET_TYPE_PATH_READABLE;
    else if (type == Constants::ASSET_TYPE_PHYSICS)
      return Constants::ASSET_TYPE_PHYSICS_READABLE;
    else if (type == Constants::ASSET_TYPE_SCRIPT)
      return Constants::ASSET_TYPE_SCRIPT_READABLE;
    else if (type == Constants::ASSET_TYPE_SHADER)
      return Constants::ASSET_TYPE_SHADER_READABLE;
    else if (type == Constants::ASSET_TYPE_TEXTURE)
      return Constants::ASSET_TYPE_TEXTURE_READABLE;
    return "";
  }
}
