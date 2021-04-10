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
