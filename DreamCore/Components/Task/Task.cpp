#include "Task.h"

#include <algorithm>
#include <sstream>
#include "Common/Logger.h"

using std::find;
using std::stringstream;
using std::unique_lock;
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
        : LockableObject("Task"),
          mTaskID(TaskIDGenerator++),
          mThreadID(INVALID_THREAD_ID),
          mDeferralCount(0),
          mTaskName(taskName)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mTaskID = taskIDGenerator();
        mWaitingFor.clear();
        mWaitingForMe.clear();
        clearState();
    }

    Task::~Task()
    {}

    int Task::getTaskID()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mTaskID;
    }

    string Task::getDebugString()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        stringstream ss;
        ss << "["
		   << (getThreadID() == INVALID_THREAD_ID ? "NONE" : to_string(getThreadID()))
           << "." << getTaskID() << "]"
           << getTaskName();
        return ss.str();
    }


    bool Task::operator==(Task& other)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        return mTaskID == other.getTaskID();
    }

    string Task::getTaskName()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mTaskName;
    }

    int Task::getThreadID()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mThreadID;
    }

    void Task::incrementDeferralCount()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mDeferralCount++;
        LOG_INFO("Task: {} Deferred for {} time", getDebugString(), mDeferralCount);
        //assert(mDeferralCount < 1000);
    }

    void Task::setThreadID(int t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        assert(t > INVALID_THREAD_ID);
        mThreadID = t;
    }

    unsigned int Task::getDeferralCount()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mDeferralCount;
    }

    void Task::clearState()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mThreadID = INVALID_THREAD_ID;
        mTaskState = TaskState::TASK_STATE_CONSTRUCTED;
        mDeferralCount = 0;
    }

    void Task::clearDependency(Task* t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("Task: {} clearing dependency on {}", getDebugString(), t->getDebugString());

        auto itr = std::find(mWaitingFor.begin(), mWaitingFor.end(), t);
        if (itr != mWaitingFor.end())
        {
            mWaitingFor.erase(itr);
        	LOG_TRACE("Task: {} has {} dependencies left", getDebugString(), mWaitingFor.size());
        }
        else
        {
            LOG_CRITICAL("Task: *** WAIT, WHAT!!! *** {} was not waiting for 'dependency' {}",getDebugString(),t->getDebugString());
            assert(false);
        }
    }

    void Task::notifyTasksWaitingForMe()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
    	LOG_TRACE("Task: {} is notifying dependant it's {} dependants", getDebugString(), mWaitingForMe.size());
        for (Task* t : mWaitingForMe)
        {
            LOG_TRACE("Task: {} is notifying dependant task {} of completion", getDebugString(), t->getDebugString());
            t->clearDependency(this);
        }
        mWaitingForMe.clear();
    }

    bool Task::isWaitingForDependencies()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        bool is_waiting = !mWaitingFor.empty();

        if (is_waiting)
        {
            stringstream ss;
            for (Task* t : mWaitingFor)
            {
                ss << "\n\t--> " << t->getDebugString();
            }
            LOG_TRACE("Task: {} is waiting for {} dependencies to finish {}", getDebugString(), mWaitingFor.size(), ss.str());
        }
        return is_waiting;
    }

    void Task::isWaitingForMe(Task* t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        LOG_TRACE("Task: {} is dependant on me: {}",t->getDebugString(), getDebugString());
        mWaitingForMe.push_back(t);
    }

    void Task::dependsOn(Task* t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        LOG_TRACE("Task: {} is dependant on: {}",getDebugString(), t->getDebugString());
        t->isWaitingForMe(this);

        mWaitingFor.push_back(t);
    }

    void Task::setState(TaskState s)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

      	mTaskState = s;
        LOG_TRACE("Task: {} is changing state to {}",getDebugString(), stateToString(getState()));
    }

    TaskState Task::getState()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mTaskState;
    }

    int Task::taskIDGenerator()
    {

        return TaskIDGenerator++;
    }


    // DestructionTask =====================================================

    DestructionTask::DestructionTask(const string& taskName)
        : Task(taskName)
    {

    }
}
