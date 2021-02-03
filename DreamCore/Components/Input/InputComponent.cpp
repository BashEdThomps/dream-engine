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
using std::unique_lock;

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("InputComponent: Constructing");
        //mJoystickNavigation = make_shared<Joystick2DPlaneNavigation>(&mJoystickState,&mJoystickMapping);
        mJoystickNavigation = make_shared<JoystickFaceForwardNavigation>(&mJoystickState,&mJoystickMapping);
    }

    InputComponent::~InputComponent
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("InputComponent: Destructing");
    }

    bool
    InputComponent::init
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG("InputComponent: Initialising...");
        return true;
    }

    bool
    InputComponent::executeInputScript
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mLastKeyboardState = mKeyboardState;
        mLastMouseState = mMouseState;
        mLastJoystickState = mJoystickState;
    }

    bool
    InputComponent::isKeyDown
    (int key)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mKeyboardState.KeysDown[key];
    }

    KeyboardState&
    InputComponent::getKeyboardState
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mKeyboardState;
    }

    void
    InputComponent::setKeyboardState
    (const KeyboardState& keyboardState)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mKeyboardState = keyboardState;
    }

    MouseState&
    InputComponent::getMouseState
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mMouseState;
    }

    void
    InputComponent::setMouseState
    (const MouseState& mouseState)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mMouseState = mouseState;
    }

    JoystickState&
    InputComponent::getJoystickState
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mJoystickState;
    }

    void
    InputComponent::setJoystickState
    (const JoystickState& joystickState)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJoystickState = joystickState;
    }

    float
    InputComponent::mouseDeltaX
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mMouseState.PosX - mLastMouseState.PosX;
    }

    float
    InputComponent::mouseDeltaY
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mMouseState.PosY - mLastMouseState.PosY;
    }

    JoystickMapping&
    InputComponent::getJoystickMapping
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mJoystickMapping;
    }

    InputPollDataTask*
    InputComponent::getPollDataTask
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return &mPollDataTask;
    }

    InputExecuteScriptTask*
    InputComponent::getExecuteScriptTask
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return &mExecuteScriptTask;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mCurrentSceneRuntime = currentSceneRuntime;
    }

    JoystickNavigation*
    InputComponent::getJoystickNavigation
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJoystickCount = joystickCount;
    }
}
