#pragma once
#include "../Common/DreamObject.h"
#include <vector>
#include <atomic>
#include <mutex>

using std::mutex;
using std::atomic;
using std::vector;

namespace Dream
{

    enum TaskState
    {
        CLEAR,
        QUEUED,
        WAITING,
        ACTIVE,
        COMPLETED
    };

    class Task : public DreamObject
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

    class DestructionTask : public Task
    {
        inline void execute() {}
    };
}
