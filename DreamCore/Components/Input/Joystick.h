#pragma once
#include <string>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "../Time.h"
#include "../Transform.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/Actor/ActorRuntime.h"
#include "../Graphics/Camera.h"

using glm::mat4;
using glm::quat;
using std::string;
using std::cout;
using std::endl;
using glm::mat4;
using glm::vec3;

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

        void setAxisData(unsigned int index, float data)
        {
            mAxisData[index] = data;
        }

        bool getButtonData(int idx)
        {
            return mButtonData[idx] != 0;
        }

        void setButtonData(unsigned int index, bool data)
        {
            mButtonData[index] = data ? 1 : 0;
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

    class JoystickNavigation
    {
    protected:
        JoystickState* mJoystickState;
        JoystickMapping* mJoystickMapping;
        Vector2 mHeading;
    public:
        JoystickNavigation(JoystickState* state, JoystickMapping* mapping)
            : mJoystickState(state), mJoystickMapping(mapping)
        {

        }

        virtual void update(SceneRuntime* rt) = 0;

        void setHeading(const Vector2& h)
        {
            mHeading = h;
        }

        Vector2& getHeading()
        {
            return mHeading;
        }
    };

    class JoystickFaceForwardNavigation : public JoystickNavigation
    {
    public:
        JoystickFaceForwardNavigation(JoystickState* state, JoystickMapping* mapping)
            : JoystickNavigation(state,mapping) {}

        void update(SceneRuntime* rt) override
        {
            if (!rt) return;

            float leftX = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftXAxis);
            float leftY = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftYAxis);
            float rightX = mJoystickState->getAxisData(mJoystickMapping->AnalogRightXAxis);
            float rightY = mJoystickState->getAxisData(mJoystickMapping->AnalogRightYAxis);

            auto playerObject = rt->getPlayerObject();
            if (playerObject)
            {
                auto& playerTransform = playerObject->getTransform();
                auto time = rt->getProjectRuntime()->getTime();
                auto camera = rt->getCamera();
                static mat4 ident(1.0f);
                Vector3 translation = playerTransform.getTranslation();
                mat4 mtx = glm::translate(ident,translation.toGLM());
                auto totalYaw = 0.0f;
                setHeading(Vector2(cos(totalYaw),-sin(totalYaw)));
                mtx = glm::translate(mtx,vec3(time->perSecond(-leftY)*10.0f,0,time->perSecond(leftX)*10.0f));
                playerTransform.setMatrix(mtx);
            }
            else
            {
                cout << "No player object" << endl;
            }


        }
    };

    class Joystick2DPlaneNavigation : public JoystickNavigation
    {
        float mLeftVelocity;
        float mRightVelocity;
        float mLeftTheta;
        float mRightTheta;

        float mLastLeftVelocity;
        float mLastRightVelocity;
        float mLastLeftTheta;
        float mLastRightTheta;


    public:
        Joystick2DPlaneNavigation
        (JoystickState* state, JoystickMapping* mapping)
            : JoystickNavigation(state,mapping),
              mLeftVelocity(0.0f),
              mRightVelocity(0.0f),
              mLeftTheta(0.0f),
              mRightTheta(0.0f)
        {}

        void update(SceneRuntime* rt) override
        {
            if (!rt) return;

            float leftX = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftXAxis);
            float leftY = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftYAxis);
            float rightX = mJoystickState->getAxisData(mJoystickMapping->AnalogRightXAxis);
            float rightY = mJoystickState->getAxisData(mJoystickMapping->AnalogRightYAxis);

            if (mJoystickState->clearsDeadzone(leftX) || mJoystickState->clearsDeadzone(leftY))
            {
                mLeftVelocity = fabs(sqrt((leftX*leftX)+(-leftY * -leftY)));

                mLeftTheta = atan2(-leftY,leftX);

                if (mLeftTheta != 0.0f)
                {
                   mLeftTheta -= (M_PI/2);
                }
            }
            else
            {
                mLeftVelocity = 0.0f;
            }

            if (mJoystickState->clearsDeadzone(rightX) || mJoystickState->clearsDeadzone(rightY))
            {
                mRightVelocity = fabs(sqrt((rightX * rightX) + (-rightY * -rightY)));

                mRightTheta = atan2(-rightY,rightX);

                if (mRightTheta != 0.0f)
                {
                     mRightTheta -= (M_PI/2);
                }
            }
            else
            {
                mRightVelocity = 0.0f;
            }

            auto playerObject = rt->getPlayerObject();
            if (playerObject)
            {
                auto& playerTransform = playerObject->getTransform();
                auto time = rt->getProjectRuntime()->getTime();
                auto camera = rt->getCamera();
                static mat4 ident(1.0f);
                static vec3 yAxis(0.0f,1.0f,0.0f);
                Vector3 translation = playerTransform.getTranslation();
                mat4 mtx = glm::translate(ident,translation.toGLM());
                auto camYaw = camera->getYaw();
                auto totalYaw = getLeftTheta()-camYaw;
                setHeading(Vector2(cos(totalYaw),-sin(totalYaw)));
                mtx = glm::rotate(mtx,totalYaw,yAxis);
                mtx = glm::translate(mtx,vec3(time->perSecond(getLeftVelocity()*10.0f),0,0));
                playerTransform.setMatrix(mtx);
            }
            else
            {
                cout << "No player object" << endl;
            }
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
    };
}
