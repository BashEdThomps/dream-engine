#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        atomic<bool> mExpired;
        atomic<int>  mThreadId;
        atomic<bool> mDeferred;
        atomic<int>  mDeferralCount;
    public:
        Task() :
            DreamObject("Task"),
            mExpired(false),
            mThreadId(-1),
            mDeferred(false),
            mDeferralCount(0)
        {}

        virtual ~Task() {}
        virtual void execute() = 0;

        inline bool
        hasExpired
        () const
        {
            return mExpired;
        }

        inline void
        setExpired
        (bool e)
        {
            mExpired = e;
        }

        inline void setThreadId(int t)
        {
            mThreadId = t;
        }

        inline void setDeferred()
        {
            mDeferralCount++;
            mDeferred = true;
        }

        inline bool isDeferred()
        {
            return mDeferred;
        }

        inline void clearDeferred()
        {
            mDeferred = false;
        }

        inline int getDeferralCount()
        {
            return mDeferralCount;
        }
    };
}
