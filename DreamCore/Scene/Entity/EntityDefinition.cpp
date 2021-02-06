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

using std::regex;
using std::cmatch;
using std::pair;


namespace octronic::dream
{
    EntityDefinition::EntityDefinition
    (EntityDefinition* parent,
     SceneDefinition* sceneDefinition,
     const json &jsonData,
     bool randomUuid
     )
        : Definition("EntityDefinition",jsonData),
          mParentEntity(parent),
          mSceneDefinition(sceneDefinition)
    {

        LOG_TRACE( "EntityDefinition: Constructing {}",getNameAndUuidString());
        if (randomUuid)
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
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
    {
        if(dreamTryLock()) {
            dreamLock();

            return mChildDefinitions.size();
        } dreamElseLockFailed
    }

    Transform
    EntityDefinition::getTransform
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return Transform(mJson[Constants::TRANSFORM]);
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setTransform
    (Transform tform)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::TRANSFORM] = tform.getJson();
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setHasCameraFocus
    (bool fc)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_HAS_CAMERA_FOCUS] = fc;
        } dreamElseLockFailed
    }

    bool
    EntityDefinition::getHasCameraFocus
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ENTITY_HAS_CAMERA_FOCUS) == mJson.end())
            {
                mJson[Constants::ENTITY_HAS_CAMERA_FOCUS] = false;
            }
            return mJson[Constants::ENTITY_HAS_CAMERA_FOCUS];
        } dreamElseLockFailed
    }

    void
    EntityDefinition::loadChildEntityDefinitions
    (bool randomUuid)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    EntityDefinition::deleteChildEntityDefinitions
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            for (auto child : mChildDefinitions)
            {
                delete child;
            }
            mChildDefinitions.clear();
        } dreamElseLockFailed
    }

    vector<EntityDefinition*>&
    EntityDefinition::getChildDefinitionsList
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mChildDefinitions;
        } dreamElseLockFailed
    }

    void
    EntityDefinition::addChildDefinition
    (EntityDefinition* child)
    {
        if(dreamTryLock()) {
            dreamLock();
            mChildDefinitions.push_back(child);
        } dreamElseLockFailed
    }

    void
    EntityDefinition::adoptChildDefinition
    (EntityDefinition* child)
    {
        if(dreamTryLock()) {
            dreamLock();
            child->setParentEntity(this);
            mChildDefinitions.push_back(child);
        } dreamElseLockFailed
    }

    void
    EntityDefinition::removeChildDefinition
    (EntityDefinition* child, bool andDelete)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    EntityDefinition*
    EntityDefinition::createNewChildDefinition
    (json* fromJson)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }


    SceneDefinition*
    EntityDefinition::getSceneDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mSceneDefinition;
        } dreamElseLockFailed
    }

    json
    EntityDefinition::getJson
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_CHILDREN] = json::array();
            for (EntityDefinition* sceneObjectDefinition : mChildDefinitions)
            {
                mJson[Constants::ENTITY_CHILDREN].push_back(sceneObjectDefinition->getJson());
            }
            return mJson;
        } dreamElseLockFailed
    }

    bool EntityDefinition::getAlwaysDraw()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ENTITY_ALWAYS_DRAW) == mJson.end())
            {
                mJson[Constants::ENTITY_ALWAYS_DRAW] = false;
            }
            return mJson[Constants::ENTITY_ALWAYS_DRAW];
        } dreamElseLockFailed
    }

    void EntityDefinition::setAlwaysDraw(bool alwaysDraw)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_ALWAYS_DRAW] = alwaysDraw;
        } dreamElseLockFailed
    }

    void EntityDefinition::setIsTemplate(bool d)
    {
        if(dreamTryLock()) {
            mJson[Constants::ENTITY_TEMPLATE] = d;
        } dreamElseLockFailed
    }

    bool EntityDefinition::getIsTemplate()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ENTITY_TEMPLATE) == mJson.end())
            {
                mJson[Constants::ENTITY_TEMPLATE] = false;
            }
            return mJson[Constants::ENTITY_TEMPLATE];
        } dreamElseLockFailed
    }

    long
    EntityDefinition::getDeferred
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ENTITY_DEFERRED].is_number())
            {
                mJson[Constants::ENTITY_DEFERRED] = 0;
            }
            return mJson[Constants::ENTITY_DEFERRED];
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setDeferred
    (long d)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_DEFERRED] = d;
        } dreamElseLockFailed
    }

    long
    EntityDefinition::getDieAfter
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ENTITY_DIE_AFTER].is_number())
            {
                mJson[Constants::ENTITY_DIE_AFTER] = 0;
            }
            return mJson[Constants::ENTITY_DIE_AFTER];
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setDieAfter
    (long d)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_DIE_AFTER] = d;
        } dreamElseLockFailed
    }


    void EntityDefinition::setHidden(bool d)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ENTITY_HIDDEN] = d;
        } dreamElseLockFailed
    }

    bool EntityDefinition::getHidden()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ENTITY_HIDDEN) == mJson.end())
            {
                mJson[Constants::ENTITY_HIDDEN] = false;
            }
            return mJson[Constants::ENTITY_HIDDEN];
        } dreamElseLockFailed
    }

    EntityDefinition*
    EntityDefinition::getParentEntity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mParentEntity;
        } dreamElseLockFailed
    }

    EntityDefinition*
    EntityDefinition::duplicate()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    int
    EntityDefinition::getSelectedAssetIndex
    (AssetType type)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto asset = getAssetDefinition(type);
            if (asset == Uuid::INVALID)
            {
                return -1;
            }
            return mSceneDefinition->getProjectDefinition()->getAssetDefinitionIndex(type,asset);
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setSelectedAssetIndex
    (AssetType type, int index)
    {
        if(dreamTryLock()) {
            dreamLock();

            auto typesVector = mSceneDefinition->getProjectDefinition()->getAssetDefinitionsVector(type);
            auto asset = typesVector.at(index);
            setAssetDefinition(type,asset->getUuid());
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setAssetDefinition
    (AssetType type, UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();

            auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
            if (mJson.find(Constants::ENTITY_ASSET_INSTANCES) == mJson.end() ||
                mJson[Constants::ENTITY_ASSET_INSTANCES].is_array())
            {
                setEmptyAssetsObject();
            }
            mJson[Constants::ENTITY_ASSET_INSTANCES][typeStr] = uuid;
        } dreamElseLockFailed
    }

    map<AssetType, UuidType>
    EntityDefinition::getAssetDefinitionsMap
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

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
        } dreamElseLockFailed
    }

    UuidType
    EntityDefinition::getAssetDefinition
    (AssetType type)
    {
        if(dreamTryLock()) {
            dreamLock();

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
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setEmptyAssetsObject()
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ENTITY_ASSET_INSTANCES] = json::object();
        } dreamElseLockFailed
    }

    void
    EntityDefinition::setParentEntity
    (EntityDefinition* parentEntity)
    {
        if(dreamTryLock()) {
            dreamLock();

            mParentEntity = parentEntity;
        } dreamElseLockFailed
    }

    void EntityDefinition::setFontColor(const Vector3& color)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ENTITY_FONT_COLOR] =  color.toJson();
        } dreamElseLockFailed
    }

    Vector3 EntityDefinition::getFontColor()
    {
        if(dreamTryLock()) {
            dreamLock();

            Vector3 retval(1.f);
            if (mJson.find(Constants::ENTITY_FONT_COLOR) == mJson.end())
            {
                mJson[Constants::ENTITY_FONT_COLOR] = retval.toJson();
            }
            retval = Vector3(mJson[Constants::ENTITY_FONT_COLOR]);
            return retval;
        } dreamElseLockFailed
    }

    void EntityDefinition::setFontText(const string& text)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ENTITY_FONT_TEXT] = text;
        } dreamElseLockFailed
    }

    string EntityDefinition::getFontText()
    {
        if(dreamTryLock()) {
            dreamLock();

            string retval = "";
            if (mJson.find(Constants::ENTITY_FONT_TEXT) == mJson.end())
            {
                mJson[Constants::ENTITY_FONT_TEXT] = retval;
            }
            retval = mJson[Constants::ENTITY_FONT_TEXT];
            return retval;
        } dreamElseLockFailed
    }

    void EntityDefinition::setFontScale(float s)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ENTITY_FONT_SCALE] = s;
        } dreamElseLockFailed
    }

    float EntityDefinition::getFontScale()
    {
        if(dreamTryLock()) {
            dreamLock();

            float retval = 1.f;
            if (mJson.find(Constants::ENTITY_FONT_SCALE) == mJson.end())
            {
                mJson[Constants::ENTITY_FONT_SCALE] = retval;
            }
            retval = mJson[Constants::ENTITY_FONT_SCALE];
            return retval;
        } dreamElseLockFailed
    }
}
