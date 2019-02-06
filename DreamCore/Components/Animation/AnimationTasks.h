#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    class AnimationRuntime;

   class AnimationUpdateTask : public Task
   {
   protected:
        AnimationRuntime* mAnimationRuntime;
   public:
        AnimationUpdateTask(AnimationRuntime* rt);
        ~AnimationUpdateTask();
        void execute();
   };
}
