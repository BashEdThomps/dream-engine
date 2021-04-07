#pragma once

#include "JoystickState.h"
#include "JoystickMapping.h"

#include <glm/vec2.hpp>
#include <functional>

using glm::vec2;
using std::reference_wrapper;

namespace octronic::dream
{
  class SceneRuntime;
  class JoystickNavigation
  {
  public:
    JoystickNavigation(JoystickState& state, JoystickMapping& mapping);
    virtual ~JoystickNavigation();
    virtual void update(SceneRuntime& rt) = 0;
    void setHeading(const vec2& h);
    vec2 getHeading();
  protected:
    reference_wrapper<JoystickState> mJoystickState;
    reference_wrapper<JoystickMapping> mJoystickMapping;
    vec2 mHeading;
  };
}
