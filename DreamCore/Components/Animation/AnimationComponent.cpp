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


#include "AnimationComponent.h"

#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    AnimationComponent::AnimationComponent
    ()
        : Component()

    {
        #ifdef DREAM_LOG
        setLogClassName("AnimationComponent");
        #endif
    }


    AnimationComponent::~AnimationComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Destroying Object");
        #endif
    }


    bool
    AnimationComponent::init
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Initialisation Done");
        #endif
        return true;
    }


    void
    AnimationComponent::updateComponent
    (SceneRuntime* sr)
    {
        if (!mEnabled)
        {
            #ifdef DREAM_LOG
            getLog()->warn("Update Disabled");
            #endif
            return;
        }

        beginUpdate();
        if (sr != nullptr)
        {
            sr->getRootSceneObjectRuntime()->applyToAll
            (
                function<SceneObjectRuntime*(SceneObjectRuntime*) >
                (
                    [&](SceneObjectRuntime* currentSceneObject)
                    {
                        if (currentSceneObject->hasAnimationRuntime())
                        {
                            auto anim = currentSceneObject->getAnimationRuntime();
                            anim->stepAnimation(mTime->getFrameTimeDelta());
                        }
                        return nullptr;
                    }
                )
            );
        }
        endUpdate();
    }
}
