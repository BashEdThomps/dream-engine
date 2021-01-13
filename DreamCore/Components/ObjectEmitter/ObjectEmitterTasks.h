#pragma once

#include "TaskManager/Task.h"

namespace Dream
{
    class ObjectEmitterRuntime;

    class ObjectEmitterUpdateTask : public Task
    {
       ObjectEmitterRuntime* mRuntime;
    public:
       ObjectEmitterUpdateTask(ObjectEmitterRuntime* rt);
       void execute() override;
    };
}
