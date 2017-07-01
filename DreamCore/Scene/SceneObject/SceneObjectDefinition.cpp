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

#include "../../Common/Constants.h"

#include "../../Components/Transform3D.h"

#include "../../Utilities/Uuid.h"

namespace Dream
{
    SceneObjectDefinition::SceneObjectDefinition
    (SceneObjectDefinition* parentHandle, SceneDefinition* sceneDefinitionHandle, json jsonData)
        : IDefinition(jsonData),
          mParentSceneObjectHandle(parentHandle),
          mSceneDefinitionHandle(sceneDefinitionHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectDefinition: Constructing "
                 << getNameAndUuidString()
                 << endl;
        }
        mTransform = Transform3D(jsonData[Constants::TRANSFORM]);
        loadChildSceneObjectDefinitions(jsonData);
    }

    SceneObjectDefinition::~SceneObjectDefinition
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectDefinition: Destructing "
                 << getNameAndUuidString()
                 << endl;
        }
    }

    Transform3D&
    SceneObjectDefinition::getTransform
    ()
    {
        return mTransform;
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
    SceneObjectDefinition::addAssetDefinitionUuidToLoadQueue
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].push_back(uuid);
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
        if (Constants::DEBUG)
        {
            cout << "SceneObjectDefinition: " << mJson.dump(1) << endl;
        }
    }

    void
    SceneObjectDefinition::loadChildSceneObjectDefinitions
    (json definition)
    {
        json childrenArray = definition[Constants::SCENE_OBJECT_CHILDREN];

        if (!childrenArray.is_null() || childrenArray.is_array() )
        {
            for (json childDefinition : childrenArray)
            {
                mChildDefinitions.push_back
                (
                    unique_ptr<SceneObjectDefinition>(new SceneObjectDefinition(this, mSceneDefinitionHandle, childDefinition))
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
}
