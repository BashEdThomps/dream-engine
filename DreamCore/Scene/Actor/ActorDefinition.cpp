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
#include "ActorDefinition.h"

#include "ActorRuntime.h"
#include "../SceneDefinition.h"
#include "../../Project/ProjectDefinition.h"

#include "../../Components/AssetDefinition.h"
#include "../../Components/Transform.h"
#include "../../Common/Uuid.h"

#include <regex>

namespace Dream
{
    ActorDefinition::ActorDefinition
    (ActorDefinition* parent,
            SceneDefinition* sceneDefinition,
            const json &jsonData,
            bool randomUuid
            )
        : Definition(jsonData),
          mParentActor(parent),
          mSceneDefinition(sceneDefinition)
    {
        #ifdef DREAM_LOG
        setLogClassName("ActorDefinition");
        getLog()->trace( "Constructing {}",getNameAndUuidString());
        #endif
        if (randomUuid)
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
            #ifdef DREAM_LOG
            getLog()->trace( "With new UUID",getNameAndUuidString());
            #endif
        }
        mJson[Constants::TRANSFORM] = jsonData[Constants::TRANSFORM];
    }

    ActorDefinition::~ActorDefinition
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace( "Destructing {}", getNameAndUuidString() );
        #endif
        deleteChildActorDefinitions();
    }

    int
    ActorDefinition::getChildCount
    ()
    const
    {
        return mChildDefinitions.size();
    }

    Transform
    ActorDefinition::getTransform
    ()
    {
        return Transform(mJson[Constants::TRANSFORM]);
    }

    void
    ActorDefinition::setTransform
    (const Transform& tform)
    {
        mJson[Constants::TRANSFORM] = tform.getJson();
    }

    void
    ActorDefinition::setHasCameraFocus
    (bool fc)
    {
        mJson[Constants::ACTOR_HAS_CAMERA_FOCUS] = fc;
    }

    bool
    ActorDefinition::getHasCameraFocus
    ()
    {
        if (mJson[Constants::ACTOR_HAS_CAMERA_FOCUS].is_null())
        {
            mJson[Constants::ACTOR_HAS_CAMERA_FOCUS] = false;
        }
        return mJson[Constants::ACTOR_HAS_CAMERA_FOCUS];
    }

    void
    ActorDefinition::loadChildActorDefinitions
    (bool randomUuid)
    {
        json childrenArray = mJson[Constants::ACTOR_CHILDREN];

        if (!childrenArray.is_null() && childrenArray.is_array())
        {
            for (const json& childDefinition : childrenArray)
            {
                auto sod = new ActorDefinition
                (
                    this,
                    mSceneDefinition,
                    childDefinition,
                    randomUuid
                );
                sod->loadChildActorDefinitions(randomUuid);
                mChildDefinitions.push_back(sod);
            }
        }
    }

    void
    ActorDefinition::deleteChildActorDefinitions
    ()
    {
       for (auto child : mChildDefinitions)
       {
           delete child;
       }
       mChildDefinitions.clear();
    }

    vector<ActorDefinition*>&
    ActorDefinition::getChildDefinitionsList
    ()
    {
        return mChildDefinitions;
    }

    void
    ActorDefinition::addChildDefinition
    (ActorDefinition* child)
    {
        mChildDefinitions.push_back(child);
    }

    void
    ActorDefinition::adoptChildDefinition
    (ActorDefinition* child)
    {
        child->setParentActor(this);
        mChildDefinitions.push_back(child);
    }

    void
    ActorDefinition::removeChildDefinition
    (ActorDefinition* child, bool andDelete)
    {
        auto iter = begin(mChildDefinitions);
        auto endPos = end(mChildDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == child)
            {
                #ifdef DREAM_LOG
                getLog()->debug
                (
                    "Found child to {} remove from {}",
                    child->getNameAndUuidString(),
                    getNameAndUuidString()
                );
                #endif
                if (andDelete)
                {
                    delete (*iter);
                }
                else
                {
                    (*iter)->setParentActor(nullptr);
                }
                mChildDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    ActorDefinition*
    ActorDefinition::createNewChildDefinition
    (json* fromJson)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Creating new child scene object");
        #endif

        json defJson;

        if (fromJson == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->debug("from scratch");
            #endif
            defJson[Constants::NAME] = Constants::ACTOR_DEFAULT_NAME;

            Transform transform;
            defJson[Constants::TRANSFORM] = transform.getJson();
        }
        else
        {
            #ifdef DREAM_LOG
            getLog()->debug("from template copy");
            #endif
            defJson = json::parse(fromJson->dump());
        }

        ActorDefinition* soDefinition;
        soDefinition = new ActorDefinition
        (
            this,
            mSceneDefinition,
            defJson,
            true
        );
        soDefinition->loadChildActorDefinitions(true);
        addChildDefinition(soDefinition);

        return soDefinition;
    }


    SceneDefinition*
    ActorDefinition::getSceneDefinition
    ()
    {
        return mSceneDefinition;
    }

    json
    ActorDefinition::getJson
    ()
    {
        mJson[Constants::ACTOR_CHILDREN] = json::array();
        for (ActorDefinition* sod : mChildDefinitions)
        {
            mJson[Constants::ACTOR_CHILDREN].push_back(sod->getJson());
        }
        return mJson;
    }

    bool ActorDefinition::getAlwaysDraw()
    {
        if (mJson[Constants::ACTOR_ALWAYS_DRAW].is_null())
        {
            mJson[Constants::ACTOR_ALWAYS_DRAW] = false;
        }
        return mJson[Constants::ACTOR_ALWAYS_DRAW];
    }

    void ActorDefinition::setAlwaysDraw(bool alwaysDraw)
    {
        mJson[Constants::ACTOR_ALWAYS_DRAW] = alwaysDraw;
    }

    void ActorDefinition::setIsTemplate(bool d)
    {
        mJson[Constants::ACTOR_TEMPLATE] = d;
    }

    bool ActorDefinition::getIsTemplate()
    {
        if (mJson[Constants::ACTOR_TEMPLATE].is_null())
        {
            mJson[Constants::ACTOR_TEMPLATE] = false;
        }
        return mJson[Constants::ACTOR_TEMPLATE];
    }

    long
    ActorDefinition::getDeferred
    ()
    {
        if (!mJson[Constants::ACTOR_DEFERRED].is_number())
        {
            mJson[Constants::ACTOR_DEFERRED] = 0;
        }
        return mJson[Constants::ACTOR_DEFERRED];
    }

    void
    ActorDefinition::setDeferred
    (long d)
    {
        mJson[Constants::ACTOR_DEFERRED] = d;
    }

    long
    ActorDefinition::getDieAfter
    ()
    {
        if (!mJson[Constants::ACTOR_DIE_AFTER].is_number())
        {
            mJson[Constants::ACTOR_DIE_AFTER] = 0;
        }
        return mJson[Constants::ACTOR_DIE_AFTER];
    }

    void
    ActorDefinition::setDieAfter
    (long d)
    {
        mJson[Constants::ACTOR_DIE_AFTER] = d;
    }


    void ActorDefinition::setHidden(bool d)
    {
        mJson[Constants::ACTOR_HIDDEN] = d;
    }

    bool ActorDefinition::getHidden()
    {
        if (mJson[Constants::ACTOR_HIDDEN].is_null())
        {
            mJson[Constants::ACTOR_HIDDEN] = false;
        }
        return mJson[Constants::ACTOR_HIDDEN];
    }

    ActorDefinition*
    ActorDefinition::getParentActor
    () const
    {
        return mParentActor;
    }

    ActorDefinition*
    ActorDefinition::duplicate()
    {
        // Nothing to assign duplicate to
        if (mParentActor == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("Cannot Duplicate. No parent to assign duplicate to");
            #endif
            return nullptr;
        }

        auto newSOD = new ActorDefinition(mParentActor,mSceneDefinition,getJson(),true);
        newSOD->loadChildActorDefinitions(true);
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

        mParentActor->addChildDefinition(newSOD);
        return newSOD;
    }

    int
    ActorDefinition::getSelectedAssetIndex
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
    ActorDefinition::setSelectedAssetIndex
    (AssetType type, int index)
    {
        auto typesVector = mSceneDefinition->getProjectDefinition()->getAssetDefinitionsVector(type);
        auto asset = typesVector.at(index);
        setAssetDefinition(type,asset->getUuid());
    }

    void
    ActorDefinition::setAssetDefinition
    (AssetType type, uint32_t uuid)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::ACTOR_ASSET_INSTANCES].is_null() ||
            mJson[Constants::ACTOR_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        mJson[Constants::ACTOR_ASSET_INSTANCES][typeStr] = uuid;
    }

    map<AssetType, uint32_t>
    ActorDefinition::getAssetDefinitionsMap
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
    ActorDefinition::getAssetDefinition
    (AssetType type)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::ACTOR_ASSET_INSTANCES].is_null() ||
            mJson[Constants::ACTOR_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }

        if (!mJson[Constants::ACTOR_ASSET_INSTANCES][typeStr].is_number())
        {
            return 0;
        }

        #ifdef DREAM_LOG
        getLog()->trace("Found {} Runtime",typeStr);
        #endif
        return mJson[Constants::ACTOR_ASSET_INSTANCES][typeStr];
    }

    void
    ActorDefinition::setEmptyAssetsObject()
    {
        mJson[Constants::ACTOR_ASSET_INSTANCES] = json::object();
    }

    void
    ActorDefinition::setParentActor
    (ActorDefinition* parentActor)
    {
        mParentActor = parentActor;
    }
}
