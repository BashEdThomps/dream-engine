#pragma once

#include "Common/LockableObject.h"

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
        TASK_STATE_CONSTRUCTED,  // Constructed but not yet queued
        TASK_STATE_QUEUED,       // Constructed and queued for execution
        TASK_STATE_ACTIVE,       // Currently being executed
        TASK_STATE_FAILED,       // Execution has failed
        TASK_STATE_COMPLETED     // Execution has completed
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
    class Task : public LockableObject
    {
    public:
        static int TaskIDGenerator;
        const static int INVALID_THREAD_ID;
        const static int INVALID_TASK_ID;

        Task(const string& taskName);
        virtual ~Task();

        virtual void execute() = 0;

        int getTaskID();
        int getThreadID();

        void setThreadID(int t);
        string getTaskName();

        unsigned int getDeferralCount();
        void incrementDeferralCount();

        void clearState();
        void setState(TaskState s);
        TaskState getState();

        void clearDependency(Task* t);
        void notifyTasksWaitingForMe();
        bool isWaitingForDependencies();
        void dependsOn(Task* t);

        bool operator==(Task& other);

        virtual string getDebugString();
        static string stateToString(TaskState);
        static int taskIDGenerator();

    private:
        void isWaitingForMe(Task* t);

    protected:

    private:
        int mTaskID;
        int mThreadID;
        unsigned int  mDeferralCount;
        string mTaskName;
        TaskState mTaskState;
        vector<Task*> mWaitingFor;
        vector<Task*> mWaitingForMe;
    };

    /**
     * @brief The DestructionTask class is used for tasks that need to exist beyond
     * their owner's scope. These tasks are copied into the TaskThread's Execution
     * Queue instead of being called by pointer. They will typically be used for
     * clean-up operations after their parent object has been destroyed, or is otherwise
     * no longer available.
     *
     * Must be instanciated on stack as shared pointers
     */
    class DestructionTask : public Task
    {
    public:
        DestructionTask(const string& taskName);
    };
}
