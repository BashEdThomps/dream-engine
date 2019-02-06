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


#include "LightRuntime.h"
#include "LightDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    LightRuntime::LightRuntime
    (
        LightDefinition* definition,
        SceneObjectRuntime* transform
    ) : DiscreteAssetRuntime(definition,transform),
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
        #ifdef DREAM_LOG
        setLogClassName("LightRuntime");
        #endif
    }

    LightRuntime::~LightRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Destroying Object" );
        #endif
    }

    vec3
    LightRuntime::getAmbient
    () const
    {
        return mAmbient;
    }

    void LightRuntime::setAmbient(const vec3& ambient)
    {
        mAmbient = ambient;
    }

    vec3 LightRuntime::getDiffuse() const
    {
        return mDiffuse;
    }

    void LightRuntime::setDiffuse(const vec3& diffuse)
    {
        mDiffuse = diffuse;
    }

    vec3 LightRuntime::getSpecular() const
    {
        return mSpecular;
    }

    void LightRuntime::setSpecular(const vec3& specular)
    {
        mSpecular = specular;
    }

    float LightRuntime::getConstant() const
    {
        return mConstant;
    }

    void LightRuntime::setConstant(float constant)
    {
        mConstant = constant;
    }

    float LightRuntime::getLinear() const
    {
        return mLinear;
    }

    void LightRuntime::setLinear(float linear)
    {
        mLinear = linear;
    }

    float LightRuntime::getQuadratic() const
    {
        return mQuadratic;
    }

    void LightRuntime::setQuadratic(float quadratic)
    {
        mQuadratic = quadratic;
    }

    float LightRuntime::getCutOff() const
    {
        return mCutOff;
    }

    void LightRuntime::setCutOff(float cutOff)
    {
        mCutOff = cutOff;
    }

    float LightRuntime::getOuterCutOff() const
    {
        return mOuterCutOff;
    }

    void
    LightRuntime::setOuterCutOff
    (float outerCutOff)
    {
        mOuterCutOff = outerCutOff;
    }

    LightType
    LightRuntime::getType
    () const
    {
        return mType;
    }

    void
    LightRuntime::setType
    (const LightType& type)
    {
        mType = type;
    }

    void LightRuntime::loadType()
    {
        auto assetDef = static_cast<AssetDefinition*>(mDefinition);
        if (assetDef->getFormat() == Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL)
        {
            mType = LightType::LT_DIRECTIONAL;
        }
        else if (assetDef->getFormat() == Constants::ASSET_FORMAT_LIGHT_POINT)
        {
            mType = LightType::LT_POINT;
        }
        else if (assetDef->getFormat() == Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT)
        {
            mType = LightType::LT_SPOTLIGHT;
        }
    }

    PointLight
    LightRuntime::getPointLightData
    ()
    const
    {
        vec3 tx = mSceneObjectRuntime->getTransform().getMatrix()[3];
        return PointLight
        {
            tx,
            mConstant,
            mLinear,
            mQuadratic,
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }

    SpotLight
    LightRuntime::getSpotLightData
    ()
    const
    {
        MatrixDecomposition tx = mSceneObjectRuntime->getTransform().decomposeMatrix();
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

    DirLight
    LightRuntime::getDirectionalLightData
    ()
    const
    {
        MatrixDecomposition tx = mSceneObjectRuntime->getTransform().decomposeMatrix();
        return DirLight
        {
            eulerAngles(tx.rotation),
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }

    bool
    LightRuntime::useDefinition
    ()
    {
        auto lightDef = static_cast<LightDefinition*>(mDefinition);

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
