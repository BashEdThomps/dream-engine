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


#pragma once

#include "../IComponent.h"

using std::vector;

namespace Dream
{
    enum InputSource
    {
        // Joystick
        JS_FaceButtonNorth,
        JS_FaceButtonEast,
        JS_FaceButtonWest,
        JS_FaceButtonSouth,
        JS_FaceHome,
        JS_FaceStart,
        JS_FaceSelect,
        JS_ShoulderLeft,
        JS_ShoulderRight,
        JS_TriggerLeft,
        JS_TriggerRight,
        JS_DPadNorth,
        JS_DPadSouth,
        JS_DPadEast,
        JS_DPadWest,
        JS_AnalogLeftStickX,
        JS_AnalogLeftStickY,
        JS_AnalogLeftButton,
        JS_AnalogRightStickX,
        JS_AnalogRightStickY,
        JS_AnalogRightButton,
        // Keyboard
        KB_UP,
        KB_DOWN,
        KB_LEFT,
        KB_RIGHT,
        KB_SPACE,
        KB_RETURN
    };

    class InputComponent : public IComponent
    {
    public:
        InputComponent(bool useKeyboard = false, bool useMouse = false, bool useJoystick = false);
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;

        void debugKeyboard() const;
        void debugMouse() const;
        void debugGamepad() const;

        bool usingKeyboard() const;
        bool usingMouse() const;
        bool usingJoystick() const;

    private:
        const static float ANALOG_DEAD_ZONE;
        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;
    };

}
