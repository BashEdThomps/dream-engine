#pragma once

#include "../../TaskManager/Task.h"
#include "../Scroller/ScrollerRuntime.h"

namespace Dream
{
   class ScrollerUpdateTask : public Task
   {
   protected:
       ScrollerRuntime* mScrollerRuntime;
   public:
        inline ScrollerUpdateTask(ScrollerRuntime* rt)
            : Task(), mScrollerRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("ScrollerUpdateTask");
            #endif
            rt->setUpdateTask(this);
        }

        inline
        ~ScrollerUpdateTask() {}

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mScrollerRuntime->tryLock())
            {
                mScrollerRuntime->update();
                mScrollerRuntime->setUpdateTask(nullptr);
                mScrollerRuntime->unlock();
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
        }
   };
}
