#include "ParticleEmitterInstance.h"

#include "ParticleEmitterDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    ParticleEmitterInstance::ParticleEmitterInstance
    (
        ParticleEmitterDefinition* definition,
        SceneObjectRuntime* transform
    ) : IAssetInstance(definition,transform),
        ILoggable("ParticleEmitterInstance"),
        mColor(glm::vec3(0.0f,0.0f,0.0f)),
        mIntensity(0.0f)

    {
        loadExtraAttributes(definition->getJson());
    }

    ParticleEmitterInstance::~ParticleEmitterInstance
    ()
    {
        auto log = getLog();
        log->info("Destroying Object" );
        return;
    }

    bool
    ParticleEmitterInstance::load
    (string projectDir)
    {
        mLoaded = true;
        return mLoaded;
    }

    void
    ParticleEmitterInstance::loadExtraAttributes
    (nlohmann::json json)
    {
        auto log = getLog();
    }

} // End of Dream
