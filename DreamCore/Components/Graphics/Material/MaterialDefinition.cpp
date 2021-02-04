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
        if(dreamTryLock()) {
            dreamLock();
            if(!mJson[Constants::ASSET_ATTR_MATERIAL_SHADER].is_number())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_SHADER];
        } dreamElseLockFailed


    }

    void
    MaterialDefinition::setShader
    (UuidType val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
        } dreamElseLockFailed
    }

    // Textures
    UuidType
    MaterialDefinition::getDiffuseTexture
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(!mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE].is_number())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setDiffuseTexture
    (UuidType val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE]  = val;
        } dreamElseLockFailed
    }

    UuidType
    MaterialDefinition::getSpecularTexture
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(!mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE].is_number())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setSpecularTexture
    (UuidType val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE]  = val;
        } dreamElseLockFailed
    }

    UuidType
    MaterialDefinition::getNormalTexture
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(!mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE].is_number())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setNormalTexture
    (UuidType val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE]  = val;
        } dreamElseLockFailed
    }

    UuidType
    MaterialDefinition::getDisplacementTexture
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(!mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE].is_number())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE];
        } dreamElseLockFailed

    }

    void
    MaterialDefinition::setDisplacementTexture
    (UuidType val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE]  = val;
        } dreamElseLockFailed
    }

    // Colour
    Vector3
    MaterialDefinition::getDiffuseColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = black();
            }

            return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR]);
        } dreamElseLockFailed

    }

    void
    MaterialDefinition::setDiffuseColour
    (Vector3 val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = val.toJson();
        } dreamElseLockFailed
    }

    Vector3
    MaterialDefinition::getSpecularColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = black();
            }

            return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR]);
        } dreamElseLockFailed

    }

    void
    MaterialDefinition::setSpecularColour
    (Vector3 val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = val.toJson();
        } dreamElseLockFailed
    }

    Vector3
    MaterialDefinition::getAmbientColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = black();
            }

            return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR]);
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setAmbientColour
    (Vector3 val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = val.toJson();
        } dreamElseLockFailed
    }

    Vector3
    MaterialDefinition::getReflectiveColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = black();
            }

            return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR]);
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setReflectiveColour
    (Vector3 val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = val.toJson();
        } dreamElseLockFailed
    }

    Vector3
    MaterialDefinition::getEmissiveColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = black();
            }

            return Vector3(mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR]);
        } dreamElseLockFailed

    }

    void
    MaterialDefinition::setEmissiveColour
    (Vector3 val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = val.toJson();
        } dreamElseLockFailed
    }

    json
    MaterialDefinition::black
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return Vector3(0.f).toJson();
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getOpacity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_OPACITY) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setOpacity
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = val;
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getBumpScaling
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING];
        } dreamElseLockFailed

    }

    void
    MaterialDefinition::setBumpScaling
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = val;
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getHardness
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_HARDNESS) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setHardness
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = val;
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getReflectivity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setReflectivity
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = val;
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getShininessStrength
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setShininessStrength
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = val;
        } dreamElseLockFailed
    }

    float
    MaterialDefinition::getRefractionIndex
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if(mJson.find(Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX];
        } dreamElseLockFailed
    }

    void
    MaterialDefinition::setRefractionIndex
    (float val)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = val;
        } dreamElseLockFailed
    }

    bool MaterialDefinition::getIgnore()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_MATERIAL_IGNORE) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = false;
            }
            return mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE];
        } dreamElseLockFailed
    }

    void MaterialDefinition::setIgnore(bool ignore)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = ignore;
        } dreamElseLockFailed
    }
}

