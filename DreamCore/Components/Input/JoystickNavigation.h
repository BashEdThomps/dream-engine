#pragma once

#include "JoystickState.h"
#include "JoystickMapping.h"

#include <glm/vec2.hpp>

using glm::vec2;
using std::shared_ptr;
using std::weak_ptr;

namespace octronic::dream
{
  class SceneRuntime;
  class JoystickNavigation
  {
  public:
    JoystickNavigation(
        const weak_ptr<JoystickState>& state,
        const weak_ptr<JoystickMapping>& mapping);
    virtual ~JoystickNavigation();
    virtual void update(const weak_ptr<SceneRuntime>& rt) = 0;
    void setHeading(const vec2& h);
    vec2 getHeading();
  protected:
    shared_ptr<JoystickState> mJoystickState;
    shared_ptr<JoystickMapping> mJoystickMapping;
    vec2 mHeading;
  };
}
