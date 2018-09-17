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
        log->info("Initialising...");

        mInputMap = new InputMap(mInputManager);

        if (mUseKeyboard)
        {
            log->info("Creating Keyboard Device");
            auto keyboardDevice = mInputManager.CreateDevice<InputDeviceKeyboard>();
            mDevices.push_back(keyboardDevice);
            mInputMap->MapBool(Key_UP,keyboardDevice,KeyUp);
            mInputMap->MapBool(Key_DOWN,keyboardDevice,KeyDown);
            mInputMap->MapBool(Key_LEFT,keyboardDevice,KeyLeft);
            mInputMap->MapBool(Key_RIGHT,keyboardDevice,KeyRight);
            mInputMap->MapBool(Key_RETURN,keyboardDevice,KeyReturn);
            mInputMap->MapBool(Key_SPACE,keyboardDevice,KeySpace);
        }

        if (mUseMouse)
        {
            log->info("Creating Mouse Device");
            auto mouseDevice = mInputManager.CreateDevice<InputDeviceMouse>();
            mDevices.push_back(mouseDevice);
        }

        if (mUseJoystick)
        {
            log->info("Creating Joystick Device");
            auto gamepad = mInputManager.CreateDevice<InputDevicePad>();
            mDevices.push_back(gamepad);

            // Face Buttons
            mInputMap->MapBool(FaceButtonSouth,gamepad,PadButtonA);
            mInputMap->MapBool(FaceButtonEast,gamepad,PadButtonB);
            mInputMap->MapBool(FaceButtonWest,gamepad,PadButtonX);
            mInputMap->MapBool(FaceButtonNorth,gamepad,PadButtonY);

            // Left Analog
            mInputMap->MapFloat(AnalogLeftStickX,gamepad,PadButtonLeftStickX);
            mInputMap->MapFloat(AnalogLeftStickY,gamepad,PadButtonLeftStickY);
            mInputMap->MapBool(AnalogLeftButton,gamepad,PadButtonL3);

            // Right Analog
            mInputMap->MapFloat(AnalogRightStickX,gamepad,PadButtonRightStickX);
            mInputMap->MapFloat(AnalogRightStickY,gamepad,PadButtonRightStickY);
            mInputMap->MapBool(AnalogRightButton,gamepad,PadButtonR3);

            // Shoulders
            mInputMap->MapBool(ShoulderLeft,gamepad,PadButtonL1);
            mInputMap->MapBool(ShoulderRight,gamepad,PadButtonR1);

            // Triggers
            mInputMap->MapFloat(TriggerLeft,gamepad,PadButtonAxis4);
            mInputMap->MapFloat(TriggerRight,gamepad,PadButtonAxis5);

            // DPad
            mInputMap->MapBool(DPadNorth,gamepad,PadButtonUp);
            mInputMap->MapBool(DPadSouth,gamepad,PadButtonDown);
            mInputMap->MapBool(DPadEast,gamepad,PadButtonRight);
            mInputMap->MapBool(DPadWest,gamepad,PadButtonLeft);

            // Extra
            mInputMap->MapBool(FaceHome,gamepad,PadButtonHome);
            mInputMap->MapBool(FaceStart,gamepad,PadButtonStart);
            mInputMap->MapBool(FaceSelect,gamepad,PadButtonSelect);
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

             mInputMap->GetBool(FaceButtonNorth),
             mInputMap->GetBool(FaceButtonEast),
             mInputMap->GetBool(FaceButtonWest),
             mInputMap->GetBool(FaceButtonSouth),

             mInputMap->GetBool(FaceHome),
             mInputMap->GetBool(FaceStart),
             mInputMap->GetBool(FaceSelect),

             mInputMap->GetBool(ShoulderLeft),
             mInputMap->GetBool(ShoulderRight),

             mInputMap->GetFloat(TriggerLeft),
             mInputMap->GetFloat(TriggerRight),

             mInputMap->GetBool(DPadNorth),
             mInputMap->GetBool(DPadSouth),
             mInputMap->GetBool(DPadEast),
             mInputMap->GetBool(DPadWest),

             mInputMap->GetFloat(AnalogLeftStickX),
             mInputMap->GetFloat(AnalogLeftStickY),
             mInputMap->GetBool(AnalogLeftButton),

             mInputMap->GetFloat(AnalogRightStickX),
             mInputMap->GetFloat(AnalogRightStickY),
             mInputMap->GetBool(AnalogRightButton)
        );
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

            mInputMap->GetBool(Key_UP),
            mInputMap->GetBool(Key_DOWN),
            mInputMap->GetBool(Key_LEFT),
            mInputMap->GetBool(Key_RIGHT),

            mInputMap->GetBool(Key_SPACE),
            mInputMap->GetBool(Key_RETURN)
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
