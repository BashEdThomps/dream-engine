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


#include <algorithm>

#include "ScrollerRuntime.h"
#include "ScrollerTasks.h"
#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    ScrollerRuntime::ScrollerRuntime
    (
        ScrollerDefinition* definition,
        EntityRuntime* runtime
    ) : DiscreteAssetRuntime(definition,runtime),
        mVelocity(0.0f),
        mRangeBegin(0.0f),
        mRangeEnd(0.0f),
        mUpdateTask(this)
    {
        LOG_TRACE("Constructing Object");
    }

    ScrollerRuntime::~ScrollerRuntime
    ()
    {
        LOG_TRACE("Destroying Object");
    }

    bool
    ScrollerRuntime::useDefinition
    ()
    {
        LOG_DEBUG("Creating ScrollerRuntime Children for {}", mEntityRuntime->getNameAndUuidString());
        auto scrollerDef = static_cast<ScrollerDefinition*>(mDefinition);
        auto items = scrollerDef->getItemsArray();
        mVelocity = scrollerDef->getVelocity();
        mRangeBegin =  scrollerDef->getRangeBegin();
        mRangeEnd =  scrollerDef->getRangeEnd();

        for (const ScrollerItem& item : items)
        {
            auto newChild = createChlidRuntime(item);
            mAssetsMap.insert(pair<uint32_t,uint32_t>(newChild->getUuid(),item.uuid));
            mPreRange.push_back(newChild);
        }
        mLoaded = true;
        return mLoaded;
    }

    void
    ScrollerRuntime::addAssets
    (EntityRuntime* runt)
    {
        runt->replaceAssetUuid(AssetType::MODEL,mAssetsMap[runt->getUuid()]);
    }

    ScrollerRuntime::Range
    ScrollerRuntime::checkRange
    (EntityRuntime* runt)
    const
    {
        Range inRangeX = None;
        //Range inRangeY = None;
        //Range inrangeZ = None;

        vec3 tx = runt->getTransform().getMatrix()[3];

        // Range check is dependent on heading, determined by velocity.

        // Going from + -> -
        if (mVelocity.x() < 0)
        {
           if (tx.x > mRangeBegin.x())
           {
               inRangeX = PreRange;
           }
           else if (tx.x < mRangeEnd.x())
           {
               inRangeX = PostRange;
           }
           else
           {
               inRangeX = InRange;
           }
        }
        // Going from - -> +
        else if (mVelocity.x() > 0)
        {
           if (tx.x < mRangeBegin.x())
           {
               inRangeX = PreRange;
           }
           else if (tx.x > mRangeEnd.x())
           {
               inRangeX = PostRange;
           }
           else
           {
               inRangeX = InRange;
           }
        }
        // Not Moving
        else
        {
           inRangeX = None;
        }
        return inRangeX;
    }

    EntityRuntime*
    ScrollerRuntime::createChlidRuntime
    (const ScrollerItem& item)
    {
        EntityRuntime* newChild = new EntityRuntime(nullptr,mEntityRuntime->getSceneRuntime(),true);
        newChild->setParentRuntime(mEntityRuntime);
        newChild->setName(mEntityRuntime->getName()+"_Scroller_Child_"+std::to_string(item.index));
        newChild->initTransform();
        newChild->getInitialTransform().translate(item.origin);
        newChild->getTransform().translate(item.origin);
        mEntityRuntime->addChildRuntime(newChild);
        return newChild;
    }

    void
    ScrollerRuntime::update
    ()
    {
        LOG_TRACE("Updating Runtime");
        auto time =
            mEntityRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getTime();
        if (time->getFrameTimeDelta() > Time::DELTA_MAX)
        {
            return;
        }

        Vector3 delta
        (
           time->perSecond(mVelocity.x()),
           time->perSecond(mVelocity.y()),
           time->perSecond(mVelocity.z())
        );

        mEntityRuntime->lock();
        auto children = mEntityRuntime->getChildRuntimes();
        LOG_TRACE("Child has {} children", children.size());
        for (auto* child : children)
        {
            child->lock();
            LOG_TRACE("Translating Child with delta vel {},{},{}", delta.x(), delta.y(), delta.z());
            child->getTransform().translate(delta);
            child->unlock();
        }
        mEntityRuntime->unlock();

        for (auto iter = mPreRange.begin(); iter != mPreRange.end();)
        {
           if (checkRange(*iter) == Range::InRange)
           {
              LOG_ERROR("New Child in range {}",(*iter)->getNameAndUuidString());
              addAssets(*iter);
              mInRange.push_back(*iter);
              mPreRange.erase(iter);
           }
           else
           {
            iter++;
           }
        }

        for (auto iter = mInRange.begin(); iter != mInRange.end();)
        {
           if (checkRange(*iter) == Range::PostRange)
           {
               LOG_ERROR("Child has gone out of range {}",(*iter)->getNameAndUuidString());
               mPostRange.push_back(*iter);
               mInRange.erase(iter);
           }
           else
           {
            iter++;
           }
        }
        collectGarbage();
    }

    void
    ScrollerRuntime::collectGarbage
    ()
    {
        mEntityRuntime->lock();
        for (EntityRuntime* runt : mPostRange)
        {
            runt->lock();
            LOG_ERROR("Garbage! {}",runt->getNameAndUuidString());
            mEntityRuntime->removeChildRuntime(runt);
            runt->unlock();
        }
        mPostRange.clear();
        mEntityRuntime->unlock();
    }

    ScrollerUpdateTask* ScrollerRuntime::getUpdateTask()
    {
        return &mUpdateTask;
    }

}
