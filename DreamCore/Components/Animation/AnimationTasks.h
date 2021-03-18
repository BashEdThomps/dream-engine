#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class AnimationRuntime;

   class AnimationUpdateTask : public Task
   {
   protected:
        weak_ptr<AnimationRuntime> mAnimationRuntime;
   public:
        AnimationUpdateTask(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<AnimationRuntime>& rt);
        void execute();
   };
}
