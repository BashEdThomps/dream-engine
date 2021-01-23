#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
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
