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
#include <regex>

#include "../../Components/IAssetDefinition.h"
#include "../../Common/Constants.h"
#include "../../Components/Transform3D.h"
#include "../../Utilities/Uuid.h"

namespace Dream
{
    SceneObjectDefinition* SceneObjectDefinition::getParentSceneObjectHandle() const
    {
        return mParentSceneObjectHandle;
    }

    SceneObjectDefinition* SceneObjectDefinition::duplicate()
    {
        if (mParentSceneObjectHandle == nullptr)
        {
            return nullptr;
        }

        auto newSOD = new SceneObjectDefinition(mParentSceneObjectHandle,mSceneDefinitionHandle,mJson,true);
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

        mParentSceneObjectHandle->addChildSceneObjectDefinition(newSOD);
        return newSOD;
    }



    SceneObjectDefinition::SceneObjectDefinition
    (SceneObjectDefinition* parentHandle, SceneDefinition* sceneDefinitionHandle, json jsonData, bool randomUuid)
        : IDefinition(jsonData),
          ILoggable ("SceneObjectDefinition"),
          mParentSceneObjectHandle(parentHandle),
          mSceneDefinitionHandle(sceneDefinitionHandle)
    {
        auto log = getLog();
        log->trace( "Constructing {}",getNameAndUuidString());
        if (randomUuid)
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
            log->trace( "With new UUID",getNameAndUuidString());
        }
        mTransform = Transform3D(jsonData[Constants::TRANSFORM]);
        loadChildSceneObjectDefinitions(jsonData,randomUuid);
    }

    SceneObjectDefinition::~SceneObjectDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}",
                    getNameAndUuidString()
                    );
    }

    Transform3D&
    SceneObjectDefinition::getTransform
    ()
    {
        return mTransform;
    }

    void SceneObjectDefinition::setTransform(Transform3D& tform)
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
    SceneObjectDefinition::hasFocus
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
    SceneObjectDefinition::followsCamera
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA].is_null())
        {
            mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA] = false;
        }
        return mJson[Constants::SCENE_OBJECT_FOLLOWS_CAMERA];
    }

    void
    SceneObjectDefinition::addAssetDefinitionToLoadQueue
    (IAssetDefinition* adHandle)
    {
        addAssetDefinitionUuidToLoadQueue(adHandle->getUuid());
    }

    void
    SceneObjectDefinition::addAssetDefinitionUuidToLoadQueue
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].push_back(uuid);
    }

    void
    SceneObjectDefinition::removeAssetDefinitionUuidFromLoadQueue
    (string uuid)
    {
        auto log = getLog();
        log->info("Attempting to remove instance of {} from {}",uuid,getNameAndUuidString());
        auto iter = find
                (
                    begin(mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES]),
                end(mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES]),
                uuid
                );
        if (iter != end(mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES]))
        {
            mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].erase(iter);
        }
    }

    void
    SceneObjectDefinition::removeAssetDefinitionFromLoadQueue
    (IAssetDefinition* adHandle)
    {
        removeAssetDefinitionUuidFromLoadQueue(adHandle->getUuid());
    }

    vector<string>
    SceneObjectDefinition::getAssetDefinitionLoadQueue
    ()
    {
        vector<string> toLoad;
        for (json uuid : mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES])
        {
            toLoad.push_back(uuid);
        }
        return toLoad;
    }

    void
    SceneObjectDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->info( mJson.dump(1) );
    }

    void
    SceneObjectDefinition::loadChildSceneObjectDefinitions
    (json definition, bool randomUuid)
    {
        json childrenArray = definition[Constants::SCENE_OBJECT_CHILDREN];

        if (!childrenArray.is_null() || childrenArray.is_array() )
        {
            for (json childDefinition : childrenArray)
            {
                mChildDefinitions.push_back(
                    unique_ptr<SceneObjectDefinition>(
                        new SceneObjectDefinition(
                            this, mSceneDefinitionHandle, childDefinition, randomUuid
                        )
                    )
                );
            }
        }
    }

    vector<SceneObjectDefinition*>
    SceneObjectDefinition::getChildDefinitionsHandleList
    ()
    {
        vector<SceneObjectDefinition*> list;
        for (auto it = begin(mChildDefinitions); it != end(mChildDefinitions); it++)
        {
            list.push_back((*it).get());
        }
        return list;
    }

    void
    SceneObjectDefinition::addChildSceneObjectDefinition
    (SceneObjectDefinition* child)
    {
        mChildDefinitions.push_back(unique_ptr<SceneObjectDefinition>(child));
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
            if ((*iter).get() == child)
            {
                log->info(
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
        log->info("Creating new child scene object");

        json defJson;

        if (fromJson == nullptr)
        {
            log->info("from scratch");
            defJson[Constants::NAME] = Constants::SCENE_OBJECT_DEFAULT_NAME;

            Transform3D transform;
            transform.setScale(1.0f,1.0f,1.0f);
            defJson[Constants::TRANSFORM] = transform.getJson();
        }
        else
        {
            log->info("from template copy");
            defJson = json::parse(fromJson->dump());
        }

        SceneObjectDefinition *soDefinition;
        soDefinition = new SceneObjectDefinition(this,mSceneDefinitionHandle,defJson,true);


        addChildSceneObjectDefinition(soDefinition);

        return soDefinition;
    }


    SceneDefinition*
    SceneObjectDefinition::getSceneDefinitionHandle
    ()
    {
        return mSceneDefinitionHandle;
    }

    json
    SceneObjectDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_OBJECT_CHILDREN] = json::array();
        mJson[Constants::TRANSFORM] = mTransform.getJson();
        for (SceneObjectDefinition* sodHandle : getChildDefinitionsHandleList())
        {
            mJson[Constants::SCENE_OBJECT_CHILDREN].push_back(sodHandle->getJson());
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




}
