/*
 * ScenePropertiesItem.h
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
    class SceneDefinition;
}

using Dream::SceneDefinition;

enum SceneProperty
{
    SCENE_PROPERTY_NAME,
    SCENE_PROPERTY_NOTES,

    SCENE_PROPERTY_CAMERA_TRANSLATION_X,
    SCENE_PROPERTY_CAMERA_TRANSLATION_Y,
    SCENE_PROPERTY_CAMERA_TRANSLATION_Z,

    SCENE_PROPERTY_CAMERA_ROTATION_X,
    SCENE_PROPERTY_CAMERA_ROTATION_Y,
    SCENE_PROPERTY_CAMERA_ROTATION_Z,

    SCENE_PROPERTY_CAMERA_SPEED,

    SCENE_PROPERTY_CLEAR_RED,
    SCENE_PROPERTY_CLEAR_GREEN,
    SCENE_PROPERTY_CLEAR_BLUE,

    SCENE_PROPERTY_AMBIENT_RED,
    SCENE_PROPERTY_AMBIENT_GREEN,
    SCENE_PROPERTY_AMBIENT_BLUE,
    SCENE_PROPERTY_AMBIENT_ALPHA,

    SCENE_PROPERTY_PHYSICS_GRAVITY_X,
    SCENE_PROPERTY_PHYSICS_GRAVITY_Y,
    SCENE_PROPERTY_PHYSICS_GRAVITY_Z,
    SCENE_PROPERTY_PHYSICS_DEBUG,

    SCENE_PROPERTY_NONE
};

class ScenePropertiesItem : public AbstractPropertiesItem
{
public:
    ScenePropertiesItem
    (
        QString title,
        SceneDefinition* sdHandle,
        SceneProperty property = SCENE_PROPERTY_NONE,
        QItemDelegate* delegate = nullptr,
        AbstractPropertiesItem *parent = nullptr
    );

    ~ScenePropertiesItem();

    QVariant data(int column) override;
    bool setData(int column, const QVariant &value) override;

private:
    SceneDefinition* mSceneDefinitionHandle;
    SceneProperty mProperty;
};
