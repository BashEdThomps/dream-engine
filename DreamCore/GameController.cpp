/*
 * GameController.cpp
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
#include "GameController.h"

namespace Dream
{
    GameController::GameController()
        : ILuaExposable()
    {
        zeroAll();
    }

    GameController::~GameController()
    {
        zeroAll();
    }

    void GameController::zeroAll()
    {
        zeroButtons();
        zeroAxis();
    }

    void GameController::zeroButtons()
    {
        mAButton = 0;
        mBButton = 0;
        mXButton = 0;
        mYButton = 0;
        mBackButton = 0;
        mGuideButton = 0;
        mStartButton = 0;
        mLeftShoulderButton = 0;
        mRightShoulderButton = 0;
        mLeftAnalogButton = 0;
        mRightAnalogButton = 0;
    }

    void GameController::zeroAxis()
    {
        mLeftAnalogX = 0;
        mLeftAnalogY = 0;
        mLeftAnalogX = 0;
        mLeftAnalogY = 0;
        mLeftTrigger = 0;
        mRightTrigger = 0;
    }

    void GameController::updateControllerState(SDL_Event &e)
    {
       if (VERBOSE)
       {
           cout << "GameController: State Update" << endl;
       }

       switch(e.type)
       {
            case SDL_CONTROLLERBUTTONDOWN:
                onButtonEvent(e);
                break;
            case SDL_CONTROLLERAXISMOTION:
                onAxisEvent(e);
                break;
        }
    }

    void GameController::onButtonEvent(SDL_Event &e)
    {
        if (VERBOSE)
        {
            cout << "GameController: ButtonEvent"
                 << " on button: " << (int)e.cbutton.button
                 << " state: " << (int)e.cbutton.state
                 << endl;
        }

        int state = e.cbutton.state;

        switch (e.cbutton.button)
        {
            // Face Buttons
            case SDL_CONTROLLER_BUTTON_A:
                mAButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                mBButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_X:
                mXButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                mYButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_BACK:
                mBackButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_GUIDE:
                mGuideButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_START:
                mStartButton = state;
                break;
                // Stick Buttons
            case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                mLeftAnalogButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                mRightAnalogButton = state;
                break;
                // Shoulder Buttons
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                mLeftShoulderButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                mRightShoulderButton = state;
                break;
                // D Pad
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                mDPadUpButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                mDPadDownButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                mDPadLeftButton = state;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                mDPadRightButton = state;
                break;
        }
    }

    void GameController::onAxisEvent(SDL_Event &e)
    {
        if (VERBOSE)
        {
            cout << "GameController: AxisEvent on axis "
                 << (int)e.caxis.axis
                 << " value: "
                 << e.caxis.value
                 << endl;
        }
        int value = e.caxis.value;
        switch (e.caxis.axis)
        {
            case SDL_CONTROLLER_AXIS_LEFTX:
                mLeftAnalogX = value;
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                mLeftAnalogY = value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                mRightAnalogX = value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                mRightAnalogY = value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                mLeftTrigger = value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                mRightTrigger = value;
                break;
        }
    }

    int GameController::getButtonValue(ControllerButton btn)
    {
        switch (btn)
        {
            // Face Buttons
            case A:
                return mAButton;
            case B:
                return mBButton;
            case X:
                return mXButton;
            case Y:
                return mYButton;
            case BACK:
                return mBackButton;
            case GUIDE:
                return mGuideButton;
            case START:
                return mStartButton;
                // Stick Buttons
            case LEFT_ANALOG:
                return mLeftAnalogButton;
            case RIGHT_ANALOG:
                return mRightAnalogButton;
                // Shoulder Buttons
            case LEFT_SHOULDER:
                return mLeftShoulderButton;
            case RIGHT_SHOULDER:
                return mRightShoulderButton;
                // D Pad
            case DPAD_UP:
                return  mDPadUpButton;
            case DPAD_DOWN:
                return mDPadDownButton;
            case DPAD_LEFT:
                return mDPadLeftButton;
            case DPAD_RIGHT:
                return mDPadRightButton;
        }
        // Error ?
        return 0;
    }

    int GameController::getAxisValue(ControllerAxis axis)
    {
        switch (axis)
        {
            case LEFT_TRIGGER:
                return mLeftTrigger;
            case RIGHT_TRIGGER:
                return mRightTrigger;
            case LEFT_ANALOG_X:
                return mLeftAnalogX;
            case LEFT_ANALOG_Y:
                return mLeftAnalogY;
            case RIGHT_ANALOG_X:
                return mRightAnalogX;
            case RIGHT_ANALOG_Y:
                return mRightAnalogY;
        }
    }

    void GameController::exposeLuaApi(lua_State* state) {
        luabind::module(state)
        [
            luabind::class_<GameController>("GameController")
                .def("getButtonValue",&GameController::getButtonValue)
                .def("getAxisValue",&GameController::getAxisValue)
                .enum_("ControllerButton")
                [
                // Face Buttons
                luabind::value("A_BTN",ControllerButton::A),
                luabind::value("B_BTN",ControllerButton::B),
                luabind::value("X_BTN",ControllerButton::X),
                luabind::value("Y_BTN",ControllerButton::Y),
                // Shoulders
                luabind::value("LEFT_SHOULDER",ControllerButton::LEFT_SHOULDER),
                luabind::value("RIGHT_SHOULDER",ControllerButton::RIGHT_SHOULDER),
                // Analog
                luabind::value("LEFT_ANALOG_BUTTON",ControllerButton::LEFT_ANALOG),
                luabind::value("RIGHT_ANALOG_BUTTON",ControllerButton::RIGHT_ANALOG),
                // D-Pad
                luabind::value("DPAD_UP",ControllerButton::DPAD_UP),
                luabind::value("DPAD_DOWN",ControllerButton::DPAD_DOWN),
                luabind::value("DPAD_LEFT",ControllerButton::DPAD_LEFT),
                luabind::value("DPAD_RIGHT",ControllerButton::DPAD_RIGHT)
                ]
                .enum_("ControllerAxis")
                [
                // Left Analog
                luabind::value("LEFT_ANALOG_X",ControllerAxis::LEFT_ANALOG_X),
                luabind::value("LEFT_ANALOG_Y",ControllerAxis::LEFT_ANALOG_Y),
                // Right Analog
                luabind::value("RIGHT_ANALOG_X",ControllerAxis::RIGHT_ANALOG_X),
                luabind::value("RIGHT_ANALOG_Y",ControllerAxis::RIGHT_ANALOG_Y),
                // Triggers
                luabind::value("LEFT_TRIGGER",ControllerAxis::LEFT_TRIGGER),
                luabind::value("RIGHT_TRIGGER",ControllerAxis::RIGHT_TRIGGER)
            ]
        ];
    }
}
