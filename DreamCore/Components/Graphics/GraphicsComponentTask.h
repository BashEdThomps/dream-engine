#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    // Type Wrapper
    class GraphicsComponentTask : public Task
    {
    public:
        inline GraphicsComponentTask() : Task() {}
        inline ~GraphicsComponentTask() {}
        inline void execute() {}
    };

    class GraphicsComponentDestructionTask : public DestructionTask
    {
    public:
        inline GraphicsComponentDestructionTask() : DestructionTask() {}
        inline ~GraphicsComponentDestructionTask() {}
        inline void execute() {}
    };

}
