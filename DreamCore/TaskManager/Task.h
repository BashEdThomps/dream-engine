#pragma once

#include <vector>
#include <atomic>
#include <mutex>
#include <map>

using std::map;
using std::mutex;
using std::atomic;
using std::vector;

namespace Dream
{

    enum TaskState
    {
        NEW,
        CLEAR,
        QUEUED,
        WAITING,
        ACTIVE,
        COMPLETED,
        EXPIRED
    };

    /**
     * @brief A Task is a nugget of work that can will
     * be dispatched to one of the TaskThread instances to run.
     */
    class Task
    {
        static int TaskID;

        static mutex TaskStatesMutex;
        static map<int,TaskState> TaskStates;

        static mutex WaitingForMutex;
        static map<int,vector<Task*> > WaitingFor;

        static mutex WaitingForMeMutex;
        static map<int,vector<Task*> > WaitingForMe;

    protected:
        int mTaskId;
        int  mThreadId;
        unsigned int  mDeferralCount;

    public:
        Task();
        Task(const Task& other);
        virtual ~Task();
        virtual void execute() = 0;

        void clearState();

        void setThreadId(int t);

        unsigned int getDeferralCount();
        void incrementDeferralCount();

        void setState(const TaskState& s);
        TaskState getState() const;

        void clearDependency(Task* t);
        void notifyDependents();
        bool isWaitingForDependencies();

        void dependsOn(Task* t);

        bool operator==(const Task& other);

        int getTaskId() const;
    };

    /**
     * @brief The DestructionTask class is used for tasks that will exist beyond
     * their owner's scope. These tasks are copied into the TaskThread's Execution
     * Queue instead of being called by pointer.
     *
     * Must be stacked as shared pointers
     */
    class DestructionTask : public Task {};
}
