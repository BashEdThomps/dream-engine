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
#include "AssetDefinitionTypeComboDelegate.h"
#include "AssetDefinitionFormatComboDelegate.h"
#include "BrowseForAdditionalFilesDelegate.h"
#include "OpenInTextEditorDelegate.h"

#include "Shader/ShaderTemplateComboDelegate.h"
#include "Script/ScriptTemplateComboDelegate.h"
#include "Model/ModelFileBrowseDelegate.h"

#include <QDebug>
// Index Constants
const int AssetDefinitionPropertiesModel::NAME_INDEX = 0;
const int AssetDefinitionPropertiesModel::TYPE_INDEX = 1;
const int AssetDefinitionPropertiesModel::FORMAT_INDEX = 2;
const int AssetDefinitionPropertiesModel::SCRIPT_TEMPLATE_INDEX = 3;
const int AssetDefinitionPropertiesModel::SHADER_TEMPLATE_INDEX = 3;
const int AssetDefinitionPropertiesModel::MODEL_ASSIMP_FILE_INDEX = 3;
const int AssetDefinitionPropertiesModel::MODEL_OTHER_FILE_INDEX = 4;

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
    mRootItem.reset(new PropertiesItem(rootData,mAssetDefinitionHandle));
}

void
AssetDefinitionPropertiesModel::createNameProperty
()
{
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mAssetDefinitionHandle->getName());
    PropertiesItem *nameProperty = new PropertiesItem(nameData,mAssetDefinitionHandle);
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
    PropertiesItem *typeProperty = new PropertiesItem(typeData,mAssetDefinitionHandle);
    mRootItem->appendChild(typeProperty);

    setTreeViewDelegateForRow(TYPE_INDEX, new AssetDefinitionTypeComboDelegate());
}

void
AssetDefinitionPropertiesModel::createFormatProperty
()
{
    QList<QVariant> formatData;
    formatData << "Format" << QString::fromStdString(mAssetDefinitionHandle->getFormat());
    PropertiesItem *formatProperty = new PropertiesItem(formatData,mAssetDefinitionHandle);
    mRootItem->appendChild(formatProperty);

    setTreeViewDelegateForRow(
        FORMAT_INDEX,
        new AssetDefinitionFormatComboDelegate(mAssetDefinitionHandle->getType())
    );
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

    setTreeViewDelegateForRow
    (
        MODEL_ASSIMP_FILE_INDEX,
        mModelFileBrowseDelegateHandle
    );

    QList<QVariant> mfData;
    mfData << "Model File" << "";
    PropertiesItem *mfProperty = new PropertiesItem(mfData,mAssetDefinitionHandle);
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
    setTreeViewDelegateForRow
    (
        MODEL_OTHER_FILE_INDEX,
        mModelAdditionalFilesDelegateHandle
    );

    QList<QVariant> data;
    data << "Additional Files" << "";
    PropertiesItem *property = new PropertiesItem(data,mAssetDefinitionHandle);
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
    mTreeViewHandle->setItemDelegateForRow(
        SCRIPT_TEMPLATE_INDEX,
        new ScriptTemplateComboDelegate()
    );

    // Template
    QList<QVariant> templateData;
    templateData << "Template" << "";
    PropertiesItem *templateProperty = new PropertiesItem(templateData,mAssetDefinitionHandle);
    mRootItem->appendChild(templateProperty);
}

void
AssetDefinitionPropertiesModel::createShaderTemplateProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Shader Template Delegate";
    mTreeViewHandle->setItemDelegateForRow(
        SHADER_TEMPLATE_INDEX,
        new ShaderTemplateComboDelegate()
    );

    // Template
    QList<QVariant> templateData;
    templateData << "Template" << "";
    PropertiesItem *templateProperty = new PropertiesItem(templateData,mAssetDefinitionHandle);
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

    PropertiesItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
    {
        emit dataChanged(index, index);
    }

    return result;
}

