#include "ParticleEmitterInstance.h"

#include "ParticleEmitterDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    ParticleEmitterInstance::ParticleEmitterInstance
    (
        const shared_ptr<ParticleEmitterDefinition>& definition,
        const shared_ptr<SceneObjectRuntime>& transform
    ) : IAssetInstance(definition,transform),
        mColor(glm::vec3(0.0f,0.0f,0.0f)),
        mIntensity(0.0f)

    {
        setLogClassName("ParticleEmitterInstance");
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
