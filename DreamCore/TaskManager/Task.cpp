#include "Task.h"

#include <algorithm>

using std::find;

namespace Dream
{
    int Task::TaskID = 0;

    mutex Task::TaskStatesMutex;
    map<int,TaskState> Task::TaskStates;

    mutex Task::WaitingForMutex;
    map<int,vector<Task*> > Task::WaitingFor;

    mutex Task::WaitingForMeMutex;
    map<int,vector<Task*> > Task::WaitingForMe;

    int Task::getTaskId() const
    {
        return mTaskId;
    }

    Task::Task()
        : DreamObject("Task"),
          mTaskId(TaskID++),
            mThreadId(-1),
            mDeferralCount(0)
        {
            clearState();
            TaskStatesMutex.lock();
            if (TaskStates.count(mTaskId) == 0)
            {
                TaskStates[mTaskId] = CLEAR;
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
            : DreamObject ("Task"),
            mTaskId(other.mTaskId),
            mThreadId(other.mThreadId),
            mDeferralCount(other.mDeferralCount)
        {

        }

        Task::~Task() {}

        bool Task::operator==(const Task& other)
        {
            return mTaskId == other.mTaskId;
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
            TaskStates[mTaskId] = TaskState::CLEAR;
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
                #ifdef DREAM_LOG
                getLog()->critical("*** WHAT THE F JEFF!!! *** {} was not waiting for {}",getClassName(), t->getClassName());
                #endif
            }
            WaitingForMutex.unlock();
        }

        void Task::notifyDependents()
        {
            WaitingForMeMutex.lock();
           for (Task* t : WaitingForMe[mTaskId])
           {
               #ifdef DREAM_LOG
               getLog()->critical("is notifying dependant {} of completion",t->getClassName());
               #endif
               t->clearDependency(this);
           }
           WaitingForMe[mTaskId].clear();
           WaitingForMeMutex.unlock();
        }

        bool Task::isWaitingForDependencies()
        {
            WaitingForMutex.lock();
            bool retval = !WaitingFor[mTaskId].empty();
            #ifdef DREAM_LOG
            if (retval)
            {
                TaskStatesMutex.lock();
                TaskStates[mTaskId] = TaskState::WAITING;
                TaskStatesMutex.unlock();
                getLog()->critical("is waiting for {} dependencies to finish",WaitingFor[mTaskId].size());
            }
            #endif
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
}
