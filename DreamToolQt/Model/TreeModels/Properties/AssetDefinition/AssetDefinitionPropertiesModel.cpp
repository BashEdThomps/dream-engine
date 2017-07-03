/*
 * AssetDefinitionPropertiesModel.cpp
 *
 * Created: 17 2017 by Ashley
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

#include "AssetDefinitionPropertiesModel.h"

#include "AssetDefinitionPropertiesItem.h"
#include "AssetDefinitionPropertiesTreeDelegate.h"

#include <DreamCore.h>
#include <QDebug>

using Dream::Constants;

AssetDefinitionPropertiesModel::AssetDefinitionPropertiesModel
(AssetDefinition *definition, QTreeView* parent)
    : AbstractPropertiesModel(new AssetDefinitionPropertiesTreeDelegate(this), parent),
      mAssetDefinitionHandle(definition)
{
    qDebug() <<  "AssetDefinitionPropertiesModel: Constructing";
    createRoot();
    createProperties();
}

AssetDefinitionPropertiesModel::~AssetDefinitionPropertiesModel
()
{
    qDebug() <<  "AssetDefinitionPropertiesModel: Destructing";
}

void
AssetDefinitionPropertiesModel::createRoot
()
{
    mRootItem.reset
    (
        new AssetDefinitionPropertiesItem
        (
            QString::fromStdString(mAssetDefinitionHandle->getName()),
            mAssetDefinitionHandle
        )
    );
}

void
AssetDefinitionPropertiesModel::createNameProperty
()
{
    AssetDefinitionPropertiesItem *nameProperty = new AssetDefinitionPropertiesItem
    (
        "Name",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_NAME
    );
    mRootItem->appendChild(nameProperty);
}

void
AssetDefinitionPropertiesModel::onModelFileBrowseButtonClicked
(AssetDefinition *adHandle)
{
    emit notifyModelFileBrowseButtonClicked(adHandle);
}

void
AssetDefinitionPropertiesModel::onModelAdditionalFilesButtonClicked
(AssetDefinition *adHandle)
{
    emit notifyModelAdditionalFilesButtonClicked(adHandle);
}

void
AssetDefinitionPropertiesModel::createTypeProperty
()
{
    AssetDefinitionPropertiesItem *typeProperty = new AssetDefinitionPropertiesItem
    (
        "Type",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_TYPE
    );
    mRootItem->appendChild(typeProperty);

}

void
AssetDefinitionPropertiesModel::createFormatProperty
()
{
    AssetDefinitionPropertiesItem *formatProperty = new AssetDefinitionPropertiesItem
    (
        "Format",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FORMAT
    );
    mRootItem->appendChild(formatProperty);
}

void
AssetDefinitionPropertiesModel::createProperties
()
{
    createNameProperty();
    createTypeProperty();
    createFormatProperty();

    if (mAssetDefinitionHandle->isTypeAnimation())
    {

    }
    else if (mAssetDefinitionHandle->isTypeAudio())
    {
        createAudioLoopProperty();
        createAudioFileProperty();
    }
    else if (mAssetDefinitionHandle->isTypeFont())
    {
        createFontColorProperty();
        createFontSizeProperty();
        createFontFileProperty();
    }
    else if (mAssetDefinitionHandle->isTypeLight())
    {
        createLightColorProperty();
    }
    else if (mAssetDefinitionHandle->isTypeModel())
    {
        createModelFileProperty();
        createModelAdditionalFilesProperty();
    }
    else if (mAssetDefinitionHandle->isTypePhysicsObject())
    {
        craetePhysicsCollisionShapeProperty();
        createPhysicsMassProperty();
        createPhysicsMarginProperty();
        createPhysicsKinematicProperty();
    }
    else if (mAssetDefinitionHandle->isTypeScript())
    {
        createScriptTemplateProperty();
    }
    else if (mAssetDefinitionHandle->isTypeShader())
    {
        createShaderTemplateProperty();
    }
    else if (mAssetDefinitionHandle->isTypeSprite())
    {
        createSpriteTileSizeProperty();
        createSpriteFileProperty();
    }
}

void
AssetDefinitionPropertiesModel::createAudioLoopProperty
()
{

}

void
AssetDefinitionPropertiesModel::createAudioFileProperty
()
{

}

void
AssetDefinitionPropertiesModel::createFontColorProperty
()
{

}

void
AssetDefinitionPropertiesModel::createFontSizeProperty
()
{

}

void
AssetDefinitionPropertiesModel::createFontFileProperty
()
{

}

void
AssetDefinitionPropertiesModel::createLightColorProperty
()
{

}

void
AssetDefinitionPropertiesModel::createModelFileProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Model Assimp File Delegate";

    AssetDefinitionPropertiesItem *mfProperty = new AssetDefinitionPropertiesItem
    (
        "Model File" ,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_FILE
    );
    mRootItem->appendChild(mfProperty);
}

void
AssetDefinitionPropertiesModel::createModelAdditionalFilesProperty
()
{
    qDebug() << "AssetDefinitionPropertiesModel: Create Model Additional Files Delegate";
    AssetDefinitionPropertiesItem *property = new AssetDefinitionPropertiesItem
    (
        "Additional Files",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES
    );
    mRootItem->appendChild(property);
}

void
AssetDefinitionPropertiesModel::craetePhysicsCollisionShapeProperty
()
{

}

void
AssetDefinitionPropertiesModel::createPhysicsMassProperty
()
{

}

void
AssetDefinitionPropertiesModel::createPhysicsMarginProperty
()
{

}

void
AssetDefinitionPropertiesModel::createPhysicsKinematicProperty
()
{

}

void
AssetDefinitionPropertiesModel::createScriptTemplateProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Script Template Delegate";
    // Template
    AssetDefinitionPropertiesItem *templateProperty = new AssetDefinitionPropertiesItem
    (
        "Template",
        mAssetDefinitionHandle
    );
    mRootItem->appendChild(templateProperty);
}

void
AssetDefinitionPropertiesModel::createShaderTemplateProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Shader Template Delegate";

    // Template
    AssetDefinitionPropertiesItem *templateProperty = new AssetDefinitionPropertiesItem
    (
        "Template",
        mAssetDefinitionHandle
    );
    mRootItem->appendChild(templateProperty);
}

void
AssetDefinitionPropertiesModel::createSpriteTileSizeProperty
()
{

}

void
AssetDefinitionPropertiesModel::createSpriteFileProperty
()
{

}
