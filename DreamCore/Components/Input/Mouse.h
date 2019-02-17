#pragma once

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

namespace Dream
{
    struct MouseState
    {
        float PosX = 0;
        float PosY = 0;
        float ScrollX = 0;
        float ScrollY = 0;
        bool ButtonsDown[5] = {false};
    };
}
