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
        return false;
    }

    switch(getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_NAME:
            mAssetDefinitionHandle->setName(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_TYPE:
            mAssetDefinitionHandle->setType(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            mAssetDefinitionHandle->setFormat(value.toString().toStdString());
            break;
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
        case ASSET_DEFINITION_PROPERTY_NAME:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getName()));
        case ASSET_DEFINITION_PROPERTY_TYPE:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getType()));
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getFormat()));
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
}
