#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <map>

using std::map;
using std::mutex;
using std::atomic;
using std::vector;
using std::string;

namespace octronic::dream
{

    enum TaskState
    {
        TASK_STATE_NEW,
        TASK_STATE_QUEUED,
        TASK_STATE_WAITING,
        TASK_STATE_ACTIVE,
        TASK_STATE_COMPLETED,
        TASK_STATE_FAILED,
        TASK_STATE_EXPIRED
    };

    /**
     * @brief Task is the base class of any Runtime Logic thread task, allowing
     * logic to be dispatched to any available thread managed by the TaskManager.
     * A Task is an atomic nugget of work that can be independant or dependant
     * on the completion of other Tasks.
     *
     * *** A Task should ONLY CALL logic functions, not contain them ***
     *
     * *** A DestructionTask MAY implement some logic, as the parent ***
     * *** object may have been destroyed.                           ***
     *
     * Dependencies are specified (and usually resolved) at run time.
     * @see Task::dependsOn
     * @see Task::clearDependency
     * @see Task::notifyDependents
     * @see Task::isWaitingForDependencies
     * @see Task::dependsOn
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
        string mTaskName;

    public:
        Task(const string& taskName);
        Task(const Task& other);
        virtual ~Task();
        virtual void execute() = 0;

        void clearState();

        void setThreadId(int t);
        string getTaskName() const;

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
     * @brief The DestructionTask class is used for tasks that need to exist beyond
     * their owner's scope. These tasks are copied into the TaskThread's Execution
     * Queue instead of being called by pointer. They will typically be used for
     * clean-up operations after their parent object has been destroyed, or is otherwise
     * no longer available.
     *
     * Must be stacked as shared pointers
     */
    class DestructionTask : public Task
    {
    public:
        DestructionTask(const string& taskName);
    };
}
