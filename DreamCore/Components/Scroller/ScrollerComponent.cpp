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


#include "ScrollerComponent.h"

#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "ScrollerRuntime.h"

namespace Dream
{
    ScrollerComponent::ScrollerComponent
    ()
        : Component()
    {
#ifdef DREAM_LOG
        setLogClassName("ScrollerComponent");
#endif
    }


    ScrollerComponent::~ScrollerComponent
    ()
    {
#ifdef DREAM_LOG
        getLog()->debug("Destroying Object");
#endif
    }


    bool
    ScrollerComponent::init
    ()
    {
#ifdef DREAM_LOG
        getLog()->debug("Initialisation Done");
#endif
        return true;
    }


    void
    ScrollerComponent::updateComponent
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
        if (sr != nullptr)
        {
            sr->getRootSceneObjectRuntime()->applyToAll
            (
                function< SceneObjectRuntime* (SceneObjectRuntime*) >
                (
                    [&](SceneObjectRuntime* currentSceneObject)
                    {
                        if (currentSceneObject->hasScrollerRuntime())
                        {
                                ScrollerRuntime* scroller = currentSceneObject->getScrollerRuntime();
                                scroller->update(sr);
                        }
                        return nullptr;
                    }
                )
            );
        }
        endUpdate();
    }
} // End of Dream
