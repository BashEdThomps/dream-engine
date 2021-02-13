#pragma once

#include "Common/Vector.h"

#include <string>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

using glm::mat4;
using glm::quat;
using std::string;
using glm::mat4;
using glm::vec3;

namespace octronic::dream
{
    class SceneRuntime;
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

        13, // DPadNorth
        14, // DPadWest
        15, // DPadSouth
        16  // DPadEast
    };

    class JoystickState
    {

    public:

        float getAxisData(int idx);
        void setAxisData(unsigned int index, float data);
        bool getButtonData(int idx);
        void setButtonData(unsigned int index, bool data);
        float getDeadZone() const;
        const float* getAxisDataPointer() const;
        const unsigned char* getButtonDataPointer() const;
        void setName(const string& name);
        string getName() const;
        int getAxisCount() const;
        void setAxisCount(int count);
        int getButtonCount() const;
        void setButtonCount(int count);
        bool clearsDeadzone(float val) const;
    private:
        string mName = "";
        int mButtonCount = 0;
        unsigned char mButtonData[32] = {0};
        int mAxisCount = 0;
        float mAxisData[32] = {0.0f};
        float mDeadZone = 0.15f;
    };

    class JoystickNavigation
    {
    public:
        JoystickNavigation(JoystickState* state, JoystickMapping* mapping);
        virtual ~JoystickNavigation();
        virtual void update(SceneRuntime* rt) = 0;
        void setHeading(const Vector2& h);
        Vector2& getHeading();
    protected:
        JoystickState* mJoystickState;
        JoystickMapping* mJoystickMapping;
        Vector2 mHeading;
    };

    class JoystickFaceForwardNavigation : public JoystickNavigation
    {
    public:
        JoystickFaceForwardNavigation(JoystickState* state, JoystickMapping* mapping);
        ~JoystickFaceForwardNavigation();
        void update(SceneRuntime* rt) override;
    };

    class Joystick2DPlaneNavigation : public JoystickNavigation
    {
    public:
        Joystick2DPlaneNavigation(JoystickState* state, JoystickMapping* mapping);
        ~Joystick2DPlaneNavigation();

        void update(SceneRuntime* rt) override;
        void show();
        float getRightVelocity() const;
        float getLeftTheta() const;
        float getRightTheta() const;
        float getLeftVelocity() const;
    private:
        float mLeftVelocity;
        float mRightVelocity;
        float mLeftTheta;
        float mRightTheta;

        float mLastLeftVelocity;
        float mLastRightVelocity;
        float mLastLeftTheta;
        float mLastRightTheta;
    };
}
