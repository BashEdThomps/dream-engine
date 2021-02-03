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

    string GraphicsComponentTask::getDebugString()
    {
        stringstream ss;
        ss <<"[GFX." << getTaskID() << "]" << getTaskName();
        return ss.str();
    }

    // GraphicsComponentDestructionTask ========================================

    GraphicsComponentDestructionTask::GraphicsComponentDestructionTask
    (const string& taskName)
        : DestructionTask(taskName)
    {

    }

    string GraphicsComponentDestructionTask::getDebugString()
    {
        stringstream ss;
        ss <<"[GFX." << getTaskID() << "]" << getTaskName();
        return ss.str();
    }
}
