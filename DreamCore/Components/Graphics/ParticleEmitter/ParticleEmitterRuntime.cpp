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
#include "ParticleEmitterRuntime.h"

#include "ParticleEmitterDefinition.h"
#include "Common/Logger.h"
#include "Components/Time.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    ParticleEmitterRuntime::ParticleEmitterRuntime
    (
        ParticleEmitterDefinition* definition,
        EntityRuntime* transform
    ) : DiscreteAssetRuntime(definition,transform)
    {
        LOG_TRACE("Constructing");
    }

    void
    ParticleEmitterRuntime::update
    ()
    {
        // Update Velocity
        auto time = mEntityRuntime->getSceneRuntime()->getProjectRuntime()->getTime();
        for (Particle p : mParticles)
        {
            p.mVelocity.x += time->perSecond(p.mVelocity.x);
            /*
                update current velocity by velocity
                increate lifetime
                if lifetime > max life time then dead == true
            */
        }
    }

    ParticleEmitterRuntime::~ParticleEmitterRuntime
    ()
    {
        LOG_TRACE("Destroying");
    }

    bool
    ParticleEmitterRuntime::useDefinition
    ()
    {
        mLoaded = true;
        return mLoaded;
    }
}
