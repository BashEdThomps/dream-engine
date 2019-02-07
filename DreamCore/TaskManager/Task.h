#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        atomic<bool> mActive;
        atomic<bool> mExpired;
        atomic<int>  mThreadId;
        atomic<bool> mCompleted;
        atomic<unsigned int>  mDeferralCount;
        atomic<unsigned int>  mWaitingForDependencies;
        vector<Task*> mWaitingForMe;

    public:
        Task();
        virtual ~Task();
        virtual void execute() = 0;
        void incrementDeferralCount();
        bool hasExpired() const;
        void setExpired(bool e);
        void setThreadId(int t);
        bool isCompleted();
        unsigned int getDeferralCount();
        void notifyDependents();
        bool isWaitingForDependencies();
        void dependsOn(Task* t);
        void setCompleted(bool a);
        bool isActive() const;
        void setActive(bool);
        void clearDependencies();
    };
}
