#include "RuntimeTasks.h"

#include "DeferredLoadRuntime.h"
#include "Common/Logger.h"

using std::to_string;

namespace octronic::dream
{
	RuntimeLoadFromDefinitionTask::RuntimeLoadFromDefinitionTask
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<DeferredLoadRuntime>& runtime)
        : Task(pr,"LoadFromDefinitionTask"),
        mRuntime(runtime)
	{
        LOG_TRACE("RuntimeLoadFromDefTask: Constructing for Runtime: {}", mRuntime.lock()->getNameAndUuidString());
	}

    void RuntimeLoadFromDefinitionTask::execute()
    {
        if (auto rtLock = mRuntime.lock())
        {
			LOG_TRACE("RuntimeLoadFromDefTask: Executing for Runtime: {}", rtLock->getNameAndUuidString());
			if (rtLock->loadFromDefinition())
			{
				setState(TASK_STATE_COMPLETED);
			}
			else
			{
				setState(TASK_STATE_DEFERRED);
			}
        }
    }

    string
    RuntimeLoadFromDefinitionTask::getNameAndIDString
    ()
    const
	{
        stringstream ss;
        ss << "[" << to_string(getID()) << "]" << getName()
           << "(" << mRuntime.lock()->getNameAndUuidString() << ")";
        return ss.str();
    }
}
