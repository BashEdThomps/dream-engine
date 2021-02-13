#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class AnimationRuntime;

   class AnimationUpdateTask : public Task
   {
   protected:
        AnimationRuntime* mAnimationRuntime;
   public:
        AnimationUpdateTask(ProjectRuntime* pr, AnimationRuntime* rt);
        void execute();
   };
}
