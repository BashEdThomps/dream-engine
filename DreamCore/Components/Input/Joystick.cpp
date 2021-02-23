#include "Joystick.h"

#include "Components/Time.h"
#include "Math/Transform.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{

    // JoystickState ===========================================================

    float JoystickState::getAxisData(int idx)
    {
        return mAxisData[idx];
    }

    void JoystickState::setAxisData(unsigned int index, float data)
    {
        mAxisData[index] = data;
    }

    bool JoystickState::getButtonData(int idx)
    {
        return mButtonData[idx] != 0;
    }

    void JoystickState::setButtonData(unsigned int index, bool data)
    {
        mButtonData[index] = data ? 1 : 0;
    }

    float JoystickState::getDeadZone() const
    {
        return mDeadZone;
    }

    const float* JoystickState::getAxisDataPointer() const
    {
        return &mAxisData[0];
    }

    const unsigned char* JoystickState::getButtonDataPointer() const
    {
        return &mButtonData[0];
    }

    void JoystickState::setName(const string& name)
    {
        mName = name;
    }

    string JoystickState::getName() const
    {
        return mName;
    }

    int JoystickState::getAxisCount() const
    {
        return mAxisCount;
    }

    void JoystickState::setAxisCount(int count)
    {
        mAxisCount = count;
    }

    int JoystickState::getButtonCount() const
    {
        return mButtonCount;
    }

    void JoystickState::setButtonCount(int count)
    {
        mButtonCount = count;
    }

    bool JoystickState::clearsDeadzone(float val) const
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

    // JoystickNavigation ======================================================

    JoystickNavigation::JoystickNavigation(JoystickState* state, JoystickMapping* mapping)
        : mJoystickState(state), mJoystickMapping(mapping)
    {

    }

    JoystickNavigation::~JoystickNavigation()
    {}

    void JoystickNavigation::setHeading(const vec2& h)
    {
        mHeading = h;
    }

    vec2& JoystickNavigation::getHeading()
    {
        return mHeading;
    }

    JoystickFaceForwardNavigation::JoystickFaceForwardNavigation
    (JoystickState* state, JoystickMapping* mapping)
        : JoystickNavigation(state,mapping)
    {

    }

    JoystickFaceForwardNavigation::~JoystickFaceForwardNavigation()
    {

    }

    void JoystickFaceForwardNavigation::update(SceneRuntime* rt)
    {
        if (!rt) return;

        float leftX = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftXAxis);
        float leftY = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftYAxis);
        float rightX = mJoystickState->getAxisData(mJoystickMapping->AnalogRightXAxis);
        float rightY = mJoystickState->getAxisData(mJoystickMapping->AnalogRightYAxis);

        auto playerObject = rt->getPlayerEntity();
        if (playerObject)
        {
            Transform playerTransform = playerObject->getTransform();
            Time* time = rt->getProjectRuntime()->getTime();
            Camera* camera = rt->getCamera();
            static mat4 ident(1.0f);
            vec3 translation = playerTransform.getTranslation();
            mat4 mtx = glm::translate(ident,translation);
            auto totalYaw = 0.0f;
            setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
            mtx = glm::translate(mtx,vec3(time->perSecond(-leftY)*10.0f,0,time->perSecond(leftX)*10.0f));
            playerObject->setTransform(playerTransform);
        }
        else
        {
            LOG_ERROR("Joystick: No player object");
        }


    }

    Joystick2DPlaneNavigation::Joystick2DPlaneNavigation
    (JoystickState* state, JoystickMapping* mapping)
        : JoystickNavigation(state,mapping),
          mLeftVelocity(0.0f),
          mRightVelocity(0.0f),
          mLeftTheta(0.0f),
          mRightTheta(0.0f)
    {}

    Joystick2DPlaneNavigation::~Joystick2DPlaneNavigation() {}

    void Joystick2DPlaneNavigation::update(SceneRuntime* rt)
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

        auto playerObject = rt->getPlayerEntity();
        if (playerObject)
        {
            /*
            Transform playerTransform = playerObject->getTransform();
            Time* time = rt->getProjectRuntime()->getTime();
            Camera* camera = rt->getCamera();
            static mat4 ident(1.0f);
            static vec3 yAxis(0.0f,1.0f,0.0f);
            vec3 translation = playerTransform.getTranslation();
            mat4 mtx = glm::translate(ident,translation.toGLM());
            auto camYaw = camera->getYaw();
            auto totalYaw = getLeftTheta()-camYaw;
            setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
            mtx = glm::rotate(mtx,totalYaw,yAxis);
            mtx = glm::translate(mtx,vec3(time->perSecond(getLeftVelocity()*10.0f),0,0));
            playerObject->setTransform(Transform(mtx));
            */
        }
        else
        {
            LOG_ERROR("Joystick: No player object");
        }
    }

    void Joystick2DPlaneNavigation::show()
    {
        LOG_ERROR("JoystickSNav3D = Lv: {}  Lt: {} Rv: {} Rt: {}",
                    mLeftVelocity, mLeftTheta,
                    mRightVelocity, mRightTheta);
    }

    float Joystick2DPlaneNavigation::getRightVelocity() const
    {
        return mRightVelocity;
    }

    float Joystick2DPlaneNavigation::getLeftTheta() const
    {
        return mLeftTheta;
    }

    float Joystick2DPlaneNavigation::getRightTheta() const
    {
        return mRightTheta;
    }

    float Joystick2DPlaneNavigation::getLeftVelocity() const
    {
        return mLeftVelocity;
    }
}
