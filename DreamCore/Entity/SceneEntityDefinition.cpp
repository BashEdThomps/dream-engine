#include "SceneEntityDefinition.h"

#include "Common/Constants.h"
#include "Math/Transform.h"
#include "Math/Vector.h"

#include <regex>

using std::regex;
using std::cmatch;
using std::make_unique;

namespace octronic::dream
{
  SceneEntityDefinition::SceneEntityDefinition
  (SceneDefinition& scene,
   optional<reference_wrapper<SceneEntityDefinition>> parent,
   const json& js)
    : Definition(js),
      mSceneDefinition(scene),
      mParentDefinition(parent)
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_CHILDREN) != mJson.end())
    {
      for (auto& child : mJson[Constants::SCENE_ENTITY_DEFINITION_CHILDREN])
      {
        mChildDefinitions.emplace_back(make_unique<SceneEntityDefinition>(mSceneDefinition,*this,child));
      }
    }
  }

  Transform
  SceneEntityDefinition::getTransform
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_TRANSFORM) == mJson.end())
    {
      return Transform();
    }
    return Transform(mJson[Constants::SCENE_ENTITY_DEFINITION_TRANSFORM]);
  }

  void
  SceneEntityDefinition::setTransform
  (const Transform& tx)
  {
    mJson[Constants::SCENE_ENTITY_DEFINITION_TRANSFORM] = tx.toJson();
  }

  UuidType SceneEntityDefinition::getTemplateUuid
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_TEMPLATE_UUID) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::SCENE_ENTITY_DEFINITION_TEMPLATE_UUID];
  }

  void
  SceneEntityDefinition::setTemplateUuid(UuidType id)
  {
    mJson[Constants::SCENE_ENTITY_DEFINITION_TEMPLATE_UUID] = id;
  }

  SceneEntityDefinition&
  SceneEntityDefinition::createChildDefinition
  ()
  {
    return *mChildDefinitions.emplace_back(make_unique<SceneEntityDefinition>(mSceneDefinition,*this,json::object()));
  }


  SceneEntityDefinition&
  SceneEntityDefinition::createChildDefinitionFrom
  (SceneEntityDefinition& other)
  {
    return *mChildDefinitions.emplace_back(make_unique<SceneEntityDefinition>(mSceneDefinition,*this, other.toJson()));
  }

  void
  SceneEntityDefinition::removeChildDefinition
  (SceneEntityDefinition& child)
  {
    auto itr = find_if(mChildDefinitions.begin(),
                       mChildDefinitions.end(),
                       [&](unique_ptr<SceneEntityDefinition>& next)
    { return next->getUuid() == child.getUuid(); });

    if (itr != mChildDefinitions.end())
    {
      mChildDefinitions.erase(itr);
    }
  }

  SceneDefinition&
  SceneEntityDefinition::getSceneDefinition
  ()
  {
    return mSceneDefinition.get();
  }


  void
  SceneEntityDefinition::setParentDefinition
  (SceneEntityDefinition& parent)
  {
    mParentDefinition = parent;
  }

  optional<reference_wrapper<SceneEntityDefinition>>&
  SceneEntityDefinition::getParentDefinition
  ()
  {
    return mParentDefinition;
  }


  vector<reference_wrapper<SceneEntityDefinition>>
  SceneEntityDefinition::getChildDefinitionsVector
  ()
  const
  {
    vector<reference_wrapper<SceneEntityDefinition>> ret;
    for (auto& child : mChildDefinitions)
    {
     ret.push_back(*child);
    }
    return ret;
  }

  size_t
  SceneEntityDefinition::countChildDefinitions
  ()
  const
  {
    return mChildDefinitions.size();
  }

  bool
  SceneEntityDefinition::hasParentDefinition
  ()
  const
  {
    return mParentDefinition.has_value();
  }

  SceneEntityDefinition&
  SceneEntityDefinition::duplicateDefinition
  ()
  {
    if (mParentDefinition)
    {
      auto& parent = mParentDefinition.value().get();
      auto& newEntityDef = parent.createChildDefinition();

      newEntityDef.setJson(mJson);
      newEntityDef.setUuid(Uuid::generateUuid());

      string name = newEntityDef.getName();
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
        newEntityDef.setName(name);
      }
      else
      {
        newEntityDef.setName(getName()+".1");
      }
      return newEntityDef;
    }
    throw std::exception();
  }

  void
  SceneEntityDefinition::setFontColor
  (const vec4& color)
  {
    mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_COLOR] = Vector4::toJson(color);
  }

  vec4
  SceneEntityDefinition::getFontColor
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_FONT_COLOR) == mJson.end())
    {
      return vec4(0.f);
    }
    return Vector4::fromJson(mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_COLOR]);
  }

  void
  SceneEntityDefinition::setFontText
  (const string& text)
  {
    mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_TEXT] = text;
  }

  string SceneEntityDefinition::getFontText()
  const
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_FONT_TEXT) == mJson.end())
    {
      return "";
    }
    return mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_TEXT];
  }

  void
  SceneEntityDefinition::setFontScale
  (float s)
  {
    mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_SCALE] = s;
  }

  float
  SceneEntityDefinition::getFontScale
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENTITY_DEFINITION_FONT_SCALE) == mJson.end())
    {
      return 1.f;
    }
    return mJson[Constants::SCENE_ENTITY_DEFINITION_FONT_SCALE];
  }


  optional<reference_wrapper<SceneEntityDefinition>>
  SceneEntityDefinition::getChildDefinitionByUuid
  (UuidType uuid)
  {
    if (hasUuid(uuid)) return *this;

    for (auto& child : mChildDefinitions)
    {
     if (child->hasUuid(uuid)) return *child;

     auto child_result = child->getChildDefinitionByUuid(uuid);

     if (child_result) return child_result;
    }
    return std::nullopt;
  }
}

