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

using std::unique_lock;

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
    }

    // Textures
    UuidType
    MaterialDefinition::getDiffuseTexture
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getSpecularTexture
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getNormalTexture
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE]  = val;
    }

    UuidType
    MaterialDefinition::getDisplacementTexture
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE]  = val;
    }

    // Colour
    RGB
    MaterialDefinition::getDiffuseColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = black();
        }

        return unwrapColourObject(
            mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR]
        );

    }

    void
    MaterialDefinition::setDiffuseColour
    (RGB val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR] = wrapColourObject(val);
    }

    RGB
    MaterialDefinition::getSpecularColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = black();
        }

        return unwrapColourObject(
            mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR]
        );

    }

    void
    MaterialDefinition::setSpecularColour
    (RGB val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR] = wrapColourObject(val);
    }

    RGB
    MaterialDefinition::getAmbientColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = black();
        }

        return unwrapColourObject(
            mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR]
        );
    }

    void
    MaterialDefinition::setAmbientColour
    (RGB val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR] = wrapColourObject(val);
    }

    RGB
    MaterialDefinition::getReflectiveColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = black();
        }

        return unwrapColourObject(
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR]
        );

    }

    void
    MaterialDefinition::setReflectiveColour
    (RGB val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR] = wrapColourObject(val);
    }

    RGB
    MaterialDefinition::getEmissiveColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = black();
        }

        return unwrapColourObject(
            mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR]
        );

    }

    void
    MaterialDefinition::setEmissiveColour
    (RGB val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR] = wrapColourObject(val);
    }

    json
    MaterialDefinition::wrapColourObject
    (RGB v)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto obj = json::object();
        obj[Constants::RED]   = v.r;
        obj[Constants::GREEN] = v.g;
        obj[Constants::BLUE]  = v.b;
        return obj;
    }

    RGB
    MaterialDefinition::unwrapColourObject
    (json obj)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       float r = obj[Constants::RED];
       float g = obj[Constants::GREEN];
       float b = obj[Constants::BLUE];
       return RGB{r,g,b};
    }

    json
    MaterialDefinition::black
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return wrapColourObject(RGB{0.f,0.f,0.f});
    }

    float
    MaterialDefinition::getOpacity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY];
    }

    void
    MaterialDefinition::setOpacity
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_OPACITY] = val;
    }

    float
    MaterialDefinition::getBumpScaling
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING];

    }

    void
    MaterialDefinition::setBumpScaling
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING] = val;
    }

    float
    MaterialDefinition::getHardness
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS];
    }

    void
    MaterialDefinition::setHardness
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        mJson[Constants::ASSET_ATTR_MATERIAL_HARDNESS] = val;
    }

    float
    MaterialDefinition::getReflectivity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY];
    }

    void
    MaterialDefinition::setReflectivity
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY] = val;
    }

    float
    MaterialDefinition::getShininessStrength
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH];
    }

    void
    MaterialDefinition::setShininessStrength
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH] = val;
    }

    float
    MaterialDefinition::getRefractionIndex
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if(mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX];
    }

    void
    MaterialDefinition::setRefractionIndex
    (float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX] = val;
    }

    bool MaterialDefinition::getIgnore()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE].is_null())
        {
            mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = false;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE];
    }

    void MaterialDefinition::setIgnore(bool ignore)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
            mJson[Constants::ASSET_ATTR_MATERIAL_IGNORE] = ignore;
    }
}

