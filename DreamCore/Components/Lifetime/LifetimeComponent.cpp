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


#include "LifetimeComponent.h"

#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    LifetimeComponent::LifetimeComponent
    () : Component()
    {
        #ifdef DREAM_LOG
        setLogClassName("LifetimeComponent");
        #endif
    }


    LifetimeComponent::~LifetimeComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Destroying Object");
        #endif
    }


    bool
    LifetimeComponent::init
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Initialisation Done");
        #endif
        return true;
    }

    void
    LifetimeComponent::updateComponent
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
            updateSceneObjectLifetimes(sr);
        }
        endUpdate();
    }

    void
    LifetimeComponent::updateSceneRuntimeTime
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
    LifetimeComponent::updateSceneObjectLifetimes
    (SceneRuntime* sr)
    {
       long timeDelta = mTime->getFrameTimeDelta();
       sr->getRootSceneObjectRuntime()->applyToAll(
            std::function<SceneObjectRuntime*(SceneObjectRuntime*)>([&](SceneObjectRuntime* runt)
            {
                long deferredFor = runt->getDeferredFor();
                if (deferredFor > 0)
                {
                    long deferral = deferredFor-timeDelta;
                    #ifdef DREAM_LOG
                    getLog()->critical("Reducing defferal by {} to {} for {}", timeDelta, deferral, runt->getNameAndUuidString());
                    #endif
                    runt->setDeferredFor(deferral);
                    if (deferral < 0) {
                        #ifdef DREAM_LOG
                        getLog()->critical("Loading Deferred Runtime {}", runt->getNameAndUuidString());
                        #endif
                        runt->loadDeferred();
                    }
                } else
                {
                    runt->increaseLifetime(timeDelta);
                }
                return nullptr;
            }
        ));
    }
}
