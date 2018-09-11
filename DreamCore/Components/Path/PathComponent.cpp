/*
* PathComponent
*
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


#include "PathComponent.h"

#include "../../Common/Constants.h"
#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Utilities/String.h"

namespace Dream
{
    PathComponent::PathComponent
    ()
        : IComponent()

    {
        setLogClassName("PathComponent");
    }


    PathComponent::~PathComponent
    ()
    {
        getLog()->info("Destroying Object");
    }


    bool
    PathComponent::init
    ()
    {
        getLog()->debug("Initialisation Done");
        return true;
    }


    void
    PathComponent::updateComponent
    ()
    {
                beginUpdate();
                auto activeSceneRuntime = mActiveSceneRuntime.lock();
                if (activeSceneRuntime != nullptr)
                {
                    activeSceneRuntime->getRootSceneObjectRuntime()->applyToAll
                    (
                        function< shared_ptr<SceneObjectRuntime> (shared_ptr<SceneObjectRuntime>) >
                        (
                            [&](shared_ptr<SceneObjectRuntime> currentSceneObject)
                            {
                                if (currentSceneObject->hasPathInstance())
                                {
                                    shared_ptr<PathInstance> animInstance = currentSceneObject->getPathInstance();
                                    // TODO: Fix dis
                                }
                                return nullptr;
                            }
                        )
                    );
                }
                endUpdate();
    }
} // End of Dream
