#pragma once

#include "../../TaskManager/Task.h"
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

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
             if(mAnimationRuntime->tryLock())
             {
                 mAnimationRuntime->update();
                 mAnimationRuntime->setUpdateTask(nullptr);
                 mAnimationRuntime->unlock();
                 clearDeferred();
             }
             else
             {
                 setDeferred();
             }
        }
   };
}
