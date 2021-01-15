#include "AnimationRuntime.h"

#include "TaskManager/Task.h"
#include "Common/Logger.h"

namespace Dream
{
    AnimationUpdateTask::AnimationUpdateTask
    (AnimationRuntime* rt)
        : Task(),
          mAnimationRuntime(rt)
    {
    }

    void AnimationUpdateTask::execute()
    {
        LOG_CRITICAL("AnimationUpdateTask: Executing on thread {}",mThreadId);

        if(mAnimationRuntime->tryLock())
        {
            mAnimationRuntime->update();
            mAnimationRuntime->unlock();
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }
}
