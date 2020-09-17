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
#include "../Time.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/Actor/ActorRuntime.h"
#include "../../Project/ProjectRuntime.h"
namespace Dream
{

    ScrollerRuntime::ScrollerRuntime
    (
        ScrollerDefinition* definition,
        ActorRuntime* runtime
    ) : DiscreteAssetRuntime(definition,runtime),
        mVelocity(0.0f),
        mRangeBegin(0.0f),
        mRangeEnd(0.0f),
        mUpdateTask(this)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScrollerRuntime");
        auto log = getLog();
        log->trace("Constructing Object");
        #endif
    }

    ScrollerRuntime::~ScrollerRuntime
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destroying Object");
        #endif
    }

    bool
    ScrollerRuntime::useDefinition
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Creating ScrollerRuntime Children for {}", mActorRuntime->getNameAndUuidString());
        #endif
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
    (ActorRuntime* runt)
    {
        runt->replaceAssetUuid(AssetType::MODEL,mAssetsMap[runt->getUuid()]);
    }

    ScrollerRuntime::Range
    ScrollerRuntime::checkRange
    (ActorRuntime* runt)
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

    ActorRuntime*
    ScrollerRuntime::createChlidRuntime
    (const ScrollerItem& item)
    {
        ActorRuntime* newChild = new ActorRuntime(nullptr,mActorRuntime->getSceneRuntime(),true);
        newChild->setParentRuntime(mActorRuntime);
        newChild->setName(mActorRuntime->getName()+"_Scroller_Child_"+std::to_string(item.index));
        newChild->initTransform();
        newChild->getInitialTransform().translate(item.origin);
        newChild->getTransform().translate(item.origin);
        mActorRuntime->addChildRuntime(newChild);
        return newChild;
    }

    void
    ScrollerRuntime::update
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Updating Runtime");
        #endif
        auto time =
            mActorRuntime
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

        mActorRuntime->lock();
        auto children = mActorRuntime->getChildRuntimes();
        #ifdef DREAM_LOG
        log->trace("Child has {} children", children.size());
        #endif
        for (auto* child : children)
        {
            child->lock();
            #ifdef DREAM_LOG
            log->trace("Translating Child with delta vel {},{},{}", delta.x(), delta.y(), delta.z());
            #endif
            child->getTransform().translate(delta);
            child->unlock();
        }
        mActorRuntime->unlock();

        for (auto iter = mPreRange.begin(); iter != mPreRange.end();)
        {
           if (checkRange(*iter) == Range::InRange)
           {
              #ifdef DREAM_LOG
              getLog()->error("New Child in range {}",(*iter)->getNameAndUuidString());
              #endif
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
               #ifdef DREAM_LOG
               getLog()->error("Child has gone out of range {}",(*iter)->getNameAndUuidString());
               #endif
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
        mActorRuntime->lock();
        for (ActorRuntime* runt : mPostRange)
        {
            runt->lock();
            #ifdef DREAM_LOG
            getLog()->error("Garbage! {}",runt->getNameAndUuidString());
            #endif
            mActorRuntime->removeChildRuntime(runt);
            runt->unlock();
        }
        mPostRange.clear();
        mActorRuntime->unlock();
    }

    ScrollerUpdateTask* ScrollerRuntime::getUpdateTask()
    {
        return &mUpdateTask;
    }

}
