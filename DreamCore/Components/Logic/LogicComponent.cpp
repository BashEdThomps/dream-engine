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


#include "LogicComponent.h"

#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Animation/AnimationRuntime.h"
#include "../Path/PathRuntime.h"
#include "../Scroller/ScrollerRuntime.h"

namespace Dream
{
    LogicComponent::LogicComponent
    () : Component()
    {
        #ifdef DREAM_LOG
        setLogClassName("LogicComponent");
        #endif
    }


    LogicComponent::~LogicComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Destroying Object");
        #endif
    }


    bool
    LogicComponent::init
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Initialisation Done");
        #endif
        return true;
    }

    void
    LogicComponent::updateComponent
    (SceneRuntime* sr)
    {
        if (!mEnabled)
        {
            #ifdef DREAM_LOG
            auto log = getLog();
            log->warn("Update Disabled");
            #endif
            return;
        }

        beginUpdate();
        long timeDelta = mTime->getFrameTimeDelta();
        if (timeDelta <= Time::DELTA_MAX)
        {
            updateSceneRuntimeTime(sr);
            updateSceneObjects(sr);
        }
        endUpdate();
    }
    void
    LogicComponent::updateSceneObjects
    (SceneRuntime* sr)
    {
       for (auto* runt : mUpdateQueue)
       {
           if(updateSceneObjectLifetime(runt))
           {
               updateAnimation(runt);
               updatePath(runt);
               updateScroller(runt);
           }
       }
       clearUpdateQueue();
    }


    void
    LogicComponent::updateSceneRuntimeTime
    (SceneRuntime* sr)
    {
       long frameTime = mTime->getCurrentFrameTime();
       if (sr->getSceneStartTime() <= 0)
       {
           sr->setSceneStartTime(frameTime);
       }
       sr->setSceneCurrentTime(frameTime-sr->getSceneStartTime());
    }

    void
    LogicComponent::updateAnimation(SceneObjectRuntime* runt)
    {
        if (runt->hasAnimationRuntime())
        {
            runt->getAnimationRuntime()->update();
        }
    }

    void
    LogicComponent::updateScroller(SceneObjectRuntime* runt)
    {
        if (runt->hasScrollerRuntime())
        {
            runt->getScrollerRuntime()->update();
        }
    }

    void
    LogicComponent::updatePath(SceneObjectRuntime* runt)
    {
        if (runt->hasPathRuntime())
        {
            runt->getPathRuntime()->update();
        }
    }

    bool
    LogicComponent::updateSceneObjectLifetime
    (SceneObjectRuntime* runt)
    {
       long timeDelta = mTime->getFrameTimeDelta();
       long deferredFor = runt->getDeferredFor();
       if (deferredFor > 0)
       {
            long deferral = deferredFor-timeDelta;
            #ifdef DREAM_LOG
            getLog()->trace("Reducing defferal by {} to {} for {}", timeDelta, deferral, runt->getNameAndUuidString());
            #endif
            runt->setDeferredFor(deferral);
            if (deferral < 0)
            {
                #ifdef DREAM_LOG
                getLog()->debug("Loading Deferred Runtime {}", runt->getNameAndUuidString());
                #endif
                runt->loadDeferred();
            }
        }
        else
        {
            runt->increaseLifetime(timeDelta);
            return true;
        }
        return false;
    }
}
