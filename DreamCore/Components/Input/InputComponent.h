#pragma once

#include "../IComponent.h"
#include <gainput/gainput.h>
#include <gainput/GainputInputMap.h>

using namespace gainput;

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

    class LuaComponent;

    class InputComponent : public IComponent
    {
    public:
        InputComponent(bool parallel = false);
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;
        void setLuaComponent(shared_ptr<LuaComponent> lua);

    private:
        const static float ANALOG_DEAD_ZONE;
        shared_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
        shared_ptr<LuaComponent> mLuaComponent;
        bool mInputMapSet;
    };

}
