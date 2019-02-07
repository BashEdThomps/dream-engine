#include "../../TaskManager/Task.h"
#include "../Animation/AnimationRuntime.h"

namespace Dream
{
    AnimationUpdateTask::AnimationUpdateTask
    (AnimationRuntime* rt)
        : Task(),
          mAnimationRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("AnimationUpdateTask");
        #endif
    }

    void AnimationUpdateTask::execute()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif
         if(mAnimationRuntime->tryLock())
         {
             mAnimationRuntime->update();
             mAnimationRuntime->unlock();
             mState = TaskState::COMPLETED;
         }
         else
         {
             mState = TaskState::WAITING;
             mDeferralCount++;
         }
    }
}
