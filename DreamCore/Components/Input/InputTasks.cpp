#include "InputTasks.h"

#include "InputComponent.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"

namespace Dream
{
    InputPollDataTask::InputPollDataTask
    (InputComponent* cp)
        : Task(), mComponent(cp)
    {
    }

    void
    InputPollDataTask::execute
    ()
    {
        if (mComponent->tryLock())
        {
            LOG_CRITICAL("Executing on worker {}",mThreadId);
            mComponent->pollData();
            mComponent->unlock();
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }

    InputExecuteScriptTask::InputExecuteScriptTask
    (InputComponent* cp)
        : Task(), mComponent(cp)
    {
    }

    void InputExecuteScriptTask::execute()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);

        if (mComponent->tryLock())
        {
            LOG_CRITICAL("Executing...");

            if (mComponent->executeInputScript())
            {
                setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
                mDeferralCount++;
            }
            mComponent->unlock();
        }
        else
        {
            mDeferralCount++;
        }
    }
}
