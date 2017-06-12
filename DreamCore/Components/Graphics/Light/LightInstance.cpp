#include "LightInstance.h"

namespace Dream
{
    LightInstance::LightInstance
    (
        AssetDefinition* definition,
        Transform3D* transform
    )
        : IAssetInstance(definition,transform)
    {
        mColor     = glm::vec3(0.0f,0.0f,0.0f);
        mIntensity = 0.0f;
        loadExtraAttributes(definition->toJson());
    }

    LightInstance::~LightInstance
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "LightInstance: Destroying Object" << endl;
        }
        return;
    }

    bool
    LightInstance::load
    (string projectDir)
    {
        mLoaded = true;
        return mLoaded;
    }

    void
    LightInstance::loadExtraAttributes
    (nlohmann::json json)
    {
        nlohmann::json jsonColor = json[Constants::ASSET_ATTR_LIGHT_COLOR];
        if (!jsonColor.is_null())
        {
            mColor.r   = static_cast<float>(jsonColor[Constants::ASSET_ATTR_LIGHT_COLOR_R]);
            mColor.g   = static_cast<float>(jsonColor[Constants::ASSET_ATTR_LIGHT_COLOR_G]);
            mColor.b   = static_cast<float>(jsonColor[Constants::ASSET_ATTR_LIGHT_COLOR_B]);
            mIntensity = static_cast<float>(jsonColor[Constants::ASSET_ATTR_LIGHT_COLOR_A]);
        }
        else
        {
            cerr <<"LightInstance: Unable to find color in json" << endl;
        }
    }

    glm::vec3
    LightInstance::getColor
    ()
    {
        return mColor;
    }

    float
    LightInstance::getIntensity
    ()
    {
        return mIntensity;
    }

} // End of Dream
