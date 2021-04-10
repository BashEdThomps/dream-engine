#include "Definition.h"
#include "Common/Constants.h"

namespace octronic::dream
{

  Definition::Definition
  (const json& data)
    : mJson(data)
  {
  }

  bool
  Definition::operator==
  (const Definition& other) const
  {
    return getUuid() == other.getUuid();
  }

  bool
  Definition::operator!=
  (const Definition& other) const
  {
    return getUuid() != other.getUuid();
  }

  bool
  Definition::operator==
  (const UuidType& other) const
  {
    return getUuid() == other;
  }

  bool
  Definition::operator!=
  (const UuidType& other) const
  {
    return getUuid() != other;
  }

  void Definition::setJson(const json& js)
  {
    mJson = js;
  }

  json
  Definition::getJson
  ()
  {
    return mJson;
  }

  bool
  Definition::hasName(const string& name)
  const
  {
    return getName().compare(name) == 0;
  }

  string
  Definition::getName()
  const
  {
    if (mJson.find(Constants::NAME) == mJson.end())
    {
      return "";
    }
    return mJson[Constants::NAME];
  }

  void
  Definition::setName(const string& name)
  {
    mJson[Constants::NAME] = name;
  }

  bool
  Definition::hasUuid(UuidType uuid)
  const
  {
    return getUuid() == uuid;
  }

  UuidType
  Definition::getUuid()
  const
  {
    if (mJson.find(Constants::UUID) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::UUID];
  }

  void
  Definition::setUuid(UuidType uuid)
  {
    mJson[Constants::UUID] = uuid;
  }

  string
  Definition::getNameAndUuidString()
  const
  {
    stringstream ss;
    ss << "[" << getName() << " : " << getUuid() << "]";
    return ss.str();
  }

  void
  Definition::setGroup
  (const string& group)
  {
    mJson[Constants::GROUP] = group;
  }

  string Definition::getGroup() const
  {
    if (mJson.find(Constants::GROUP) == mJson.end())
    {
      return Constants::DEFAULT_GROUP;
    }
    return mJson[Constants::GROUP];
  }
}
