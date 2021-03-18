#include "Joystick2DPlaneNavigation.h"

#include "Math/Transform.h"
#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"
#include <glm/vec3.hpp>

using glm::vec3;

namespace octronic::dream
{
  Joystick2DPlaneNavigation::Joystick2DPlaneNavigation
  (const weak_ptr<JoystickState>& state,
   const weak_ptr<JoystickMapping>& mapping)
    : JoystickNavigation(state,mapping),
      mLeftVelocity(0.0f),
      mRightVelocity(0.0f),
      mLeftTheta(0.0f),
      mRightTheta(0.0f)
  {}

  Joystick2DPlaneNavigation::~Joystick2DPlaneNavigation
  ()
  {}

  void
  Joystick2DPlaneNavigation::update
  (const weak_ptr<SceneRuntime>& rt)
  {
    if (auto rtLock = rt.lock())
    {

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
      Transform playerTransform;
      if (auto prLock = rtLock->getProjectRuntime().lock())
      {
        if (auto time = prLock->getTime().lock())
        {
          if (auto camera = rtLock->getCamera().lock())
          {
            static mat4 ident(1.0f);
            static vec3 yAxis(0.0f,1.0f,0.0f);
            vec3 translation = playerTransform.getTranslation();
            mat4 mtx = glm::translate(ident,translation);
            float camYaw = camera->getTransform().getYaw();
            float totalYaw = getLeftTheta()-camYaw;
            setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
            mtx = glm::rotate(mtx,totalYaw,yAxis);
            mtx = glm::translate(mtx,vec3(time->perSecond(getLeftVelocity()*10.0f),0,0));
          }
        }
      }
    }
  }

  void
  Joystick2DPlaneNavigation::show
  ()
  {
    LOG_ERROR("JoystickSNav3D = Lv: {}  Lt: {} Rv: {} Rt: {}",
              mLeftVelocity, mLeftTheta, mRightVelocity, mRightTheta);
  }

  float
  Joystick2DPlaneNavigation::getRightVelocity
  ()
  const
  {
    return mRightVelocity;
  }

  float
  Joystick2DPlaneNavigation::getLeftTheta
  ()
  const
  {
    return mLeftTheta;
  }

  float
  Joystick2DPlaneNavigation::getRightTheta
  ()
  const
  {
    return mRightTheta;
  }

  float
  Joystick2DPlaneNavigation::getLeftVelocity
  ()
  const
  {
    return mLeftVelocity;
  }
}
