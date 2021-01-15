#include "ScrollerTasks.h"

#include "ScrollerRuntime.h"
#include "Common/Logger.h"

namespace Dream
{
        ScrollerUpdateTask::ScrollerUpdateTask(ScrollerRuntime* rt)
            : Task(),
              mScrollerRuntime(rt)
        {
        }

        void ScrollerUpdateTask::execute()
        {
            LOG_CRITICAL("ScrollerUpdateTask: Executing on thread {}",mThreadId);
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
