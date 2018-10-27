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
#include "../../Components/Transform3D.h"
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
        mTransform = new Transform3D(jsonData[Constants::TRANSFORM]);
    }

    SceneObjectDefinition::~SceneObjectDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );

        if (mTransform != nullptr)
        {
            delete mTransform;
            mTransform = nullptr;
        }

        deleteChildSceneObjectDefinitions();
    }

    Transform3D*
    SceneObjectDefinition::getTransform
    ()
    {
        return mTransform;
    }

    void
    SceneObjectDefinition::setTransform
    (Transform3D* tform)
    {
        mTransform = tform;
    }

    void
    SceneObjectDefinition::setHasFocus
    (bool focus)
    {
        mJson[Constants::SCENE_OBJECT_HAS_FOCUS] = focus;
    }

    bool
    SceneObjectDefinition::getHasFocus
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            mJson[Constants::SCENE_OBJECT_HAS_FOCUS] = false;
        }

        return mJson[Constants::SCENE_OBJECT_HAS_FOCUS];
    }

    void
    SceneObjectDefinition::setFollowsCamera
    (bool fc)
    {
        mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA] = fc;
    }

    bool
    SceneObjectDefinition::getFollowsCamera
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA].is_null())
        {
            mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA] = false;
        }
        return mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA];
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
    SceneObjectDefinition::addChildSceneObjectDefinition
    (SceneObjectDefinition* child)
    {
        mChildDefinitions.push_back(child);
    }

    void
    SceneObjectDefinition::removeChildSceneObjectDefinition
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
                mChildDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    SceneObjectDefinition*
    SceneObjectDefinition::createNewChildSceneObjectDefinition
    (json* fromJson)
    {
        auto log = getLog();
        log->debug("Creating new child scene object");

        json defJson;

        if (fromJson == nullptr)
        {
            log->debug("from scratch");
            defJson[Constants::NAME] = Constants::SCENE_OBJECT_DEFAULT_NAME;

            Transform3D transform;
            transform.setScale(1.0f,1.0f,1.0f);
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
        addChildSceneObjectDefinition(soDefinition);

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
        mJson[Constants::TRANSFORM] = mTransform->getJson();
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
            newSOD->setName(getName()+" (Copy)");
        }

        mParentSceneObject->addChildSceneObjectDefinition(newSOD);
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
        log->debug("Getting definition {} of in {}",typeStr, getNameAndUuidString());
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null() ||
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_array())
        {
            setEmptyAssetsObject();
        }
        if (mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr].is_null())
        {
            log->debug("No Instance");
            return "";
        }
        log->debug("Found Instance");
        return mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES][typeStr];
    }

    void
    SceneObjectDefinition::setEmptyAssetsObject()
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES] = json::object();
    }
}
