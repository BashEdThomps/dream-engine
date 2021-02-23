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

#include "InputComponent.h"
#include "InputTasks.h"

#include "Common/Logger.h"
#include "Components/Time.h"
#include "Math/Transform.h"
#include "Components/Script/ScriptRuntime.h"
#include "Components/Graphics/Camera.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;


namespace octronic::dream
{
    InputComponent::InputComponent
    (ProjectRuntime* rt)
        : Component (rt),
          mCurrentSceneRuntime(nullptr),
          mJoystickMapping(JsPsxMapping),
          mJoystickNavigation(nullptr),
          mPollDataTask(nullptr),
          mExecuteScriptTask(nullptr),
          mJoystickCount(0)
    {
        LOG_TRACE("InputComponent: Constructing");
        //mJoystickNavigation = make_shared<Joystick2DPlaneNavigation>(&mJoystickState,&mJoystickMapping);
        mJoystickNavigation = make_shared<JoystickFaceForwardNavigation>(&mJoystickState,&mJoystickMapping);
    }

    InputComponent::~InputComponent
    ()
    {
        LOG_TRACE("InputComponent: Destructing");
    }

    bool
    InputComponent::init
    ()
    {
        LOG_DEBUG("InputComponent: Initialising...");
        return true;
    }

    bool
    InputComponent::executeInputScript
    ()
    {
        if (mCurrentSceneRuntime)
        {
            mJoystickNavigation->update(mCurrentSceneRuntime);
            auto inputScript = mCurrentSceneRuntime->getInputScript();
            if (inputScript)
            {
                return inputScript->executeOnInput(this, mCurrentSceneRuntime);
            }
        }
        return true;
    }

    void
    InputComponent::pollData
    ()
    {
        mLastKeyboardState = mKeyboardState;
        mLastMouseState = mMouseState;
        mLastJoystickState = mJoystickState;
    }

    bool
    InputComponent::isKeyDown
    (int key)
    {
        return mKeyboardState.KeysDown[key];
    }

    KeyboardState&
    InputComponent::getKeyboardState
    ()
    {
        return mKeyboardState;
    }

    void
    InputComponent::setKeyboardState
    (const KeyboardState& keyboardState)
    {
        mKeyboardState = keyboardState;
    }

    MouseState&
    InputComponent::getMouseState
    ()
    {
        return mMouseState;
    }

    void
    InputComponent::setMouseState
    (const MouseState& mouseState)
    {
        mMouseState = mouseState;
    }

    JoystickState&
    InputComponent::getJoystickState
    ()
    {
        return mJoystickState;
    }

    void
    InputComponent::setJoystickState
    (const JoystickState& joystickState)
    {
        mJoystickState = joystickState;
    }

    float
    InputComponent::mouseDeltaX
    ()
    {
        return mMouseState.PosX - mLastMouseState.PosX;
    }

    float
    InputComponent::mouseDeltaY
    ()
    {
        return mMouseState.PosY - mLastMouseState.PosY;
    }

    JoystickMapping&
    InputComponent::getJoystickMapping
    ()
    {
        return mJoystickMapping;
    }

    shared_ptr<InputPollDataTask>
    InputComponent::getPollDataTask
    ()
    {
        return mPollDataTask;
    }

    shared_ptr<InputExecuteScriptTask>
    InputComponent::getExecuteScriptTask
    ()
    {
        return mExecuteScriptTask;
    }

    SceneRuntime*
    InputComponent::getCurrentSceneRuntime
    () const
    {
        return mCurrentSceneRuntime;
    }

    void
    InputComponent::setCurrentSceneRuntime
    (SceneRuntime *currentSceneRuntime)
    {
        mCurrentSceneRuntime = currentSceneRuntime;
    }

    JoystickNavigation*
    InputComponent::getJoystickNavigation
    ()
    {
        return mJoystickNavigation.get();
    }

    int
    InputComponent::getJoystickCount
    ()
    const
    {
        return mJoystickCount;
    }

    void
    InputComponent::setJoystickCount
    (int joystickCount)
    {
        mJoystickCount = joystickCount;
    }

    void
    InputComponent::pushTasks()
    {
        /*
        // Input component needs to be constructed
        if (mInputScript)
        {
            mInputScript->pushTasks();

			inputComponent->setCurrentSceneRuntime(this);

            // Process Input
            InputExecuteScriptTask* inputExecuteTask = inputComponent->getExecuteScriptTask();
			inputExecuteTask->dependsOn(inputPollDataTask);
			taskManager->pushTasks();
        }
        */
    }
}
