/*
 * AssetDefinitionPropertiesItem.h
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
    class IAssetDefinition;
}

using Dream::IAssetDefinition;

enum AssetDefinitionProperty
{
    // Common
    ASSET_DEFINITION_PROPERTY_NAME,
    ASSET_DEFINITION_PROPERTY_TYPE,
    ASSET_DEFINITION_PROPERTY_FORMAT,
    ASSET_DEFINITION_PROPERTY_REMOVE_FILES,

    // Path
    ASSET_DEFINITION_PROPERTY_PATH_LIST,
    ASSET_DEFINITION_PROPERTY_PATH_TYPE,

    // Audio
    ASSET_DEFINITION_PROPERTY_AUDIO_FILE,
    ASSET_DEFINITION_PROPERTY_AUDIO_LOOP,

    // Font
    ASSET_DEFINITION_PROPERTY_FONT_FILE,
    ASSET_DEFINITION_PROPERTY_FONT_SIZE,
    ASSET_DEFINITION_PROPERTY_FONT_COLOUR,
    ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED,
    ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN,
    ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE,

    // Light
    ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR,
    ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED,
    ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN,
    ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE,
    ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA,

    // Model
    ASSET_DEFINITION_PROPERTY_MODEL_FILE,
    ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES,
    ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE,

    // Physics Object
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HEIGHT,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONTROLLABLE,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD,
    ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE,

    // Script
    ASSET_DEFINITION_PROPERTY_SCRIPT_FILE,

    // Shader
    ASSET_DEFINITION_PROPERTY_SHADER_FILES,

    // Sprite
    ASSET_DEFINITION_PROPERTY_SPRITE_FILE,

    // None
    ASSET_DEFINITION_PROPERTY_NONE
};

class AssetDefinitionPropertiesItem : public AbstractPropertiesItem
{
public:
    AssetDefinitionPropertiesItem
    (
        QString title,
        IAssetDefinition* adHandle,
        AssetDefinitionProperty property = ASSET_DEFINITION_PROPERTY_NONE,
        AbstractPropertiesItem *parent = nullptr
    );
    ~AssetDefinitionPropertiesItem() override;

    bool setData(int column,const QVariant &value) override;
    QVariant data(int column) override;
    AssetDefinitionProperty getProperty();
    IAssetDefinition* getAssetDefinitionHandle();

private:
    IAssetDefinition* mAssetDefinitionHandle;
    AssetDefinitionProperty mProperty;
};
