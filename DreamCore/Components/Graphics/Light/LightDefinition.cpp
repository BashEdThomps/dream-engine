/*
 * LightDefinition.cpp
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

#include "LightDefinition.h"


namespace Dream
{
    LightDefinition::LightDefinition
    (ProjectDefinition* pd, json js)
        : IAssetDefinition(pd,js) {}

    void
    LightDefinition::setDiffuse
    (vec3 diffuse)
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE] = json::object();
        }

        mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::RED]   = diffuse.r;
        mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::GREEN] = diffuse.g;
        mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::BLUE]  = diffuse.b;
    }

    vec3
    LightDefinition::getDiffuse
    ()
    {
        vec3 diffuse;
        if (mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE]=json::object();
            mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::RED] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::GREEN] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::BLUE] = 0.0f;
        }
        diffuse.r = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::RED]);
        diffuse.g = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::GREEN]);
        diffuse.b = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIFFUSE][Constants::BLUE]);
        return diffuse;
    }

    void
    LightDefinition::setAmbient
    (vec3 ambient)
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT] = json::object();
        }
        mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::RED]   = ambient.r;
        mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::GREEN] = ambient.g;
        mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::BLUE]  = ambient.b;
    }

    vec3
    LightDefinition::getAmbient
    ()
    {
        vec3 ambient;
        if (mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT]=json::object();
            mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::RED] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::GREEN] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::BLUE] = 0.0f;
        }
        ambient.r = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::RED]);
        ambient.g = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::GREEN]);
        ambient.b = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_AMBIENT][Constants::BLUE]);
        return ambient;
    }

    void
    LightDefinition::setSpecular
    (vec3 specular)
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR] = json::object();
        }
        mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::RED]   = specular.r;
        mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::GREEN] = specular.g;
        mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::BLUE]  = specular.b;
    }

    vec3
    LightDefinition::getSpecular()
    {
        vec3 specular;
        if (mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR]=json::object();
            mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::RED] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::GREEN] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::BLUE] = 0.0f;
        }
        specular.r = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::RED]);
        specular.g = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::GREEN]);
        specular.b = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_SPECULAR][Constants::BLUE]);
        return specular;
    }

    void LightDefinition::setDirection(vec3 direction)
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION] = json::object();
        }
        mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::X] = direction.x;
        mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Y] = direction.y;
        mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Z] = direction.z;

    }

    vec3
    LightDefinition::getDirection
    ()
    {
        vec3 direction;
        if (mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION]=json::object();
            mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::X] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Y] = 0.0f;
            mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Z] = 0.0f;
        }
        direction.x = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::X]);
        direction.y = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Y]);
        direction.z = static_cast<float>(mJson[Constants::ASSET_ATTR_LIGHT_DIRECTION][Constants::Z]);
        return direction;

    }

    void LightDefinition::setType(LightType type)
    {
        string typeStr;
        switch (type)
        {
           case LT_DIRECTIONAL:
                typeStr = Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL;
                break;
           case LT_POINT:
                typeStr = Constants::ASSET_FORMAT_LIGHT_POINT;
                break;
           case LT_SPOTLIGHT:
                typeStr = Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT;
                break;
            case LT_NONE:
                typeStr = "";
        }
        setFormat(typeStr);
    }

    LightType
    LightDefinition::getType
    ()
    {
        LightType type;
        if (getFormat().compare(Constants::ASSET_FORMAT_LIGHT_POINT) == 0)
        {
            type = LT_POINT;
        }
        else if (getFormat().compare(Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL) == 0)
        {
            type = LT_DIRECTIONAL;
        }
        else if (getFormat().compare(Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT) == 0)
        {
            type = LT_SPOTLIGHT;
        }
        else
        {
            type = LT_NONE;
        }
        return type;
    }

    void LightDefinition::setCutOff(float cutOff)
    {
        mJson[Constants::ASSET_ATTR_LIGHT_CUTOFF] = cutOff;
    }

    float LightDefinition::getCutOff()
    {
        if(mJson[Constants::ASSET_ATTR_LIGHT_CUTOFF].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_CUTOFF] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_LIGHT_CUTOFF];
    }

    void LightDefinition::setOuterCutOff(float outerCutOff)
    {
        mJson[Constants::ASSET_ATTR_LIGHT_OUTER_CUTOFF] = outerCutOff;
    }

    float LightDefinition::getOuterCutOff()
    {
        if(mJson[Constants::ASSET_ATTR_LIGHT_OUTER_CUTOFF].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_OUTER_CUTOFF] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_LIGHT_OUTER_CUTOFF];
    }

    void LightDefinition::setConstant(float ant)
    {
        mJson[Constants::ASSET_ATTR_LIGHT_CONSTANT] = ant;
    }

    float LightDefinition::getConstant()
    {
        if(mJson[Constants::ASSET_ATTR_LIGHT_CONSTANT].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_CONSTANT] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_LIGHT_CONSTANT];
    }

    void LightDefinition::setLinear(float linear)
    {
        mJson[Constants::ASSET_ATTR_LIGHT_LINEAR] = linear;
    }

    float LightDefinition::getLinear()
    {
        if(mJson[Constants::ASSET_ATTR_LIGHT_LINEAR].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_LINEAR] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_LIGHT_LINEAR];
    }

    void LightDefinition::setQuadratic(float quadratic)
    {
        mJson[Constants::ASSET_ATTR_LIGHT_QUADRATIC] = quadratic;
    }

    float LightDefinition::getQuadratic()
    {
        if(mJson[Constants::ASSET_ATTR_LIGHT_QUADRATIC].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_QUADRATIC] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_LIGHT_QUADRATIC];
    }

    float LightDefinition::getDiffuseBlue()
    {
        return getDiffuse().b;
    }

    void LightDefinition::setDiffuseBlue(float diffuseBlue)
    {
        auto current = getDiffuse();
        current.b = diffuseBlue;
        setDiffuse(current);
    }

    float LightDefinition::getDiffuseGreen()
    {
        return getDiffuse().g;
    }

    void LightDefinition::setDiffuseGreen(float diffuseGreen)
    {
        auto current = getDiffuse();
        current.g = diffuseGreen;
        setDiffuse(current);
    }

    float LightDefinition::getDiffuseRed()
    {
        return getDiffuse().r;
    }

    void LightDefinition::setDiffuseRed(float diffuseRed)
    {
        auto current = getDiffuse();
        current.r = diffuseRed;
        setDiffuse(current);
    }

    float LightDefinition::getAmbientBlue()
    {
        return getAmbient().b;
    }

    void LightDefinition::setAmbientBlue(float blue)
    {
        auto current = getAmbient();
        current.b = blue;
        setAmbient(current);
    }

    float LightDefinition::getAmbientGreen()
    {
        return getAmbient().g;
    }

    void LightDefinition::setAmbientGreen(float ambientGreen)
    {
        auto current = getAmbient();
        current.g = ambientGreen;
        setAmbient(current);
    }

    float LightDefinition::getAmbientRed()
    {
        return getAmbient().r;
    }

    void LightDefinition::setAmbientRed(float ambientRed)
    {
        auto current = getAmbient();
        current.r = ambientRed ;
        setAmbient(current);
    }

    float LightDefinition::getSpecularBlue()
    {
        return getSpecular().b;
    }

    void LightDefinition::setSpecularBlue(float specularBlue)
    {
        auto current = getSpecular();
        current.b = specularBlue;
        setSpecular(current);
    }

    float LightDefinition::getSpecularGreen()
    {
        return getSpecular().g;
    }

    void LightDefinition::setSpecularGreen(float specularGreen)
    {
        auto current = getSpecular();
        current.g = specularGreen;
        setSpecular(current);
    }

    float LightDefinition::getSpecularRed()
    {
        return getSpecular().r;
    }

    void LightDefinition::setSpecularRed(float specularRed)
    {
        auto current = getSpecular();
        current.r = specularRed;
        setSpecular(current);
    }

    float
    LightDefinition::getDirectionX
    ()
    {
       return getDirection().x;
    }

    void
    LightDefinition::setDirectionX
    (float dir)
    {
       auto current = getDirection();
       current.x = dir;
       setDirection(current);
    }

    float
    LightDefinition::getDirectionY
    ()
    {
       return getDirection().y;
    }

    void
    LightDefinition::setDirectionY
    (float dir)
    {
       auto current = getDirection();
       current.y = dir;
       setDirection(current);
    }

    float
    LightDefinition::getDirectionZ
    ()
    {
        return getDirection().z;
    }

    void
    LightDefinition::setDirectionZ
    (float dir)
    {
       auto current = getDirection();
       current.z = dir;
       setDirection(current);
    }
}
