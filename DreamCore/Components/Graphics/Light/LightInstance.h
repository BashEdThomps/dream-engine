#pragma once

#include "../../../Common/Constants.h"
#include "../../../Common/AssetDefinition.h"
#include "../../../Common/IAssetInstance.h"

namespace Dream
{
    class LightInstance : public IAssetInstance
    {
        glm::vec3 mColor;
        float mIntensity;

    public:
        LightInstance(AssetDefinition*, Transform3D*);
        ~LightInstance();
        bool load(string);
        void loadExtraAttributes(nlohmann::json);

        glm::vec3 getColor();
        float getIntensity();

    }; // End of LightInstance

} // End of Dream
