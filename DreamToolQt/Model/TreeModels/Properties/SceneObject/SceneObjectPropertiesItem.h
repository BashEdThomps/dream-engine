/*
 * SceneObjectPropertiesItem.h
 *
 * Created: 29 2017 by Ashley
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

#pragma once

#include "../AbstractPropertiesItem.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

enum SceneObjectProperty
{
    SCENE_OBJECT_PROPERTY_NAME,
    SCENE_OBJECT_PROPERTY_TRANSLATION_X,
    SCENE_OBJECT_PROPERTY_TRANSLATION_Y,
    SCENE_OBJECT_PROPERTY_TRANSLATION_Z,
    SCENE_OBJECT_PROPERTY_ROTATION_X,
    SCENE_OBJECT_PROPERTY_ROTATION_Y,
    SCENE_OBJECT_PROPERTY_ROTATION_Z,
    SCENE_OBJECT_PROPERTY_SCALE_X,
    SCENE_OBJECT_PROPERTY_SCALE_Y,
    SCENE_OBJECT_PROPERTY_SCALE_Z,
    SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE,
    SCENE_OBJECT_PROPERTY_HAS_FOCUS,
    SCENE_OBJECT_PROPERTY_NONE
};

class SceneObjectPropertiesItem : public AbstractPropertiesItem
{
public:
    SceneObjectPropertiesItem
    (
        const QList<QVariant> &data,
        SceneObjectDefinition* sodHandle,
        SceneObjectProperty property = SCENE_OBJECT_PROPERTY_NONE,
        QItemDelegate* delegate = nullptr,
        AbstractPropertiesItem *parent = nullptr
    );
    ~SceneObjectPropertiesItem();
private:
    SceneObjectDefinition *mSceneObjectDefinitionHandle;
    SceneObjectProperty mProperty;
};
