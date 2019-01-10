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
#include "SceneObjectDefinition.h"

#include "SceneObjectRuntime.h"
#include "../SceneDefinition.h"
#include "../../Project/ProjectDefinition.h"

#include "../../Components/AssetDefinition.h"
#include "../../Components/Transform.h"
#include "../../Utilities/Uuid.h"

#include <regex>

namespace Dream
{
    SceneObjectDefinition::SceneObjectDefinition
    (
            SceneObjectDefinition* parent,
            SceneDefinition* sceneDefinition,
            json jsonData,
            bool randomUuid
            )
        : Definition(jsonData),
          mParentSceneObject(parent),
          mSceneDefinition(sceneDefinition)
    {

#ifdef DREAM_DEBUG
        setLogClassName("SceneObjectDefinition");
        auto log = getLog();
        log->trace( "Constructing {}",getNameAndUuidString());
#endif
        if (randomUuid)
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
#ifdef DREAM_DEBUG
            log->trace( "With new UUID",getNameAndUuidString());
#endif
        }
        mJson[Constants::TRANSFORM] = jsonData[Constants::TRANSFORM];
    }

    SceneObjectDefinition::~SceneObjectDefinition
    ()
    {
#ifdef DREAM_DEBUG
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );
#endif
        deleteChildSceneObjectDefinitions();
    }

    int
    SceneObjectDefinition::getChildCount
    ()
    const
    {
        return mChildDefinitions.size();
    }

    Transform
    SceneObjectDefinition::getTransform
    ()
    {
        return Transform(mJson[Constants::TRANSFORM]);
    }

    void
    SceneObjectDefinition::setTransform
    (const Transform& tform)
    {
        mJson[Constants::TRANSFORM] = tform.getJson();
    }

    void
    SceneObjectDefinition::setHasCameraFocus
    (bool fc)
    {
        mJson[Constants::SCENE_OBJECT_HAS_CAMERA_FOCUS] = fc;
    }

    bool
    SceneObjectDefinition::getHasCameraFocus
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_HAS_CAMERA_FOCUS].is_null())
        {
            mJson[Constants::SCENE_OBJECT_HAS_CAMERA_FOCUS] = false;
        }
        return mJson[Constants::SCENE_OBJECT_HAS_CAMERA_FOCUS];
    }

    void
    SceneObjectDefinition::loadChildSceneObjectDefinitions
    (bool randomUuid)
    {
        json childrenArray = mJson[Constants::SCENE_OBJECT_CHILDREN];

        if (!childrenArray.is_null() && childrenArray.is_array())
        {
            for (json childDefinition : childrenArray)
            {
                auto sod = new SceneObjectDefinition
                (
                    this,
                    mSceneDefinition,
                    childDefinition,
                    randomUuid
                );
                sod->loadChildSceneObjectDefinitions(randomUuid);
                mChildDefinitions.push_back(sod);
            }
        }
    }

    void
    SceneObjectDefinition::deleteChildSceneObjectDefinitions
    ()
    {
       for (auto child : mChildDefinitions)
       {
           delete child;
       }
       mChildDefinitions.clear();
    }

    vector<SceneObjectDefinition*>&
    SceneObjectDefinition::getChildDefinitionsList
    ()
    {
        return mChildDefinitions;
    }

    void
    SceneObjectDefinition::addChildDefinition
    (SceneObjectDefinition* child)
    {
        mChildDefinitions.push_back(child);
    }

    void
    SceneObjectDefinition::adoptChildDefinition
    (SceneObjectDefinition* child)
    {
        child->setParentSceneObject(this);
        mChildDefinitions.push_back(child);
    }

    void
    SceneObjectDefinition::removeChildDefinition
    (SceneObjectDefinition* child, bool andDelete)
    {
#ifdef DREAM_DEBUG
        auto log = getLog();
#endif
        auto iter = begin(mChildDefinitions);
        auto endPos = end(mChildDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == child)
            {
#ifdef DREAM_DEBUG
                log->debug
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
                    (*iter)->setParentSceneObject(nullptr);
                }
                mChildDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    SceneObjectDefinition*
    SceneObjectDefinition::createNewChildDefinition
    (json* fromJson)
    {
#ifdef DREAM_DEBUG
        auto log = getLog();
        log->debug("Creating new child scene object");
#endif

        json defJson;

        if (fromJson == nullptr)
        {
#ifdef DREAM_DEBUG
            log->debug("from scratch");
#endif
            defJson[Constants::NAME] = Constants::SCENE_OBJECT_DEFAULT_NAME;

            Transform transform;
            defJson[Constants::TRANSFORM] = transform.getJson();
        }
        else
        {
#ifdef DREAM_DEBUG
            log->debug("from template copy");
#endif
            defJson = json::parse(fromJson->dump());
        }

        SceneObjectDefinition* soDefinition;
        soDefinition = new SceneObjectDefinition
        (
            this,
            mSceneDefinition,
            defJson,
            true
        );
        soDefinition->loadChildSceneObjectDefinitions(true);
        addChildDefinition(soDefinition);

        return soDefinition;
    }


    SceneDefinition*
    SceneObjectDefinition::getSceneDefinition
    ()
    {
        return mSceneDefinition;
    }

    json
    SceneObjectDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_OBJECT_CHILDREN] = json::array();
        for (SceneObjectDefinition* sod : mChildDefinitions)
        {
            mJson[Constants::SCENE_OBJECT_CHILDREN].push_back(sod->getJson());
        }
        return mJson;
    }

    bool SceneObjectDefinition::getAlwaysDraw()
    {
        if (mJson[Constants::SCENE_OBJECT_ALWAYS_DRAW].is_null())
        {
            mJson[Constants::SCENE_OBJECT_ALWAYS_DRAW] = false;
        }
        return mJson[Constants::SCENE_OBJECT_ALWAYS_DRAW];
    }

    void SceneObjectDefinition::setAlwaysDraw(bool alwaysDraw)
    {
        mJson[Constants::SCENE_OBJECT_ALWAYS_DRAW] = alwaysDraw;
    }

    void SceneObjectDefinition::setIsTemplate(bool d)
    {
        mJson[Constants::SCENE_OBJECT_TEMPLATE] = d;
    }

    bool SceneObjectDefinition::getIsTemplate()
    {
        if (mJson[Constants::SCENE_OBJECT_TEMPLATE].is_null())
        {
            mJson[Constants::SCENE_OBJECT_TEMPLATE] = false;
        }
        return mJson[Constants::SCENE_OBJECT_TEMPLATE];
    }

    void SceneObjectDefinition::setHidden(bool d)
    {
        mJson[Constants::SCENE_OBJECT_HIDDEN] = d;
    }

    bool SceneObjectDefinition::getHidden()
    {
        if (mJson[Constants::SCENE_OBJECT_HIDDEN].is_null())
        {
            mJson[Constants::SCENE_OBJECT_HIDDEN] = false;
        }
        return mJson[Constants::SCENE_OBJECT_HIDDEN];
    }



    SceneObjectDefinition*
    SceneObjectDefinition::getParentSceneObject
    () const
    {
        return mParentSceneObject;
    }

    SceneObjectDefinition*
    SceneObjectDefinition::duplicate()
    {
#ifdef DREAM_DEBUG
        auto log = getLog();
#endif
        // Nothing to assign duplicate to
        if (mParentSceneObject == nullptr)
        {
#ifdef DREAM_DEBUG
            log->error("Cannot Duplicate. No parent to assign duplicate to");
#endif
            return nullptr;
        }

        auto newSOD = new SceneObjectDefinition(mParentSceneObject,mSceneDefinition,getJson(),true);
        newSOD->loadChildSceneObjectDefinitions(true);
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

        mParentSceneObject->addChildDefinition(newSOD);
        return newSOD;
    }

    int
    SceneObjectDefinition::getSelectedAssetIndex
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
    SceneObjectDefinition::setSelectedAssetIndex
    (AssetType type, int index)
    {
        auto typesVector = mSceneDefinition->getProjectDefinition()->getAssetDefinitionsVector(type);
        auto asset = typesVector.at(index);
        setAssetDefinition(type,asset->getUuid());
    }

    void
    SceneObjectDefinition::setAssetDefinition
    (AssetType type, uint32_t uuid)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null() ||
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr] = uuid;
    }

    map<AssetType, uint32_t>
    SceneObjectDefinition::getAssetDefinitionsMap
    ()
    {
        map<AssetType, uint32_t> assetsMap;
        for (auto typePair : Constants::DREAM_ASSET_TYPES_MAP)
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
    SceneObjectDefinition::getAssetDefinition
    (AssetType type)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Getting definition {} of in {}",typeStr, getNameAndUuidString());
#endif
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null() ||
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        if (!mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr].is_number())
        {
#ifdef DREAM_LOG
            log->trace("No Instance");
#endif
            return 0;
        }
#ifdef DREAM_LOG
        log->trace("Found Instance");
#endif
        return mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr];
    }

    void
    SceneObjectDefinition::setEmptyAssetsObject()
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES] = json::object();
    }

    void
    SceneObjectDefinition::setParentSceneObject
    (SceneObjectDefinition* parentSceneObject)
    {
        mParentSceneObject = parentSceneObject;
    }
}
