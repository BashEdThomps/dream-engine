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
        void setLuaComponentHandle(LuaComponent* luaHandle);

    private:
        const static float ANALOG_DEAD_ZONE;
        unique_ptr<InputMap> mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
        LuaComponent* mLuaComponentHandle;
        bool mInputMapSet;
    };

}
