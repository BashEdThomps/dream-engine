#pragma once
#include <string>
#include "../../Common/Math.h"
#include <glm/fwd.hpp>

using glm::mat4;
using glm::quat;
using std::string;
using std::cout;
using std::endl;

namespace Dream
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
        string mName = "";
        int mButtonCount = 0;
        unsigned char mButtonData[32] = {0};
        int mAxisCount = 0;
        float mAxisData[32] = {0.0f};
        float mDeadZone = 0.15f;
    public:

        float getAxisData(int idx)
        {
           return mAxisData[idx];
        }

        bool getButtonData(int idx)
        {
            return mButtonData[idx] != 0;
        }

        float getDeadZone() const
        {
           return mDeadZone;
        }

        const float* getAxisDataPointer() const
        {
            return &mAxisData[0];
        }

        const unsigned char* getButtonDataPointer() const
        {
            return &mButtonData[0];
        }

        void setName(const string& name)
        {
            mName = name;
        }

        string getName() const
        {
            return mName;
        }

        int getAxisCount() const
        {
            return mAxisCount;
        }

        void setAxisCount(int count)
        {
            mAxisCount = count;
        }

        int getButtonCount() const
        {
            return mButtonCount;
        }

        void setButtonCount(int count)
        {
            mButtonCount = count;
        }

        bool clearsDeadzone(float val) const
        {
            if(val > mDeadZone)
            {
                return true;
            }
            else if (val < -mDeadZone)
            {
                return true;
            }
            return false;
        }
    };

    class JoystickNavigation3D
    {
        float mLeftVelocity;
        float mRightVelocity;
        float mLeftTheta;
        float mRightTheta;

        float mLastLeftVelocity;
        float mLastRightVelocity;
        float mLastLeftTheta;
        float mLastRightTheta;

        Vector2 mHeading;
        JoystickState* mJoystickState;
        JoystickMapping* mJoystickMapping;

    public:
        JoystickNavigation3D
        (JoystickState* state, JoystickMapping* mapping)
            : mLeftVelocity(0.0f),
              mRightVelocity(0.0f),
              mLeftTheta(0.0f),
              mRightTheta(0.0f),

              mLastLeftVelocity(0.0f),
              mLastRightVelocity(0.0f),
              mLastLeftTheta(0.0f),
              mLastRightTheta(0.0f),

              mJoystickState(state),
              mJoystickMapping(mapping)
        {}

        void calculate()
        {
            captureLast();

            float leftX = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftXAxis);
            float leftY = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftYAxis);
            float rightX = mJoystickState->getAxisData(mJoystickMapping->AnalogRightXAxis);
            float rightY = mJoystickState->getAxisData(mJoystickMapping->AnalogRightYAxis);

            if (mJoystickState->clearsDeadzone(leftX) || mJoystickState->clearsDeadzone(leftY))
            {
                mLeftVelocity = fabs(sqrt((leftX*leftX)+(-leftY * -leftY)));

                if (leftY < 0.0f)
                {
                    mLeftVelocity *= -1.0f;
                }

                mLeftTheta = atan2(-leftY,leftX);

                if (mLeftTheta != 0.0f)
                {
                   mLeftTheta -= (M_PI/2);
                }
            }
            if (mJoystickState->clearsDeadzone(rightX) || mJoystickState->clearsDeadzone(rightY))
            {
                mRightVelocity = fabs(sqrt((rightX * rightX) + (-rightY * -rightY)));

                if (rightY < 0.0f)
                {
                    mRightVelocity *= -1.0f;
                }

                mRightTheta = atan2(-rightY,rightX);

                if (mRightTheta != 0.0f)
                {
                     mRightTheta -= (M_PI/2);
                }
            }
        }

        void captureLast()
        {
            mLastLeftVelocity = mLeftVelocity;
            mLastRightVelocity = mRightVelocity;
            mLastLeftTheta = mLeftTheta;
            mLastRightTheta = mRightTheta;
        }

        void show()
        {
            cout << "JSNav3D ="
                   " Lv: " << mLeftVelocity
                << " Lt: " << mLeftTheta
                << " Rv: " << mRightVelocity
                << " Rt: " << mRightTheta
                << endl;
        }

        float getRightVelocity() const
        {
            return mRightVelocity;
        }

        float getLeftTheta() const
        {
            return mLeftTheta;
        }

        float getRightTheta() const
        {
            return mRightTheta;
        }

        float getLeftVelocity() const
        {
            return mLeftVelocity;
        }

        void setHeading(const Vector2& h)
        {
            mHeading = h;
        }

        Vector2& getHeading()
        {
            return mHeading;
        }
    };
}
