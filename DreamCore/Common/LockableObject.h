#pragma once

#include "Common/Logger.h"

#include <mutex>
#include <string>

using std::mutex;
using std::string;

namespace octronic::dream
{
    /**
     * @brief LockableObject is an Abstract class that is inherited to create
     * object-level mutual exclusion in a multi-threaded environment.
     *
     * Mutex Practice:
     * Public methods should call:
     *     lock_guard<mutex> lg(getMutex());
     * in their first line to lock the object for the current thread. Protected
     * and private methods DO NOT need to lock the mutex, as they are not accessed
     * from outside of the class. Any access to a protected/private method will
     * be done though a public method, which will lock the object for them.
     */
    class LockableObject
    {

    public:
        LockableObject(const string& className);
        virtual  ~LockableObject();
    protected:
        mutex& getMutex();
    private:
        mutex mMutex;
        string mClassName;
    };
}
