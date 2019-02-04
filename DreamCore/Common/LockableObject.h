#pragma once

#include "DreamObject.h"
#include <mutex>

using std::mutex;

namespace Dream
{
    class LockableObject : public DreamObject
    {
    private:
        mutex mMutex;
    public:
        inline void
        lock
        ()
        {
            mMutex.lock();
        }

        inline bool tryLock()
        {
            return mMutex.try_lock();
        }

        inline void
        unlock
        ()
        {
            mMutex.unlock();
        }

        inline LockableObject(string name) : DreamObject(name) {}
        virtual inline ~LockableObject() {}
    };
}
