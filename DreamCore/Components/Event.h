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

#include <string>

#include "../Common/DreamObject.h"
#include <glm/vec3.hpp>

using glm::vec3;

namespace Dream
{
    class SceneObjectRuntime;

    struct CollisionData
    {
        bool present = false;
        float impulse = 0.0;
        vec3 position = vec3(0.0f);
    };

    class Event : public DreamObject
    {
    private:
        SceneObjectRuntime* mSender;
        CollisionData mCollisionData;

    public:
        Event(SceneObjectRuntime*);
        ~Event();

        SceneObjectRuntime* getSender() const;

        CollisionData getCollisionData() const;
        void setCollisionData(const CollisionData& collisionData);
    };
}
