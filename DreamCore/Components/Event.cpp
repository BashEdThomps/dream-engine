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

#include "Event.h"

#include "../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    Event::Event
    (SceneObjectRuntime* sender) : DreamObject("Event"),
        mSender(sender)
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Creating");
#endif
    }

    CollisionData Event::getCollisionData() const
    {
        return mCollisionData;
    }

    void Event::setCollisionData(const CollisionData& collisionData)
    {
        mCollisionData = collisionData;
    }

    Event::~Event()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destroying");
#endif
    }

    SceneObjectRuntime*
    Event::getSender
    ()
    const
    {
        return mSender;
    }
}
