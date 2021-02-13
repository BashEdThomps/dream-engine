#include "AnimationRuntime.h"

#include "Task/Task.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    AnimationUpdateTask::AnimationUpdateTask
    (ProjectRuntime* pr, AnimationRuntime* rt)
        : Task(pr, "AnimationUpdateTask"),
          mAnimationRuntime(rt)
    {
    }

    void AnimationUpdateTask::execute()
    {
        LOG_CRITICAL("AnimationUpdateTask: Executing task {}",getID());
		mAnimationRuntime->update();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
