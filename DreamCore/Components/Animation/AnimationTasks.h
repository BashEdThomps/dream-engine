#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    class AnimationRuntime;

   class AnimationUpdateTask : public Task
   {
   protected:
        AnimationRuntime* mAnimationRuntime;
   public:
        AnimationUpdateTask(AnimationRuntime* rt);
        void execute();
   };
}
