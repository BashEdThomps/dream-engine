#include "RuntimeLoadFromDefinitionTask.h"

#include "Components/AssetRuntime.h"
#include "Common/Logger.h"

using std::to_string;

namespace octronic::dream
{
	RuntimeLoadFromDefinitionTask::RuntimeLoadFromDefinitionTask
    (ProjectRuntime* pr, Runtime* runtime)
        : Task(pr,"LoadFromDefinitionTask"),
        mRuntime(runtime)
	{
        LOG_TRACE("RuntimeLoadFromDefTask: Constructing for Runtime: {}", mRuntime->getNameAndUuidString());
	}

    void RuntimeLoadFromDefinitionTask::execute()
    {
        LOG_TRACE("RuntimeLoadFromDefTask: Executing for Runtime: {}", mRuntime->getNameAndUuidString());
        if (mRuntime->loadFromDefinition())
        {
        	setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    string RuntimeLoadFromDefinitionTask::getNameAndIDString()
	{
        stringstream ss;
        ss << "[" << to_string(getID()) << "]" << getName()
           << "(" << mRuntime->getNameAndUuidString() << ")";
        return ss.str();
    }
}
