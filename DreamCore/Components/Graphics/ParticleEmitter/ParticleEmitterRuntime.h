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


#pragma once

#include <deque>
#include <glm/glm.hpp>

#include "../../DiscreteAssetRuntime.h"
#include "../../Transform.h"

using namespace glm;
using std::deque;

namespace Dream
{
    struct Particle
    {
        vec3 mVelocity;
        Transform mTransform;
        float mLifetime;
        float mElapsedTime;
    };

    class ParticleEmitterDefinition;
    class ParticleEmitterRuntime : public DiscreteAssetRuntime
    {
    public:
        ParticleEmitterRuntime(ParticleEmitterDefinition*, SceneObjectRuntime*);
        ~ParticleEmitterRuntime() override;
        bool useDefinition() override;

        Transform getTransform() const;
        void setTransform(const Transform& transform);

        float getLifetime() const;
        void setLifetime(float lifetime);

        float getElapsedTime() const;

        void setElapsedTime(float elapsedTime);

        bool isDead();
        void update();
    private:
        deque<Particle> mParticles;

    };
}
