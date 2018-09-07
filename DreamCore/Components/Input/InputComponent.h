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

    class IScriptComponent;

    class InputComponent : public IComponent
    {
    public:
        InputComponent();
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;
        void setScriptComponent(shared_ptr<IScriptComponent> sc);

    private:
        const static float ANALOG_DEAD_ZONE;
        shared_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
        shared_ptr<IScriptComponent> mScriptComponent;
        bool mInputMapSet;
    };

}
