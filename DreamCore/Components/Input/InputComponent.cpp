/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "InputComponent.h"

namespace Dream
{
    InputComponent::InputComponent(bool useKeyboard, bool useMouse, bool useJoystick)
        : IComponent (),
          mUseKeyboard(useKeyboard),
          mUseMouse(useMouse),
          mUseJoystick(useJoystick)
    {
        setLogClassName("InputComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    InputComponent::~InputComponent()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    bool InputComponent::init()
    {
        auto log = getLog();
        log->debug("Initialising...");


        if (mUseKeyboard)
        {
            log->debug("Creating Keyboard Device");
        }

        if (mUseMouse)
        {
            log->debug("Creating Mouse Device");
        }

        if (mUseJoystick)
        {
            log->debug("Creating Joystick Device");

        }

        return true;
    }

    void InputComponent::updateComponent(SceneRuntime*)
    {
        auto log = getLog();
        beginUpdate();
        endUpdate();
    }

    void
    InputComponent::debugGamepad
    ()
    const
    {
        if (!mUseKeyboard)
        {
            return;
        }

        auto log = getLog();
        /*
        log->trace(
            "Gamepad State\n"

            "FaceButtonNorth.....{}\n"
            "FaceButtonEast......{}\n"
            "FaceButtonWest......{}\n"
            "FaceButtonSouth.....{}\n"

            "FaceHome............{}\n"
            "FaceStart...........{}\n"
            "FaceSelect..........{}\n"

            "ShoulderLeft........{}\n"
            "ShoulderRight.......{}\n"

            "TriggerLeft.........{}\n"
            "TriggerRight........{}\n"

            "DPadNorth...........{}\n"
            "DPadSouth...........{}\n"
            "DPadEast............{}\n"
            "DPadWest............{}\n"

            "AnalogLeftStickX....{}\n"
            "AnalogLeftStickY....{}\n"
            "AnalogLeftButton....{}\n"

            "AnalogRightStickX...{}\n"
            "AnalogRightStickY...{}\n"
            "AnalogRightButton...{}\n",
        );
        */
    }

    bool InputComponent::usingKeyboard() const
    {
        return mUseKeyboard;
    }

    bool InputComponent::usingMouse() const
    {
        return mUseMouse;
    }

    bool InputComponent::usingJoystick() const
    {
       return mUseJoystick;
    }

    void
    InputComponent::debugKeyboard
    ()
    const
    {
        if (!mUseKeyboard)
        {
            return;
        }

        auto log = getLog();
        /*
        log->trace
        (
            "Keyboard State\n"
            "Up....................{}\n"
            "Down..................{}\n"
            "Left..................{}\n"
            "Right.................{}\n"

            "Space.................{}\n"
            "Return................{}\n",

            mInputMap->GetBool(KB_UP),
            mInputMap->GetBool(KB_DOWN),
            mInputMap->GetBool(KB_LEFT),
            mInputMap->GetBool(KB_RIGHT),

            mInputMap->GetBool(KB_SPACE),
            mInputMap->GetBool(KB_RETURN)
        );
        */

    }

    void
    InputComponent::debugMouse
    ()
    const
    {
        if (!mUseMouse)
        {
            return;
        }
    }

    const float InputComponent::ANALOG_DEAD_ZONE = 0.1f;
}
