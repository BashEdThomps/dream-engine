/*
 * AssetDefinitionPropertiesItem.cpp
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
#include "AssetDefinitionPropertiesItem.h"

#include <DreamCore.h>

using Dream::AnimationDefinition;
using Dream::AudioDefinition;
using Dream::FontDefinition;
using Dream::LightDefinition;
using Dream::ModelDefinition;
using Dream::PhysicsObjectDefinition;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;
using Dream::SpriteDefinition;

AssetDefinitionPropertiesItem::AssetDefinitionPropertiesItem
(
    QString title,
    IAssetDefinition* mAssetDefinitionHandle,
    AssetDefinitionProperty property,
    AbstractPropertiesItem *parent
)
    : AbstractPropertiesItem(title,nullptr,parent),
      mAssetDefinitionHandle(mAssetDefinitionHandle),
      mProperty(property)
{

}

AssetDefinitionPropertiesItem::~AssetDefinitionPropertiesItem
()
{

}

AssetDefinitionProperty
AssetDefinitionPropertiesItem::getProperty
()
{
    return mProperty;
}

IAssetDefinition*
AssetDefinitionPropertiesItem::getAssetDefinitionHandle
()
{
    return mAssetDefinitionHandle;
}

bool
AssetDefinitionPropertiesItem::setData
(int column, const QVariant &value)
{
    if (column == 0)
    {
        return true;
    }

    switch(getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setConstant(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setRadius(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setNormalX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setNormalY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setNormalZ(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setHalfExtentsX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setHalfExtentsY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setHalfExtentsZ(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setMass(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setMargin(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->setKinematic(value.toBool());
            break;

            // Common
        case ASSET_DEFINITION_PROPERTY_NAME:
            mAssetDefinitionHandle->setName(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_TYPE:
            mAssetDefinitionHandle->setType(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            mAssetDefinitionHandle->setFormat(value.toString().toStdString());
            break;

            // Audio
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            dynamic_cast<AudioDefinition*>(mAssetDefinitionHandle)->setLoop(value.toBool());
            break;

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->setSize(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->setColourRed(value.toFloat());
            break;

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
            dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
            dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->setColourGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
            dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->setColourBlue(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->setIntensity(value.toFloat());
            break;

            // Not used
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_TEMPLATE:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
            break;
    }
    return true;
}

QVariant
AssetDefinitionPropertiesItem::data
(int column)
{
    if (column == 0)
    {
        return QVariant(mTitle);
    }

    switch(getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getConstant());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getRadius());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getNormalX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getNormalY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getNormalZ());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            return QVariant();

            // Physics Object
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getHalfExtentsX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getHalfExtentsY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getHalfExtentsZ());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getMass());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getMargin());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            return QVariant(dynamic_cast<PhysicsObjectDefinition*>(mAssetDefinitionHandle)->getKinematic());

            // Audio
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            return QVariant(dynamic_cast<AudioDefinition*>(mAssetDefinitionHandle)->getLoop());

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            return QVariant(dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->getSize());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            return QVariant(dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->getColourRed());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            return QVariant(dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->getColourGreen());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            return QVariant(dynamic_cast<FontDefinition*>(mAssetDefinitionHandle)->getColourBlue());

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
            return QVariant(dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->getColourRed());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
            return QVariant(dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->getColourGreen());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
            return QVariant(dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->getColourBlue());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            return QVariant(dynamic_cast<LightDefinition*>(mAssetDefinitionHandle)->getIntensity());

            // Common
        case ASSET_DEFINITION_PROPERTY_NAME:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getName()));
        case ASSET_DEFINITION_PROPERTY_TYPE:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getType()));
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getFormat()));

            // Not Used
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_TEMPLATE:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
            return QVariant();
    }
    return QVariant();
}
