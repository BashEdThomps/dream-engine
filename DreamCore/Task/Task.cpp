#include "Task.h"

#include <algorithm>
#include <sstream>
#include "Common/Logger.h"

using std::find;
using std::stringstream;
using std::to_string;

namespace octronic::dream
{
  // Static ==================================================================

  int Task::TaskIDGenerator = 0;
  const int Task::INVALID_TASK_ID = -1;

  // Task ====================================================================

  Task::Task(ProjectRuntime& pr,const string& taskName)
    : mProjectRuntime(pr),
      mID(TaskIDGenerator++),
      mName(taskName),
      mState(TASK_STATE_QUEUED)
  {
    mID = taskIDGenerator();
  }

  int
  Task::getID
  ()
  const
  {
    return mID;
  }

  bool
  Task::operator==
  (Task& other)
  const
  {
    return mID == other.getID();
  }

  string
  Task::getName
  ()
  const
  {
    return mName;
  }

  void
  Task::setState
  (const TaskState& s)
  {
    mState = s;
    LOG_TRACE("Task: {} is changing state to {}",getName(), TaskTaskStateToString(getState()));
  }

  TaskState
  Task::getState
  ()
  const
  {
    return mState;
  }

  int
  Task::taskIDGenerator
  ()
  {
    return TaskIDGenerator++;
  }

  bool
  Task::hasState
  (const TaskState& s)
  const
  {
    return mState == s;
  }

  string
  Task::getNameAndIDString
  ()
  const
  {
    stringstream ss;
    ss << "[" << to_string(getID()) << "]" << getName();
    return ss.str();
  }


  ProjectRuntime&
  Task::getProjectRuntime
  ()
  const
  {
    return mProjectRuntime.get();
  }


  // DestructionTask =====================================================

  DestructionTask::DestructionTask
  (ProjectRuntime& pr, const string& taskName)
    : Task(pr, taskName)
  {

  }
}
