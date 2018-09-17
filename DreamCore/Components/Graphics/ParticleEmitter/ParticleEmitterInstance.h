#pragma once

#include <glm/glm.hpp>

#include "../../IAssetInstance.h"

using glm::vec3;

namespace Dream
{
    class ParticleEmitterDefinition;
    class ParticleEmitterInstance : public IAssetInstance
    {
        vec3 mColor;
        float mIntensity;

    public:
        ParticleEmitterInstance(
            ParticleEmitterDefinition*,
            SceneObjectRuntime*
        );
        ~ParticleEmitterInstance();

        bool load(string);
        void loadExtraAttributes(json);

        vec3 getColor();
        float getIntensity();

    }; // End of ParticleEmitterInstance

} // End of Dream
