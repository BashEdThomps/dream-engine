#include "AnimationRuntime.h"

#include "Task/Task.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    AnimationUpdateTask::AnimationUpdateTask
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<AnimationRuntime>& rt)
        : Task(pr, "AnimationUpdateTask"),
          mAnimationRuntime(rt)
    {
    }

    void AnimationUpdateTask::execute()
    {
        if (auto animLock = mAnimationRuntime.lock())
        {
        	LOG_CRITICAL("AnimationUpdateTask: Executing task {}",getID());
			animLock->update();
			setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
        	setState(TASK_STATE_DEFERRED);
        }
    }
}
