#include "PathTasks.h"

#include "PathRuntime.h"
#include "Common/Logger.h"

namespace octronic::dream
{
  PathUpdateTask::PathUpdateTask
  (ProjectRuntime& pr, PathRuntime& rt)
    : Task(pr, "PathUpdateTask"),
      mPathRuntime(rt)
  {
  }

  PathRuntime& PathUpdateTask::getPathRuntime() const
  {
    return mPathRuntime.get();
  }

  void PathUpdateTask::execute()
  {
    LOG_CRITICAL("PathUpdateTask: Executing {}",getID());
    getPathRuntime().update();
    setState(TaskState::TASK_STATE_COMPLETED);
  }
}
