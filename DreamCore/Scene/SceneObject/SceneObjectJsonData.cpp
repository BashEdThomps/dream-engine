/*
 * SceneObjectJsonData.cpp
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
#include "SceneObjectJsonData.h"

#include "SceneObjectRuntime.h"

#include "../Common/Constants.h"
#include "../Common/Transform3D.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    SceneObjectJsonData::SceneObjectJsonData(json jsonData, SceneObject* parentHandle)
        : JsonData(jsonData),
          mParentHandle(parentHandle)
    {

    }

    bool
    SceneObjectJsonData::hasName
    (string name)
    {
        return getName().compare(name) == 0;
    }

    void
    SceneObjectJsonData::setName
    (string name)
    {
        mJson[Constants::SCENE_OBJECT_NAME] = name;
    }

    string
    SceneObjectJsonData::getName
    ()
    {
        return mJson[Constants::SCENE_OBJECT_NAME];
    }


    bool
    SceneObjectJsonData::hasUuid
    (string uuid)
    {
        return (getUuid().compare(uuid) == 0);
    }

    void
    SceneObjectJsonData::setUuid
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_UUID] = uuid;
    }

    string
    SceneObjectJsonData::getUuid
    ()
    {
        return mJson[Constants::SCENE_OBJECT_UUID];
    }

    void
    SceneObjectJsonData::applyDataToRuntime
    (SceneObjectRuntime* runtime)
    {
        if (!mJson.is_object())
        {
           mJson[Constants::SCENE_OBJECT_UUID] = Uuid::generateUuid();
           return;
        }

        string transformType;
        if (!mJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE].is_null())
        {
            transformType = mJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE];
            runtime->getTransform()->setTransformType(transformType);
        }
        else
        {
            transformType = Constants::SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
            runtime->getTransform()->setTransformType(transformType);
        }

        if (!mJson[Constants::SCENE_OBJECT_TRANSLATION].is_null())
        {
            json translation = mJson[Constants::SCENE_OBJECT_TRANSLATION];
            runtime->setTranslation(
                translation[Constants::X],
                translation[Constants::Y],
                translation[Constants::Z]
            );
        }
        else
        {
            runtime->resetTranslation();
        }

        if (!mJson[Constants::SCENE_OBJECT_ROTATION].is_null())
        {
            json rotation = mJson[Constants::SCENE_OBJECT_ROTATION];
            runtime->setRotation(
                rotation[Constants::X],
                rotation[Constants::Y],
                rotation[Constants::Z]
            );
        }
        else
        {
            runtime->resetRotation();
        }

        if (!mJson[Constants::SCENE_OBJECT_SCALE].is_null())
        {
            json scale = mJson[Constants::SCENE_OBJECT_SCALE];
            runtime->setScale(
                scale[Constants::X],
                scale[Constants::Y],
                scale[Constants::Z]
            );
        }
        else
        {
            runtime->resetScale();
        }

        if(!mJson[Constants::SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            bool focus = mJson[Constants::SCENE_OBJECT_HAS_FOCUS];
            runtime->setHasFocus(focus);
        }
    }

    void
    SceneObjectJsonData::setHasFocus
    (bool focus)
    {
        mJson[Constants::SCENE_OBJECT_HAS_FOCUS] = focus;
    }

    bool
    SceneObjectJsonData::hasFocus
    ()
    {
        if (mJson[Constants::SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            mJson[Constants::SCENE_OBJECT_HAS_FOCUS] = false;
        }

        return mJson[Constants::SCENE_OBJECT_HAS_FOCUS];
    }

    void
    SceneObjectJsonData::addAssetDefUuidToLoad
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].push_back(uuid);
    }

    vector<string>
    SceneObjectJsonData::getAssetDefUuidsToLoad
    ()
    {
        vector<string> toLoad;
        for (json uuid : mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES])
        {
            toLoad.push_back(uuid);
        }
        return toLoad;
    }
}
