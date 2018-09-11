#pragma once

#include "../IComponent.h"
#include <gainput/gainput.h>
#include <gainput/GainputInputMap.h>

using namespace gainput;
using std::vector;

namespace Dream
{
    enum InputSource
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

        shared_ptr<InputMap> getInputMap() const;

    private:
        const static float ANALOG_DEAD_ZONE;
        shared_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
    };

}
