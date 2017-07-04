/*
 * ScenePropertiesItem.cpp
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
#include "ScenePropertiesItem.h"

#include <DreamCore.h>

using Dream::Constants;

ScenePropertiesItem::ScenePropertiesItem
(
    QString title,
    SceneDefinition* sdHandle,
    SceneProperty property,
    QItemDelegate* delegate,
    AbstractPropertiesItem *parent
)
    : AbstractPropertiesItem(title,delegate,parent),
      mSceneDefinitionHandle(sdHandle),
      mProperty(property)
{

}

ScenePropertiesItem::~ScenePropertiesItem
()
{

}

SceneProperty
ScenePropertiesItem::getProperty
()
{
    return mProperty;
}

QVariant
ScenePropertiesItem::data
(int column)
{
    if (column == 0)
    {
        return QVariant(mTitle);
    }
    else if (column == 1)
    {
        switch (getProperty())
        {
            case SCENE_PROPERTY_NAME:
                return QVariant(QString::fromStdString(mSceneDefinitionHandle->getName()));
            case SCENE_PROPERTY_NOTES:
                return QVariant(QString::fromStdString(mSceneDefinitionHandle->getNotes()));
            case SCENE_PROPERTY_CAMERA_TRANSLATION_X:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getTranslationX());
            case SCENE_PROPERTY_CAMERA_TRANSLATION_Y:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getTranslationY());
            case SCENE_PROPERTY_CAMERA_TRANSLATION_Z:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getTranslationZ());
            case SCENE_PROPERTY_CAMERA_ROTATION_X:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getRotationX());
            case SCENE_PROPERTY_CAMERA_ROTATION_Y:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getRotationY());
            case SCENE_PROPERTY_CAMERA_ROTATION_Z:
                return QVariant(mSceneDefinitionHandle->getCameraTransform().getRotationZ());
            case SCENE_PROPERTY_CAMERA_SPEED:
                return QVariant(mSceneDefinitionHandle->getCameraMovementSpeed());
            case SCENE_PROPERTY_CLEAR_RED:
                return QVariant(mSceneDefinitionHandle->getClearColour()[Constants::RED_INDEX]);
            case SCENE_PROPERTY_CLEAR_GREEN:
                return QVariant(mSceneDefinitionHandle->getClearColour()[Constants::GREEN_INDEX]);
            case SCENE_PROPERTY_CLEAR_BLUE:
                return QVariant(mSceneDefinitionHandle->getClearColour()[Constants::BLUE_INDEX]);
            case SCENE_PROPERTY_AMBIENT_RED:
                return QVariant(mSceneDefinitionHandle->getAmbientColour()[Constants::RED_INDEX]);
            case SCENE_PROPERTY_AMBIENT_GREEN:
                return QVariant(mSceneDefinitionHandle->getAmbientColour()[Constants::GREEN_INDEX]);
            case SCENE_PROPERTY_AMBIENT_BLUE:
                return QVariant(mSceneDefinitionHandle->getAmbientColour()[Constants::BLUE_INDEX]);
            case SCENE_PROPERTY_AMBIENT_ALPHA:
                return QVariant(mSceneDefinitionHandle->getAmbientColour()[Constants::ALPHA_INDEX]);
            case SCENE_PROPERTY_PHYSICS_GRAVITY_X:
                return QVariant(mSceneDefinitionHandle->getGravity()[Constants::X_INDEX]);
            case SCENE_PROPERTY_PHYSICS_GRAVITY_Y:
                return QVariant(mSceneDefinitionHandle->getGravity()[Constants::Y_INDEX]);
            case SCENE_PROPERTY_PHYSICS_GRAVITY_Z:
                return QVariant(mSceneDefinitionHandle->getGravity()[Constants::Z_INDEX]);
            case SCENE_PROPERTY_PHYSICS_DEBUG:
                return QVariant(mSceneDefinitionHandle->getPhysicsDebug());
            case SCENE_PROPERTY_NONE:
                return QVariant();
        }
    }
    return QVariant();
}

bool
ScenePropertiesItem::setData
(int column, const QVariant &value)
{
    if (column == 0)
    {
        return false;
    }
    else if (column == 1)
    {
        switch(getProperty())
        {
            case SCENE_PROPERTY_NAME:
                mSceneDefinitionHandle->setName(value.toString().toStdString());
                break;
            case SCENE_PROPERTY_NOTES:
                mSceneDefinitionHandle->setNotes(value.toString().toStdString());
                break;
            case SCENE_PROPERTY_CAMERA_TRANSLATION_X:
                mSceneDefinitionHandle->getCameraTransform().setTranslationX(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_TRANSLATION_Y:
                mSceneDefinitionHandle->getCameraTransform().setTranslationY(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_TRANSLATION_Z:
                mSceneDefinitionHandle->getCameraTransform().setTranslationZ(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_ROTATION_X:
                mSceneDefinitionHandle->getCameraTransform().setRotationX(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_ROTATION_Y:
                mSceneDefinitionHandle->getCameraTransform().setRotationY(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_ROTATION_Z:
                mSceneDefinitionHandle->getCameraTransform().setRotationZ(value.toFloat());
                break;
            case SCENE_PROPERTY_CAMERA_SPEED:
                mSceneDefinitionHandle->setCameraMovementSpeed(value.toFloat());
                break;
            case SCENE_PROPERTY_CLEAR_RED:
                mSceneDefinitionHandle->setClearColourR(value.toFloat());
                break;
            case SCENE_PROPERTY_CLEAR_GREEN:
                mSceneDefinitionHandle->setClearColourG(value.toFloat());
                break;
            case SCENE_PROPERTY_CLEAR_BLUE:
                mSceneDefinitionHandle->setClearColourB(value.toFloat());
                break;
            case SCENE_PROPERTY_AMBIENT_RED:
                mSceneDefinitionHandle->setAmbientColourR(value.toFloat());
                break;
            case SCENE_PROPERTY_AMBIENT_GREEN:
                mSceneDefinitionHandle->setAmbientColourG(value.toFloat());
                break;
            case SCENE_PROPERTY_AMBIENT_BLUE:
                mSceneDefinitionHandle->setAmbientColourB(value.toFloat());
                break;
            case SCENE_PROPERTY_AMBIENT_ALPHA:
                mSceneDefinitionHandle->setAmbientColourA(value.toFloat());
                break;
            case SCENE_PROPERTY_PHYSICS_GRAVITY_X:
                mSceneDefinitionHandle->setGravityX(value.toFloat());
                break;
            case SCENE_PROPERTY_PHYSICS_GRAVITY_Y:
                mSceneDefinitionHandle->setGravityY(value.toFloat());
                break;
            case SCENE_PROPERTY_PHYSICS_GRAVITY_Z:
                mSceneDefinitionHandle->setGravityZ(value.toFloat());
                break;
            case SCENE_PROPERTY_PHYSICS_DEBUG:
                mSceneDefinitionHandle->setPhysicsDebug(value.toBool());
                break;
            case SCENE_PROPERTY_NONE:
                break;
        }

        return true;
    }

    return false;
}
