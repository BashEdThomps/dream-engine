#pragma once

#include "Base/ThreadLockFailedException.h"

#include <mutex>
#include <string>

using std::string;

namespace octronic::dream
{
    typedef  std::recursive_mutex dream_mutex_type;
	typedef  std::lock_guard<dream_mutex_type> dream_lock_type;
    /**
     * @brief LockableObject is an Abstract class that is inherited to create
     * object-level mutual exclusion in a multi-threaded environment.
     *
     * Mutex Practice:
     * Public (non-static, non-const, non-ctor, non-dtor) methods should use the pattern:
     *
     * void somePublicMethod() {
     *     if (dreamTryLock())) {
     *          dreamLock();
     *     	    // Do Work
     *     }
     *     dreamElseLockFailed
     * }
     *
     * to lock the object for the current thread. Protected and private methods
     * DO NOT need to lock the mutex, as they are not accessed from outside of
     * the class. Any access to a protected/private method will be done though a
     * public method, which will lock the object for them.
     */
    class LockableObject
    {

    public:
        LockableObject(const string& className);
        virtual  ~LockableObject();
    protected:
        dream_mutex_type& getMutex();
    private:
        dream_mutex_type mMutex;
        string mClassName;
    };
}

#define dreamLock() const dream_lock_type lg(getMutex(), std::adopt_lock)
#define dreamTryLock() getMutex().try_lock()
#define dreamElseLockFailed else { throw ThreadLockFailedException(); }
//#define dreamWaitForLock() dream_lock_type lg(getMutex())
