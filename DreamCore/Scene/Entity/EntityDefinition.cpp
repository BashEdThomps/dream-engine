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
#include "Components/Transform.h"

#include <regex>

namespace Dream
{
    EntityDefinition::EntityDefinition
    (EntityDefinition* parent,
            SceneDefinition* sceneDefinition,
            const json &jsonData,
            bool randomUuid
            )
        : Definition(jsonData),
          mParentEntity(parent),
          mSceneDefinition(sceneDefinition)
    {
        LOG_TRACE( "EntityDefinition: Constructing {}",getNameAndUuidString());
        if (randomUuid)
        {
            mJson[Constants::UUID] = UuidTools::generateUuid();
            LOG_TRACE( "EntityDefinition: With new UUID",getNameAndUuidString());
        }
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
    {
        return Transform(mJson[Constants::TRANSFORM]);
    }

    void
    EntityDefinition::setTransform
    (const Transform& tform)
    {
        mJson[Constants::TRANSFORM] = tform.getJson();
    }

    void
    EntityDefinition::setHasCameraFocus
    (bool fc)
    {
        mJson[Constants::ENTITY_HAS_CAMERA_FOCUS] = fc;
    }

    bool
    EntityDefinition::getHasCameraFocus
    ()
    {
        if (mJson[Constants::ENTITY_HAS_CAMERA_FOCUS].is_null())
        {
            mJson[Constants::ENTITY_HAS_CAMERA_FOCUS] = false;
        }
        return mJson[Constants::ENTITY_HAS_CAMERA_FOCUS];
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
                auto sod = new EntityDefinition
                (
                    this,
                    mSceneDefinition,
                    childDefinition,
                    randomUuid
                );
                sod->loadChildEntityDefinitions(randomUuid);
                mChildDefinitions.push_back(sod);
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
                LOG_DEBUG
                (
                    "EntityDefinition: Found child to {} remove from {}",
                    child->getNameAndUuidString(),
                    getNameAndUuidString()
                );
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
            defJson[Constants::TRANSFORM] = transform.getJson();
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
        for (EntityDefinition* sod : mChildDefinitions)
        {
            mJson[Constants::ENTITY_CHILDREN].push_back(sod->getJson());
        }
        return mJson;
    }

    bool EntityDefinition::getAlwaysDraw()
    {
        if (mJson[Constants::ENTITY_ALWAYS_DRAW].is_null())
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
        if (mJson[Constants::ENTITY_TEMPLATE].is_null())
        {
            mJson[Constants::ENTITY_TEMPLATE] = false;
        }
        return mJson[Constants::ENTITY_TEMPLATE];
    }

    long
    EntityDefinition::getDeferred
    ()
    {
        if (!mJson[Constants::ENTITY_DEFERRED].is_number())
        {
            mJson[Constants::ENTITY_DEFERRED] = 0;
        }
        return mJson[Constants::ENTITY_DEFERRED];
    }

    void
    EntityDefinition::setDeferred
    (long d)
    {
        mJson[Constants::ENTITY_DEFERRED] = d;
    }

    long
    EntityDefinition::getDieAfter
    ()
    {
        if (!mJson[Constants::ENTITY_DIE_AFTER].is_number())
        {
            mJson[Constants::ENTITY_DIE_AFTER] = 0;
        }
        return mJson[Constants::ENTITY_DIE_AFTER];
    }

    void
    EntityDefinition::setDieAfter
    (long d)
    {
        mJson[Constants::ENTITY_DIE_AFTER] = d;
    }


    void EntityDefinition::setHidden(bool d)
    {
        mJson[Constants::ENTITY_HIDDEN] = d;
    }

    bool EntityDefinition::getHidden()
    {
        if (mJson[Constants::ENTITY_HIDDEN].is_null())
        {
            mJson[Constants::ENTITY_HIDDEN] = false;
        }
        return mJson[Constants::ENTITY_HIDDEN];
    }

    EntityDefinition*
    EntityDefinition::getParentEntity
    () const
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
        newSOD->setUuid(UuidTools::generateUuid());
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
        if (asset == 0)
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
    (AssetType type, uint32_t uuid)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::ENTITY_ASSET_INSTANCES].is_null() ||
            mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr] = uuid;
    }

    map<AssetType, uint32_t>
    EntityDefinition::getAssetDefinitionsMap
    ()
    {
        map<AssetType, uint32_t> assetsMap;
        for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            AssetType type = typePair.first;
            uint32_t uuid = getAssetDefinition(type);
            if (uuid != 0)
            {
                assetsMap.insert(pair<AssetType,uint32_t>(type, uuid));
            }
        }
        return assetsMap;
    }

    uint32_t
    EntityDefinition::getAssetDefinition
    (AssetType type)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::ENTITY_ASSET_INSTANCES].is_null() ||
            mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }

        if (!mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr].is_number())
        {
            return 0;
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
}
