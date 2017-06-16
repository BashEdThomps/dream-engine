#pragma once

#include <glm/glm.hpp>

#include "../../IAssetInstance.h"

using glm::vec3;

namespace Dream
{
    class LightInstance : public IAssetInstance
    {
        vec3 mColor;
        float mIntensity;

    public:
        LightInstance(AssetDefinition*, Transform3D*);
        ~LightInstance();

        bool load(string);
        void loadExtraAttributes(json);

        vec3 getColor();
        float getIntensity();

    }; // End of LightInstance

} // End of Dream
