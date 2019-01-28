#pragma once

#include "DreamObject.h"

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
