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
    class AssetDefinition;
}

using Dream::AssetDefinition;

enum AssetDefinitionProperty
{
    ASSET_DEFINITION_PROPERTY_NAME,
    ASSET_DEFINITION_PROPERTY_TYPE,
    ASSET_DEFINITION_PROPERTY_FORMAT,
    ASSET_DEFINITION_PROPERTY_ANIMATION_FILE,
    ASSET_DEFINITION_PROPERTY_AUDIO_FILE,
    ASSET_DEFINITION_PROPERTY_FONT_FILE,
    ASSET_DEFINITION_PROPERTY_MODEL_FILE,
    ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES,
    ASSET_DEFINITION_PROPERTY_SCRIPT_FILE,
    ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE,
    ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE,
    ASSET_DEFINITION_PROPERTY_SPRITE_FILE,
    ASSET_DEFINITION_PROPERTY_NONE
};

class AssetDefinitionPropertiesItem : public AbstractPropertiesItem
{
public:
    AssetDefinitionPropertiesItem
    (
        QString title,
        AssetDefinition* adHandle,
        AssetDefinitionProperty property = ASSET_DEFINITION_PROPERTY_NONE,
        AbstractPropertiesItem *parent = nullptr
    );

    ~AssetDefinitionPropertiesItem();

    AssetDefinitionProperty getProperty();

    AssetDefinition* getAssetDefinitionHandle();

    bool setData(int column,const QVariant &value) override;
    QVariant data(int column) override;
private:
    AssetDefinition* mAssetDefinitionHandle;
    AssetDefinitionProperty mProperty;
};
