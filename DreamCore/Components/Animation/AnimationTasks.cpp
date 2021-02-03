#include "AnimationRuntime.h"

#include "Components/Task/Task.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    AnimationUpdateTask::AnimationUpdateTask
    (AnimationRuntime* rt)
        : Task("AnimationUpdateTask"),
          mAnimationRuntime(rt)
    {
    }

    void AnimationUpdateTask::execute()
    {
        LOG_CRITICAL("AnimationUpdateTask: Executing on thread {}",getThreadID());
		mAnimationRuntime->update();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
