#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    // Type Wrappers
    class GraphicsComponentTask : public Task
    {
    public:
        GraphicsComponentTask(const string& taskName)
            : Task(taskName)
        {

        }
    };

    class GraphicsComponentDestructionTask : public DestructionTask
    {
    public:
        GraphicsComponentDestructionTask(const string& taskName)
            : DestructionTask(taskName)
        {

        }
    };
}
