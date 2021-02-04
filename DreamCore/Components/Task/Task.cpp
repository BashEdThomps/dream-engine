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
    const int Task::INVALID_THREAD_ID = -1;
    const int Task::INVALID_TASK_ID = -1;

    string Task::stateToString(TaskState state)
    {
        switch(state)
        {
            case TASK_STATE_CONSTRUCTED: return "Constructed";
            case TASK_STATE_QUEUED:      return "Queued";
            case TASK_STATE_ACTIVE:      return "Active";
            case TASK_STATE_COMPLETED:   return "Completed";
            case TASK_STATE_FAILED:      return "Failed";
            default:                     return "Unknown?";
        }
    }

    // Task ====================================================================

    Task::Task(const string& taskName)
        :
          mTaskID(TaskIDGenerator++),
          mThreadID(INVALID_THREAD_ID),
          mDeferralCount(0),
          mTaskName(taskName)
    {
        mTaskID = taskIDGenerator();
        mWaitingFor.clear();
        mWaitingForMe.clear();
        clearState();
    }

    Task::~Task()
    {}

    int Task::getTaskID() const
    {
        return mTaskID;
    }

    bool Task::operator==(Task& other)
    {
        return mTaskID == other.getTaskID();
    }

    string Task::getTaskName() const
    {
        return mTaskName;
    }

    int Task::getThreadID() const
    {
        return mThreadID;
    }

    void Task::incrementDeferralCount()
    {
        mDeferralCount++;
        LOG_INFO("Task: {} Deferred for {} time", getTaskName(), mDeferralCount);
    }

    void Task::setThreadID(int t)
    {
        assert(t > INVALID_THREAD_ID);
        mThreadID = t;
    }

    unsigned int Task::getDeferralCount() const
    {
        return mDeferralCount;
    }

    void Task::clearState()
    {
        mThreadID = INVALID_THREAD_ID;
        mTaskState = TaskState::TASK_STATE_CONSTRUCTED;
        mDeferralCount = 0;
    }

    void Task::clearDependency(Task* t)
    {

        LOG_TRACE("Task: {} clearing dependency on {}", getTaskName(), t->getTaskName());

        auto itr = std::find(mWaitingFor.begin(), mWaitingFor.end(), t);
        if (itr != mWaitingFor.end())
        {
            mWaitingFor.erase(itr);
            LOG_TRACE("Task: {} has {} dependencies left", getTaskName(), mWaitingFor.size());
        }
        else
        {
            LOG_CRITICAL("Task: *** WAIT, WHAT!!! *** {} was not waiting for 'dependency' {}",getTaskName(),t->getTaskName());
            assert(false);
        }
    }

    void Task::notifyTasksWaitingForMe()
    {
        LOG_TRACE("Task: {} is notifying dependant it's {} dependants", getTaskName(), mWaitingForMe.size());
        for (Task* t : mWaitingForMe)
        {
            LOG_TRACE("Task: {} is notifying dependant task {} of completion", getTaskName(), t->getTaskName());
            t->clearDependency(this);
        }
        mWaitingForMe.clear();
    }

    bool Task::isWaitingForDependencies()
    {

        bool is_waiting = !mWaitingFor.empty();

        if (is_waiting)
        {
            stringstream ss;
            for (Task* t : mWaitingFor)
            {
                ss << "\n\t--> " << t->getTaskName();
            }
            LOG_TRACE("Task: {} is waiting for {} dependencies to finish {}", getTaskName(), mWaitingFor.size(), ss.str());
        }
        return is_waiting;
    }

    void Task::isWaitingForMe(Task* t)
    {
        LOG_TRACE("Task: {} is dependant on me: {}",t->getTaskName(), getTaskName());
        mWaitingForMe.push_back(t);
    }

    void Task::dependsOn(Task* t)
    {
        LOG_TRACE("Task: {} is dependant on: {}",getTaskName(), t->getTaskName());
        t->isWaitingForMe(this);

        mWaitingFor.push_back(t);
    }

    void Task::setState(TaskState s)
    {
        mTaskState = s;
        LOG_TRACE("Task: {} is changing state to {}",getTaskName(), stateToString(getState()));
    }

    TaskState Task::getState() const
    {
        return mTaskState;
    }

    int Task::taskIDGenerator()
    {
        return TaskIDGenerator++;
    }

    bool Task::readyToPush() const
    {
        return mTaskState == TaskState::TASK_STATE_CONSTRUCTED ||
                mTaskState == TaskState::TASK_STATE_COMPLETED;
    }

    bool Task::hasState(const TaskState& s) const
    {
        return mTaskState == s;
    }


    // DestructionTask =====================================================

    DestructionTask::DestructionTask(const string& taskName)
        : Task(taskName)
    {

    }
}
