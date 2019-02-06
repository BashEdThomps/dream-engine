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

namespace Dream
{
    struct ScrollerItem
    {
       unsigned int index = 0;
       uint32_t uuid = 0;
       vec3 origin = vec3(0.0f);
    };

    class ScrollerDefinition : public AssetDefinition
    {
    public:
        ScrollerDefinition(ProjectDefinition*, const json&);
        ~ScrollerDefinition() override;

        vec3 getVelocity();
        void setVelocity(vec3);

        bool getLoop();
        void setLoop(bool loop);

        vec3 getRangeBegin();
        void setRangeBegin(vec3);

        vec3 getRangeEnd();
        void setRangeEnd(vec3);

        void updateScrollerItem(const ScrollerItem& item);
        void removeScrollerItem(const ScrollerItem& item);
        vector<ScrollerItem> getItemsArray();
        void addItem(const ScrollerItem& item);
        unsigned int getNextItemIndex();

        void moveScrollerItem(const ScrollerItem& item, bool forward);
        void copyScrollerItem(const ScrollerItem& item);


    protected:
        json wrapScrollerItem(const ScrollerItem& item);
        ScrollerItem unwrapScrollerItem(const json& j);

        void checkItemsArray();
    };
}
