#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
  class AnimationRuntime;

  class AnimationUpdateTask : public Task
  {

  public:
    AnimationUpdateTask(ProjectRuntime& pr, AnimationRuntime& rt);
    void execute();
  protected:
    reference_wrapper<AnimationRuntime> mAnimationRuntime;
  };
}
