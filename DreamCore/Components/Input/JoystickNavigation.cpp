#include "JoystickNavigation.h"

namespace octronic::dream
{
  JoystickNavigation::JoystickNavigation
  (JoystickState& state,
   JoystickMapping& mapping)
    : mJoystickState(state), mJoystickMapping(mapping)
  {

  }

  JoystickNavigation::~JoystickNavigation
  ()
  {}

  void
  JoystickNavigation::setHeading
  (const vec2& h)
  {
    mHeading = h;
  }

  vec2
  JoystickNavigation::getHeading
  ()
  {
    return mHeading;
  }
}
