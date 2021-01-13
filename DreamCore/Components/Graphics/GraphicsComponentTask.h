#pragma once

#include "TaskManager/Task.h"

namespace Dream
{
    // Type Wrappers
    class GraphicsComponentTask : public Task {};
    class GraphicsComponentDestructionTask : public DestructionTask {};
}
