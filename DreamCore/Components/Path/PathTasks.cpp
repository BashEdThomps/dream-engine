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
        LOG_CRITICAL("PathUpdateTask: Executing on thread {}",mThreadId);
        if( mPathRuntime->tryLock())
        {
            mPathRuntime->update();
            mPathRuntime->unlock();
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }
}
