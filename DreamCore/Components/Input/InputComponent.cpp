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
#include "../../Scene/SceneRuntime.h"
#include "../Script/ScriptRuntime.h"

namespace Dream
{
    InputComponent::InputComponent
    (ProjectRuntime* rt, bool useKeyboard, bool useMouse, bool useJoystick)
        : Component (rt),
          mUseKeyboard(useKeyboard),
          mUseMouse(useMouse),
          mUseJoystick(useJoystick),
          mJoystickMapping(JsPsxMapping),
          mPollDataTask(nullptr),
          mExecuteScriptTask(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("InputComponent");
        auto log = getLog();
        log->trace("Constructing");
        #endif
    }

    InputComponent::~InputComponent
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destructing");
        #endif

        if (mPollDataTask)
        {
            mPollDataTask->setExpired(true);
            mPollDataTask = nullptr;
        }

        if (mExecuteScriptTask)
        {
            mExecuteScriptTask->setExpired(true);
            mExecuteScriptTask = nullptr;
        }
    }

    bool
    InputComponent::init
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Initialising...");
        #endif
        return true;
    }

    bool
    InputComponent::executeInputScript
    (SceneRuntime* sRunt)
    {
        auto inputScript = sRunt->getInputScript();
        if (inputScript)
        {
            return inputScript->executeOnInput(this, sRunt);
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
    InputComponent::usingKeyboard
    ()
    const
    {
        return mUseKeyboard;
    }

    bool
    InputComponent::usingMouse
    ()
    const
    {
        return mUseMouse;
    }

    bool
    InputComponent::usingJoystick
    ()
    const
    {
        return mUseJoystick;
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

    float
    InputComponent::clearDeadzone
    (float val)
    {
        if(val > mJoystickState.DeadZone)
        {
            return  val - mJoystickState.DeadZone;
        }
        else if (val < -mJoystickState.DeadZone)
        {
            return val+mJoystickState.DeadZone;
        }
        return 0.0f;
    }

    JoystickMapping InputComponent::getJoystickMapping() const
    {
        return mJoystickMapping;
    }

    void
    InputComponent::setPollDataTask
    (InputPollDataTask* t)
    {
        mPollDataTask = t;
    }

    bool
    InputComponent::hasPollDataTask
    () const
    {
       return mPollDataTask != nullptr;
    }

    void
    InputComponent::setExecuteScriptTask
    (InputExecuteScriptTask* t)
    {
        mExecuteScriptTask = t;
    }

    bool
    InputComponent::hasExecuteScriptTask
    () const
    {
       return mExecuteScriptTask != nullptr;
    }

}
