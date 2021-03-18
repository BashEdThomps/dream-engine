#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class PathRuntime;
    class PathUpdateTask : public Task
    {
    protected:
        PathRuntime* mPathRuntime;
    public:
         PathUpdateTask(const shared_ptr<ProjectRuntime>& pr, PathRuntime* rt);
         void execute();
    };
}
