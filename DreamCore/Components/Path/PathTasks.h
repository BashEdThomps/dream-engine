#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    class PathRuntime;
    class PathUpdateTask : public Task
    {
    protected:
        PathRuntime* mPathRuntime;
    public:
         PathUpdateTask(PathRuntime* rt);
         void execute();
    };
}
