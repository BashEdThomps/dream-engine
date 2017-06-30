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
#include "AssetDefinitionTypeComboDelegate.h"
#include "AssetDefinitionFormatComboDelegate.h"
#include "BrowseForAdditionalFilesDelegate.h"
#include "OpenInTextEditorDelegate.h"
#include "Shader/ShaderTemplateComboDelegate.h"
#include "Script/ScriptTemplateComboDelegate.h"
#include "Model/ModelFileBrowseDelegate.h"

#include <DreamCore.h>
#include <QDebug>

using Dream::Constants;

AssetDefinitionPropertiesModel::AssetDefinitionPropertiesModel
(Dream::AssetDefinition *definition, QTreeView* parent)
    : PropertiesModel(parent),
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
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mAssetDefinitionHandle->getName())
             << QString::fromStdString(mAssetDefinitionHandle->getUuid());
    mRootItem.reset(new AssetDefinitionPropertiesItem(rootData,mAssetDefinitionHandle));
}

void
AssetDefinitionPropertiesModel::createNameProperty
()
{
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mAssetDefinitionHandle->getName());
    AssetDefinitionPropertiesItem *nameProperty = new AssetDefinitionPropertiesItem
    (
        nameData,
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
    QList<QVariant> typeData;
    typeData << "Type" << QString::fromStdString(mAssetDefinitionHandle->getType());
    AssetDefinitionPropertiesItem *typeProperty = new AssetDefinitionPropertiesItem
    (
        typeData,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_TYPE,
        new AssetDefinitionTypeComboDelegate(mAssetDefinitionHandle)
    );
    mRootItem->appendChild(typeProperty);

}

void
AssetDefinitionPropertiesModel::createFormatProperty
()
{
    QList<QVariant> formatData;
    formatData << "Format" << QString::fromStdString(mAssetDefinitionHandle->getFormat());
    AssetDefinitionPropertiesItem *formatProperty = new AssetDefinitionPropertiesItem
    (
        formatData,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FORMAT,
        new AssetDefinitionFormatComboDelegate(mAssetDefinitionHandle)
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
    mModelFileBrowseDelegateHandle = new ModelFileBrowseDelegate(mAssetDefinitionHandle, this);

    QList<QVariant> mfData;
    mfData << "Model File" << "";
    AssetDefinitionPropertiesItem *mfProperty = new AssetDefinitionPropertiesItem
    (
        mfData,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_FILE,
        mModelFileBrowseDelegateHandle
    );
    mRootItem->appendChild(mfProperty);

    connect
    (
        mModelFileBrowseDelegateHandle,SIGNAL(notifyModelFileBrowseButtonClicked(AssetDefinition*)),
        this, SLOT(onModelFileBrowseButtonClicked(AssetDefinition*))
    );
}

void
AssetDefinitionPropertiesModel::createModelAdditionalFilesProperty
()
{
    qDebug() << "AssetDefinitionPropertiesModel: Create Model Additional Files Delegate";
    mModelAdditionalFilesDelegateHandle = new BrowseForAdditionalFilesDelegate(mAssetDefinitionHandle,this);

    QList<QVariant> data;
    data << "Additional Files" << "";
    AssetDefinitionPropertiesItem *property = new AssetDefinitionPropertiesItem
    (
        data,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES,
        mModelAdditionalFilesDelegateHandle
    );
    mRootItem->appendChild(property);

    connect
    (
        mModelAdditionalFilesDelegateHandle,SIGNAL(notifyBrowseButtonClicked(AssetDefinition*)),
        this, SLOT(onModelAdditionalFilesButtonClicked(AssetDefinition*))
    );
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
    qDebug() << "AssetDefintionPropertiesModel: Creating Shader Template Delegate";

    // Template
    QList<QVariant> templateData;
    templateData << "Template" << "";
    AssetDefinitionPropertiesItem *templateProperty = new AssetDefinitionPropertiesItem
    (
        templateData,
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
    QList<QVariant> templateData;
    templateData << "Template" << "";
    AssetDefinitionPropertiesItem *templateProperty = new AssetDefinitionPropertiesItem
    (
        templateData,
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

bool
AssetDefinitionPropertiesModel::setData
(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "AssetDefinitionPropertiesModel: setData ::"
             << "index.row=" << index.row()
             << "index.column=" << index.column()
             << "value=" << value;

    if (role != Qt::EditRole)
    {
        qDebug() << "AssetDefinitionPropertiesModel: Role is not Qt::EditRole :/";
        return false;
    }

    AssetDefinitionPropertiesItem *item = static_cast<AssetDefinitionPropertiesItem*>(getItem(index));
    AssetDefinition* adHandle = item->getAssetDefinitionHandle();

    switch(item->getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_NAME:
            adHandle->setName(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_TYPE:
            adHandle->setType(value.toString().toStdString());
            break;
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            adHandle->setFormat(value.toString().toStdString());
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

    bool result = item->setData(index.column(),value);

    if (result)
    {
        emit dataChanged(index, index);
    }

    return result;
}

