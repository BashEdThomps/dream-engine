#pragma once

#include "../Task.h"

namespace Dream
{
    // Type Wrapper
    class GraphicsComponentTask : public Task
    {
    public:
       virtual bool execute() = 0;
    };
}
