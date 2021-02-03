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

    mutex& LockableObject::getMutex()
    {
        return mMutex;
    }
}
