#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    class GraphicsComponentTask : public Task
    {
    public:
        GraphicsComponentTask(const string& taskName);
    };

    class GraphicsComponentDestructionTask : public DestructionTask
    {
    public:
        GraphicsComponentDestructionTask(const string& taskName);
    };
}
