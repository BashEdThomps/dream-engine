#pragma once

#include "../IComponent.h"
#include <gainput/gainput.h>
#include <gainput/GainputInputMap.h>

using namespace gainput;
using std::vector;

namespace Dream
{
    enum JSInputSource
    {
        FaceButtonNorth,
        FaceButtonEast,
        FaceButtonWest,
        FaceButtonSouth,

        FaceHome,
        FaceStart,
        FaceSelect,

        ShoulderLeft,
        ShoulderRight,

        TriggerLeft,
        TriggerRight,

        DPadNorth,
        DPadSouth,
        DPadEast,
        DPadWest,

        AnalogLeftStickX,
        AnalogLeftStickY,
        AnalogLeftButton,

        AnalogRightStickX,
        AnalogRightStickY,
        AnalogRightButton
    };

    enum KeyboardInputSource
    {

    };

    enum MouseInputSource
    {

    };

    class InputComponent : public IComponent
    {
    public:
        InputComponent(bool useKeyboard = false, bool useMouse = false, bool useJoystick = false);
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;

        shared_ptr<InputMap> getInputMap() const;

    private:
        const static float ANALOG_DEAD_ZONE;
        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;
        shared_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
    };

}
