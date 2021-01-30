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
        if (mComponent->tryLock())
        {
            LOG_TRACE("InputPollDataTask: Executing on worker {}",mThreadId);
            mComponent->pollData();
            mComponent->unlock();
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
          	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
            setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }

    InputExecuteScriptTask::InputExecuteScriptTask
    (InputComponent* cp)
        : Task("InputExecuteScriptTask"), mComponent(cp)
    {
    }

    void InputExecuteScriptTask::execute()
    {
        LOG_TRACE("InputExecuteScriptTask: Executing on thread {}",mThreadId);

        if (mComponent->tryLock())
        {
            LOG_TRACE("InputExecuteScriptTask: Executing...");

            if (mComponent->executeInputScript())
            {
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
                setState(TaskState::TASK_STATE_WAITING);
                mDeferralCount++;
            }
            mComponent->unlock();
        }
        else
        {
          	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
        	setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }
}
