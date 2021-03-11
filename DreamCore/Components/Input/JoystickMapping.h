#pragma once

namespace octronic::dream
{
    class JoystickMapping
    {
    public:
        // Axis
        int AnalogLeftXAxis;
        int AnalogLeftYAxis;
        int AnalogRightXAxis;
        int AnalogRightYAxis;
        // Buttons
        int FaceButtonNorth; // Y
        int FaceButtonEast; // B
        int FaceButtonSouth; // A
        int FaceButtonWest; // X

        int FaceButtonSelect;
        int FaceButtonStart;
        int FaceButtonHome;

        int ShoulderLeft;
        int ShoulderRight;

        int TriggerLeftButton;
        int TriggerRightButton;

        int AnalogLeftButton;
        int AnalogRightButton;

        int DPadNorth;
        int DPadWest;
        int DPadSouth;
        int DPadEast;
    };
}
