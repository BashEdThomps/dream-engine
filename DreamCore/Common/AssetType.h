#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

namespace octronic::dream
{
  enum AssetType
  {
    ASSET_TYPE_ENUM_ANIMATION,
    ASSET_TYPE_ENUM_AUDIO,
    ASSET_TYPE_ENUM_FONT,
    ASSET_TYPE_ENUM_MATERIAL,
    ASSET_TYPE_ENUM_MODEL,
    ASSET_TYPE_ENUM_PATH,
    ASSET_TYPE_ENUM_PHYSICS,
    ASSET_TYPE_ENUM_SCRIPT,
    ASSET_TYPE_ENUM_SHADER,
    ASSET_TYPE_ENUM_TEXTURE,
    ASSET_TYPE_ENUM_NONE
  };

  class AssetTypeHelper
  {
  private:
    AssetTypeHelper();
  public:
    static string GetAssetTypeStringFromTypeEnum(AssetType type);
    static AssetType GetAssetTypeEnumFromString(const string &type);
    static string GetAssetTypeReadableNameFromString(const string&);
    static map<AssetType,string> TypesMap;
    static map<AssetType,string> ReadableTypesMap;
  };
}
