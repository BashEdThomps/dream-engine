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

#include <spdlog/spdlog.h>

using Dream::Constants;
using Dream::LightDefinition;
using Dream::LightType;

AssetDefinitionPropertiesModel::AssetDefinitionPropertiesModel
(IAssetDefinition* definition, QTreeView* parent)
    : AbstractPropertiesModel(new AssetDefinitionPropertiesTreeDelegate(this, parent), parent),
      mAssetDefinitionHandle(definition)
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("AssetDefinitionPropertiesModel");
    }
    log->info( "Constructing");
    createRoot();
    createProperties();
    createDelegateConnections();
}

AssetDefinitionPropertiesModel::~AssetDefinitionPropertiesModel
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info( "Destructing");
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
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    createNameProperty();
    createTypeProperty();
    createFormatProperty();

    if (mAssetDefinitionHandle->isTypePath())
    {
        createPathSplineFormatProperty();
        createPathStepProperty();
        createPathListProperty();
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
        auto lightDef = dynamic_cast<LightDefinition*>(mAssetDefinitionHandle);

        createLightAmbientProperty();
        createLightDiffuseProperty();
        createLightSpecularProperty();

        log->info("Light Type is {}",lightDef->getFormat());

        switch (lightDef->getType())
        {
            case LightType::LT_SPOTLIGHT:
                createLightDirectionProperty();
                createLightCutOffProperty();
                createLightOuterCutOffProperty();
                createLightConstantProperty();
                createLightLinearProperty();
                createLightQuadraticProperty();
                break;
            case LightType::LT_DIRECTIONAL:
                createLightDirectionProperty();
                break;
            case LightType::LT_POINT:
                createLightConstantProperty();
                createLightLinearProperty();
                createLightQuadraticProperty();
                break;
            case LightType::LT_NONE:
                break;
        }
    }
    else if (mAssetDefinitionHandle->isTypeModel())
    {
        createModelFileProperty();
        createModelAdditionalFilesProperty();
        createModelMaterialShaderProperty();
        createRemoveFilesProperty();
    }
    else if (mAssetDefinitionHandle->isTypePhysicsObject())
    {
        createPhysicsMassProperty();
        createPhysicsMarginProperty();
        createPhysicsKinematicProperty();
        createPhysicsControllableProperty();

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
        else if (mAssetDefinitionHandle->getFormat() == Constants::COLLISION_SHAPE_CAPSULE)
        {
            createPhysicsRadiusProperty();
            createPhysicsHeightProperty();
        }
    }
    else if (mAssetDefinitionHandle->isTypeScript())
    {
        createScriptFileProperty();
        createRemoveFilesProperty();
    }
    else if (mAssetDefinitionHandle->isTypeShader())
    {
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
AssetDefinitionPropertiesModel::createLightAmbientProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Creating Light Ambient Properties");
    auto ambientProperty = new AssetDefinitionPropertiesItem
    (
        "Ambient",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT
    );

    log->info("Creating Light Colour Property red");
    auto colourPropertyRed = new AssetDefinitionPropertiesItem
    (
        "Red",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_RED
    );

    log->info("Creating Light Colour Property green");
    auto colourPropertyGreen = new AssetDefinitionPropertiesItem
    (
        "Green",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_GREEN
    );

    log->info("Creating Light Colour Property blue");
    auto colourPropertyBlue = new AssetDefinitionPropertiesItem
    (
        "Blue",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_BLUE
    );

    ambientProperty->appendChild(colourPropertyRed);
    ambientProperty->appendChild(colourPropertyGreen);
    ambientProperty->appendChild(colourPropertyBlue);

    mRootItem->appendChild(ambientProperty);
}

