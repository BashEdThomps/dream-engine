#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        atomic<bool> mExpired;
        atomic<int>  mThreadId;
        atomic<bool> mCompleted;
        atomic<unsigned int>  mDeferralCount;
        atomic<unsigned int>  mWaitingForDependencies;
        vector<Task*> mWaitingForMe;

    public:
        inline Task() :
            DreamObject("Task"),
            mExpired(false),
            mThreadId(-1),
            mCompleted(false),
            mDeferralCount(0),
            mWaitingForDependencies(0)
        {}

        inline ~Task() {}
        inline void execute() {}

        inline void incrementDeferralCount()
        {
            mDeferralCount++;
        }

        inline bool hasExpired() const
        {
            return mExpired;
        }

        inline void setExpired(bool e)
        {
            mExpired = e;
        }

        inline void setThreadId(int t)
        {
            mThreadId = t;
        }

        inline bool isCompleted()
        {
            return mCompleted;
        }

        inline unsigned int getDeferralCount()
        {
            return mDeferralCount;
        }

        inline void notifyDependents()
        {
           for (Task* t : mWaitingForMe)
           {
               #ifdef DREAM_LOG
               getLog()->critical("is notifying dependant {} of completion",t->getClassName());
               #endif
               t->mWaitingForDependencies--;
           }
        }

        inline bool isWaitingForDependencies()
        {
            bool retval = mWaitingForDependencies != 0;
            #ifdef DREAM_LOG
            if (retval)
            {
                getLog()->critical("Waiting for {} dependencies to finish",mWaitingForDependencies);
            }
            #endif
            return retval;
        }

        inline Task& dependsOn(Task& t)
        {
            mWaitingForDependencies++;
            t.mWaitingForMe.emplace_back(this);
            return *this;
        }

        inline void setCompleted()
        {
            mCompleted = true;
        }
    };
}
