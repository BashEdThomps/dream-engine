#include "Task.h"

namespace Dream
{
        Task::Task()
            : DreamObject("Task"),
            mActive(false),
            mExpired(false),
            mThreadId(-1),
            mCompleted(false),
            mDeferralCount(0),
            mWaitingForDependencies(0)
        {}

        Task::~Task() {}

        void Task::incrementDeferralCount()
        {
            mDeferralCount++;
        }

        bool Task::hasExpired() const
        {
            return mExpired;
        }

        void Task::setExpired(bool e)
        {
            mExpired = e;
        }

        void Task::setThreadId(int t)
        {
            mThreadId = t;
        }

        bool Task::isCompleted()
        {
            return mCompleted;
        }

        unsigned int Task::getDeferralCount()
        {
            return mDeferralCount;
        }

        void Task::notifyDependents()
        {
           for (Task* t : mWaitingForMe)
           {
               #ifdef DREAM_LOG
               getLog()->critical("is notifying dependant {} of completion",t->getClassName());
               #endif
               t->mWaitingForDependencies--;
           }
        }

        bool Task::isWaitingForDependencies()
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

        void Task::dependsOn(Task* t)
        {
            mWaitingForDependencies++;
            t->mWaitingForMe.emplace_back(this);
        }

        void Task::setCompleted(bool a)
        {
            mCompleted = a;
        }

        bool Task::isActive() const
        {
            return mActive;
        }

        void Task::setActive(bool a)
        {
            mActive = a;
        }

        void Task::clearDependencies()
        {
           mWaitingForMe.clear();
           mWaitingForDependencies = 0;
        }
}
