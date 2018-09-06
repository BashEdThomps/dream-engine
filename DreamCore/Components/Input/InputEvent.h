/*
 * InputEvent.h
 *
 * Created: 08 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include "../../Common/DreamObject.h"

namespace Dream
{
    enum InputSource
    {
        INPUT_SOURCE_KEYBOARD,
        INPUT_SOURCE_MOUSE,
        INPUT_SOURCE_GAMEPAD,
        INPUT_SOURCE_NONE
    };

    enum MouseEventType
    {
        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        MOUSE_SCROLL,
        MOUSE_MOTION,
        MOUSE_NONE
    };

    enum KeyEventType
    {
        KEY_PRESSED,
        KEY_RELEASED,
        KEY_NONE
    };

    enum GamepadEventType
    {
        GAMEPAD_AXIS,
        GAMEPAD_BUTTON_PRESSED,
        GAMEPAD_BUTTON_RELEASED,
        GAMEPAD_NONE
    };

    class InputEvent : public DreamObject
    {
    public:
        InputEvent(InputSource source = INPUT_SOURCE_NONE);
        ~InputEvent();

        InputSource getSource();
        void setSource(InputSource source);

        MouseEventType getMouseEventType();
        void setMouseEventType(MouseEventType type);

        KeyEventType getKeyEventType();
        void setKeyEventType(KeyEventType type);

        GamepadEventType getGamepadEventType();
        void setGamepadEventType(GamepadEventType type);

        int getX();
        void setX(int);

        int getY();
        void setY(int);

        bool isPressed();
        void setPressed(bool);

        char getKey();
        void setKey(char);

        char getButton();
        void setButton(char);

    private:
        InputSource mSource;
        KeyEventType mKeyEventType;
        MouseEventType mMouseEventType;
        GamepadEventType mGamepadEventType;
        int mX;
        int mY;
        bool mPressed;
        char mButton;
    };
}
