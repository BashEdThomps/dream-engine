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

namespace Dream
{
    Event::Event(string sender, string type)
    {
        setAttribute(EVENT_SENDER,sender);
        setAttribute(EVENT_TYPE,type);
    }

    Event::~Event()
    {
        if (VERBOSE)
        {
            cout << "Event: Destroying Object" << endl;
        }
        mAttributes.clear();
    }

    string Event::getSender()
    {
        return getAttribute(EVENT_SENDER);
    }

    string Event::getType()
    {
        return getAttribute(EVENT_TYPE);
    }

    void Event::setAttribute(string key, string value)
    {
        mAttributes.insert(pair<string,string>(key,value));
    }

    string Event::getAttribute(string key)
    {
        for (pair<string,string> it : mAttributes)
        {
            if (it.first.compare(key)==0)
            {
                return it.second;
            }
        }
        return "";
    }

} // End of Dream
