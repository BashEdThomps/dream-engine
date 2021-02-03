#include "PathTasks.h"

#include "PathRuntime.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    PathUpdateTask::PathUpdateTask(PathRuntime* rt)
        : Task("PathUpdateTask"), mPathRuntime(rt)
    {
    }

    void PathUpdateTask::execute()
    {
        LOG_CRITICAL("PathUpdateTask: Executing on thread {}",getThreadID());
		mPathRuntime->update();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
