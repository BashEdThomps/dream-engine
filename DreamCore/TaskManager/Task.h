#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        atomic<int>  mThreadId;
        atomic<bool> mCompleted;
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

        bool isCompleted();
        void setCompleted(bool a);

        void clearDependency(Task* t);
        void notifyDependents();
        bool isWaitingForDependencies();

        void dependsOn(Task* t);

    };
}
