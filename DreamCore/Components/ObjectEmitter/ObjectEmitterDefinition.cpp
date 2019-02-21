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
#include "ObjectEmitterDefinition.h"

namespace Dream
{
    ObjectEmitterDefinition::ObjectEmitterDefinition
    (ProjectDefinition* pd, const json &js)
        : AssetDefinition(pd,js)
    {

    }

    ObjectEmitterDefinition::~ObjectEmitterDefinition()
    {

    }

    int
    ObjectEmitterDefinition::getObjectCount
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_OBJECT_COUNT].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_OBJECT_COUNT] = 1;
        }
        return mJson[Constants::OBJECT_EMITTER_OBJECT_COUNT];
    }

    void
    ObjectEmitterDefinition::setObjectCount
    (int count)
    {
        mJson[Constants::OBJECT_EMITTER_OBJECT_COUNT] = count;
    }

    uint32_t
    ObjectEmitterDefinition::getSceneObjectUuid
    ()
    {
        if(!mJson[Constants::OBJECT_EMITTER_UUID].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_UUID] = 0;
        }
        return mJson[Constants::OBJECT_EMITTER_UUID];
    }

    void
    ObjectEmitterDefinition::setSceneObjectUuid
    (uint32_t uuid)
    {
        mJson[Constants::OBJECT_EMITTER_UUID] = uuid;
    }

    int
    ObjectEmitterDefinition::getEmitInterval
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_EMIT_INTERVAL].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_EMIT_INTERVAL] = 1000;
        }
        return mJson[Constants::OBJECT_EMITTER_EMIT_INTERVAL];
    }

    void
    ObjectEmitterDefinition::setEmitInterval
    (int f)
    {
        mJson[Constants::OBJECT_EMITTER_EMIT_INTERVAL] = f;
    }

    int
    ObjectEmitterDefinition::getLoopInterval
    ()
    {
        if(!mJson[Constants::OBJECT_EMITTER_LOOP_INTERVAL].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_LOOP_INTERVAL] = 1000;
        }
        return mJson[Constants::OBJECT_EMITTER_LOOP_INTERVAL];
    }

    void
    ObjectEmitterDefinition::setLoopInterval
    (int pause)
    {
        mJson[Constants::OBJECT_EMITTER_LOOP_INTERVAL] = pause;
    }

    int
    ObjectEmitterDefinition::getLoops
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_LOOPS].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_LOOPS] = 1;
        }
        return mJson[Constants::OBJECT_EMITTER_LOOPS];
    }

    void
    ObjectEmitterDefinition::setLoops
    (int repeats)
    {
        mJson[Constants::OBJECT_EMITTER_LOOPS] = repeats;
    }

    float
    ObjectEmitterDefinition::getObjectVelocity
    ()
    {
        if(!mJson[Constants::OBJECT_EMITTER_VELOCITY].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_VELOCITY] = 1.0f;
        }
        return mJson[Constants::OBJECT_EMITTER_VELOCITY];
    }

    void
    ObjectEmitterDefinition::setObjectVelocity
    (float v)
    {
        mJson[Constants::OBJECT_EMITTER_VELOCITY] = v;
    }

    float
    ObjectEmitterDefinition::getStartTheta
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_START_THETA].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_START_THETA] = 0;
        }
        return mJson[Constants::OBJECT_EMITTER_START_THETA];
    }

    void
    ObjectEmitterDefinition::setStartTheta
    (float t)
    {
        mJson[Constants::OBJECT_EMITTER_START_THETA] = t;
    }

    float
    ObjectEmitterDefinition::getEndTheta
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_END_THETA].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_END_THETA] = 0;
        }
        return mJson[Constants::OBJECT_EMITTER_END_THETA];
    }

    void
    ObjectEmitterDefinition::setEndTheta
    (float t)
    {
        mJson[Constants::OBJECT_EMITTER_END_THETA] = t;
    }

    float
    ObjectEmitterDefinition::getStartRadius
    ()
    {
        if (!mJson[Constants::OBJECT_EMITTER_START_RADIUS].is_number())
        {
            mJson[Constants::OBJECT_EMITTER_START_RADIUS] = 0;
        }
        return mJson[Constants::OBJECT_EMITTER_START_RADIUS];
    }

    void
    ObjectEmitterDefinition::setStartRadius
    (float t)
    {
        mJson[Constants::OBJECT_EMITTER_START_RADIUS] = t;
    }

}
