#pragma once

class WindowInputState
{
public:
    WindowInputState();

    int  mouseLastX;
    int  mouseLastY;

    int mouseWheelLastX;
    int mouseWheelLastY;

    bool shiftPressed;
    bool altPressed;
    bool ctrlPressed;

    bool wPressed;
    bool aPressed;
    bool sPressed;
    bool dPressed;

    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
};
