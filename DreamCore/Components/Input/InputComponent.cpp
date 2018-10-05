#include "InputComponent.h"
#include "../Scripting/IScriptComponent.h"

namespace Dream
{
    InputComponent::InputComponent(bool useKeyboard, bool useMouse, bool useJoystick)
        : IComponent (),
          mUseKeyboard(useKeyboard),
          mUseMouse(useMouse),
          mUseJoystick(useJoystick)
    {
        setLogClassName("InputComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    InputComponent::~InputComponent()
    {
        auto log = getLog();
        log->trace("Destructing");
        if (mInputMap == nullptr)
        {
            delete mInputMap;
            mInputMap = nullptr;
        }
    }

    bool InputComponent::init()
    {
        auto log = getLog();
        log->debug("Initialising...");

        mInputMap = new InputMap(mInputManager);

        if (mUseKeyboard)
        {
            log->debug("Creating Keyboard Device");
            auto keyboardDevice = mInputManager.CreateDevice<InputDeviceKeyboard>();
            mDevices.push_back(keyboardDevice);
            mInputMap->MapBool(KB_UP,keyboardDevice,KeyUp);
            mInputMap->MapBool(KB_DOWN,keyboardDevice,KeyDown);
            mInputMap->MapBool(KB_LEFT,keyboardDevice,KeyLeft);
            mInputMap->MapBool(KB_RIGHT,keyboardDevice,KeyRight);
            mInputMap->MapBool(KB_RETURN,keyboardDevice,KeyReturn);
            mInputMap->MapBool(KB_SPACE,keyboardDevice,KeySpace);
        }

        if (mUseMouse)
        {
            log->debug("Creating Mouse Device");
            auto mouseDevice = mInputManager.CreateDevice<InputDeviceMouse>();
            mDevices.push_back(mouseDevice);
        }

        if (mUseJoystick)
        {
            log->debug("Creating Joystick Device");
            auto gamepad = mInputManager.CreateDevice<InputDevicePad>();
            mDevices.push_back(gamepad);

            // Face Buttons
            mInputMap->MapBool(JS_FaceButtonSouth,gamepad,PadButtonA);
            mInputMap->MapBool(JS_FaceButtonEast,gamepad,PadButtonB);
            mInputMap->MapBool(JS_FaceButtonWest,gamepad,PadButtonX);
            mInputMap->MapBool(JS_FaceButtonNorth,gamepad,PadButtonY);

            // Left Analog
            mInputMap->MapFloat(JS_AnalogLeftStickX,gamepad,PadButtonLeftStickX);
            mInputMap->MapFloat(JS_AnalogLeftStickY,gamepad,PadButtonLeftStickY);
            mInputMap->MapBool (JS_AnalogLeftButton,gamepad,PadButtonL3);

            // Right Analog
            mInputMap->MapFloat(JS_AnalogRightStickX,gamepad,PadButtonRightStickX);
            mInputMap->MapFloat(JS_AnalogRightStickY,gamepad,PadButtonRightStickY);
            mInputMap->MapBool (JS_AnalogRightButton,gamepad,PadButtonR3);

            // Shoulders
            mInputMap->MapBool(JS_ShoulderLeft,gamepad,PadButtonL1);
            mInputMap->MapBool(JS_ShoulderRight,gamepad,PadButtonR1);

            // Triggers
            mInputMap->MapFloat(JS_TriggerLeft,gamepad,PadButtonAxis4);
            mInputMap->MapFloat(JS_TriggerRight,gamepad,PadButtonAxis5);

            // DPad
            mInputMap->MapBool(JS_DPadNorth,gamepad,PadButtonUp);
            mInputMap->MapBool(JS_DPadSouth,gamepad,PadButtonDown);
            mInputMap->MapBool(JS_DPadEast,gamepad,PadButtonRight);
            mInputMap->MapBool(JS_DPadWest,gamepad,PadButtonLeft);

            // Extra
            mInputMap->MapBool(JS_FaceHome,gamepad,PadButtonHome);
            mInputMap->MapBool(JS_FaceStart,gamepad,PadButtonStart);
            mInputMap->MapBool(JS_FaceSelect,gamepad,PadButtonSelect);
        }

        return true;
    }

    void InputComponent::updateComponent()
    {
        auto log = getLog();
        beginUpdate();
        if (mInputMap == nullptr)
        {
            log->error("Cannot update with null input map");
            endUpdate();
            return;
        }
        mInputManager.Update();

        debugKeyboard();

        endUpdate();
    }

    void
    InputComponent::debugGamepad
    ()
    const
    {
        if (!mUseKeyboard)
        {
            return;
        }

        auto log = getLog();
        log->trace(
            "Gamepad State\n"

            "FaceButtonNorth.....{}\n"
            "FaceButtonEast......{}\n"
            "FaceButtonWest......{}\n"
            "FaceButtonSouth.....{}\n"

            "FaceHome............{}\n"
            "FaceStart...........{}\n"
            "FaceSelect..........{}\n"

            "ShoulderLeft........{}\n"
            "ShoulderRight.......{}\n"

            "TriggerLeft.........{}\n"
            "TriggerRight........{}\n"

            "DPadNorth...........{}\n"
            "DPadSouth...........{}\n"
            "DPadEast............{}\n"
            "DPadWest............{}\n"

            "AnalogLeftStickX....{}\n"
            "AnalogLeftStickY....{}\n"
            "AnalogLeftButton....{}\n"

            "AnalogRightStickX...{}\n"
            "AnalogRightStickY...{}\n"
            "AnalogRightButton...{}\n",

             mInputMap->GetBool (JS_FaceButtonNorth),
             mInputMap->GetBool (JS_FaceButtonEast),
             mInputMap->GetBool (JS_FaceButtonWest),
             mInputMap->GetBool (JS_FaceButtonSouth),

             mInputMap->GetBool (JS_FaceHome),
             mInputMap->GetBool (JS_FaceStart),
             mInputMap->GetBool (JS_FaceSelect),

             mInputMap->GetBool (JS_ShoulderLeft),
             mInputMap->GetBool (JS_ShoulderRight),

             mInputMap->GetFloat(JS_TriggerLeft),
             mInputMap->GetFloat(JS_TriggerRight),

             mInputMap->GetBool (JS_DPadNorth),
             mInputMap->GetBool (JS_DPadSouth),
             mInputMap->GetBool (JS_DPadEast),
             mInputMap->GetBool (JS_DPadWest),

             mInputMap->GetFloat(JS_AnalogLeftStickX),
             mInputMap->GetFloat(JS_AnalogLeftStickY),
             mInputMap->GetBool (JS_AnalogLeftButton),

             mInputMap->GetFloat(JS_AnalogRightStickX),
             mInputMap->GetFloat(JS_AnalogRightStickY),
             mInputMap->GetBool (JS_AnalogRightButton)
                    );
    }

    bool InputComponent::usingKeyboard() const
    {
        return mUseKeyboard;
    }

    bool InputComponent::usingMouse() const
    {
        return mUseMouse;
    }

    bool InputComponent::usingJoystick() const
    {
       return mUseJoystick;
    }

    void
    InputComponent::debugKeyboard
    ()
    const
    {
        if (!mUseKeyboard)
        {
            return;
        }

        auto log = getLog();
        log->trace
        (
            "Keyboard State\n"
            "Up....................{}\n"
            "Down..................{}\n"
            "Left..................{}\n"
            "Right.................{}\n"

            "Space.................{}\n"
            "Return................{}\n",

            mInputMap->GetBool(KB_UP),
            mInputMap->GetBool(KB_DOWN),
            mInputMap->GetBool(KB_LEFT),
            mInputMap->GetBool(KB_RIGHT),

            mInputMap->GetBool(KB_SPACE),
            mInputMap->GetBool(KB_RETURN)
        );

    }

    void
    InputComponent::debugMouse
    ()
    const
    {
        if (!mUseMouse)
        {
            return;
        }
    }

    InputMap* InputComponent::getInputMap() const
    {
        return mInputMap;
    }

    const float InputComponent::ANALOG_DEAD_ZONE = 0.1f;
}
