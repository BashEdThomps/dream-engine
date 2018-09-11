#include "InputComponent.h"
#include "../Scripting/IScriptComponent.h"

namespace Dream
{
    InputComponent::InputComponent()
        : IComponent ()
    {
        setLogClassName("InputComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    InputComponent::~InputComponent()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    bool InputComponent::init()
    {
        auto log = getLog();
        log->info("Initialising...");
        auto gamepad = mInputManager.CreateDevice<InputDevicePad>();

        mDevices.push_back(gamepad);
        mInputMap = make_shared<InputMap>(mInputManager);

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

        return true;
    }

    void InputComponent::updateComponent()
    {
        auto log = getLog();
        beginUpdate();
        mInputManager.Update();
        /*
        log->warn(
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
        */

        endUpdate();
    }

    shared_ptr<InputMap> InputComponent::getInputMap() const
    {
        return mInputMap;
    }

    const float InputComponent::ANALOG_DEAD_ZONE = 0.1f;
}
