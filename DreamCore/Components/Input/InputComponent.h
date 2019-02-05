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

#include "../Component.h"
#include "InputMapping.h"
#include "InputState.h"

using namespace std;

namespace Dream
{
    class InputPollDataTask;
    class InputExecuteScriptTask;

    class InputComponent : public Component
    {
    public:
        InputComponent(ProjectRuntime* rt, bool useKeyboard = false, bool useMouse = false, bool useJoystick = false);
        ~InputComponent() override;

        bool init() override;
        bool executeInputScript(SceneRuntime* sr);
        void pollData();

        void debugKeyboard() const;
        void debugMouse() const;
        void debugState() const;

        bool usingKeyboard() const;
        bool usingMouse() const;
        bool usingJoystick() const;

        bool isKeyDown(int key);

        KeyboardState& getKeyboardState();
        void setKeyboardState(const KeyboardState& keyboardState);

        MouseState& getMouseState();
        void setMouseState(const MouseState& mouseState);

        JoystickState& getJoystickState();
        void setJoystickState(const JoystickState& joystickState);

        int getInputModeFlags() const;
        void setInputModeFlags(const int inputMode);

        float outsizeDeadzone(int axis);
        float mouseDeltaX();
        float mouseDeltaY();

        float clearDeadzone(float val);

        JoystickMapping getJoystickMapping() const;
        void setPollDataTask(InputPollDataTask* t);
        bool hasPollDataTask() const;
        void setExecuteScriptTask(InputExecuteScriptTask* t);
        bool hasExecuteScriptTask() const;


    private:
        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;

        KeyboardState mKeyboardState;
        KeyboardState mLastKeyboardState;

        MouseState mMouseState;
        MouseState mLastMouseState;

        JoystickState mJoystickState;
        JoystickState mLastJoystickState;
        JoystickMapping mJoystickMapping;
        InputPollDataTask* mPollDataTask;
        InputExecuteScriptTask* mExecuteScriptTask;

    };
}
