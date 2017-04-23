/*
 * GameController.h
 *
 * Created: 22/04/2017 2017 by Ashley
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
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include <SDL2/SDL.h>
#include "Constants.h"

using namespace std;

namespace Dream {

    enum ControllerAxis {

        LEFT_ANALOG_X,
        LEFT_ANALOG_Y,

        RIGHT_ANALOG_Y,
        RIGHT_ANALOG_X,

        LEFT_TRIGGER,
        RIGHT_TRIGGER

    };

    enum ControllerButton {

        A,
        B,
        X,
        Y,
        START,
        BACK,
        GUIDE,

        LEFT_ANALOG,
        RIGHT_ANALOG,

        LEFT_SHOULDER,
        RIGHT_SHOULDER,

        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT

    };

    class GameController
    {

    private:
        // Left Analog
        int mLeftAnalogX;
        int mLeftAnalogY;
        int mLeftAnalogButton;
        // Right Analog
        int mRightAnalogX;
        int mRightAnalogY;
        int mRightAnalogButton;
        // Triggers
        int mLeftTrigger;
        int mRightTrigger;
        // Face Button
        int mAButton;
        int mBButton;
        int mXButton;
        int mYButton;
        int mStartButton;
        int mBackButton;
        int mGuideButton;
        // Shoulders
        int mLeftShoulderButton;
        int mRightShoulderButton;
        // D-Pad Buttons
        int mDPadLeftButton;
        int mDPadRightButton;
        int mDPadUpButton;
        int mDPadDownButton;

    public:
      GameController();
      ~GameController();
      void zeroAll();
      void zeroButtons();
      void zeroAxis();
      void onButtonEvent(SDL_Event&);
      void onAxisEvent(SDL_Event&);
      void updateControllerState(SDL_Event &e);
      int  getButtonValue(ControllerButton btn);
      int  getAxisValue(ControllerAxis axis);

    };
}

#endif // GAMECONTROLLER_H
