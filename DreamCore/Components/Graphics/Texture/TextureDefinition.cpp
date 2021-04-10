#include "TextureDefinition.h"

#include "Common/Constants.h"

namespace octronic::dream
{
  TextureDefinition::TextureDefinition
  (ProjectDefinition& pd, const json& js)
    : AssetDefinition(pd,js)
  {
  }

  // Is Environment Texture ==================================================

  bool TextureDefinition::getIsEnvironmentTexture()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT];
  }

  void TextureDefinition::setIsEnvironmentTexture(bool b)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT] = b;
  }

  // Flip Vertical ===========================================================

  bool TextureDefinition::getFlipVertical()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL];
  }

  void TextureDefinition::setFlipVertical(bool b)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL] = b;
  }

  // EquiToCubeMap Shader ====================================================

  UuidType TextureDefinition::getEquiToCubeMapShader()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER];
  }

  void TextureDefinition::setEquiToCubeMapShader(UuidType u)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER] = u;
  }

  // Irradiance Shader =======================================================

  UuidType TextureDefinition::getIrradianceMapShader()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER];
  }

  void TextureDefinition::setIrradianceMapShader(UuidType u)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER] = u;
  }

  // PreFilter Shader ========================================================

  UuidType TextureDefinition::getPreFilterShader()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER];
  }

  void TextureDefinition::setPreFilterShader(UuidType u)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER] = u;
  }

  // BRDF LUT Shader =========================================================

  UuidType TextureDefinition::getBrdfLutShader()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER];
  }

  void TextureDefinition::setBrdfLutShader(UuidType u)
  {
    mJson[Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER] = u;
  }
}

