#include "AnimationRuntime.h"

#include "Components/Task/Task.h"
#include "Common/Logger.h"

namespace octronic::dream
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
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }
}