void
AssetDefinitionPropertiesModel::createLightDiffuseProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Creating Light Colour Properties");
    auto diffuseProperty = new AssetDefinitionPropertiesItem
    (
        "Diffuse",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE
    );

    log->info("Creating Light Colour Property red");
    auto colourPropertyRed = new AssetDefinitionPropertiesItem
    (
        "Red",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_RED
    );

    log->info("Creating Light Colour Property green");
    auto colourPropertyGreen = new AssetDefinitionPropertiesItem
    (
        "Green",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_GREEN
    );

    log->info("Creating Light Colour Property blue");
    auto colourPropertyBlue = new AssetDefinitionPropertiesItem
    (
        "Blue",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_BLUE
    );

    diffuseProperty->appendChild(colourPropertyRed);
    diffuseProperty->appendChild(colourPropertyGreen);
    diffuseProperty->appendChild(colourPropertyBlue);

    mRootItem->appendChild(diffuseProperty);
}
void
AssetDefinitionPropertiesModel::createLightSpecularProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Creating Light Specular Properties");
    auto colourProperty = new AssetDefinitionPropertiesItem
    (
        "Specular",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR
    );

    log->info("Creating Light Colour Property red");
    auto colourPropertyRed = new AssetDefinitionPropertiesItem
    (
        "Red",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_RED
    );

    log->info("Creating Light Colour Property green");
    auto colourPropertyGreen = new AssetDefinitionPropertiesItem
    (
        "Green",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_GREEN
    );

    log->info("Creating Light Colour Property blue");
    auto colourPropertyBlue = new AssetDefinitionPropertiesItem
    (
        "Blue",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_BLUE
    );

    colourProperty->appendChild(colourPropertyRed);
    colourProperty->appendChild(colourPropertyGreen);
    colourProperty->appendChild(colourPropertyBlue);

    mRootItem->appendChild(colourProperty);
}

void
AssetDefinitionPropertiesModel::createLightDirectionProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Creating Light Direction Properties");
    auto directionProperty = new AssetDefinitionPropertiesItem
    (
        "Direction",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION
    );

    log->info("Creating Light Colour Property red");
    auto colourPropertyRed = new AssetDefinitionPropertiesItem
    (
        "X",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_X
    );

    log->info("Creating Light Colour Property green");
    auto colourPropertyGreen = new AssetDefinitionPropertiesItem
    (
        "Y",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Y
    );

    log->info("Creating Light Colour Property blue");
    auto colourPropertyBlue = new AssetDefinitionPropertiesItem
    (
        "Z",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Z
    );

    directionProperty->appendChild(colourPropertyRed);
    directionProperty->appendChild(colourPropertyGreen);
    directionProperty->appendChild(colourPropertyBlue);

    mRootItem->appendChild(directionProperty);
}

void
AssetDefinitionPropertiesModel::createLightConstantProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Constant",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_LIGHT_CONSTANT
        )
    );
}

void
AssetDefinitionPropertiesModel::createLightQuadraticProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Quadratic",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_LIGHT_QUADRATIC
        )
    );
}

void
AssetDefinitionPropertiesModel::createLightLinearProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Linear",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_LIGHT_LINEAR
        )
    );
}

void
AssetDefinitionPropertiesModel::createLightCutOffProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Cut off",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_LIGHT_CUTOFF
        )
    );
}

void
AssetDefinitionPropertiesModel::createLightOuterCutOffProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Outer Cut off",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_LIGHT_OUTER_CUTOFF
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsBvhTriangleMeshFileProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("AssetDefintionPropertiesModel: Creating Physics BvhTriangleMesh File Property");

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
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("AssetDefintionPropertiesModel: Creating Model Assimp File Delegate");

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
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Create Model Additional Files Delegate");
    AssetDefinitionPropertiesItem *property = new AssetDefinitionPropertiesItem
    (
        "Additional Files",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES
    );
    mRootItem->appendChild(property);
}


