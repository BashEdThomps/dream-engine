#include "Runtime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;

namespace octronic::dream
{
    Runtime::Runtime
    (const weak_ptr<Definition>& def)
        : mDefinition(def)
	{
        if (auto defLock = mDefinition.lock())
        {
			mUuid = defLock->getUuid();
			mName = defLock->getName();
        }
        else
        {
            mUuid = Uuid::INVALID;
			mName = "";
        }
    }

    Runtime::~Runtime
    ()
    {

    }

    UuidType
    Runtime::getUuid
    ()
    const
    {
        return mUuid;
    }

    string
    Runtime::getUuidString
    ()
    const
    {
        return std::to_string(mUuid);
    }

    void
    Runtime::setUuid
    (UuidType uuid)
    {
        mUuid = uuid;
    }

    bool
    Runtime::hasUuid
    (UuidType uuid)
    const
    {
        return mUuid == uuid;
    }

    string
    Runtime::getName
    ()
    const
    {
        return mName;
    }

    void
    Runtime::setName
    (const string& name)
    {
        mName = name;
    }

    bool
    Runtime::hasName
    (const string& name)
    const
    {
        return getName().compare(name) == 0;
    }

    string
    Runtime::getNameAndUuidString
    ()
    const
    {
        stringstream ss;
        ss << "[" << getName() << " : " << getUuid() << "]";
        return ss.str();
    }

    weak_ptr<Definition>
    Runtime::getDefinition
    ()
    const
    {
        return mDefinition;
    }
}
