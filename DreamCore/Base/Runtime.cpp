#include "Runtime.h"

namespace octronic::dream
{
    Runtime::Runtime(const string& className, Definition* def)
        : LockableObject(className),
          mDefinition(def),
          mUuid(def == nullptr ? 0 : def->getUuid()),
          mName(def == nullptr ? "" : def->getName()) {}

    Runtime::~Runtime()
    {

    }

    UuidType Runtime::getUuid() const
    {
        return mUuid;
    }

    string Runtime::getUuidString() const
    {
        return std::to_string(mUuid);
    }

    void Runtime::setUuid(UuidType uuid)
    {
        mUuid = uuid;
    }

    bool Runtime::hasUuid(UuidType uuid) const
    {
        return mUuid == uuid;
    }

    string Runtime::getName() const
    {
        return mName;
    }

    void Runtime::setName(const string& name)
    {
        mName = name;
    }

    bool Runtime::hasName(const string& name) const
    {
        return getName().compare(name) == 0;
    }

    string Runtime::getNameAndUuidString()
    {
        stringstream ss;
        ss << "[" << getName() << " : " << getUuid() << "]";
        return ss.str();
    }

    Definition* Runtime::getDefinition()
    {
        return mDefinition;
    }
}
