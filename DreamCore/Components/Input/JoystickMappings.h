#pragma once

#include "JoystickMapping.h"

namespace octronic::dream
{
    const static JoystickMapping JsPsxMapping
    {
        // Axis
        0,    // AnalogLeftXAxis
        1,    // AnalogLeftYAxis
        2,    // AnalogRightXAxis
        3,    // AnalogRightYAxis

        // Buttons
        12,   // FaceButtonNorth
        13,   // FaceButtonEast
        14,   // FaceButtonSouth
        15,   // FaceButtonWest
        0,   // FaceButtonSelect
        3,   // FaceButtonStart
        16,   // FaceButtonHome
        10,   // ShoulderLeft
        11,   // ShoulderRight
        8,   // LeftTriggerButton
        9,   // RighTriggerButton
        1,   // AnalogLeftButton
        2,   // AnalogRightButton
		// Dpad
        4,   // DPadNorth
        5,   // DPadWest
        6,   // DPadSouth
        7    // DPadEast
    };

    const static JoystickMapping JsXboxMapping
    {
        // Axis
        0, // AnalogLeftXAxis
        1, // AnalogLeftYAxis
        2, // AnalogRightXAxis
        3, // AnalogRightYAxis
        // Buttons
        0, // Y
        1, // B
        2, // A
        3, // X
        8,  // FaceButtonSelect
        9,  // FaceButtonStart
        12, // FaceButtonHome
        4, // ShoulderLeft
        5, // ShoulderRight
        6,  // LeftTriggerButton
        7,  // RighTriggerButton
        10, // AnalogLeftButton
        11, // AnalogRightButton
		// Dpad
        13, // DPadNorth
        14, // DPadWest
        15, // DPadSouth
        16  // DPadEast
    };
}
