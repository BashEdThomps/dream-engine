#include "PathTasks.h"
#include "../Path/PathRuntime.h"

namespace Dream
{
    PathUpdateTask::PathUpdateTask(PathRuntime* rt)
    : Task(), mPathRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("PathUpdateTask");
        #endif
    }

    void PathUpdateTask::execute()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif
        if( mPathRuntime->tryLock())
        {
            mPathRuntime->update();
            mPathRuntime->unlock();
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
            mDeferralCount++;
        }
    }
}
