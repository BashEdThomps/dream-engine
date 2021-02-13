/*
 * MaterialDefinition.cpp
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

#include "MaterialDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"



namespace octronic::dream
{
    MaterialDefinition::MaterialDefinition
    (ProjectDefinition* pd, const json &js)
        :AssetDefinition("MaterialDefinition",pd,js)
    {
        LOG_TRACE("MaterialDefinition: Constructing");
    }

    MaterialDefinition::~MaterialDefinition()
    {
        LOG_TRACE("MaterialDefinition: Destructing");
    }

    // Shader
    UuidType
    MaterialDefinition::getShader
    ()
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_SHADER].is_number())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_SHADER];
    }

    void
    MaterialDefinition::setShader
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
    }

    // Textures
    UuidType
    MaterialDefinition::getDiffuseTexture
    ()
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE].is_number())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE];
    }

    void
    MaterialDefinition::setDiffuseTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getSpecularTexture
    ()
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE].is_number())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE];
    }

    void
    MaterialDefinition::setSpecularTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getNormalTexture
    ()
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE].is_number())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE];
    }

    void
    MaterialDefinition::setNormalTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getDisplacementTexture
    ()
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE].is_number())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE];
    }

    void
    MaterialDefinition::setDisplacementTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE]  = val;
    }

    // Colour
    Vector3
    MaterialDefinition::getDiffuseColour
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = black();
        }
        return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR]);
    }

    void
    MaterialDefinition::setDiffuseColour
    (Vector3 val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = val.toJson();
    }

    Vector3
    MaterialDefinition::getSpecularColour
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = black();
        }
        return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR]);
    }

    void
    MaterialDefinition::setSpecularColour
    (Vector3 val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = val.toJson();
    }

    Vector3
    MaterialDefinition::getAmbientColour
    ()
    {
        if (mJson.find(Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = black();
        }
        return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR]);
    }

    void
    MaterialDefinition::setAmbientColour
    (Vector3 val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = val.toJson();
    }

    Vector3
    MaterialDefinition::getReflectiveColour
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = black();
        }
        return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR]);
    }

    void
    MaterialDefinition::setReflectiveColour
    (Vector3 val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = val.toJson();
    }

    Vector3
    MaterialDefinition::getEmissiveColour
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = black();
        }
        return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR]);
    }

    void
    MaterialDefinition::setEmissiveColour
    (Vector3 val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = val.toJson();
    }

    json
    MaterialDefinition::black
    ()
    {
        return Vector3(0.f).toJson();
    }

    float
    MaterialDefinition::getOpacity
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_OPACITY) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY];
    }

    void
    MaterialDefinition::setOpacity
    (float val)
    {

        mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = val;

    }

    float
    MaterialDefinition::getBumpScaling
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING];
    }

    void
    MaterialDefinition::setBumpScaling
    (float val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = val;
    }

    float
    MaterialDefinition::getHardness
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_HARDNESS) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS];
    }

    void
    MaterialDefinition::setHardness
    (float val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = val;
    }

    float
    MaterialDefinition::getReflectivity
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY];
    }

    void
    MaterialDefinition::setReflectivity
    (float val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = val;
    }

    float
    MaterialDefinition::getShininessStrength
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH];
    }

    void
    MaterialDefinition::setShininessStrength
    (float val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = val;
    }

    float
    MaterialDefinition::getRefractionIndex
    ()
    {
        if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX];
    }

    void
    MaterialDefinition::setRefractionIndex
    (float val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = val;
    }

    bool MaterialDefinition::getIgnore()
    {
        if (mJson.find(Constants::ASSET_ATTR_MATERIAL_IGNORE) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = false;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE];
    }

    void MaterialDefinition::setIgnore(bool ignore)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = ignore;
    }
}

