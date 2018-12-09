/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "LightInstance.h"
#include "LightDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    LightInstance::LightInstance
    (
        LightDefinition* definition,
        SceneObjectRuntime* transform
    ) : DiscreteAssetInstance(definition,transform),
        mAmbient(glm::vec3(0.0f,0.0f,0.0f)),
        mDiffuse(glm::vec3(0.0f,0.0f,0.0f)),
        mSpecular(glm::vec3(0.0f,0.0f,0.0f)),
        mConstant(0.0f),
        mLinear(0.0f),
        mQuadratic(0.0f),
        mCutOff(0.0f),
        mOuterCutOff(0.0f),
        mType(LightType::LT_NONE)

    {
        setLogClassName("LightInstance");
    }

    LightInstance::~LightInstance
    ()
    {
        auto log = getLog();
        log->debug("Destroying Object" );
    }

    vec3 LightInstance::getAmbient() const
    {
        return mAmbient;
    }

    void LightInstance::setAmbient(const vec3& ambient)
    {
        mAmbient = ambient;
    }

    vec3 LightInstance::getDiffuse() const
    {
        return mDiffuse;
    }

    void LightInstance::setDiffuse(const vec3& diffuse)
    {
        mDiffuse = diffuse;
    }

    vec3 LightInstance::getSpecular() const
    {
        return mSpecular;
    }

    void LightInstance::setSpecular(const vec3& specular)
    {
        mSpecular = specular;
    }

    float LightInstance::getConstant() const
    {
        return mConstant;
    }

    void LightInstance::setConstant(float constant)
    {
        mConstant = constant;
    }

    float LightInstance::getLinear() const
    {
        return mLinear;
    }

    void LightInstance::setLinear(float linear)
    {
        mLinear = linear;
    }

    float LightInstance::getQuadratic() const
    {
        return mQuadratic;
    }

    void LightInstance::setQuadratic(float quadratic)
    {
        mQuadratic = quadratic;
    }

    float LightInstance::getCutOff() const
    {
        return mCutOff;
    }

    void LightInstance::setCutOff(float cutOff)
    {
        mCutOff = cutOff;
    }

    float LightInstance::getOuterCutOff() const
    {
        return mOuterCutOff;
    }

    void LightInstance::setOuterCutOff(float outerCutOff)
    {
        mOuterCutOff = outerCutOff;
    }

    LightType LightInstance::getType() const
    {
        return mType;
    }

    void LightInstance::setType(const LightType& type)
    {
        mType = type;
    }

    void LightInstance::loadType()
    {
        if (mDefinition->getFormat().compare(Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL) == 0)
        {
            mType = LightType::LT_DIRECTIONAL;
        }
        else if (mDefinition->getFormat().compare(Constants::ASSET_FORMAT_LIGHT_POINT) == 0)
        {
            mType = LightType::LT_POINT;
        }
        else if (mDefinition->getFormat().compare(Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT) == 0)
        {
            mType = LightType::LT_SPOTLIGHT;
        }
    }

    PointLight LightInstance::getPointLightData()
    {
        auto tx = mSceneObjectRuntime->getTransform().decomposeMatrix();
        return PointLight
        {
            tx.translation,
            mConstant,
            mLinear,
            mQuadratic,
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }

    SpotLight LightInstance::getSpotLightData()
    {
        auto tx = mSceneObjectRuntime->getTransform().decomposeMatrix();
        return SpotLight
        {
            tx.translation,
            eulerAngles(tx.rotation),
            mAmbient,
            mDiffuse,
            mSpecular,
            mCutOff,
            mOuterCutOff,
            mConstant,
            mLinear,
            mQuadratic,
        };
    }

    DirLight LightInstance::getDirectionalLightData()
    {
        auto tx = mSceneObjectRuntime->getTransform().decomposeMatrix();
        return DirLight
        {
            eulerAngles(tx.rotation),
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }

    bool
    LightInstance::load
    ()
    {
        auto log = getLog();
        auto lightDef = dynamic_cast<LightDefinition*>(mDefinition);

        mAmbient = lightDef->getAmbient();
        mDiffuse = lightDef->getDiffuse();
        mSpecular = lightDef->getSpecular();

        loadType();

        switch(mType)
        {
            case Dream::LT_NONE:
                break;
            case Dream::LT_POINT:
                mConstant    = lightDef->getConstant();
                mLinear      = lightDef->getLinear();
                mQuadratic   = lightDef->getQuadratic();
                break;
            case Dream::LT_SPOTLIGHT:
                mCutOff      = lightDef->getCutOff();
                mOuterCutOff = lightDef->getOuterCutOff();
                mConstant    = lightDef->getConstant();
                mLinear      = lightDef->getLinear();
                mQuadratic   = lightDef->getQuadratic();
                break;
            case Dream::LT_DIRECTIONAL:
                break;
        }

        mLoaded = true;
        return mLoaded;
    }
} // End of Dream
