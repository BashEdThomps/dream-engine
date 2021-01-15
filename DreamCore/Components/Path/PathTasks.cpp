#include "PathTasks.h"

#include "PathRuntime.h"
#include "Common/Logger.h"

namespace Dream
{
    PathUpdateTask::PathUpdateTask(PathRuntime* rt)
    : Task(), mPathRuntime(rt)
    {
    }

    void PathUpdateTask::execute()
    {
        LOG_CRITICAL("PathUpdateTask: Executing on thread {}",mThreadId);
        if( mPathRuntime->tryLock())
        {
            mPathRuntime->update();
            mPathRuntime->unlock();
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }
}
