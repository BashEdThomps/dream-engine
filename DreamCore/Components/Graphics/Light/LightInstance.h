#pragma once

#include <glm/glm.hpp>

#include "../../IAssetInstance.h"
#include "../../../Common/ILoggable.h"

using glm::vec3;

namespace Dream
{
    class LightDefinition;
    class LightInstance : public IAssetInstance, ILoggable
    {
        vec3 mColor;
        float mIntensity;

    public:
        LightInstance(LightDefinition*, SceneObjectRuntime*);
        ~LightInstance();

        bool load(string);
        void loadExtraAttributes(json);

        vec3 getColor();
        float getIntensity();

    }; // End of LightInstance

} // End of Dream
