#include "AssetDefinition.h"

#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Project/ProjectDefinition.h"

#include <string>
#include <sstream>
#include <regex>

using std::regex;
using std::cmatch;

namespace octronic::dream
{
  AssetDefinition::AssetDefinition
  (ProjectDefinition& parent,
   const json &jsonDef)
    : Definition(jsonDef),
      mProjectDefinition(parent)
  {
    LOG_TRACE("AssetDefinition: Constructing {}", getNameAndUuidString());
  }

  AssetType
  AssetDefinition::getAssetType
  ()
  const
  {
    return AssetTypeHelper::GetAssetTypeEnumFromString(mJson[Constants::ASSET_TYPE]);
  }

  void
  AssetDefinition::setType
  (const string& type)
  {
    mJson[Constants::ASSET_TYPE] = type;
  }

  string
  AssetDefinition::getType
  ()
  const
  {
    if (mJson.find(Constants::ASSET_TYPE) == mJson.end())
    {
      return "";
    }
    return mJson[Constants::ASSET_TYPE];
  }

  void
  AssetDefinition::setFormat
  (const string& format)
  {
    mJson[Constants::ASSET_FORMAT] = format;
  }

  string
  AssetDefinition::getFormat
  ()
  const
  {
    if (mJson.find(Constants::ASSET_FORMAT) == mJson.end())
    {
      return "";
    }
    return mJson[Constants::ASSET_FORMAT];
  }

  bool
  AssetDefinition::hasType
  (AssetType type)
  const
  {
    return getType() == AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);
  }


  bool AssetDefinition::hasType(string type)
  const
  {
    return getType().compare(type) == 0;
  }

  ProjectDefinition&
  AssetDefinition::getProject
  ()
  const
  {
    return mProjectDefinition;
  }

  void
  AssetDefinition::duplicateInto
  (AssetDefinition& newDef)
  {
    newDef.mJson = mJson;
    newDef.setUuid(Uuid::RandomUuid());
    string name = newDef.getName();
    regex numRegex("(\\d+)$");
    cmatch match;
    string resultStr;
    auto num = -1;

    if (regex_search(name.c_str(),match,numRegex))
    {
      resultStr = match[0].str();
      num = atoi(resultStr.c_str());
    }

    if (num > -1)
    {
      num++;
      name = name.substr(0,name.length()-resultStr.length());
      name.append(std::to_string(num));
      newDef.setName(name);
    }
    else
    {
      newDef.setName(getName()+".1");
    }
  }
}
