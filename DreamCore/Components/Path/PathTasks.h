#pragma once

#include "../../TaskManager/Task.h"
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

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if( mPathRuntime->tryLock())
            {
                mPathRuntime->update();
                mPathRuntime->setUpdateTask(nullptr);
                mPathRuntime->unlock();
                clearDeferred();
            }
            else
            {
                setDeferred();
            }
        }
    };
}
