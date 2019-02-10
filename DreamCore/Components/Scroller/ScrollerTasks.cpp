#include "ScrollerTasks.h"
#include "ScrollerRuntime.h"

namespace Dream
{
        ScrollerUpdateTask::ScrollerUpdateTask(ScrollerRuntime* rt)
            : Task(), mScrollerRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("ScrollerUpdateTask");
            #endif
        }

        void ScrollerUpdateTask::execute()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mScrollerRuntime->tryLock())
            {
                mScrollerRuntime->update();
                mScrollerRuntime->unlock();
                setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
                mDeferralCount++;
            }
        }
}
