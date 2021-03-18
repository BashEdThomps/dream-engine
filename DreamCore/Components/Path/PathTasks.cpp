#include "PathTasks.h"

#include "PathRuntime.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    PathUpdateTask::PathUpdateTask(const shared_ptr<ProjectRuntime>& pr, PathRuntime* rt)
        : Task(pr, "PathUpdateTask"),
          mPathRuntime(rt)
    {
    }

    void PathUpdateTask::execute()
    {
        LOG_CRITICAL("PathUpdateTask: Executing {}",getID());
		mPathRuntime->update();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
