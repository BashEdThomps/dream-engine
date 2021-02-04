#include "GraphicsComponentTask.h"

#include <sstream>
using std::stringstream;

namespace octronic::dream
{
    // GraphicsComponentTask ===================================================

    GraphicsComponentTask::GraphicsComponentTask(const string& taskName)
        : Task(taskName)
    {

    }

    // GraphicsComponentDestructionTask ========================================

    GraphicsComponentDestructionTask::GraphicsComponentDestructionTask
    (const string& taskName)
        : DestructionTask(taskName)
    {

    }

}
