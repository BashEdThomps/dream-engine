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

using Dream::PathDefinition;
using Dream::AudioDefinition;
using Dream::FontDefinition;
using Dream::LightDefinition;
using Dream::ModelDefinition;
using Dream::PhysicsObjectDefinition;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;
using Dream::SpriteDefinition;

AssetDefinitionPropertiesItem::AssetDefinitionPropertiesItem
(
    QString title,
    shared_ptr<IAssetDefinition> mAssetDefinitionHandle,
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

shared_ptr<IAssetDefinition>
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


        case ASSET_DEFINITION_PROPERTY_PATH_TYPE:
            dynamic_pointer_cast<PathDefinition>(mAssetDefinitionHandle)->setSplineType(value.toString().toStdString());
            break;

        case ASSET_DEFINITION_PROPERTY_PATH_STEP:
            dynamic_pointer_cast<PathDefinition>(mAssetDefinitionHandle)->setStepScalar(value.toDouble());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setConstant(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HEIGHT:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setHeight(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setRadius(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setNormalX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setNormalY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setNormalZ(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setHalfExtentsX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setHalfExtentsY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setHalfExtentsZ(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setMass(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setMargin(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setKinematic(value.toBool());
            break;
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONTROLLABLE:
            dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->setControllableCharacter(value.toBool());
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
            dynamic_pointer_cast<AudioDefinition>(mAssetDefinitionHandle)->setLoop(value.toBool());
            break;

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->setSize(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->setColourRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->setColourGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->setColourBlue(value.toFloat());
            break;

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_RED:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setAmbientRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_GREEN:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setAmbientGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_BLUE:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setAmbientBlue(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_RED:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDiffuseRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_GREEN:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDiffuseGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_BLUE:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDiffuseBlue(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_RED:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setSpecularRed(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_GREEN:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setSpecularGreen(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_BLUE:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setSpecularBlue(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_X:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDirectionX(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Y:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDirectionY(value.toFloat());
            break;
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Z:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setDirectionZ(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_CONSTANT:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setConstant(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_LINEAR:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setLinear(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_QUADRATIC:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setQuadratic(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_CUTOFF:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setCutOff(value.toFloat());
            break;

        case ASSET_DEFINITION_PROPERTY_LIGHT_OUTER_CUTOFF:
            dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->setOuterCutOff(value.toFloat());
            break;

            // Not used
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR:
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT:
        case ASSET_DEFINITION_PROPERTY_PATH_LIST:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FILES:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
        case ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE:
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
        case ASSET_DEFINITION_PROPERTY_PATH_TYPE:
            return QVariant(QString::fromStdString(dynamic_pointer_cast<PathDefinition>(mAssetDefinitionHandle)->getSplineType()));

        case ASSET_DEFINITION_PROPERTY_PATH_STEP:
            return QVariant(dynamic_pointer_cast<PathDefinition>(mAssetDefinitionHandle)->getStepScalar());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getConstant());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HEIGHT:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getHeight());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getRadius());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getNormalX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getNormalY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getNormalZ());

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
            return QVariant();

            // Physics Object
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getHalfExtentsX());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getHalfExtentsY());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getHalfExtentsZ());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getMass());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getMargin());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getKinematic());
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONTROLLABLE:
            return QVariant(dynamic_pointer_cast<PhysicsObjectDefinition>(mAssetDefinitionHandle)->getControllableCharacter());

            // Audio
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            return QVariant(dynamic_pointer_cast<AudioDefinition>(mAssetDefinitionHandle)->getLoop());

            // Font
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
            return QVariant(dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->getSize());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
            return QVariant(dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->getColourRed());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
            return QVariant(dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->getColourGreen());
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
            return QVariant(dynamic_pointer_cast<FontDefinition>(mAssetDefinitionHandle)->getColourBlue());

            // Light
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_RED:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getAmbientRed());
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_GREEN:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getAmbientGreen());
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT_BLUE:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getAmbientBlue());

        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_RED:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDiffuseRed());
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_GREEN:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDiffuseGreen());
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE_BLUE:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDiffuseBlue());

        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_RED:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getSpecularRed());
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_GREEN:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getSpecularGreen());
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR_BLUE:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getSpecularBlue());

        case ASSET_DEFINITION_PROPERTY_LIGHT_CONSTANT:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getConstant());

        case ASSET_DEFINITION_PROPERTY_LIGHT_LINEAR:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getLinear());

        case ASSET_DEFINITION_PROPERTY_LIGHT_QUADRATIC:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getQuadratic());

        case ASSET_DEFINITION_PROPERTY_LIGHT_CUTOFF:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getCutOff());

        case ASSET_DEFINITION_PROPERTY_LIGHT_OUTER_CUTOFF:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getOuterCutOff());

        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_X:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDirectionX());
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Y:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDirectionY());
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION_Z:
            return QVariant(dynamic_pointer_cast<LightDefinition>(mAssetDefinitionHandle)->getDirectionZ());

            // Common
        case ASSET_DEFINITION_PROPERTY_NAME:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getName()));
        case ASSET_DEFINITION_PROPERTY_TYPE:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getType()));
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            return QVariant(QString::fromStdString(mAssetDefinitionHandle->getFormat()));

            // Not Used
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIFFUSE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_SPECULAR:
        case ASSET_DEFINITION_PROPERTY_LIGHT_DIRECTION:


        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_LIGHT_AMBIENT:
        case ASSET_DEFINITION_PROPERTY_PATH_LIST:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FILES:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
        case ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE:
            return QVariant();
    }
    return QVariant();
}
