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

#include "Components/Component.h"

#include "MouseState.h"
#include "KeyboardState.h"
#include "InputTasks.h"
#include "JoystickNavigation.h"
#include "JoystickMapping.h"
#include "JoystickState.h"

#include <memory>

using std::shared_ptr;

namespace octronic::dream
{
    class ScriptRuntime;
    class InputComponent : public Component
    {
    public:
        InputComponent(ProjectRuntime& rt);
        InputComponent(InputComponent&&) = default;
        InputComponent& operator=(InputComponent&&) = default;

        bool init() override;

        // Keyboard ============================================================
        KeyboardState& getKeyboardState();
        void setKeyboardState(const KeyboardState& keyboardState);
        // Mouse ===============================================================
        MouseState& getMouseState();
        void setMouseState(const MouseState& mouseState);
        // Joystick ============================================================
        JoystickMapping& getJoystickMapping();
        JoystickNavigation* getJoystickNavigation();
        int getJoystickCount() const;
        void setJoystickCount(int joystickCount);
        JoystickState& getJoystickState();
        void setJoystickState(const JoystickState& joystickState);
        // Scripting ===========================================================
        bool registerInputScript();
        bool executeInputScript();
        bool removeInputScript(UuidType id);
        // Tasks ===============================================================
        shared_ptr<InputRegisterScriptTask> getRegisterScriptTask() const;
        shared_ptr<InputRemoveScriptTask> getRemoveScriptTask() const;
        void pollData();
        void pushTasks() override;
    private:
        // Keyboard
        KeyboardState mKeyboardState;
        // Mouse
        MouseState mMouseState;
        // Joystick
        int mJoystickCount;
        JoystickState mJoystickState;
        JoystickMapping mJoystickMapping;
        shared_ptr<JoystickNavigation> mJoystickNavigation;
        // Tasks
        shared_ptr<InputPollDataTask> mPollDataTask;
        shared_ptr<InputRegisterScriptTask> mRegisterScriptTask;
        shared_ptr<InputExecuteScriptTask> mExecuteScriptTask;
        shared_ptr<InputRemoveScriptTask> mRemoveScriptTask;
    };
}
