#include "LockableObject.h"

#include "Common/Logger.h"

namespace octronic::dream
{
    LockableObject::LockableObject(const string& className)
        : mClassName(className)
    {

    }

    LockableObject::~LockableObject()
    {

    }

    dream_mutex_type& LockableObject::getMutex()
    {
        return mMutex;
    }
}
