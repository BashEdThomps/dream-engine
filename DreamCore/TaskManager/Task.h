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
    protected:
        atomic<int>  mThreadId;
        atomic<TaskState> mState;
        atomic<unsigned int>  mDeferralCount;

        mutex mWaitingForMutex;
        vector<Task*> mWaitingFor;

        mutex mWaitingForMeMutex;
        vector<Task*> mWaitingForMe;

    public:
        Task();
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

    };
}
