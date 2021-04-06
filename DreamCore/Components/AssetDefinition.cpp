/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
  AssetDefinition::isTypeAnimation
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_ANIMATION;
  }


  bool AssetDefinition::isTypeMaterial()
  const
  {
    return getType() == Constants::ASSET_TYPE_MATERIAL;
  }

  bool
  AssetDefinition::isTypeFont
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_FONT;
  }

  bool
  AssetDefinition::isTypePhysicsObject
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_PHYSICS_OBJECT;
  }

  bool
  AssetDefinition::isTypeTexture
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_TEXTURE;
  }

  bool
  AssetDefinition::isTypePath
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_PATH;
  }

  bool
  AssetDefinition::isTypeAudio
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_AUDIO;
  }

  bool
  AssetDefinition::isTypeModel
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_MODEL;
  }

  bool
  AssetDefinition::isTypeScript
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_SCRIPT;
  }

  bool
  AssetDefinition::isTypeShader
  ()
  const
  {
    return getType() == Constants::ASSET_TYPE_SHADER;
  }

  string
  AssetDefinition::getGroup
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_GROUP) == mJson.end())
    {
      return Constants::ASSET_ATTR_GROUP_DEFAULT;
    }
    return mJson[Constants::ASSET_ATTR_GROUP];
  }

  void
  AssetDefinition::setGroup
  (const string& group)
  {
    mJson[Constants::ASSET_ATTR_GROUP] = group;
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
    newDef.setUuid(Uuid::generateUuid());
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
