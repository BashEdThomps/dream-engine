#include "RuntimeTasks.h"

#include "DeferredLoadRuntime.h"
#include "Common/Logger.h"

using std::to_string;

namespace octronic::dream
{
  RuntimeLoadFromDefinitionTask::RuntimeLoadFromDefinitionTask
  (ProjectRuntime& pr,
   DeferredLoadRuntime& runtime)
    : Task(pr,"LoadFromDefinitionTask"),
      mRuntime(runtime)
  {
    LOG_TRACE("RuntimeLoadFromDefTask: Constructing for Runtime: {}", getRuntime().getNameAndUuidString());
  }

  void RuntimeLoadFromDefinitionTask::execute()
  {
    LOG_TRACE("RuntimeLoadFromDefTask: Executing for Runtime: {}", getRuntime().getNameAndUuidString());
    if (getRuntime().loadFromDefinition())
    {
      setState(TASK_STATE_COMPLETED);
    }
    else
    {
      setState(TASK_STATE_DEFERRED);
    }
  }

  string
  RuntimeLoadFromDefinitionTask::getNameAndIDString
  ()
  const
  {
    stringstream ss;
    ss << "[" << to_string(getID()) << "]" << getName()
       << "(" << getRuntime().getNameAndUuidString() << ")";
    return ss.str();
  }

  DeferredLoadRuntime&
  RuntimeLoadFromDefinitionTask::getRuntime
  ()
  const
  {
  	return mRuntime.get();
  }
}
