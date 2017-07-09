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

AssetDefinitionPropertiesItem::AssetDefinitionPropertiesItem
(
    QString title,
    AssetDefinition* mAssetDefinitionHandle,
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

AssetDefinition*
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
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            mAssetDefinitionHandle->setPhysicsObjectConstant(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            mAssetDefinitionHandle->setPhysicsObjectRadius(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            mAssetDefinitionHandle->setPhysicsObjectNormalX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            mAssetDefinitionHandle->setPhysicsObjectNormalY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            mAssetDefinitionHandle->setPhysicsObjectNormalZ(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            mAssetDefinitionHandle->setPhysicsObjectHalfExtentsX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            mAssetDefinitionHandle->setPhysicsObjectHalfExtentsY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            mAssetDefinitionHandle->setPhysicsObjectHalfExtentsZ(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            mAssetDefinitionHandle->setPhysicsObjectMass(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            mAssetDefinitionHandle->setPhysicsObjectMargin(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            mAssetDefinitionHandle->setPhysicsObjectKinematic(value.toBool());
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
            mAssetDefinitionHandle->setAudioLoop(value.toBool());
            break;

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            mAssetDefinitionHandle->setSize(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            mAssetDefinitionHandle->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            mAssetDefinitionHandle->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            mAssetDefinitionHandle->setColourRed(value.toFloat());
            break;

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
            mAssetDefinitionHandle->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
            mAssetDefinitionHandle->setColourGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
            mAssetDefinitionHandle->setColourBlue(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            mAssetDefinitionHandle->setColourAlpha(value.toFloat());
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
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectConstant());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectRadius());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectNormalX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectNormalY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectNormalZ());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            return QVariant();

            // Physics Object
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectHalfExtentsX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectHalfExtentsY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectHalfExtentsZ());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectMass());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectMargin());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            return QVariant(mAssetDefinitionHandle->getPhysicsObjectKinematic());

            // Audio
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            return QVariant(mAssetDefinitionHandle->getAudioLoop());

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            return QVariant(mAssetDefinitionHandle->getSize());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            return QVariant(mAssetDefinitionHandle->getColourRed());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            return QVariant(mAssetDefinitionHandle->getColourGreen());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            return QVariant(mAssetDefinitionHandle->getColourBlue());

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
            return QVariant(mAssetDefinitionHandle->getColourRed());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
            return QVariant(mAssetDefinitionHandle->getColourGreen());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
            return QVariant(mAssetDefinitionHandle->getColourBlue());
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            return QVariant(mAssetDefinitionHandle->getColourAlpha());

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
