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


#pragma once

#include <memory>
#include "Components/Component.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Joystick.h"
#include "InputTasks.h"

using std::shared_ptr;

namespace octronic::dream
{
    class InputComponent : public Component
    {
    public:
        InputComponent(ProjectRuntime* rt);
        ~InputComponent() override;

        bool init() override;
        bool executeInputScript();
        void pollData();

        void debugKeyboard() const;
        void debugMouse() const;
        void debugState() const;

        bool isKeyDown(int key);

        KeyboardState& getKeyboardState();
        void setKeyboardState(const KeyboardState& keyboardState);

        MouseState& getMouseState();
        void setMouseState(const MouseState& mouseState);

        JoystickMapping& getJoystickMapping();
        JoystickNavigation* getJoystickNavigation();
        JoystickState& getJoystickState();
        void setJoystickState(const JoystickState& joystickState);

        float mouseDeltaX();
        float mouseDeltaY();

        shared_ptr<InputPollDataTask> getPollDataTask();
        shared_ptr<InputExecuteScriptTask> getExecuteScriptTask();

        SceneRuntime *getCurrentSceneRuntime() const;
        void setCurrentSceneRuntime(SceneRuntime *currentSceneRuntime);
        int getJoystickCount() const;
        void setJoystickCount(int joystickCount);

        void pushTasks() override;

    private:
        SceneRuntime* mCurrentSceneRuntime;

        KeyboardState mKeyboardState;
        KeyboardState mLastKeyboardState;

        MouseState mMouseState;
        MouseState mLastMouseState;

        int mJoystickCount;
        JoystickState mJoystickState;
        JoystickState mLastJoystickState;
        JoystickMapping mJoystickMapping;
        shared_ptr<InputPollDataTask> mPollDataTask;
        shared_ptr<InputExecuteScriptTask> mExecuteScriptTask;
        shared_ptr<JoystickNavigation> mJoystickNavigation;
    };
}
