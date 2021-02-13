#include "Runtime.h"

using std::make_shared;

namespace octronic::dream
{
    Runtime::Runtime(Definition* def)
        : mDefinitionHandle(def),
          mUuid(def == nullptr ? 0 : def->getUuid()),
          mName(def == nullptr ? "" : def->getName()),
          mLoadFromDefinitionTask(make_shared<RuntimeLoadFromDefinitionTask>(nullptr,this))
{}

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

    Definition* Runtime::getDefinitionHandle()
    {
        return mDefinitionHandle;
    }

    shared_ptr<RuntimeLoadFromDefinitionTask> Runtime::getLoadFromDefinitionTask()
    {
        return mLoadFromDefinitionTask;
    }
}
