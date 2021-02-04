#pragma once

namespace octronic::dream
{
	enum TaskState
    {
        TASK_STATE_CONSTRUCTED,  // Constructed but not yet queued
        TASK_STATE_QUEUED,       // Constructed and queued for execution
        TASK_STATE_ACTIVE,       // Currently being executed
        TASK_STATE_FAILED,       // Execution has failed
        TASK_STATE_COMPLETED     // Execution has completed
    };
}
