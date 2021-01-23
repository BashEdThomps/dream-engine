#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    // Type Wrappers
    class GraphicsComponentTask : public Task {};
    class GraphicsComponentDestructionTask : public DestructionTask {};
}
