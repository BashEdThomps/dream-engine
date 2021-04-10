#include "Component.h"
#include "Time.h"

namespace octronic::dream
{

  Component::Component
  ()
  {}

  Component::Component(ProjectRuntime& pr)
    : mProjectRuntime(pr)
  {

  }

  void
  Component::setProjectRuntime
  (ProjectRuntime& pr)
  {
    mProjectRuntime = pr;
  }

  ProjectRuntime&
  Component::getProjectRuntime
  ()
  {
    return mProjectRuntime.value().get();
  }
}
