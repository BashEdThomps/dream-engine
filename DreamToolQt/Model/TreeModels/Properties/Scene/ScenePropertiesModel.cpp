/*
 * ScenePropertiesListModel.cpp
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

#include "ScenePropertiesModel.h"

#include <QDebug>
#include <DreamCore.h>

#include "ScenePropertiesItem.h"
#include "../DoubleSpinBoxDelegate.h"
#include "../CheckBoxDelegate.h"

using Dream::Transform3D;
using Dream::Constants;

ScenePropertiesModel::ScenePropertiesModel
(SceneDefinition *scene, QTreeView* parent)
    : PropertiesModel(parent),
      mSceneDefinition(scene)
{
    qDebug() << "ScenePropertiesModel: Constructor called";
    createRoot();
    createProperties();
}

ScenePropertiesModel::~ScenePropertiesModel
()
{
    qDebug() << "ScenePropertiesModel: Desstructor called";
}

void
ScenePropertiesModel::createRoot
()
{
    qDebug() << "ScenePropertiesModel: craeteRoot";
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mSceneDefinition->getName())
             << QString::fromStdString(mSceneDefinition->getUuid());
    mRootItem.reset(new ScenePropertiesItem(rootData,mSceneDefinition));
}

void
ScenePropertiesModel::createProperties
()
{
    qDebug() << "ScenePropertiesModel: createProperties";
    createNameProperties();
    createNotesProperties();
    createCameraProperties();
    createRenderingProperties();
    createPhysicsProperties();
}

void
ScenePropertiesModel::createNameProperties
()
{
    qDebug() << "ScenePropertiesModel: createNameProperties";
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mSceneDefinition->getName());
    ScenePropertiesItem *nameProperty = new ScenePropertiesItem(nameData,mSceneDefinition,SCENE_PROPERTY_NAME);
    mRootItem->appendChild(nameProperty);
}

void
ScenePropertiesModel::createNotesProperties
()
{
    qDebug() << "ScenePropertiesModel: createNotesProperties";
    QList<QVariant> notesData;
    notesData << "Notes" << QString::fromStdString(mSceneDefinition->getNotes());
    ScenePropertiesItem *notesProperty = new ScenePropertiesItem(notesData,mSceneDefinition,SCENE_PROPERTY_NOTES);
    mRootItem->appendChild(notesProperty);
}

void
ScenePropertiesModel::createCameraProperties
()
{
    qDebug() << "ScenePropertiesModel: createCameraProperties";
    QList<QVariant> cameraData;
    cameraData << "Camera" << "";
    ScenePropertiesItem *cameraProperty = new ScenePropertiesItem(cameraData,mSceneDefinition);
    mRootItem->appendChild(cameraProperty);
    {
        Transform3D defaultTransform = mSceneDefinition->getCameraTransform();

        qDebug() << "ScenePropertiesModel: createCameraProperties translation";
        QList<QVariant> cameraTranslationData;
        cameraTranslationData << "Translation" << "";

        ScenePropertiesItem* cameraTranslationProperty = new ScenePropertiesItem
        (
            cameraTranslationData, mSceneDefinition
        );

        cameraProperty->appendChild(cameraTranslationProperty);
        {
            QList<QVariant> cameraTranslationX_Data;
            cameraTranslationX_Data << "X" << defaultTransform.getTranslation().x;
            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraTranslationX_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_X,
                    new DoubleSpinBoxDelegate()
                )
            );

            QList<QVariant> cameraTranslationY_Data;
            cameraTranslationY_Data << "Y" << defaultTransform.getTranslation().y;
            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraTranslationY_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Y,
                    new DoubleSpinBoxDelegate()
                )
            );

            QList<QVariant> cameraTranslationZ_Data;
            cameraTranslationZ_Data << "Z" << defaultTransform.getTranslation().z;
            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraTranslationZ_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Z,
                    new DoubleSpinBoxDelegate()
                )
            );
        }

        qDebug() << "ScenePropertiesModel: createCameraProperties rotation";
        QList<QVariant> cameraRotationData;
        cameraRotationData << "Rotation" << "";
        ScenePropertiesItem *cameraRotationProperty = new ScenePropertiesItem
        (
            cameraRotationData,
            mSceneDefinition
        );

        cameraProperty->appendChild(cameraRotationProperty);
        {
            QList<QVariant> cameraRotationX_Data;
            cameraRotationX_Data << "X" << defaultTransform.getRotation().x;
            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraRotationX_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_X,
                    new DoubleSpinBoxDelegate()
                )
            );

            QList<QVariant> cameraRotationY_Data;
            cameraRotationY_Data << "Y" << defaultTransform.getRotation().y;
            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraRotationY_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_Y,
                    new DoubleSpinBoxDelegate()
                )
            );

            QList<QVariant> cameraRotationZ_Data;
            cameraRotationZ_Data << "Z" << defaultTransform.getRotation().z;
            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraRotationZ_Data,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_Z,
                    new DoubleSpinBoxDelegate()
                )
            );

            qDebug() << "ScenePropertiesModel: createCameraProperties speed";
            QList<QVariant> cameraSpeedData;
            cameraSpeedData << "Speed" << mSceneDefinition->getCameraMovementSpeed();
            cameraProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    cameraSpeedData,
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_SPEED,
                    new DoubleSpinBoxDelegate()
                )
            );
        }
    }
}

void
ScenePropertiesModel::createRenderingProperties
()
{
    qDebug() << "ScenePropertiesModel: createRenderingProperties";
    vector<float> clear = mSceneDefinition->getClearColour();

    QList<QVariant> renderingData;
    renderingData << "Rendering" << "";
    ScenePropertiesItem *renderingProperty = new ScenePropertiesItem(renderingData,mSceneDefinition);
    mRootItem->appendChild(renderingProperty);
    {

        qDebug() << "ScenePropertiesModel: createRenderingProperties (ClearColour)";
        // Clear Color
        QList<QVariant> clearColorData;
        clearColorData << "Clear Color" << "";
        ScenePropertiesItem *clearColorProperty = new ScenePropertiesItem(clearColorData, mSceneDefinition);
        renderingProperty->appendChild(clearColorProperty);

        QList<QVariant> clearColor_Red_Data;
        clearColor_Red_Data << "Red" << clear[Constants::RED_INDEX];
        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                clearColor_Red_Data,
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_RED,
                new DoubleSpinBoxDelegate()
            )
        );

        QList<QVariant> clearColor_Green_Data;
        clearColor_Green_Data << "Green" << clear[Constants::GREEN_INDEX];
        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                clearColor_Green_Data,
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_GREEN,
                new DoubleSpinBoxDelegate()
            )
        );

        QList<QVariant> clearColor_Blue_Data;
        clearColor_Blue_Data << "Blue" << clear[Constants::BLUE_INDEX];
        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                clearColor_Blue_Data,
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_BLUE,
                new DoubleSpinBoxDelegate()
            )
        );
    }

    qDebug() << "ScenePropertiesModel: createRenderingProperties (AmbientLight)";
    vector<float> ambient = mSceneDefinition->getAmbientColour();

    QList<QVariant> ambientLightData;
    ambientLightData << "Ambient Light" << "";
    ScenePropertiesItem *ambientLightProperty = new ScenePropertiesItem(ambientLightData, mSceneDefinition);
    renderingProperty->appendChild(ambientLightProperty);
    {
        QList<QVariant> ambientLight_Red_Data;
        ambientLight_Red_Data << "Red" << ambient[Constants::RED_INDEX];
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                ambientLight_Red_Data,
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_RED,
                new DoubleSpinBoxDelegate()
            )
        );

        QList<QVariant> ambientLight_Green_Data;
        ambientLight_Green_Data << "Green" << ambient[Constants::GREEN_INDEX];
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                ambientLight_Green_Data,
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_GREEN,
                new DoubleSpinBoxDelegate()
            )
        );

        QList<QVariant> ambientLight_Blue_Data;
        ambientLight_Blue_Data << "Blue" << ambient[Constants::BLUE_INDEX];
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                ambientLight_Blue_Data,
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_BLUE,
                new DoubleSpinBoxDelegate()
            )
        );

        QList<QVariant> ambientLight_Alpha_Data;
        ambientLight_Alpha_Data << "Alpha" << ambient[Constants::ALPHA_INDEX];
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                ambientLight_Alpha_Data,
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_ALPHA,
                new DoubleSpinBoxDelegate()
            )
        );
    }
}

void
ScenePropertiesModel::createPhysicsProperties
()
{
    qDebug() << "ScenePropertiesModel: createPhysicsProperties";
    QList<QVariant> physicsData;
    physicsData << "Physics"  << "";
    ScenePropertiesItem *physicsProperty = new ScenePropertiesItem(physicsData,mSceneDefinition);
    mRootItem->appendChild(physicsProperty);
    {
        QList<QVariant> gravityData;
        gravityData << "Gravity" << "";
        ScenePropertiesItem *gravityProperty = new ScenePropertiesItem(gravityData,mSceneDefinition);
        physicsProperty->appendChild(gravityProperty);
        {
            vector<float> gravity = mSceneDefinition->getGravity();
            QList<QVariant> gravityDataX;
            gravityDataX << "X" << gravity[Constants::X_INDEX];
            ScenePropertiesItem *gravityPropertyX = new ScenePropertiesItem
            (
                gravityDataX,
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_X,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyX);

            QList<QVariant> gravityDataY;
            gravityDataY << "Y" << gravity[Constants::Y_INDEX];
            ScenePropertiesItem *gravityPropertyY = new ScenePropertiesItem
            (
                gravityDataY,
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Y,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyY);

            QList<QVariant> gravityDataZ;
            gravityDataZ << "Z" << gravity[Constants::Z_INDEX];
            ScenePropertiesItem *gravityPropertyZ = new ScenePropertiesItem
            (
                gravityDataZ,
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Z,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyZ);
        }
    }

    QList<QVariant> debugData;
    debugData << "Debug" << mSceneDefinition->getPhysicsDebug();
    ScenePropertiesItem *debugProperty = new ScenePropertiesItem
    (
        debugData,
        mSceneDefinition,
        SCENE_PROPERTY_PHYSICS_DEBUG,
        new CheckBoxDelegate()
    );
    physicsProperty->appendChild(debugProperty);
}

bool
ScenePropertiesModel::setData
(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

