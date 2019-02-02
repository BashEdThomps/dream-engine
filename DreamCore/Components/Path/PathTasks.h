#pragma once

#include "../TaskManager/Task.h"
#include "../Path/PathRuntime.h"

namespace Dream
{
    class PathUpdateTask : public Task
    {
    protected:
        PathRuntime* mPathRuntime;
    public:
        inline PathUpdateTask(PathRuntime* rt)
            : Task(), mPathRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("PathUpdateTask");
            #endif
            rt->setUpdateTask(this);
        }

        inline  ~PathUpdateTask() {}

        inline bool execute()
        {
            mPathRuntime->lock();
            mPathRuntime->update();
            mPathRuntime->clearUpdateTask();
            mPathRuntime->unlock();
            return true;
        }
    };
}
