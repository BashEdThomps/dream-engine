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
#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Scene/Entity/EntityRuntime.h"



namespace octronic::dream
{
    LightRuntime::LightRuntime
    (LightDefinition* definition,EntityRuntime* transform
            ) : DiscreteAssetRuntime("LightRuntime",definition,transform),
        mAmbient(0.0f),
        mDiffuse(0.0f),
        mSpecular(0.0f),
        mConstant(0.0f),
        mLinear(0.0f),
        mQuadratic(0.0f),
        mCutOff(0.0f),
        mOuterCutOff(0.0f),
        mType(LightType::LT_NONE)

    {
    }

    LightRuntime::~LightRuntime
    ()
    {
        LOG_DEBUG("LightRuntime: Destroying Object" );
    }

    Vector3
    LightRuntime::getAmbient
    () const
    {
        return mAmbient;
    }

    void LightRuntime::setAmbient(const Vector3& ambient)
    {
        if(dreamTryLock()) {
            dreamLock();
            mAmbient = ambient;
        } dreamElseLockFailed
    }

    Vector3 LightRuntime::getDiffuse() const
    {
        return mDiffuse;
    }

    void LightRuntime::setDiffuse(const Vector3& diffuse)
    {
        if(dreamTryLock()) {
            dreamLock();
            mDiffuse = diffuse;
        } dreamElseLockFailed
    }

    Vector3 LightRuntime::getSpecular() const
    {
        return mSpecular;
    }

    void LightRuntime::setSpecular(const Vector3& specular)
    {
        if(dreamTryLock()) {
            dreamLock();
            mSpecular = specular;
        } dreamElseLockFailed
    }

    float LightRuntime::getConstant() const
    {
        return mConstant;
    }

    void LightRuntime::setConstant(float constant)
    {
        if(dreamTryLock()) {
            dreamLock();
            mConstant = constant;
        } dreamElseLockFailed
    }

    float LightRuntime::getLinear() const
    {
        return mLinear;
    }

    void LightRuntime::setLinear(float linear)
    {
        if(dreamTryLock()) {
            dreamLock();
            mLinear = linear;
        } dreamElseLockFailed
    }

    float LightRuntime::getQuadratic() const
    {
        return mQuadratic;
    }

    void LightRuntime::setQuadratic(float quadratic)
    {
        if(dreamTryLock()) {
            dreamLock();
            mQuadratic = quadratic;
        } dreamElseLockFailed
    }

    float LightRuntime::getCutOff() const
    {
        return mCutOff;
    }

    void LightRuntime::setCutOff(float cutOff)
    {
        if(dreamTryLock()) {
            dreamLock();
            mCutOff = cutOff;
        } dreamElseLockFailed
    }

    float LightRuntime::getOuterCutOff() const
    {
        return mOuterCutOff;
    }

    void
    LightRuntime::setOuterCutOff
    (float outerCutOff)
    {
        if(dreamTryLock()) {
            dreamLock();
            mOuterCutOff = outerCutOff;
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mType = type;
        } dreamElseLockFailed
    }

    void LightRuntime::loadType()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    PointLight
    LightRuntime::getPointLightData
    ()
    const
    {
        Vector3 tx(
                    mEntityRuntime->getTransform().getMatrix()[3][0],
                mEntityRuntime->getTransform().getMatrix()[3][1],
                mEntityRuntime->getTransform().getMatrix()[3][2]);
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
        MatrixDecomposition decomp = mEntityRuntime->getTransform().decomposeMatrix();
        Vector3 tx(
                    decomp.translation.x,
                    decomp.translation.y,
                    decomp.translation.z
                    );
        auto e = eulerAngles(decomp.rotation);
        Vector3 euler(e.x,e.y,e.z);

        return SpotLight
        {
            tx,
                    euler,
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
        MatrixDecomposition decomp = mEntityRuntime->getTransform().decomposeMatrix();
        auto e = eulerAngles(decomp.rotation);
        Vector3 euler(e.x,e.y,e.z);
        return DirLight
        {
            euler,
                    mAmbient,
                    mDiffuse,
                    mSpecular
        };
    }

    bool
    LightRuntime::useDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            auto lightDef = static_cast<LightDefinition*>(mDefinition);

            mAmbient = lightDef->getAmbient();
            mDiffuse = lightDef->getDiffuse();
            mSpecular = lightDef->getSpecular();

            loadType();

            switch(mType)
            {
                case LT_NONE:
                    break;
                case LT_POINT:
                    mConstant    = lightDef->getConstant();
                    mLinear      = lightDef->getLinear();
                    mQuadratic   = lightDef->getQuadratic();
                    break;
                case LT_SPOTLIGHT:
                    mCutOff      = lightDef->getCutOff();
                    mOuterCutOff = lightDef->getOuterCutOff();
                    mConstant    = lightDef->getConstant();
                    mLinear      = lightDef->getLinear();
                    mQuadratic   = lightDef->getQuadratic();
                    break;
                case LT_DIRECTIONAL:
                    break;
            }

            mLoaded = true;
            return mLoaded;
        } dreamElseLockFailed
    }
} // End of Dream
