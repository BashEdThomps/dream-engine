#pragma once

#include "../TaskManager/Task.h"
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
            setLogClassName("LogicUpdateTask");
            #endif
            rt->setUpdateTask(this);
        }

        inline
        ~ScrollerUpdateTask() {}

        inline bool
        execute
        ()
        {
             mScrollerRuntime->lock();
             mScrollerRuntime->update();
             mScrollerRuntime->clearUpdateTask();
             mScrollerRuntime->unlock();
             return true;
        }
   };
}
