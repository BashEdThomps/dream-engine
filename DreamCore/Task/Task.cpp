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

    Task::Task(ProjectRuntime* pr, const string& taskName, bool persistent)
        : mProjectRuntime(pr),
          mID(TaskIDGenerator++),
          mName(taskName),
          mPersistent(persistent)
    {
        mID = taskIDGenerator();
        clearState();
    }

    Task::~Task()
    {}

    int Task::getID() const
    {
        return mID;
    }

    bool Task::operator==(Task& other)
    {
        return mID == other.getID();
    }

    string Task::getName() const
    {
        return mName;
    }

    void Task::setState(TaskState s)
    {
        mState = s;
        LOG_TRACE("Task: {} is changing state to {}",getName(), TaskTaskStateToString(getState()));
    }

    TaskState Task::getState() const
    {
        return mState;
    }

    int Task::taskIDGenerator()
    {
        return TaskIDGenerator++;
    }

    bool Task::hasState(const TaskState& s) const
    {
        return mState == s;
    }

    bool Task::isPersistent() const
	{
        return mPersistent;
    }

    void Task::clearState()
    {
        mState = TASK_STATE_QUEUED;
    }

    string Task::getNameAndIDString()
	{
        stringstream ss;
        ss << "[" << to_string(getID()) << "]" << getName();
        return ss.str();
    }

    // DestructionTask =====================================================

    DestructionTask::DestructionTask(ProjectRuntime* pr, const string& taskName)
        : Task(pr, taskName, false)
    {

    }
}
