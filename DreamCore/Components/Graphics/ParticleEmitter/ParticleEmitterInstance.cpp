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
        log->debug("Destroying Object" );
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
