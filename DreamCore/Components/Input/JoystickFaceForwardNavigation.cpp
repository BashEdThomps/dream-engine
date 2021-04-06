#include "JoystickFaceForwardNavigation.h"

#include "JoystickState.h"
#include "JoystickMapping.h"

#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
  JoystickFaceForwardNavigation::JoystickFaceForwardNavigation
  (JoystickState& state,
   JoystickMapping& mapping)
    : JoystickNavigation(state,mapping)
  {

  }

  JoystickFaceForwardNavigation::~JoystickFaceForwardNavigation
  ()
  {

  }

  void
  JoystickFaceForwardNavigation::update
  (SceneRuntime& rt)
  {
    auto& state = mJoystickState.get();
    auto& mapping = mJoystickMapping.get();
    float leftX = state.getAxisData(mapping.AnalogLeftXAxis);
    float leftY = state.getAxisData(mapping.AnalogLeftYAxis);
    //float rightX = state.getAxisData(mJoystickMapping->AnalogRightXAxis);
    //float rightY = state.getAxisData(mJoystickMapping->AnalogRightYAxis);

    auto& pr = rt.getProjectRuntime();
    Transform playerTransform;
    auto& time = pr.getTime();
    static mat4 ident(1.0f);
    vec3 translation = playerTransform.getTranslation();
    mat4 mtx = glm::translate(ident,translation);
    auto totalYaw = 0.0f;
    setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
    mtx = glm::translate(mtx,vec3(time.perSecond(-leftY)*10.0f,0,time.perSecond(leftX)*10.0f));
  }
}
