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
(IAssetDefinition *definition, TemplatesModel* templatesModel, QTreeView* parent)
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
        createRemoveFilesProperty();
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
        createPhysicsMassProperty();
        createPhysicsMarginProperty();
        createPhysicsKinematicProperty();

        if (mAssetDefinitionHandle->getFormat() == Constants::COLLISION_SHAPE_BOX)
        {
            createPhysicsHalfExtentsProperty();
        }
        else if (mAssetDefinitionHandle->getFormat() == Constants::COLLISION_SHAPE_SPHERE)
        {
           createPhysicsRadiusProperty();
        }
        else if (mAssetDefinitionHandle->getFormat() == Constants::COLLISION_SHAPE_STATIC_PLANE)
        {
           createPhysicsNormalProperty();
           createPhysicsConstantProperty();
        }
        else if (mAssetDefinitionHandle->getFormat() == Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH)
        {
           createPhysicsBvhTriangleMeshFileProperty();
        }
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
    createShaderEditFilesProperty();
}

void
AssetDefinitionPropertiesModel::createShaderEditFilesProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Shader Files",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_SHADER_FILES
        )
    );
}

void
AssetDefinitionPropertiesModel::onButton_AudioFile
()
{
    emit notifyButton_AudioFile(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_FontFile
()
{
    emit notifyButton_FontFile(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_PhysicsBvhTriangleMeshFile
()
{
    emit notifyButton_PhysicsBvhTriangleMeshFile(mAssetDefinitionHandle);
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

IAssetDefinition*
AssetDefinitionPropertiesModel::getAssetDefinitionHandle
()
{
   return mAssetDefinitionHandle;
}

void
AssetDefinitionPropertiesModel::createAudioLoopProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Loop",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_AUDIO_LOOP
        )
    );
}

void
AssetDefinitionPropertiesModel::createAudioFileProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Audio File",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_AUDIO_FILE
        )
    );
}

void
AssetDefinitionPropertiesModel::createFontColorProperty
()
{
    auto colour = new AssetDefinitionPropertiesItem
    (
        "Colour",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FONT_COLOUR
    );

    auto red = new AssetDefinitionPropertiesItem
    (
        "Red",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED
    );

    auto green = new AssetDefinitionPropertiesItem
    (
        "Green",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN
    );

    auto blue = new AssetDefinitionPropertiesItem
    (
        "Blue",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE
    );

    colour->appendChild(red);
    colour->appendChild(green);
    colour->appendChild(blue);
    mRootItem->appendChild(colour);
}

void
AssetDefinitionPropertiesModel::createFontSizeProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Size",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_FONT_SIZE
        )
    );
}

void
AssetDefinitionPropertiesModel::createFontFileProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Font File",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_FONT_FILE
        )
    );
}

void
AssetDefinitionPropertiesModel::createLightColorProperty
()
{
    qDebug() << "AssetDefinitionPropertiesModel: Creating Light Colour Properties";
    auto colourProperty = new AssetDefinitionPropertiesItem
    (
        "Colour",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR
    );

    qDebug() << "AssetDefinitionPropertiesModel: Creating Light Colour Property red";
    auto colourPropertyRed = new AssetDefinitionPropertiesItem
    (
        "Red",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED
    );

    qDebug() << "AssetDefinitionPropertiesModel: Creating Light Colour Property green";
    auto colourPropertyGreen = new AssetDefinitionPropertiesItem
    (
        "Green",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN
    );

    qDebug() << "AssetDefinitionPropertiesModel: Creating Light Colour Property blue";
    auto colourPropertyBlue = new AssetDefinitionPropertiesItem
    (
        "Blue",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE
    );

    qDebug() << "AssetDefinitionPropertiesModel: Creating Light Colour Property alpha";
    auto colourPropertyAlpha = new AssetDefinitionPropertiesItem
    (
        "Alpha",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA
    );

    colourProperty->appendChild(colourPropertyRed);
    colourProperty->appendChild(colourPropertyGreen);
    colourProperty->appendChild(colourPropertyBlue);
    colourProperty->appendChild(colourPropertyAlpha);

    mRootItem->appendChild(colourProperty);
}

void
AssetDefinitionPropertiesModel::createPhysicsBvhTriangleMeshFileProperty
()
{
    qDebug() << "AssetDefintionPropertiesModel: Creating Physics BvhTriangleMesh File Property";

    AssetDefinitionPropertiesItem *mfProperty = new AssetDefinitionPropertiesItem
    (
        "Mesh File" ,
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE
    );
    mRootItem->appendChild(mfProperty);
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
AssetDefinitionPropertiesModel::createPhysicsHalfExtentsProperty
()
{
    auto halfExtents = new AssetDefinitionPropertiesItem
    (
        "Half Extents",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS
    );

    halfExtents->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "X",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X
        )
    );

    halfExtents->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Y",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y
        )
    );

    halfExtents->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Z",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z
        )
    );

    mRootItem->appendChild(halfExtents);
}

void
AssetDefinitionPropertiesModel::createPhysicsRadiusProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Radius",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsNormalProperty
()
{
    auto normalProperty = new AssetDefinitionPropertiesItem
    (
        "Normal",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL
    );

    normalProperty->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "X",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X
        )
    );

    normalProperty->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Y",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y
        )
    );

    normalProperty->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Z",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z
        )
    );
    mRootItem->appendChild(normalProperty);
}

void
AssetDefinitionPropertiesModel::createPhysicsConstantProperty
()
{
    mRootItem->appendChild
    (
         new AssetDefinitionPropertiesItem
        (
            "Constant",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsMassProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Mass",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsMarginProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Margin",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsKinematicProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Kinematic",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC
        )
    );
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

    // Audio

    connect
    (
        delegate,
        SIGNAL(notifyButton_AudioFile()),
        this,
        SLOT(onButton_AudioFile())
    );

    // Font

    connect
    (
        delegate,
        SIGNAL(notifyButton_FontFile()),
        this,
        SLOT(onButton_FontFile())
    );

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
        SIGNAL(notifyButton_EditShader()),
        this,
        SLOT(onButton_EditShader())
    );

    // Script Template
    connect
    (
        delegate,
        SIGNAL(notifyCombo_ScriptTemplateChanged(QString)),
        this,
        SLOT(onCombo_ScriptTemplateChanged(QString))
    );

    // Shader Template
    connect
    (
        delegate,
        SIGNAL(notifyCombo_ShaderTemplateChanged(QString)),
        this,
        SLOT(onCombo_ShaderTemplateChanged(QString))
    );

    // Physics Object
    connect
    (
        delegate,
        SIGNAL(notifyButton_PhysicsBvhTriangleMeshFile()),
        this,
        SLOT(onButton_PhysicsBvhTriangleMeshFile())
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
AssetDefinitionPropertiesModel::onButton_EditShader
()
{
    qDebug() << "AssetDefinitionPropertiesModel: EditShader";
    emit notifyButton_EditShader(mAssetDefinitionHandle);
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
