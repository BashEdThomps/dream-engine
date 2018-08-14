/*
 * InputEvent.cpp
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

#include "InputEvent.h"

#include "../../Common/Constants.h"

namespace Dream
{
    InputEvent::InputEvent
    (InputSource source)
        : ILoggable("InputSource"), mSource(source),
          mKeyEventType(KEY_NONE),
          mMouseEventType(MOUSE_NONE),
          mGamepadEventType(GAMEPAD_NONE),
          mX(0),
          mY(0),
          mPressed(false),
          mButton(0)
    {
        auto log = getLog();
        log->trace( "Constructing" );
    }

    InputEvent::~InputEvent
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );
    }

    InputSource
    InputEvent::getSource
    ()
    {
        return mSource;
    }

    void
    InputEvent::setSource
    (InputSource source)
    {
        mSource = source;
    }

    KeyEventType
    InputEvent::getKeyEventType
    ()
    {
        return mKeyEventType;
    }

    void
    InputEvent::setKeyEventType
    (KeyEventType type)
    {
        mKeyEventType  = type;
    }

    MouseEventType
    InputEvent::getMouseEventType
    ()
    {
        return mMouseEventType;
    }

    void
    InputEvent::setMouseEventType
    (MouseEventType type)
    {
        mMouseEventType = type;
    }

    GamepadEventType
    InputEvent::getGamepadEventType
    ()
    {
        return mGamepadEventType;
    }

    void
    InputEvent::setGamepadEventType
    (GamepadEventType type)
    {
        mGamepadEventType = type;
    }

    int
    InputEvent::getX
    ()
    {
        return mX;
    }

    void
    InputEvent::setX
    (int x)
    {
        mX = x;
    }

    int
    InputEvent::getY
    ()
    {
        return mY;
    }

    void
    InputEvent::setY
    (int y)
    {
        mY = y;
    }

    bool
    InputEvent::isPressed
    ()
    {
        return mPressed;
    }

    void
    InputEvent::setPressed
    (bool pressed)
    {
        mPressed = pressed;
    }

    void
    InputEvent::setKey
    (char key)
    {
        mButton = key;
    }

    char
    InputEvent::getKey
    ()
    {
        return mButton;
    }

    void
    InputEvent::setButton(char button)
    {
        mButton = button;
    }

    char
    InputEvent::getButton
    ()
    {
        return mButton;
    }

}
