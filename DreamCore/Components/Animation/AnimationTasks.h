#pragma once

#include "../TaskManager/Task.h"
#include "../Animation/AnimationRuntime.h"

namespace Dream
{
   class AnimationUpdateTask : public Task
   {
   protected:
        AnimationRuntime* mAnimationRuntime;
   public:
        inline AnimationUpdateTask
        (AnimationRuntime* rt)
            : Task(), mAnimationRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("AnimationUpdateTask");
            #endif
            rt->setUpdateTask(this);
        }

        inline
        ~AnimationUpdateTask
        ()
        {

        }

        inline bool
        execute
        ()
        {
             mAnimationRuntime->lock();
             mAnimationRuntime->update();
             mAnimationRuntime->clearUpdateTask();
             mAnimationRuntime->unlock();
             return true;
        }
   };
}
