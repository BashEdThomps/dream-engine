#include "Task.h"

#include <algorithm>
#include <sstream>
#include "Common/Logger.h"

using std::find;
using std::stringstream;

namespace octronic::dream
{
    // Static ==================================================================

    int Task::TaskID = 0;

    mutex Task::TaskStatesMutex;
    map<int,TaskState> Task::TaskStates;

    mutex Task::WaitingForMutex;
    map<int,vector<Task*> > Task::WaitingFor;

    mutex Task::WaitingForMeMutex;
    map<int,vector<Task*> > Task::WaitingForMe;

    // Task ====================================================================

    Task::Task(const string& taskName)
        : mTaskId(TaskID++),
          mThreadId(-1),
          mDeferralCount(0),
          mTaskName(taskName)
    {
        clearState();
        TaskStatesMutex.lock();
        if (TaskStates.count(mTaskId) == 0)
        {
            TaskStates[mTaskId] = TASK_STATE_NEW;
        }
        TaskStatesMutex.unlock();

        WaitingForMutex.lock();
        if (WaitingFor.count(mTaskId) == 0)
        {
            WaitingFor[mTaskId] = vector<Task*>();
        }
        WaitingForMutex.unlock();

        WaitingForMeMutex.lock();
        if (WaitingForMe.count(mTaskId) == 0)
        {
            WaitingForMe[mTaskId] = vector<Task*>();
        }
        WaitingForMeMutex.unlock();
    }

    Task::Task(const Task& other)
        : mTaskId(other.mTaskId),
          mThreadId(other.mThreadId),
          mDeferralCount(other.mDeferralCount)
    {

    }

    Task::~Task() {}

    int Task::getTaskId() const
    {
        return mTaskId;
    }


    bool Task::operator==(const Task& other)
    {
        return mTaskId == other.mTaskId;
    }

    string Task::getTaskName() const
    {
        return mTaskName;
    }


    void Task::incrementDeferralCount()
    {
        mDeferralCount++;
    }

    void Task::setThreadId(int t)
    {
        mThreadId = t;
    }

    unsigned int Task::getDeferralCount()
    {
        return mDeferralCount;
    }

    void Task::clearState()
    {
        WaitingForMutex.lock();
        WaitingFor[mTaskId].clear();
        WaitingForMutex.unlock();

        WaitingForMeMutex.lock();
        WaitingForMe[mTaskId].clear();
        WaitingForMeMutex.unlock();

        mThreadId = -1;

        TaskStatesMutex.lock();
        TaskStates[mTaskId] = TaskState::TASK_STATE_NEW;
        TaskStatesMutex.unlock();

        mDeferralCount = 0;
    }

    void Task::clearDependency(Task* t)
    {
        WaitingForMutex.lock();
        auto itr = std::find(WaitingFor[mTaskId].begin(), WaitingFor[mTaskId].end(), t);
        if (itr != WaitingFor[mTaskId].end())
        {
            WaitingFor[mTaskId].erase(itr);
        }
        else
        {
            LOG_CRITICAL(
            	"Task: *** WAIT, WHAT!!! *** {}({})Task was not waiting for target {}({})",
            	mTaskName,mTaskId,t->getTaskName(),t->getTaskId());
        }
        WaitingForMutex.unlock();
    }

    void Task::notifyDependents()
    {
        WaitingForMeMutex.lock();
        for (Task* t : WaitingForMe[mTaskId])
        {
            LOG_TRACE("Task: {}({}) is notifying dependant task {}({}) of completion",
                      mTaskName, mTaskId, t->getTaskName(), t->getTaskId());
            t->clearDependency(this);
        }
        WaitingForMe[mTaskId].clear();
        WaitingForMeMutex.unlock();
    }

    bool Task::isWaitingForDependencies()
    {
        WaitingForMutex.lock();
        bool retval = !WaitingFor[mTaskId].empty();
        if (retval)
        {
            TaskStatesMutex.lock();
            TaskStates[mTaskId] = TaskState::TASK_STATE_WAITING;
            TaskStatesMutex.unlock();
            stringstream ss;

            for (Task* t : WaitingFor[mTaskId])
            {
               ss << "\n\t--> " << t->getTaskName() << "(" << t->getTaskId() << ")";
            }

            LOG_TRACE("Task: {}({})is waiting for {} dependencies to finish {}",
                      mTaskName, mTaskId, WaitingFor[mTaskId].size(), ss.str());
        }
        WaitingForMutex.unlock();
        return retval;
    }

    void Task::dependsOn(Task* t)
    {
        WaitingForMutex.lock();
        if (WaitingFor.count(mTaskId) == 0)
        {
            WaitingFor[mTaskId] = vector<Task*>();
        }
        WaitingFor[mTaskId].push_back(t);
        WaitingForMutex.unlock();

        WaitingForMeMutex.lock();
        if (WaitingForMe.count(t->mTaskId) == 0)
        {
            WaitingForMe[t->mTaskId] = vector<Task*>();
        }
        WaitingForMe[t->mTaskId].push_back(this);
        WaitingForMeMutex.unlock();
    }

    void Task::setState(const TaskState& s)
    {
        TaskStatesMutex.lock();
        TaskStates[mTaskId] = s;
        TaskStatesMutex.unlock();
    }

    TaskState Task::getState() const
    {
        return TaskStates[mTaskId];
    }

    // DestructionTask =====================================================

    DestructionTask::DestructionTask(const string& taskName)
        : Task(taskName)
    {

    }
}
