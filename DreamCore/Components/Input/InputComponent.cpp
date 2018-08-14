#include "InputComponent.h"

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
        mRunning = true;
        auto gamepad = mInputManager.CreateDevice<InputDevicePad>();
        mDevices.push_back(gamepad);
        mInputMap = unique_ptr<InputMap>(new InputMap(mInputManager));

        // Face Buttons
        mInputMap->MapBool(FaceButtonSouth,gamepad,PadButtonA);
        mInputMap->MapBool(FaceButtonEast,gamepad,PadButtonB);
        mInputMap->MapBool(FaceButtonWest,gamepad,PadButtonX);
        mInputMap->MapBool(FaceButtonNorth,gamepad,PadButtonY);

        // Left Analog
        mInputMap->MapFloat(AnalogLeftStickX,gamepad,PadButtonLeftStickX);
        mInputMap->MapFloat(AnalogLeftStickX,gamepad,PadButtonLeftStickX);
        mInputMap->MapBool(AnalogLeftButton,gamepad,PadButtonL3);

        // Right Analog
        mInputMap->MapFloat(AnalogRightStickX,gamepad,PadButtonRightStickX);
        mInputMap->MapFloat(AnalogRightStickX,gamepad,PadButtonRightStickX);
        mInputMap->MapBool(AnalogRightButton,gamepad,PadButtonR3);

        // Shoulders
        mInputMap->MapBool(ShoulderLeft,gamepad,PadButtonL1);
        mInputMap->MapFloat(TriggerLeft,gamepad,PadButtonL2);

        mInputMap->MapBool(ShoulderRight,gamepad,PadButtonR1);
        mInputMap->MapFloat(TriggerRight,gamepad,PadButtonR2);

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
        while(mRunning)
        {
            if (mShouldUpdate && mActiveSceneRuntimeHandle != nullptr)
            {
                beginUpdate();
                mInputManager.Update();
                endUpdate();
            }
            std::this_thread::yield();
        }
    }
}
