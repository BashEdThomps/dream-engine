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
        Key_UP,
        Key_DOWN,
        Key_LEFT,
        Key_RIGHT,
        Key_SPACE,
        Key_RETURN
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

        InputMap* getInputMap() const;

        void debugKeyboard() const;
        void debugMouse() const;
        void debugGamepad() const;

    private:
        const static float ANALOG_DEAD_ZONE;
        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;
        InputMap* mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
    };

}
