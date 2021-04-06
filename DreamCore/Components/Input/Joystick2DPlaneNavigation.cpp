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
  (JoystickState& state,
   JoystickMapping& mapping)
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
  (SceneRuntime& rt)
  {
    auto& state = mJoystickState.get();
    auto& mapping = mJoystickMapping.get();
    float leftX = state.getAxisData(mapping.AnalogLeftXAxis);
    float leftY = state.getAxisData(mapping.AnalogLeftYAxis);
    float rightX = state.getAxisData(mapping.AnalogRightXAxis);
    float rightY = state.getAxisData(mapping.AnalogRightYAxis);

    if (state.clearsDeadzone(leftX) || state.clearsDeadzone(leftY))
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

    if (state.clearsDeadzone(rightX) || state.clearsDeadzone(rightY))
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

    auto& pr = rt.getProjectRuntime();
    auto& time = pr.getTime();
    auto& camera = rt.getCameraRuntime();

    static mat4 ident(1.0f);
    static vec3 yAxis(0.0f,1.0f,0.0f);

    vec3 translation = playerTransform.getTranslation();
    mat4 mtx = glm::translate(ident,translation);

    float camYaw = camera.getTransform().getYaw();
    float totalYaw = getLeftTheta()-camYaw;

    setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
    mtx = glm::rotate(mtx,totalYaw,yAxis);
    mtx = glm::translate(mtx,vec3(time.perSecond(getLeftVelocity()*10.0f),0,0));
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
