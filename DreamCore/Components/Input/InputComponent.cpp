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
#include "Components/Transform.h"
#include "Components/Script/ScriptRuntime.h"
#include "Components/Graphics/Camera.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;


namespace octronic::dream
{
    InputComponent::InputComponent
    (ProjectRuntime* rt)
        : Component ("InputComponent",rt),
          mCurrentSceneRuntime(nullptr),
          mJoystickMapping(JsPsxMapping),
          mJoystickNavigation(nullptr),
          mPollDataTask(this),
          mExecuteScriptTask(this),
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
        if(dreamTryLock()) {
            dreamLock();
            LOG_DEBUG("InputComponent: Initialising...");
            return true;
        } dreamElseLockFailed
    }

    bool
    InputComponent::executeInputScript
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    InputComponent::pollData
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            mLastKeyboardState = mKeyboardState;
            mLastMouseState = mMouseState;
            mLastJoystickState = mJoystickState;
        } dreamElseLockFailed
    }

    bool
    InputComponent::isKeyDown
    (int key)
    {
        if(dreamTryLock()) {
            dreamLock();
            return mKeyboardState.KeysDown[key];
        } dreamElseLockFailed
    }

    KeyboardState&
    InputComponent::getKeyboardState
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mKeyboardState;
        } dreamElseLockFailed
    }

    void
    InputComponent::setKeyboardState
    (const KeyboardState& keyboardState)
    {
        if(dreamTryLock()) {
            dreamLock();
            mKeyboardState = keyboardState;
        } dreamElseLockFailed
    }

    MouseState&
    InputComponent::getMouseState
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mMouseState;
        } dreamElseLockFailed
    }

    void
    InputComponent::setMouseState
    (const MouseState& mouseState)
    {
        if(dreamTryLock()) {
            dreamLock();
            mMouseState = mouseState;
        } dreamElseLockFailed
    }

    JoystickState&
    InputComponent::getJoystickState
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mJoystickState;
        } dreamElseLockFailed
    }

    void
    InputComponent::setJoystickState
    (const JoystickState& joystickState)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJoystickState = joystickState;
        } dreamElseLockFailed
    }

    float
    InputComponent::mouseDeltaX
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mMouseState.PosX - mLastMouseState.PosX;
        } dreamElseLockFailed
    }

    float
    InputComponent::mouseDeltaY
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mMouseState.PosY - mLastMouseState.PosY;
        } dreamElseLockFailed
    }

    JoystickMapping&
    InputComponent::getJoystickMapping
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mJoystickMapping;
        } dreamElseLockFailed
    }

    InputPollDataTask*
    InputComponent::getPollDataTask
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mPollDataTask;
        } dreamElseLockFailed
    }

    InputExecuteScriptTask*
    InputComponent::getExecuteScriptTask
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mExecuteScriptTask;
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mCurrentSceneRuntime = currentSceneRuntime;
        } dreamElseLockFailed
    }

    JoystickNavigation*
    InputComponent::getJoystickNavigation
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mJoystickNavigation.get();
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mJoystickCount = joystickCount;
        } dreamElseLockFailed
    }
}
