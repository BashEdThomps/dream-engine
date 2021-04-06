#include "Runtime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
  Runtime::Runtime
  (Definition& def)
    : mDefinition(def)
  {
    mUuid = def.getUuid();
    mName = def.getName();
  }


  Runtime::~Runtime
  ()
  {

  }


  bool Runtime::operator==(const Runtime &other)
  {
    return mUuid == other.mUuid;
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

  Definition&
  Runtime::getDefinition
  ()
  const
  {
    return mDefinition.get();
  }
}
