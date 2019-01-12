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
#include "ParticleEmitterDefinition.h"

namespace Dream
{
    ParticleEmitterDefinition::ParticleEmitterDefinition
    (ProjectDefinition* pd, json js)
        : AssetDefinition(pd,js)
    {
    }

    ParticleEmitterDefinition::~ParticleEmitterDefinition() {}

    float
    ParticleEmitterDefinition::getParticleLifetime
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_LIFETIME].is_number())
        {
            mJson[Constants::PARTICLE_EMITTER_LIFETIME] = 1;
        }
        return mJson[Constants::PARTICLE_EMITTER_LIFETIME];
    }

    void
    ParticleEmitterDefinition::setParticleLifetime
    (float particleLifetime)
    {
        mJson[Constants::PARTICLE_EMITTER_LIFETIME] = particleLifetime;
    }

    float
    ParticleEmitterDefinition::getParticlesPerSecond
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_PER_SECOND].is_number())
        {
            mJson[Constants::PARTICLE_EMITTER_PER_SECOND] = 1;
        }
        return mJson[Constants::PARTICLE_EMITTER_PER_SECOND];
    }

    void
    ParticleEmitterDefinition::setParticlesPerSecond
    (float particlesPerSecond)
    {
        mJson[Constants::PARTICLE_EMITTER_PER_SECOND] = particlesPerSecond;
    }

    float
    ParticleEmitterDefinition::getGravity
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_GRAVITY].is_number())
        {
            mJson[Constants::PARTICLE_EMITTER_GRAVITY] = 1;
        }
        return mJson[Constants::PARTICLE_EMITTER_GRAVITY];
    }

    void
    ParticleEmitterDefinition::setGravity
    (float gravity)
    {
        mJson[Constants::PARTICLE_EMITTER_GRAVITY] = gravity;
    }

    float
    ParticleEmitterDefinition::getVelocity
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_VELOCITY].is_number())
        {
            mJson[Constants::PARTICLE_EMITTER_VELOCITY] = 1;
        }
        return mJson[Constants::PARTICLE_EMITTER_VELOCITY];
    }

    void
    ParticleEmitterDefinition::setVelocity
    (float velocity)
    {
        mJson[Constants::PARTICLE_EMITTER_VELOCITY] = velocity;
    }

    vec2
    ParticleEmitterDefinition::getParticleSize
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_SIZE].is_object())
        {
            initSize();
        }
        return vec2(
            mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::X],
            mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::Y]
        );
    }

    void
    ParticleEmitterDefinition::setParticleSize
    (const vec2& size)
    {
        if (!mJson[Constants::PARTICLE_EMITTER_SIZE].is_object())
        {
            initSize();
        }
        mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::X] = size.x ;
        mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::Y] = size.y ;
    }

    vec3
    ParticleEmitterDefinition::getArea
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_AREA].is_object())
        {
            initArea();
        }

        return vec3(
            mJson[Constants::PARTICLE_EMITTER_AREA][Constants::X],
            mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Y],
            mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Z]
        );
    }

    void
    ParticleEmitterDefinition::setArea
    (const vec3& area)
    {
        if (!mJson[Constants::PARTICLE_EMITTER_AREA].is_object())
        {
            initArea();
        }
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::X] = area.x;
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Y] = area.y;
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Z] = area.z;
    }

    uint32_t
    ParticleEmitterDefinition::getTexture
    ()
    {
        if (!mJson[Constants::PARTICLE_EMITTER_TEXTURE].is_number())
        {
            mJson[Constants::PARTICLE_EMITTER_TEXTURE] = 1;
        }
        return mJson[Constants::PARTICLE_EMITTER_TEXTURE];
    }

    void
    ParticleEmitterDefinition::setTexture
    (uint32_t texture)
    {
        mJson[Constants::PARTICLE_EMITTER_TEXTURE] = texture;
    }

    void
    ParticleEmitterDefinition::initSize
    ()
    {
        mJson[Constants::PARTICLE_EMITTER_SIZE] = json::object();
        mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::X] = 1;
        mJson[Constants::PARTICLE_EMITTER_SIZE][Constants::Y] = 1;
    }

    void
    ParticleEmitterDefinition::initArea
    ()
    {
        mJson[Constants::PARTICLE_EMITTER_AREA] = json::object();
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::X] = 1;
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Y] = 1;;
        mJson[Constants::PARTICLE_EMITTER_AREA][Constants::Z] = 1;;
    }
}
