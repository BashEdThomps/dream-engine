#pragma once

#include <mutex>

using std::mutex;

namespace octronic::dream
{
    class LockableObject
    {
    private:
        mutex mMutex;
    public:
        inline void lock()
        {
            mMutex.lock();
        }

        inline bool tryLock()
        {
            return mMutex.try_lock();
        }

        inline void unlock()
        {
            mMutex.unlock();
        }

        inline LockableObject() {}
        virtual inline ~LockableObject() {}
    };
}
