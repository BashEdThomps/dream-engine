/*
 * Event
 *
 * Created: 18/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
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
    (
        SceneObjectRuntime* sender,
        string type
    ) : DreamObject("Event"),
        mSender(sender)
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Creating");
#endif
        mData[Constants::EVENT_TYPE] = type;
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

    string
    Event::getType
    ()
    const
    {
        return mData[Constants::EVENT_TYPE];
    }

    void
    Event::setJson
    (string key, nlohmann::json value)
    {
        mData[key] = value;
    }

    nlohmann::json
    Event::getJson
    (string key)
    const
    {
       if (mData[key].is_null()) return nlohmann::json();
       return mData[key];
    }

    void Event::setString(string key, string value)
    {
        mData[key] = value;
    }

    string
    Event::getString
    (string key)
    const
    {
        if (mData.find(key) == mData.end() || mData[key].is_null())
        {
            return "";
        }
        return mData[key].dump();
    }

    string Event::getData() const
    {
        return mData.dump();
    }

    void Event::setNumber(string name, double number)
    {
       mData[name] = number;
    }

    double
    Event::getNumber(string name)
    {
       return mData[name];
    }
} // End of Dream
