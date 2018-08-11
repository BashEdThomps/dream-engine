/*
* AnimationComponent
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


#include "AnimationComponent.h"

#include "../../Common/Constants.h"
#include "../../Components/Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Utilities/String.h"

namespace Dream
{
    AnimationComponent::AnimationComponent
    ()
        : IComponent()

    {
        setLogClassName("AnimationComponent");
    }


    AnimationComponent::~AnimationComponent
    ()
    {
        getLog()->info("Destroying Object");
    }


    bool
    AnimationComponent::init
    ()
    {
        getLog()->debug("Initialisation Done");
        return true;
    }


    void
    AnimationComponent::updateComponent
    ()
    {
        while(mRunning)
        {
            if (mShouldUpdate && mActiveSceneRuntimeHandle != nullptr)
            {
                beginUpdate();
                mActiveSceneRuntimeHandle->getRootSceneObjectRuntimeHandle()->applyToAll
                        (
                            function< void* (SceneObjectRuntime*) >
                            (
                                [&](SceneObjectRuntime* currentSceneObject)
                {
                                if (currentSceneObject->hasAnimationInstance())
                                {
                                    AnimationInstance* animInstance = currentSceneObject->getAnimationInstance();
                                    animInstance->step(mTime->getFrameTimeDelta());
                                    // TODO: Fix dis
                                    //animInstance->applyTransform(&(currentSceneObject->getTransform()));
                                }
                                return nullptr;
                            }
                            )
                        );
                endUpdate();
            }
            std::this_thread::yield();
        }
    }


    void
    AnimationComponent::play
    (AnimationInstance *asset)
    {
        try
        {
            AnimationInstance* animAsset;
            animAsset = dynamic_cast<AnimationInstance*>(asset);
            animAsset->play();
        }
        catch (exception & ex)
        {
            getLog()->error("Exception playing animation.", ex.what());
        }
    }


    void
    AnimationComponent::pause
    (AnimationInstance *asset)
    {
        try
        {
            AnimationInstance* animAsset;
            animAsset = dynamic_cast<AnimationInstance*>(asset);
            animAsset->pause();
        }
        catch (exception & ex)
        {
            getLog()->error("Exception pausing animation.", ex.what());
        }
    }


    void
    AnimationComponent::stop
    (AnimationInstance *asset)
    {
        try
        {
            AnimationInstance* animAsset;
            animAsset = dynamic_cast<AnimationInstance*>(asset);
            animAsset->stop();
        }
        catch (exception & ex)
        {
            getLog()->error("Exception stopping animation." , ex.what());
        }
    }


    bool
    AnimationComponent::isLooping
    (AnimationInstance* asset)
    {
        auto log = getLog();
        if (!asset)
        {
            log->error("Asset is null in isLooping... ");
            return false;
        }

        try
        {
            AnimationInstance* animationAsset;
            animationAsset = dynamic_cast<AnimationInstance*>(asset);
            if (!animationAsset)
            {
                log->error("AnimationAsset is null in isLooping... ");
                return false;
            }
            return animationAsset->isLooping();
        }
        catch (exception &ex)
        {
            log->error("Exception in isLooping... ", ex.what());
        }
        return false;
    }


    void
    AnimationComponent::setLooping
    (AnimationInstance* asset, bool looping)
    {
        auto log = getLog();
        try
        {
            AnimationInstance* animationAsset;
            animationAsset = dynamic_cast<AnimationInstance*>(asset);
            animationAsset->setLooping(looping);
            log->debug(
                        "Setting", animationAsset->getNameAndUuidString(),
                        "looping: ", String::boolToYesNo(looping)
                        );
        }
        catch (exception &ex)
        {
            log->error("Exception in setLooping... ", ex.what());
        }
    }
} // End of Dream
