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
    (ProjectDefinition* pd, const json &js)
        : AssetDefinition(pd,js)
    {

    }

    Vector3
    ScrollerDefinition::getVelocity
    ()
    {
       if (!mJson[Constants::SCROLLER_VELOCITY].is_object())
       {
           setVelocity(Vector3(0.0f));
       }
       return unwrapVector3(mJson[Constants::SCROLLER_VELOCITY]);
    }

    void
    ScrollerDefinition::setVelocity
    (const Vector3& vel)
    {
        mJson[Constants::SCROLLER_VELOCITY] = wrapVector3(vel);
    }

    bool
    ScrollerDefinition::getLoop
    ()
    {
       if (!mJson[Constants::SCROLLER_LOOP].is_boolean())
       {
           mJson[Constants::SCROLLER_LOOP] = false;
       }
       return mJson[Constants::SCROLLER_LOOP];
    }

    void
    ScrollerDefinition::setLoop
    (bool loop)
    {
        mJson[Constants::SCROLLER_LOOP] = loop;
    }

    Vector3 ScrollerDefinition::getRangeBegin()
    {
       if (!mJson[Constants::SCROLLER_RANGE_BEGIN].is_object())
       {
           setRangeBegin(Vector3(0.0f));
       }
       return unwrapVector3(mJson[Constants::SCROLLER_RANGE_BEGIN]);

    }

    void ScrollerDefinition::setRangeBegin(const Vector3& range)
    {
       mJson[Constants::SCROLLER_RANGE_BEGIN] = wrapVector3(range);
    }

    Vector3 ScrollerDefinition::getRangeEnd()
    {
        if (!mJson[Constants::SCROLLER_RANGE_END].is_object())
       {
           setRangeEnd(Vector3(0.0f));
       }
       return unwrapVector3(mJson[Constants::SCROLLER_RANGE_END]);

    }

    void ScrollerDefinition::setRangeEnd(const Vector3& range)
    {
       mJson[Constants::SCROLLER_RANGE_END] = wrapVector3(range);
    }

    void
    ScrollerDefinition::updateScrollerItem
    (const ScrollerItem& item)
    {
        checkItemsArray();
        auto iter =  mJson[Constants::SCROLLER_ITEMS_ARRAY].begin();
        auto end = mJson[Constants::SCROLLER_ITEMS_ARRAY].end();
        for (; iter != end; iter++)
        {
            if ((*iter)[Constants::SCROLLER_ITEM_INDEX] == item.index)
            {
                (*iter)[Constants::SCROLLER_ITEM_ORIGIN] = wrapVector3(item.origin);
                (*iter)[Constants::UUID] = item.uuid;
                return;
            }
        }
    }

    void
    ScrollerDefinition::removeScrollerItem
    (const ScrollerItem& item)
    {
        checkItemsArray();
        auto iter =  mJson[Constants::SCROLLER_ITEMS_ARRAY].begin();
        auto end = mJson[Constants::SCROLLER_ITEMS_ARRAY].end();
        for (; iter != end; iter++)
        {
            if ((*iter)[Constants::SCROLLER_ITEM_INDEX] == item.index)
            {
                mJson[Constants::SCROLLER_ITEMS_ARRAY].erase(iter);
                return;
            }
        }

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
       j[Constants::SCROLLER_ITEM_ORIGIN] = wrapVector3(item.origin);
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
        i.origin = unwrapVector3(j[Constants::SCROLLER_ITEM_ORIGIN]);
        return i;
    }

    vector<ScrollerItem>
    ScrollerDefinition::getItemsArray
    ()
    {
        checkItemsArray();
        vector<ScrollerItem> retval;

        for (const json& item : mJson[Constants::SCROLLER_ITEMS_ARRAY])
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

    unsigned int
    ScrollerDefinition::getNextItemIndex
    ()
    {
        checkItemsArray();
        unsigned int index = 0;
        for (const json& js : mJson[Constants::SCROLLER_ITEMS_ARRAY])
        {
            unsigned int nextIndex = js[Constants::SCROLLER_ITEM_INDEX];
            if (nextIndex > index)
            {
                index = nextIndex;
            }
        }
        index++;
        return index;
    }

    void
    ScrollerDefinition::moveScrollerItem
    (const ScrollerItem& item, bool forward)
    {
        checkItemsArray();
        auto begin = mJson[Constants::SCROLLER_ITEMS_ARRAY].begin();
        auto end = mJson[Constants::SCROLLER_ITEMS_ARRAY].end();
        auto target = std::find_if
        (
            begin,end,
            [item]
            (json& a)->bool
            {
                return a[Constants::SCROLLER_ITEM_INDEX]==item.index;
            }
        );

        if (target != end)
        {
            if (forward && target+1 < end)
            {
                std::iter_swap(target,target+1);
            }
            else if (!forward && target-1 >= begin)
            {
               std::iter_swap(target,target-1);
            }
        }
    }

    void
    ScrollerDefinition::copyScrollerItem
    (const ScrollerItem& item)
    {
        checkItemsArray();
        auto target = std::find_if
        (
            mJson[Constants::SCROLLER_ITEMS_ARRAY].begin(),
            mJson[Constants::SCROLLER_ITEMS_ARRAY].end(),
            [item]
            (json& a)->bool
            {
                return a[Constants::SCROLLER_ITEM_INDEX]==item.index;
            }
        );

        if (target != mJson[Constants::SCROLLER_ITEMS_ARRAY].end())
        {
            auto newItem = item;
            newItem.index = getNextItemIndex();
            if (target != mJson[Constants::SCROLLER_ITEMS_ARRAY].begin())
            {
                auto prev = (target-1);
                Vector3 delta = newItem.origin-unwrapScrollerItem(*prev).origin;
                newItem.origin += delta;
            }
            mJson[Constants::SCROLLER_ITEMS_ARRAY].insert(target+1,wrapScrollerItem(newItem));
        }
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
