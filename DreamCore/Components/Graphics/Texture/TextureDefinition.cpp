
/*
 * TextureDefinition.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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
#include "TextureDefinition.h"

namespace octronic::dream
{
    TextureDefinition::TextureDefinition
    (ProjectDefinition* pd, const json& js)
        : AssetDefinition("TextureDefinition",pd,js)
    {
    }

    TextureDefinition::~TextureDefinition
    ()
    {

    }

    // Is Environment Texture ==================================================

    bool TextureDefinition::getIsEnvironmentTexture()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT] = false;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT];
    }

    void TextureDefinition::setIsEnvironmentTexture(bool b)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_IS_ENVIRONMENT] = b;
    }

    // Flip Vertical ===========================================================

    bool TextureDefinition::getFlipVertical()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL] = false;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL];
    }

    void TextureDefinition::setFlipVertical(bool b)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_FLIP_VERTICAL] = b;
    }

    // EquiToCubeMap Shader ====================================================

    UuidType TextureDefinition::getEquiToCubeMapShader()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER] = Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER];
    }

    void TextureDefinition::setEquiToCubeMapShader(UuidType u)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER] = u;
    }

    // Irradiance Shader =======================================================

    UuidType TextureDefinition::getIrradianceMapShader()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER] = Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER];
    }

    void TextureDefinition::setIrradianceMapShader(UuidType u)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER] = u;
    }

    // PreFilter Shader ========================================================

    UuidType TextureDefinition::getPreFilterShader()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER] = Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER];
    }

    void TextureDefinition::setPreFilterShader(UuidType u)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_PREFILTER_SHADER] = u;
    }

    // BRDF LUT Shader =========================================================

    UuidType TextureDefinition::getBrdfLutShader()
    {
        if (mJson.find(Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER] = Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER];
    }

    void TextureDefinition::setBrdfLutShader(UuidType u)
    {
        mJson[Constants::ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER] = u;
    }
}

