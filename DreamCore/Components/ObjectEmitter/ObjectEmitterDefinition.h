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

#include "../AssetDefinition.h"
#include "../../Common/Math.h"

namespace Dream
{
    class ObjectEmitterDefinition : public AssetDefinition
    {
    public:
        ObjectEmitterDefinition(ProjectDefinition* pd, const json& js);
        ~ObjectEmitterDefinition();

        int getObjectCount();
        void setObjectCount(int count);

        uint32_t getSceneObjectUuid();
        void setSceneObjectUuid(uint32_t);

        int getEmitInterval();
        void setEmitInterval(int);

        int getLoopInterval();
        void setLoopInterval(int pause);

        int getLoops();
        void setLoops(int loops);

        float getObjectVelocity();
        void setObjectVelocity(float v);
    };
}
