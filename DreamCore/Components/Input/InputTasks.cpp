#include "InputTasks.h"

#include "InputComponent.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"

namespace octronic::dream
{
    InputPollDataTask::InputPollDataTask
    (ProjectRuntime* pr, InputComponent* cp)
        : Task(pr, "InputPollDataTask"), mComponent(cp)
    {
    }

    void
    InputPollDataTask::execute
    ()
    {
		LOG_TRACE("InputPollDataTask: Executing {}",getID());
		mComponent->pollData();
		setState(TaskState::TASK_STATE_COMPLETED);
    }

    InputExecuteScriptTask::InputExecuteScriptTask
    (ProjectRuntime* pr, InputComponent* cp)
        : Task(pr, "InputExecuteScriptTask"), mComponent(cp)
    {
    }

    void InputExecuteScriptTask::execute()
    {
        LOG_TRACE("InputExecuteScriptTask: Executing {}",getID());

		if (mComponent->executeInputScript())
		{
			setState(TaskState::TASK_STATE_COMPLETED);
		}
		else
		{
			setState(TaskState::TASK_STATE_FAILED);
		}
    }
}
