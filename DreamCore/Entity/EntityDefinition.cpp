/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "EntityDefinition.h"

#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Common/Uuid.h"

#include "EntityRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Project/ProjectDefinition.h"

#include "Components/AssetDefinition.h"
#include "Math/Transform.h"
#include "Math/Vector.h"

#include <regex>

using std::regex;
using std::cmatch;
using std::pair;
using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  EntityDefinition::EntityDefinition
  (const weak_ptr<EntityDefinition>& parent,
   const weak_ptr<SceneDefinition>& sceneDefinition,
   const json &jsonData, bool randomUuid)
    : Definition("EntityDefinition",jsonData),
      mParentEntity(parent),
      mSceneDefinition(sceneDefinition)
  {
    if (randomUuid)
    {
      mJson[Constants::UUID] = Uuid::generateUuid();
      LOG_TRACE( "EntityDefinition: With new UUID",getNameAndUuidString());
    }
    LOG_TRACE( "EntityDefinition: Constructing {}",getNameAndUuidString());
    mJson[Constants::TRANSFORM] = jsonData[Constants::TRANSFORM];
  }

  EntityDefinition::~EntityDefinition
  ()
  {
    LOG_TRACE( "EntityDefinition: Destructing {}", getNameAndUuidString() );
    deleteChildEntityDefinitions();
  }

  int
  EntityDefinition::getChildCount
  ()
  const
  {
    return mChildDefinitions.size();
  }

  Transform
  EntityDefinition::getTransform
  ()
  const
  {
    return Transform(mJson[Constants::TRANSFORM]);
  }

  void
  EntityDefinition::setTransform
  (const Transform& tform)
  {
    mJson[Constants::TRANSFORM] = tform.toJson();
  }

  void
  EntityDefinition::loadChildEntityDefinitions
  (bool randomUuid)
  {
    json childrenArray = mJson[Constants::ENTITY_CHILDREN];
    if (!childrenArray.is_null() && childrenArray.is_array())
    {
      for (const json& childDefinition : childrenArray)
      {
        auto entityDefinition = make_shared<EntityDefinition>(static_pointer_cast<EntityDefinition>(shared_from_this()),mSceneDefinition,childDefinition,randomUuid);
        entityDefinition->loadChildEntityDefinitions(randomUuid);
        mChildDefinitions.push_back(entityDefinition);
      }
    }
  }

  void
  EntityDefinition::deleteChildEntityDefinitions
  ()
  {
    mChildDefinitions.clear();
  }

  vector<weak_ptr<EntityDefinition>>
  EntityDefinition::getChildDefinitionsVector
  ()
  const
  {
    vector<weak_ptr<EntityDefinition>> v;
    v.insert(v.begin(), mChildDefinitions.begin(), mChildDefinitions.end());
    return v;
  }

  vector<string>
  EntityDefinition::getChildNamesVector
  ()
  const
  {
    vector<string> names;
    names.push_back(getName());
    for (auto child : mChildDefinitions)
    {
      auto childNames = child->getChildNamesVector();
      names.insert(names.end(), childNames.begin(), childNames.end());
    }
    return names;
  }

  void
  EntityDefinition::addChildDefinition
  (const shared_ptr<EntityDefinition>& child)
  {
    mChildDefinitions.push_back(child);
  }

  void
  EntityDefinition::adoptChildDefinition
  (const shared_ptr<EntityDefinition>& child)
  {
    child->setParentEntity(static_pointer_cast<EntityDefinition>(shared_from_this()));
    mChildDefinitions.push_back(child);
  }

  void
  EntityDefinition::removeChildDefinition
  (const weak_ptr<EntityDefinition>& child)
  {
    if (auto childLock = child.lock())
    {
      std::remove_if(mChildDefinitions.begin(),
                     mChildDefinitions.end(),
                     [&](const shared_ptr<EntityDefinition>& nextChild)
      {
        return nextChild == childLock;
      });
    }
  }

  shared_ptr<EntityDefinition>
  EntityDefinition::createNewChildDefinition
  (const json& fromJson)
  {
    LOG_DEBUG("EntityDefinition: Creating new child scene object");

    json defJson;

    if (fromJson == nullptr)
    {
      LOG_DEBUG("EntityDefinition: from scratch");
      defJson[Constants::NAME] = Constants::ENTITY_DEFAULT_NAME;

      Transform transform;
      defJson[Constants::TRANSFORM] = transform.toJson();
    }
    else
    {
      LOG_DEBUG("EntityDefinition: from template copy");
      defJson = json::parse(fromJson.dump());
    }

    auto entityDefinition = make_shared<EntityDefinition>(
          static_pointer_cast<EntityDefinition>(shared_from_this()),
          mSceneDefinition,
          defJson, true);
    entityDefinition->loadChildEntityDefinitions(true);
    addChildDefinition(entityDefinition);

    return entityDefinition;
  }


  weak_ptr<SceneDefinition>
  EntityDefinition::getSceneDefinition
  ()
  const
  {
    return mSceneDefinition;
  }

  json
  EntityDefinition::toJson
  ()
  {
    mJson[Constants::ENTITY_CHILDREN] = json::array();
    for (auto entityDefinition : mChildDefinitions)
    {
      mJson[Constants::ENTITY_CHILDREN].push_back(entityDefinition->toJson());
    }
    return mJson;
  }

  void EntityDefinition::setIsTemplate(bool d)
  {
    mJson[Constants::ENTITY_TEMPLATE] = d;
  }

  bool EntityDefinition::getIsTemplate()
  const
  {
    if (mJson.find(Constants::ENTITY_TEMPLATE) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ENTITY_TEMPLATE];
  }

  weak_ptr<EntityDefinition>
  EntityDefinition::getParentEntity
  ()
  const
  {
    return mParentEntity;
  }

  shared_ptr<EntityDefinition>
  EntityDefinition::duplicate
  ()
  {
    // Nothing to assign duplicate to
    if (auto parentLock = mParentEntity.lock())
    {

      auto newEntityDef = make_shared<EntityDefinition>(mParentEntity,mSceneDefinition,toJson(),true);
      newEntityDef->loadChildEntityDefinitions(true);
      newEntityDef->setUuid(Uuid::generateUuid());
      string name = newEntityDef->getName();
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
        newEntityDef->setName(name);
      }
      else
      {
        newEntityDef->setName(getName()+".1");
      }

      parentLock->addChildDefinition(newEntityDef);
      return newEntityDef;
    }
    return nullptr;
  }

  int
  EntityDefinition::getSelectedAssetIndex
  (AssetType type)
  {
    auto asset = getAssetDefinition(type);
    if (asset == Uuid::INVALID)
    {
      return -1;
    }
    if (auto sceneDefLock = mSceneDefinition.lock())
    {
      if (auto pdLock = sceneDefLock->getProjectDefinition().lock())
      {
    		return pdLock->getAssetDefinitionIndex(type,asset);
      }
    }
    return -1;
  }

  void
  EntityDefinition::setSelectedAssetIndex
  (AssetType type, int index)
  {
    if (auto sdLock = mSceneDefinition.lock())
    {
      if (auto pdLock = sdLock->getProjectDefinition().lock())
      {
        auto typesVector = pdLock->getAssetDefinitionsVectorForType(type);
        if (auto asset = typesVector.at(index).lock())
        {
          setAssetDefinition(type,asset->getUuid());
        }
      }
    }
  }

  void
  EntityDefinition::setAssetDefinition
  (AssetType type, UuidType uuid)
  {
    auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
    if (mJson.find(Constants::ENTITY_ASSET_INSTANCES) == mJson.end() ||
        mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
    {
      mJson[Constants::ENTITY_ASSET_INSTANCES] = json::object();
    }
    mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr] = uuid;
  }

  map<AssetType, UuidType>
  EntityDefinition::getAssetDefinitionsMap
  ()
  const
  {
    map<AssetType, UuidType> assetsMap;
    for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
    {
      AssetType type = typePair.first;
      UuidType uuid = getAssetDefinition(type);
      if (uuid != Uuid::INVALID)
      {
        assetsMap.insert(pair<AssetType,UuidType>(type, uuid));
      }
    }
    return assetsMap;
  }

  UuidType
  EntityDefinition::getAssetDefinition
  (AssetType type)
  const
  {
    // TODO, is this correct?
    // Yes, if there was an array, replae with an object
    auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
    if (mJson.find(Constants::ENTITY_ASSET_INSTANCES) == mJson.end() ||
        mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
    {
      return Uuid::INVALID;
    }

    if (!mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr].is_number())
    {
      return Uuid::INVALID;
    }

    LOG_TRACE("EntityDefinition: Found {} Runtime",typeStr);
    return mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr];
  }

  void
  EntityDefinition::setParentEntity
  (const weak_ptr<EntityDefinition>& parentEntity)
  {
    mParentEntity = parentEntity;
  }

  void EntityDefinition::setFontColor(const vec4& color)
  {
    mJson[Constants::ENTITY_FONT_COLOR] =  Vector4::toJson(color);

  }

  vec4
  EntityDefinition::getFontColor
  ()
  const
  {
    if (mJson.find(Constants::ENTITY_FONT_COLOR) == mJson.end())
    {
      return vec4(0.f);
    }
    return Vector4::fromJson(mJson[Constants::ENTITY_FONT_COLOR]);
  }

  void EntityDefinition::setFontText(const string& text)
  {
    mJson[Constants::ENTITY_FONT_TEXT] = text;
  }

  string EntityDefinition::getFontText()
  const
  {
    if (mJson.find(Constants::ENTITY_FONT_TEXT) == mJson.end())
    {
      return "";
    }
    return mJson[Constants::ENTITY_FONT_TEXT];
  }

  void EntityDefinition::setFontScale(float s)
  {
    mJson[Constants::ENTITY_FONT_SCALE] = s;
  }

  float EntityDefinition::getFontScale()
  const
  {
    if (mJson.find(Constants::ENTITY_FONT_SCALE) == mJson.end())
    {
      return 1.f;
    }
    return mJson[Constants::ENTITY_FONT_SCALE];
  }
}
