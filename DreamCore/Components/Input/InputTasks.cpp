#include "InputTasks.h"

#include "InputComponent.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"

namespace octronic::dream
{
    InputPollDataTask::InputPollDataTask
    (InputComponent* cp)
        : Task("InputPollDataTask"), mComponent(cp)
    {
    }

    void
    InputPollDataTask::execute
    ()
    {
		LOG_TRACE("InputPollDataTask: Executing on worker {}",getThreadID());
		mComponent->pollData();
		setState(TaskState::TASK_STATE_COMPLETED);
    }

    InputExecuteScriptTask::InputExecuteScriptTask
    (InputComponent* cp)
        : Task("InputExecuteScriptTask"), mComponent(cp)
    {
    }

    void InputExecuteScriptTask::execute()
    {
        LOG_TRACE("InputExecuteScriptTask: Executing on thread {}",getThreadID());

		if (mComponent->executeInputScript())
		{
			setState(TaskState::TASK_STATE_COMPLETED);
		}
		else
		{
			incrementDeferralCount();
		}
    }
}
