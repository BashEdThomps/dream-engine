#pragma once

#include <string>

using namespace std;

namespace Dream
{
    struct JoystickState
    {
        string Name = "";
        // Joystick
        int ButtonCount = 0;
        unsigned char ButtonData[32] = {0};
        int AxisCount = 0;
        float AxisData[32] = {0.0f};
        float DeadZone = 0.25f;
    };

    struct MouseState
    {
        float PosX = 0;
        float PosY = 0;
        float ScrollX = 0;
        float ScrollY = 0;
        bool ButtonsDown[5] = {false};
    };

    struct KeyboardState
    {
        bool KeysDown[512] = {false};
    };


}
