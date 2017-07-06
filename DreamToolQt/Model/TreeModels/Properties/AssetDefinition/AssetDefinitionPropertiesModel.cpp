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
#include "../../../TemplatesModel.h"

#include <DreamCore.h>
#include <QDebug>

using Dream::Constants;

AssetDefinitionPropertiesModel::AssetDefinitionPropertiesModel
(AssetDefinition *definition, TemplatesModel* templatesModel, QTreeView* parent)
    : AbstractPropertiesModel(new AssetDefinitionPropertiesTreeDelegate(templatesModel, this, parent), parent),
      mAssetDefinitionHandle(definition)
{
    qDebug() <<  "AssetDefinitionPropertiesModel: Constructing";
    createRoot();
    createProperties();
    createDelegateConnections();
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
        createRemoveFilesProperty();
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
        createRemoveFilesProperty();
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
        createTemplateProperty();
        createScriptFileProperty();
        createRemoveFilesProperty();
    }
    else if (mAssetDefinitionHandle->isTypeShader())
    {
        createTemplateProperty();
        createShaderEditProperties();
        createRemoveFilesProperty();
    }
    else if (mAssetDefinitionHandle->isTypeSprite())
    {
        createSpriteTileSizeProperty();
        createSpriteFileProperty();
        createRemoveFilesProperty();
    }
}


void
AssetDefinitionPropertiesModel::createNameProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Name",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_NAME
        )
    );
}

void
AssetDefinitionPropertiesModel::createShaderEditProperties
()
{
    createShaderEditVertexProperty();
    createShaderEditFragmentProperty();
}

void AssetDefinitionPropertiesModel::createShaderEditVertexProperty()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Vertex",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE
        )
    );
}

void AssetDefinitionPropertiesModel::createShaderEditFragmentProperty()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Fragment",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE
        )
    );
}

void
AssetDefinitionPropertiesModel::onButton_ModelFile
()
{
    emit notifyButton_ModelFile(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_ModelAdditionalFiles
()
{
    emit notifyButton_ModelAdditionalFiles(mAssetDefinitionHandle);
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

AssetDefinition*
AssetDefinitionPropertiesModel::getAssetDefinitionHandle
()
{
   return mAssetDefinitionHandle;
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
AssetDefinitionPropertiesModel::createTemplateProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Template Delegate";
    // Template
    AssetDefinitionPropertiesItem *templateProperty = new AssetDefinitionPropertiesItem
    (
        "Template",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_TEMPLATE
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

void
AssetDefinitionPropertiesModel::createScriptFileProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Script File",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_SCRIPT_FILE
        )
    );
}

void
AssetDefinitionPropertiesModel::createRemoveFilesProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Remove Files",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_REMOVE_FILES
        )
    );
}

void
AssetDefinitionPropertiesModel::createDelegateConnections
()
{
    AssetDefinitionPropertiesTreeDelegate* delegate;
    delegate = static_cast<AssetDefinitionPropertiesTreeDelegate*>(mDelegateHandle);

    // Model

    connect
    (
        delegate,
        SIGNAL(notifyButton_ModelFile()),
        this,
        SLOT(onButton_ModelFile())
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_ModelAdditionalFiles()),
        this,
        SLOT(onButton_ModelAdditionalFiles())
    );

    // Remove Files
    connect
    (
        delegate,
        SIGNAL(notifyButton_RemoveFiles()),
        this,
        SLOT(onButton_RemoveFiles())
    );

    // Edit Script
    connect
    (
        delegate,
        SIGNAL(notifyButton_EditScript()),
        this,
        SLOT(onButton_EditScript())
    );


    // Edit Shader
    connect
    (
        delegate,
        SIGNAL(notifyButton_EditVertexShader()),
        this,
        SLOT(onButton_EditVertexShader())
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_EditFragmentShader()),
        this,
        SLOT(onButton_EditFragmentShader())
    );

    // Script Template
    connect
    (
        delegate,
        SIGNAL(notifyCombo_ScriptTemplateChanged(const QString&)),
        this,
        SLOT(onCombo_ScriptTemplateChanged(const QString&))
    );

    // Shader Template
    connect
    (
        delegate,
        SIGNAL(notifyCombo_ShaderTemplateChanged(const QString&)),
        this,
        SLOT(onCombo_ShaderTemplateChanged(const QString&))
    );
}

void
AssetDefinitionPropertiesModel::onButton_RemoveFiles
()
{
    qDebug() << "AssetDefinitionPropertiesModel: RemoveFiles";
    emit notifyButton_RemoveFiles(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_EditScript
()
{
    qDebug() << "AssetDefinitionPropertiesModel: EditScript";
    emit notifyButton_EditScript(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_EditFragmentShader
()
{
    qDebug() << "AssetDefinitionPropertiesModel: EditFragmentShader";
    emit notifyButton_EditFragmentShader(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_EditVertexShader
()
{
    qDebug() << "AssetDefinitionPropertiesModel: EditVertexShader";
    emit notifyButton_EditVertexShader(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onCombo_ScriptTemplateChanged
(const QString& templateName)
{
    qDebug() << "AssetDefinitionPropertiesModel: Script Template Changed";
    emit notifyCombo_ScriptTemplateChanged(mAssetDefinitionHandle,templateName);
}

void
AssetDefinitionPropertiesModel::onCombo_ShaderTemplateChanged
(const QString& templateName)
{
    qDebug() << "AssetDefinitionPropertiesModel: Shader Template Changed";
    emit notifyCombo_ShaderTemplateChanged(mAssetDefinitionHandle,templateName);
}
