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
    (SceneObjectDefinition* parentHandle, json jsonData)
        : IDefinition(jsonData),
          mParentSceneObjectHandle(parentHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectDefinition: Constructing "
                 << getNameAndUuidString()
                 << endl;
        }
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

    bool
    SceneObjectDefinition::hasName
    (string name)
    {
        return getName().compare(name) == 0;
    }

    void
    SceneObjectDefinition::setName
    (string name)
    {
        mJson[Constants::NAME] = name;
    }

    string
    SceneObjectDefinition::getName
    ()
    {
        return mJson[Constants::NAME];
    }


    bool
    SceneObjectDefinition::hasUuid
    (string uuid)
    {
        return (getUuid().compare(uuid) == 0);
    }

    void
    SceneObjectDefinition::setUuid
    (string uuid)
    {
        mJson[Constants::UUID] = uuid;
    }

    string
    SceneObjectDefinition::getUuid
    ()
    {
        return mJson[Constants::UUID];
    }

    Transform3D
    SceneObjectDefinition::getTransform
    ()
    {
        Transform3D transform;

        string transformType;
        if (!mJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE].is_null())
        {
            transformType = mJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE];
            transform.setTransformType(transformType);
        }
        else
        {
            transformType = Constants::SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
            transform.setTransformType(transformType);
        }

        if (!mJson[Constants::SCENE_OBJECT_TRANSLATION].is_null())
        {
            json translation = mJson[Constants::SCENE_OBJECT_TRANSLATION];
            transform.setTranslation(
                translation[Constants::X],
                translation[Constants::Y],
                translation[Constants::Z]
            );
        }

        if (!mJson[Constants::SCENE_OBJECT_ROTATION].is_null())
        {
            json rotation = mJson[Constants::SCENE_OBJECT_ROTATION];
            transform.setRotation(
                rotation[Constants::X],
                rotation[Constants::Y],
                rotation[Constants::Z]
            );
        }

        if (!mJson[Constants::SCENE_OBJECT_SCALE].is_null())
        {
            json scale = mJson[Constants::SCENE_OBJECT_SCALE];
            transform.setScale(
                scale[Constants::X],
                scale[Constants::Y],
                scale[Constants::Z]
            );
        }

        return transform;
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
                    unique_ptr<SceneObjectDefinition>(new SceneObjectDefinition(this, childDefinition))
                );
            }
        }
    }

    vector<unique_ptr<SceneObjectDefinition>>&
    SceneObjectDefinition::getChildDefinitions
    ()
    {
        return mChildDefinitions;
    }
}
