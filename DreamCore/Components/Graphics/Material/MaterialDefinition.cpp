#include "MaterialDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Math/Vector.h"


namespace octronic::dream
{
  MaterialDefinition::MaterialDefinition
  (ProjectDefinition& pd, const json &js)
    : AssetDefinition(pd,js)
  {
    LOG_TRACE("MaterialDefinition: Constructing");
  }

  // Shader ==================================================================

  UuidType
  MaterialDefinition::getShaderUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_SHADER];
  }

  void
  MaterialDefinition::setShaderUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
  }

  // Albedo ==================================================================

  UuidType
  MaterialDefinition::getAlbedoTextureUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_ALBEDO) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO];
  }

  void
  MaterialDefinition::setAlbedoTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO]  = val;
  }

  // Normal ==================================================================

  UuidType
  MaterialDefinition::getNormalTextureUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_NORMAL) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL];
  }

  void
  MaterialDefinition::setNormalTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL]  = val;
  }

  // Metallic ================================================================

  UuidType
  MaterialDefinition::getMetallicTextureUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_METALLIC) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC];
  }

  void
  MaterialDefinition::setMetallicTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC] = val;
  }

  // Roughness ===============================================================

  UuidType
  MaterialDefinition::getRoughnessTextureUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_ROUGHNESS) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS];
  }

  void
  MaterialDefinition::setRoughnessTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS] = val;
  }

  // Ao ======================================================================

  UuidType
  MaterialDefinition::getAoTextureUuid
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MATERIAL_AO) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_MATERIAL_AO];
  }

  void
  MaterialDefinition::setAoTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_AO]  = val;
  }
}

