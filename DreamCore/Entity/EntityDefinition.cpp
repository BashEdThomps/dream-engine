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


namespace octronic::dream
{
    EntityDefinition::EntityDefinition
    (EntityDefinition* parent,SceneDefinition* sceneDefinition,
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
    {
        return mChildDefinitions.size();
    }

    Transform
    EntityDefinition::getTransform
    ()
    {
        return Transform(mJson[Constants::TRANSFORM]);
    }

    void
    EntityDefinition::setTransform
    (Transform tform)
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
                auto sceneObjectDefinition = new EntityDefinition(this,mSceneDefinition,childDefinition,randomUuid);
                sceneObjectDefinition->loadChildEntityDefinitions(randomUuid);
                mChildDefinitions.push_back(sceneObjectDefinition);
            }
        }
    }

    void
    EntityDefinition::deleteChildEntityDefinitions
    ()
    {
        for (auto child : mChildDefinitions)
        {
            delete child;
        }
        mChildDefinitions.clear();
    }

    vector<EntityDefinition*>&
    EntityDefinition::getChildDefinitionsList
    ()
    {
        return mChildDefinitions;
    }

    void
    EntityDefinition::addChildDefinition
    (EntityDefinition* child)
    {
        mChildDefinitions.push_back(child);
    }

    void
    EntityDefinition::adoptChildDefinition
    (EntityDefinition* child)
    {
        child->setParentEntity(this);
        mChildDefinitions.push_back(child);
    }

    void
    EntityDefinition::removeChildDefinition
    (EntityDefinition* child, bool andDelete)
    {
        auto iter = begin(mChildDefinitions);
        auto endPos = end(mChildDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == child)
            {
                LOG_DEBUG("EntityDefinition: Found child to {} remove from {}",
                          child->getNameAndUuidString(), getNameAndUuidString());
                if (andDelete)
                {
                    delete (*iter);
                }
                else
                {
                    (*iter)->setParentEntity(nullptr);
                }
                mChildDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    EntityDefinition*
    EntityDefinition::createNewChildDefinition
    (json* fromJson)
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
            defJson = json::parse(fromJson->dump());
        }

        EntityDefinition* soDefinition;
        soDefinition = new EntityDefinition
                (
                    this,
                    mSceneDefinition,
                    defJson,
                    true
                    );
        soDefinition->loadChildEntityDefinitions(true);
        addChildDefinition(soDefinition);

        return soDefinition;
    }


    SceneDefinition*
    EntityDefinition::getSceneDefinition
    ()
    {
        return mSceneDefinition;
    }

    json
    EntityDefinition::getJson
    ()
    {
        mJson[Constants::ENTITY_CHILDREN] = json::array();
        for (EntityDefinition* sceneObjectDefinition : mChildDefinitions)
        {
            mJson[Constants::ENTITY_CHILDREN].push_back(sceneObjectDefinition->getJson());
        }
        return mJson;
    }

    bool EntityDefinition::getAlwaysDraw()
    {
        if (mJson.find(Constants::ENTITY_ALWAYS_DRAW) == mJson.end())
        {
            mJson[Constants::ENTITY_ALWAYS_DRAW] = false;
        }
        return mJson[Constants::ENTITY_ALWAYS_DRAW];
    }

    void EntityDefinition::setAlwaysDraw(bool alwaysDraw)
    {
        mJson[Constants::ENTITY_ALWAYS_DRAW] = alwaysDraw;
    }

    void EntityDefinition::setIsTemplate(bool d)
    {
        mJson[Constants::ENTITY_TEMPLATE] = d;
    }

    bool EntityDefinition::getIsTemplate()
    {
        if (mJson.find(Constants::ENTITY_TEMPLATE) == mJson.end())
        {
            mJson[Constants::ENTITY_TEMPLATE] = false;
        }
        return mJson[Constants::ENTITY_TEMPLATE];
    }

    void EntityDefinition::setHidden(bool d)
    {
        mJson[Constants::ENTITY_HIDDEN] = d;
    }

    bool EntityDefinition::getHidden()
    {
        if (mJson.find(Constants::ENTITY_HIDDEN) == mJson.end())
        {
            mJson[Constants::ENTITY_HIDDEN] = false;
        }
        return mJson[Constants::ENTITY_HIDDEN];
    }

    EntityDefinition*
    EntityDefinition::getParentEntity
    ()
    {
        return mParentEntity;
    }

    EntityDefinition*
    EntityDefinition::duplicate()
    {
        // Nothing to assign duplicate to
        if (mParentEntity == nullptr)
        {
            LOG_ERROR("EntityDefinition: Cannot Duplicate. No parent to assign duplicate to");
            return nullptr;
        }

        auto newSOD = new EntityDefinition(mParentEntity,mSceneDefinition,getJson(),true);
        newSOD->loadChildEntityDefinitions(true);
        newSOD->setUuid(Uuid::generateUuid());
        string name = newSOD->getName();
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
            newSOD->setName(name);
        }
        else
        {
            newSOD->setName(getName()+".1");
        }

        mParentEntity->addChildDefinition(newSOD);
        return newSOD;
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
        return mSceneDefinition->getProjectDefinition()->getAssetDefinitionIndex(type,asset);
    }

    void
    EntityDefinition::setSelectedAssetIndex
    (AssetType type, int index)
    {
        auto typesVector = mSceneDefinition->getProjectDefinition()->getAssetDefinitionsVector(type);
        auto asset = typesVector.at(index);
        setAssetDefinition(type,asset->getUuid());
    }

    void
    EntityDefinition::setAssetDefinition
    (AssetType type, UuidType uuid)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson.find(Constants::ENTITY_ASSET_INSTANCES) == mJson.end() ||
            mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr] = uuid;
    }

    map<AssetType, UuidType>
    EntityDefinition::getAssetDefinitionsMap
    ()
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
    {
        // TODO, is this correct?
        // Yes, if there was an array, replae with an object
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson.find(Constants::ENTITY_ASSET_INSTANCES) == mJson.end() ||
            mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }

        if (!mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr].is_number())
        {
            return Uuid::INVALID;
        }

        LOG_TRACE("EntityDefinition: Found {} Runtime",typeStr);
        return mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr];
    }

    void
    EntityDefinition::setEmptyAssetsObject()
    {
        mJson[Constants::ENTITY_ASSET_INSTANCES] = json::object();
    }

    void
    EntityDefinition::setParentEntity
    (EntityDefinition* parentEntity)
    {
        mParentEntity = parentEntity;
    }

    void EntityDefinition::setFontColor(const vec4& color)
    {
        mJson[Constants::ENTITY_FONT_COLOR] =  Vector4::toJson(color);

    }

    vec4 EntityDefinition::getFontColor()
    {
        vec4 retval(1.f);
        if (mJson.find(Constants::ENTITY_FONT_COLOR) == mJson.end())
        {
            mJson[Constants::ENTITY_FONT_COLOR] = Vector4::toJson(retval);
        }
        retval = Vector4::fromJson(mJson[Constants::ENTITY_FONT_COLOR]);
        return retval;
    }

    void EntityDefinition::setFontText(const string& text)
    {
        mJson[Constants::ENTITY_FONT_TEXT] = text;
    }

    string EntityDefinition::getFontText()
    {
        string retval = "";
        if (mJson.find(Constants::ENTITY_FONT_TEXT) == mJson.end())
        {
            mJson[Constants::ENTITY_FONT_TEXT] = retval;
        }
        retval = mJson[Constants::ENTITY_FONT_TEXT];
        return retval;
    }

    void EntityDefinition::setFontScale(float s)
    {
        mJson[Constants::ENTITY_FONT_SCALE] = s;
    }

    float EntityDefinition::getFontScale()
    {
        float retval = 1.f;
        if (mJson.find(Constants::ENTITY_FONT_SCALE) == mJson.end())
        {
            mJson[Constants::ENTITY_FONT_SCALE] = retval;
        }
        retval = mJson[Constants::ENTITY_FONT_SCALE];
        return retval;
    }

    TransformSpace EntityDefinition::getTransformSpace()
    {
       TransformSpace t = TRANSFORM_SPACE_WORLD;
       if (mJson.find(Constants::ENTITY_TRANSFORM_SPACE) == mJson.end())
       {
           mJson[Constants::ENTITY_TRANSFORM_SPACE] = t;
       }
       t = mJson[Constants::ENTITY_TRANSFORM_SPACE];
       return t;
    }

    void EntityDefinition::setTransformSpace(TransformSpace t)
    {
       mJson[Constants::ENTITY_TRANSFORM_SPACE] = t;
    }
}
