#pragma once

#include <glm/glm.hpp>

#include "../../IAssetInstance.h"
#include "../../../Common/ILoggable.h"

using glm::vec3;

namespace Dream
{
    class ParticleEmitterDefinition;
    class ParticleEmitterInstance : public IAssetInstance, ILoggable
    {
        vec3 mColor;
        float mIntensity;

    public:
        ParticleEmitterInstance(shared_ptr<ParticleEmitterDefinition>, shared_ptr<SceneObjectRuntime>);
        ~ParticleEmitterInstance();

        bool load(string);
        void loadExtraAttributes(json);

        vec3 getColor();
        float getIntensity();

    }; // End of ParticleEmitterInstance

} // End of Dream
