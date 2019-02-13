#pragma once

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

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
        float DeadZone = 0.15f;

        float getAxisData(int idx)
        {
           return AxisData[idx];
        }

        bool getButtonData(int idx)
        {
            return ButtonData[idx] != 0;
        }
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
