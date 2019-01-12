/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include "../../AssetDefinition.h"

namespace Dream
{
    class ParticleEmitterDefinition : public AssetDefinition
    {
    public:
        ParticleEmitterDefinition(ProjectDefinition* pd, json js);
        ~ParticleEmitterDefinition();

        uint32_t getTexture();
        void setTexture(uint32_t texture);

        vec3 getArea();
        void setArea(const vec3& area);

        vec2 getParticleSize();
        void setParticleSize(const vec2& size);

        float getVelocity();
        void setVelocity(float velocity);

        float getGravity();
        void setGravity(float gravity);

        float getParticlesPerSecond();
        void setParticlesPerSecond(float particlesPerSecond);

        float getParticleLifetime();
        void setParticleLifetime(float particleLifetime);
    private:
        void initSize();
        void initArea();
    };

}
