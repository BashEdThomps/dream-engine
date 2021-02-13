#pragma once

#include <string>
using std::string;

namespace octronic::dream
{
	enum TaskState
    {
        TASK_STATE_QUEUED,       // Constructed and queued for execution
        TASK_STATE_DEFERRED,     // Will execute next time
        TASK_STATE_COMPLETED,    // Execution has completed
        TASK_STATE_FAILED        // Execution Failed
    };

	static string TaskTaskStateToString(const TaskState& state)
    {
        switch(state)
        {
            case TASK_STATE_QUEUED:    return "Queued";
            case TASK_STATE_DEFERRED:  return "Deferred";
            case TASK_STATE_COMPLETED: return "Completed";
            case TASK_STATE_FAILED:    return "Failed";
        }
    }
}