void
AssetDefinitionPropertiesModel::createModelMaterialShaderProperty
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Create Model Material/Shader Delegate");
    AssetDefinitionPropertiesItem *property = new AssetDefinitionPropertiesItem
    (
        "Shader Map",
        mAssetDefinitionHandle,
        ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE
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
AssetDefinitionPropertiesModel::createPhysicsHeightProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Height",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HEIGHT
        )
    );
}

void
AssetDefinitionPropertiesModel::createPhysicsControllableProperty
()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "User Controllable",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONTROLLABLE
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

void AssetDefinitionPropertiesModel::createPathListProperty()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Node List",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PATH_LIST
        )
                );
}

void AssetDefinitionPropertiesModel::createPathStepProperty()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Step",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PATH_STEP
        )
    );
}

void AssetDefinitionPropertiesModel::createPathSplineFormatProperty()
{
    mRootItem->appendChild
    (
        new AssetDefinitionPropertiesItem
        (
            "Spline Type",
            mAssetDefinitionHandle,
            ASSET_DEFINITION_PROPERTY_PATH_TYPE
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

    // Additional Files

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

    // Material Shader Map
    connect
    (
        delegate,
        SIGNAL(notifyButton_ModelMaterialShaderMap()),
        this,
        SLOT(onButton_ModelMaterialShaderMap())
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

    // Physics Object
    connect
    (
        delegate,
        SIGNAL(notifyButton_PhysicsBvhTriangleMeshFile()),
        this,
        SLOT(onButton_PhysicsBvhTriangleMeshFile())
    );

    // Light
    connect
    (
        delegate,
        SIGNAL(notifyButton_LightChooseAmbient()),
        this,
        SLOT(onButton_LightChooseAmbient())
    );
    connect
    (
        delegate,
        SIGNAL(notifyButton_LightChooseDiffuse()),
        this,
        SLOT(onButton_LightChooseDiffuse())
    );
    connect
    (
        delegate,
        SIGNAL(notifyButton_LightChooseSpecular()),
        this,
        SLOT(onButton_LightChooseSpecular())
    );
    // Path
    connect
    (
        delegate,
        SIGNAL(notifyButton_PathList()),
        this,
        SLOT(onButton_PathList())
    );
}

void
AssetDefinitionPropertiesModel::onButton_RemoveFiles
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("RemoveFiles");
    emit notifyButton_RemoveFiles(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_EditScript
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("EditScript");
    emit notifyButton_EditScript(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onButton_EditShader
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("EditShader");
    emit notifyButton_EditShader(mAssetDefinitionHandle);
}

void
AssetDefinitionPropertiesModel::onCombo_ScriptTemplateChanged
( QString& templateName)
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Script Template Changed");
    emit notifyCombo_ScriptTemplateChanged(mAssetDefinitionHandle,templateName);
}

void
AssetDefinitionPropertiesModel::onCombo_ShaderTemplateChanged
( QString& templateName)
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Shader Template Changed");
    emit notifyCombo_ShaderTemplateChanged(mAssetDefinitionHandle,templateName);
}

void
AssetDefinitionPropertiesModel::onButton_ModelMaterialShaderMap
()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Material Shader Map Clicked");
    emit notifyButton_ModelMaterialShaderMap(mAssetDefinitionHandle);
}

void AssetDefinitionPropertiesModel::onButton_PathList()
{
    auto log = spdlog::get("AssetDefinitionPropertiesModel");
    log->info("Path List Button Clicked");
    emit notifyButton_PathList(mAssetDefinitionHandle);
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

void AssetDefinitionPropertiesModel::onButton_LightChooseAmbient()
{
    emit notifyButton_LightChooseAmbient(mAssetDefinitionHandle);
}

void AssetDefinitionPropertiesModel::onButton_LightChooseDiffuse()
{
    emit notifyButton_LightChooseDiffuse(mAssetDefinitionHandle);
}

void AssetDefinitionPropertiesModel::onButton_LightChooseSpecular()
{
    emit notifyButton_LightChooseSpecular(mAssetDefinitionHandle);
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
