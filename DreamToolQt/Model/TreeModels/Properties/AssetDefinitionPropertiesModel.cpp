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

#include "Shader/ShaderTemplateComboDelegate.h"
#include "Script/ScriptTemplateComboDelegate.h"

#include <QDebug>

AssetDefinitionPropertiesModel::AssetDefinitionPropertiesModel
(Dream::AssetDefinition *definition, QTreeView* parent)
    : PropertiesModel(parent)
{
    mAssetDefinition = definition;
    createDelegates();
    createRoot();
    createProperties();
}

AssetDefinitionPropertiesModel::~AssetDefinitionPropertiesModel
()
{
    delete mRootItem;
}

void
AssetDefinitionPropertiesModel::createDelegates
()
{
    mTreeView->setItemDelegateForRow(
        1 ,new AssetDefinitionTypeComboDelegate()
    );
    mTreeView->setItemDelegateForRow(
        2, new AssetDefinitionFormatComboDelegate(mAssetDefinition->getType())
    );
}

void
AssetDefinitionPropertiesModel::createRoot
()
{
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mAssetDefinition->getName())
             << QString::fromStdString(mAssetDefinition->getUuid());
    mRootItem = new PropertiesItem(rootData,mAssetDefinition);
}

void
AssetDefinitionPropertiesModel::createProperties
()
{
    // Name
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mAssetDefinition->getName());
    PropertiesItem *nameProperty = new PropertiesItem(nameData,mAssetDefinition);
    mRootItem->appendChild(nameProperty);

    // Type
    QList<QVariant> typeData;
    typeData << "Type" << QString::fromStdString(mAssetDefinition->getType());
    PropertiesItem *typeProperty = new PropertiesItem(typeData,mAssetDefinition);
    mRootItem->appendChild(typeProperty);

    // Format
    QList<QVariant> formatData;
    formatData << "Format" << QString::fromStdString(mAssetDefinition->getFormat());
    PropertiesItem *formatProperty = new PropertiesItem(formatData,mAssetDefinition);
    mRootItem->appendChild(formatProperty);

    if (mAssetDefinition->isTypeAnimation())
    {

    }
    else if (mAssetDefinition->isTypeAudio())
    {
        createAudioLoopProperty();
        createAudioFileProperty();
    }
    else if (mAssetDefinition->isTypeFont())
    {
        createFontColorProperty();
        createFontSizeProperty();
        createFontFileProperty();
    }
    else if (mAssetDefinition->isTypeLight())
    {
        createLightColorProperty();
    }
    else if (mAssetDefinition->isTypeModel())
    {
        createModelFileProperty();
        createModelOtherFilesProperty();
    }
    else if (mAssetDefinition->isTypePhysicsObject())
    {
        craetePhysicsCollisionShapeProperty();
        createPhysicsMassProperty();
        createPhysicsMarginProperty();
        createPhysicsKinematicProperty();
    }
    else if (mAssetDefinition->isTypeScript())
    {
        createScriptTemplateProperty();
    }
    else if (mAssetDefinition->isTypeShader())
    {
        createShaderTemplateProperty();
    }
    else if (mAssetDefinition->isTypeSprite())
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

}

void
AssetDefinitionPropertiesModel::createModelOtherFilesProperty
()
{

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
    mTreeView->setItemDelegateForRow(
        3 ,new ScriptTemplateComboDelegate()
    );

    // Template
    QList<QVariant> templateData;
    templateData << "Template" << "";
    PropertiesItem *templateProperty = new PropertiesItem(templateData,mAssetDefinition);
    mRootItem->appendChild(templateProperty);
}

void
AssetDefinitionPropertiesModel::createShaderTemplateProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Shader Template Delegate";
    mTreeView->setItemDelegateForRow(
        3 ,new ShaderTemplateComboDelegate()
    );

    // Template
    QList<QVariant> templateData;
    templateData << "Template" << "";
    PropertiesItem *templateProperty = new PropertiesItem(templateData,mAssetDefinition);
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


