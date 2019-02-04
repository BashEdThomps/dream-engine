#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    // Type Wrapper
    class GraphicsComponentTask : public Task
    {
    public:
       virtual void execute() = 0;
    };
}
