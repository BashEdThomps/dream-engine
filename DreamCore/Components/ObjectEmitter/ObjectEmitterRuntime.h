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

#include "../DiscreteAssetRuntime.h"
#include "../Transform.h"
#include "ObjectEmitterTasks.h"

using namespace glm;
using std::deque;

namespace Dream
{
    class ObjectEmitterDefinition;
    class ObjectEmitterRuntime : public DiscreteAssetRuntime
    {
        enum EmitterState
        {
            Emit,
            EmitInterval,
            Done
        };

        uint32_t mActorUuid;
        int mLoops;
        int mObjectCount;
        int mEmitInterval;
        float mObjectVelocity;
        float mStartRadius;
        float mStartTheta;
        float mEndTheta;
        ObjectEmitterUpdateTask mUpdateTask;

        // Running state
        EmitterState mState;
        int mLoopCount;
        int mEmitIntervalTime;

    public:
        ObjectEmitterRuntime(ObjectEmitterDefinition*, ActorRuntime*);
        ~ObjectEmitterRuntime() override;
        bool useDefinition() override;
        ObjectEmitterUpdateTask* getUpdateTask();
        bool update();
        void emitObjects();
    };
}
