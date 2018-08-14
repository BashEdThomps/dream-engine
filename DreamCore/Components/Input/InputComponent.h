#pragma once

#include "../IComponent.h"
#include <gainput/gainput.h>
#include <gainput/GainputInputMap.h>

using namespace gainput;

namespace Dream
{
    enum InputButton
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

    class InputComponent : public IComponent
    {
    public:
        InputComponent();
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;

    private:
        unique_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;

    };

}
