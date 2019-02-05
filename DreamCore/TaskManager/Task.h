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
        atomic<int>  mDeferralCount;
        atomic<int>  mWaitingForDependencies;
        vector<Task*> mWaitingForMe;

    public:
        Task() :
            DreamObject("Task"),
            mExpired(false),
            mThreadId(-1),
            mCompleted(false),
            mDeferralCount(0),
            mWaitingForDependencies(0)
        {}

        virtual ~Task() {}
        virtual void execute() = 0;

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

        inline int getDeferralCount()
        {
            return mDeferralCount;
        }

        inline void notifyDependents()
        {
           for (auto* t : mWaitingForMe)
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

        inline Task* dependsOn(Task* t)
        {
            if (t!=nullptr)
            {
                mWaitingForDependencies++;
                t->mWaitingForMe.push_back(this);
            }
            return this;
        }

        inline void setCompleted()
        {
            mCompleted = true;
        }
    };
}
