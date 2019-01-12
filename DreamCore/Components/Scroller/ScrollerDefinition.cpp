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
#include "ScrollerDefinition.h"

namespace Dream
{
    ScrollerDefinition::ScrollerDefinition
    (ProjectDefinition* pd, json js)
        : AssetDefinition(pd,js)
    {

    }

    vec3
    ScrollerDefinition::getVelocity
    ()
    {
       if (!mJson[Constants::SCROLLER_VELOCITY].is_object())
       {
           setVelocity(vec3(0.0f));
       }
       return unwrapVec3(mJson[Constants::SCROLLER_VELOCITY]);
    }

    void
    ScrollerDefinition::setVelocity
    (vec3 vel)
    {
        mJson[Constants::SCROLLER_VELOCITY] = wrapVec3(vel);
    }

    bool ScrollerDefinition::getLoop()
    {
       if (!mJson[Constants::SCROLLER_LOOP].is_boolean())
       {
           mJson[Constants::SCROLLER_LOOP] = false;
       }
       return mJson[Constants::SCROLLER_LOOP];
    }

    void ScrollerDefinition::setLoop(bool loop)
    {
       mJson[Constants::SCROLLER_LOOP] = loop;
    }

    ScrollerDefinition::~ScrollerDefinition()
    {

    }

    json
    ScrollerDefinition::wrapScrollerItem
    (const ScrollerItem& item)
    {
       json j;
       j[Constants::SCROLLER_ITEM_INDEX] = item.index;
       j[Constants::SCROLLER_ITEM_OFFSET] = wrapVec3(item.offset);
       j[Constants::UUID] = item.uuid;
       return j;
    }

    ScrollerItem
    ScrollerDefinition::unwrapScrollerItem
    (const json& j)
    {
        ScrollerItem i;
        i.index = j[Constants::SCROLLER_ITEM_INDEX];
        i.uuid = j[Constants::UUID];
        i.offset = unwrapVec3(j[Constants::SCROLLER_ITEM_OFFSET]);
        return i;
    }

    vector<ScrollerItem>
    ScrollerDefinition::getItemsArray
    ()
    {
        checkItemsArray();
        vector<ScrollerItem> retval;

        for (json item : mJson[Constants::SCROLLER_ITEMS_ARRAY])
        {
            retval.push_back(unwrapScrollerItem(item));
        }
        return retval;
    }

    void
    ScrollerDefinition::addItem
    (const ScrollerItem& item)
    {
        checkItemsArray();
        mJson[Constants::SCROLLER_ITEMS_ARRAY].push_back(wrapScrollerItem(item));
    }


    void
    ScrollerDefinition::checkItemsArray
    ()
    {
        if (!mJson[Constants::SCROLLER_ITEMS_ARRAY].is_array())
        {
            mJson[Constants::SCROLLER_ITEMS_ARRAY] = json::array();
        }
    }
}
