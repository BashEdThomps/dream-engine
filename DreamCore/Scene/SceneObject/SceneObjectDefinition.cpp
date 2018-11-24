/*
 * SceneObjectDefinition.cpp
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include "../../Components/IAssetDefinition.h"
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
        : IDefinition(jsonData),
          mParentSceneObject(parent),
          mSceneDefinition(sceneDefinition)
    {

        setLogClassName("SceneObjectDefinition");
        auto log = getLog();
        log->trace( "Constructing {}",getNameAndUuidString());
        if (randomUuid)
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
            log->trace( "With new UUID",getNameAndUuidString());
        }
        mJson[Constants::TRANSFORM] = jsonData[Constants::TRANSFORM];
    }

    SceneObjectDefinition::~SceneObjectDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );
        deleteChildSceneObjectDefinitions();
    }

    int SceneObjectDefinition::getChildCount()
    {
        if (mJson[Constants::SCENE_OBJECT_CHILDREN].is_null())
        {
            return 0;
        }
        return mJson[Constants::SCENE_OBJECT_CHILDREN].size();
    }

    Transform
    SceneObjectDefinition::getTransform
    ()
    {
        return Transform(mJson[Constants::TRANSFORM]);
    }

    void
    SceneObjectDefinition::setTransform
    (Transform tform)
    {
        mJson[Constants::TRANSFORM] = tform.getJson();
    }

    void
    SceneObjectDefinition::setHasInputFocus
    (bool focus)
    {
        mJson[Constants::SCENE_OBJECT_HAS_INPUT_FOCUS] = focus;
    }

    bool
    SceneObjectDefinition::getHasInputFocus
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_HAS_INPUT_FOCUS].is_null())
        {
            mJson[Constants::SCENE_OBJECT_HAS_INPUT_FOCUS] = false;
        }

        return mJson[Constants::SCENE_OBJECT_HAS_INPUT_FOCUS];
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
    SceneObjectDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->debug( mJson.dump(1) );
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

    void SceneObjectDefinition::deleteChildSceneObjectDefinitions()
    {
       for (auto child : mChildDefinitions)
       {
           delete child;
       }
       mChildDefinitions.clear();
    }

    vector<SceneObjectDefinition*>
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
    SceneObjectDefinition::removeChildDefinition
    (SceneObjectDefinition* child)
    {
        auto log = getLog();
        auto iter = begin(mChildDefinitions);
        auto endPos = end(mChildDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == child)
            {
                log->debug
                (
                    "Found child to {} remove from {}",
                    child->getNameAndUuidString(),
                    getNameAndUuidString()
                );
                delete (*iter);
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
        auto log = getLog();
        log->debug("Creating new child scene object");

        json defJson;

        if (fromJson == nullptr)
        {
            log->debug("from scratch");
            defJson[Constants::NAME] = Constants::SCENE_OBJECT_DEFAULT_NAME;

            Transform transform;
            defJson[Constants::TRANSFORM] = transform.getJson();
        }
        else
        {
            log->debug("from template copy");
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

    void SceneObjectDefinition::setStatic(bool d)
    {
        mJson[Constants::SCENE_OBJECT_STATIC] = d;
    }

    bool SceneObjectDefinition::getStatic()
    {
        if (mJson[Constants::SCENE_OBJECT_STATIC].is_null())
        {
            mJson[Constants::SCENE_OBJECT_STATIC] = false;
        }
        return mJson[Constants::SCENE_OBJECT_STATIC];
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
        auto log = getLog();
        // Nothing to assign duplicate to
        if (mParentSceneObject == nullptr)
        {
            log->error("Cannot Duplicate. No parent to assign duplicate to");
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
        if (asset.empty())
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

    void SceneObjectDefinition::setAssetDefinition(AssetType type, string uuid)
    {
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null() ||
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr] = uuid;
    }

    map<AssetType, string>
    SceneObjectDefinition::getAssetDefinitionsMap
    ()
    {
        map<AssetType, string> assetsMap;
        for (auto typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            AssetType type = typePair.first;
            string uuid = getAssetDefinition(type);
            if (!uuid.empty())
            {
                assetsMap.insert(pair<AssetType,string>(type, uuid));
            }
        }
        return assetsMap;
    }

    string
    SceneObjectDefinition::getAssetDefinition
    (AssetType type)
    {
        auto log = getLog();
        auto typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
        log->trace("Getting definition {} of in {}",typeStr, getNameAndUuidString());
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null() ||
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr].is_null())
        {
            log->trace("No Instance");
            return "";
        }
        log->trace("Found Instance");
        return mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr];
    }

    void
    SceneObjectDefinition::setEmptyAssetsObject()
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES] = json::object();
    }
}
