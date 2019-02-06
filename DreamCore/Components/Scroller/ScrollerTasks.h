#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
   class ScrollerRuntime;

   class ScrollerUpdateTask : public Task
   {
   protected:
       ScrollerRuntime* mScrollerRuntime;
   public:
       ScrollerUpdateTask(ScrollerRuntime* rt);
       void execute();
   };
}
