#include "LightInstance.h"

#include "LightDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    LightInstance::LightInstance
    (
        LightDefinition* definition,
        SceneObjectRuntime* transform
    ) : IAssetInstance(definition,transform),
        mAmbient(glm::vec3(0.0f,0.0f,0.0f)),
        mDiffuse(glm::vec3(0.0f,0.0f,0.0f)),
        mSpecular(glm::vec3(0.0f,0.0f,0.0f)),
        mDirection(glm::vec3(0.0f,0.0f,0.0f)),
        mConstant(0.0f),
        mLinear(0.0f),
        mQuadratic(0.0f),
        mCutOff(0.0f),
        mOuterCutOff(0.0f),
        mType(LightType::LT_NONE)

    {
        setLogClassName("LightInstance");
        loadExtraAttributes(definition->getJson());
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

    vec3 LightInstance::getDirection() const
    {
        return mDirection;
    }

    void LightInstance::setDirection(const vec3& direction)
    {
        mDirection = direction;
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
        return PointLight {
            mSceneObjectRuntime->getTransform()->getTranslation(),
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
        return SpotLight {
            mSceneObjectRuntime->getTransform()->getTranslation(),
            mDirection,
            mCutOff,
            mOuterCutOff,
            mConstant,
            mLinear,
            mQuadratic,
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }

    DirLight LightInstance::getDirectionalLightData()
    {
        return DirLight {
            mSceneObjectRuntime->getTransform()->getTranslation(),
            mAmbient,
            mDiffuse,
            mSpecular
        };
    }



    bool
    LightInstance::load
    (string)
    {
        mLoaded = true;
        return mLoaded;
    }

    void
    LightInstance::loadExtraAttributes
    (nlohmann::json)
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
            case Dream::LT_DIRECTIONAL:
                mDirection   = lightDef->getDirection();
                break;
            case Dream::LT_SPOTLIGHT:
                mDirection   = lightDef->getDirection();
                mCutOff      = lightDef->getCutOff();
                mOuterCutOff = lightDef->getOuterCutOff();
                mConstant    = lightDef->getConstant();
                mLinear      = lightDef->getLinear();
                mQuadratic   = lightDef->getQuadratic();
                break;
        }
    }

    KMap LightInstance::getKMap(int distance)
    {
        for (auto pair : K_VALUE_MAP)
        {
           if (pair.first >= distance)
           {
               return pair.second;
           }
        }
        return K_VALUE_MAP[0];
    }


    /*
     * Distance	Constant 	Linear	Quadratic
        13	    1.0	 0.35	0.44
        20	    1.0	 0.22	0.20
        32	    1.0	 0.14	0.07
        50	    1.0	 0.09	0.032
        65	    1.0	 0.07	0.017
        100	    1.0	 0.045	0.0075
        160	    1.0	 0.027	0.0028
        200	    1.0	 0.022	0.0019
        325	    1.0	 0.014	0.0007
        600	    1.0	 0.007	0.0002
        3250	    1.0	 0.0014	0.000007
     */
    map<int, KMap> LightInstance::K_VALUE_MAP
    {
        pair<int,KMap>(7,    KMap{1.0f, 0.7f,    1.8f}),
        pair<int,KMap>(13,   KMap{1.0f,	0.35f,   0.44f}),
        pair<int,KMap>(20,   KMap{1.0f,	0.22f,   0.20f}),
        pair<int,KMap>(32,   KMap{1.0f,	0.14f,   0.07f}),
        pair<int,KMap>(50,   KMap{1.0f,	0.09f,   0.032f}),
        pair<int,KMap>(65,   KMap{1.0f,	0.07f,   0.017f}),
        pair<int,KMap>(100,  KMap{1.0f,	0.045f,  0.0075f}),
        pair<int,KMap>(160,  KMap{1.0f,	0.027f,  0.0028f}),
        pair<int,KMap>(200,  KMap{1.0f,	0.022f,  0.0019f}),
        pair<int,KMap>(325,  KMap{1.0f,	0.014f,  0.0007f}),
        pair<int,KMap>(600,  KMap{1.0f,	0.007f,  0.0002f}),
        pair<int,KMap>(3250, KMap{1.0f,	0.0014f, 0.000007f})
    };

} // End of Dream
