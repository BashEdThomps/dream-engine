#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    // Type Wrapper
    class GraphicsComponentTask : public Task
    {
    public:
        GraphicsComponentTask() : Task()
        {}

        ~GraphicsComponentTask()
        {}
        inline void execute() {}
    };
}
