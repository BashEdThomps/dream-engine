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

using namespace Dream;

ScenePropertiesModel::ScenePropertiesModel
(Dream::Scene *scene, QTreeView* parent)
    : PropertiesModel(parent)
{
    qDebug() << "ScenePropertiesModel: Constructor called";
    mScene = scene;
    createRoot();
    createProperties();
}

ScenePropertiesModel::~ScenePropertiesModel
()
{
    qDebug() << "ScenePropertiesModel: Desstructor called";
    delete mRootItem;
}


void
ScenePropertiesModel::createRoot
()
{
    qDebug() << "ScenePropertiesModel: craeteRoot";
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mScene->getName())
             << QString::fromStdString(mScene->getUuid());
    mRootItem = new PropertiesItem(rootData,mScene);
}

void
ScenePropertiesModel::createDelegates
()
{

    qDebug() << "ScenePropertiesModel: createDelegates";
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
     // Name
    QList<QVariant> nameData;
    nameData << "Name"
             << QString::fromStdString(mScene->getName());
    PropertiesItem *nameProperty = new PropertiesItem(nameData,mScene);
    mRootItem->appendChild(nameProperty);
}

void
ScenePropertiesModel::createNotesProperties
()
{
    qDebug() << "ScenePropertiesModel: createNotesProperties";
    // Notes
    QList<QVariant> notesData;
    notesData << "Notes"
             << QString::fromStdString(mScene->getNotes());
    PropertiesItem *notesProperty = new PropertiesItem(notesData,mScene);
    mRootItem->appendChild(notesProperty);
}

void
ScenePropertiesModel::createCameraProperties
()
{
    qDebug() << "ScenePropertiesModel: createCameraProperties";
    // Camera
    QList<QVariant> cameraData;
    cameraData << "Camera" << "";
    PropertiesItem *cameraProperty = new PropertiesItem(cameraData,mScene);
    mRootItem->appendChild(cameraProperty);

    // Camera Translation
    QList<QVariant> cameraTranslationData;
    cameraTranslationData << "Translation" << "";
    PropertiesItem* cameraTranslationProperty = new PropertiesItem(cameraTranslationData, mScene);
    cameraProperty->appendChild(cameraTranslationProperty);

    Transform3D defaultTransform = mScene->getDefaultCameraTransform();
    QList<QVariant> cameraTranslationX_Data;
    cameraTranslationX_Data << "X" << QString::number(static_cast<double>(defaultTransform.getTranslation().x));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationX_Data,mScene));

    QList<QVariant> cameraTranslationY_Data;
    cameraTranslationY_Data << "Y" << QString::number(static_cast<double>(defaultTransform.getTranslation().y));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationY_Data,mScene));

    QList<QVariant> cameraTranslationZ_Data;
    cameraTranslationZ_Data << "Z" << QString::number(static_cast<double>(defaultTransform.getTranslation().z));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationZ_Data,mScene));

    // Camera Rotation
    QList<QVariant> cameraRotationData;
    cameraRotationData << "Rotation" << "";
    PropertiesItem *cameraRotationProperty = new PropertiesItem(cameraRotationData, mScene);
    cameraProperty->appendChild(cameraRotationProperty);

    QList<QVariant> cameraRotationX_Data;
    cameraRotationX_Data << "X" << QString::number(static_cast<double>(defaultTransform.getRotation().x));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationX_Data,mScene));

    QList<QVariant> cameraRotationY_Data;
    cameraRotationY_Data << "Y" << QString::number(static_cast<double>(defaultTransform.getRotation().y));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationY_Data,mScene));

    QList<QVariant> cameraRotationZ_Data;
    cameraRotationZ_Data << "Z" << QString::number(static_cast<double>(defaultTransform.getRotation().z));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationZ_Data,mScene));

    // Camera Speed
    QList<QVariant> cameraSpeedData;
    cameraSpeedData << "Speed" << QString::number(static_cast<double>(mScene->getCameraMovementSpeed()));
    cameraProperty->appendChild(new PropertiesItem(cameraSpeedData, mScene));
}

void
ScenePropertiesModel::createRenderingProperties
()
{
    qDebug() << "ScenePropertiesModel: createRenderingProperties";
    // Rendering
    QList<QVariant> renderingData;
    renderingData << "Rendering" << "";
    PropertiesItem *renderingProperty = new PropertiesItem(renderingData,mScene);
    mRootItem->appendChild(renderingProperty);

    qDebug() << "ScenePropertiesModel: createRenderingProperties (ClearColour)";
    // Clear Color
    QList<QVariant> clearColorData;
    clearColorData << "Clear Color" << "";
    PropertiesItem *clearColorProperty = new PropertiesItem(clearColorData, mScene);
    renderingProperty->appendChild(clearColorProperty);

    vector<float> clear = mScene->getRuntime()->getClearColour();

    QList<QVariant> clearColor_Red_Data;
    clearColor_Red_Data << "Red" << clear[0];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Red_Data,mScene));

    QList<QVariant> clearColor_Green_Data;
    clearColor_Green_Data << "Green" << clear[1];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Green_Data,mScene));

    QList<QVariant> clearColor_Blue_Data;
    clearColor_Blue_Data << "Blue" << clear[2];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Blue_Data,mScene));

    QList<QVariant> clearColor_Alpha_Data;
    clearColor_Alpha_Data << "Alpha" << clear[3];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Alpha_Data,mScene));

    qDebug() << "ScenePropertiesModel: createRenderingProperties (AmbientLight)";
    // Ambient Light Color
    QList<QVariant> ambientLightData;
    ambientLightData << "Ambient Light" << "";
    PropertiesItem *ambientLightProperty = new PropertiesItem(ambientLightData, mScene);
    renderingProperty->appendChild(ambientLightProperty);

    QList<QVariant> ambientLight_Red_Data;
    ambientLight_Red_Data
        << "Red"
        << QString::number
           (
               static_cast<double>
               (
                   mScene->getJson()[Constants::SCENE_CLEAR_COLOUR][Constants::RED]
               )
           );
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Red_Data,mScene));

    QList<QVariant> ambientLight_Green_Data;
    ambientLight_Green_Data
        << "Green"
        << QString::number
           (
               static_cast<double>
               (
                   mScene->getJson()[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]
               )
           );
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Green_Data,mScene));

    QList<QVariant> ambientLight_Blue_Data;
    ambientLight_Blue_Data
        << "Blue"
        << QString::number
           (
               static_cast<double>
               (
                   mScene->getJson()[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]
               )
           );
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Blue_Data,mScene));

    QList<QVariant> ambientLight_Alpha_Data;
    ambientLight_Alpha_Data
        << "Alpha"
        << QString::number
           (
               static_cast<double>
               (
                   mScene->getJson()[Constants::SCENE_CLEAR_COLOUR][Constants::ALPHA]
               )
           );
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Alpha_Data,mScene));
}

void
ScenePropertiesModel::createPhysicsProperties
()
{
    qDebug() << "ScenePropertiesModel: createPhysicsProperties";
    // Physics
    QList<QVariant> physicsData;
    physicsData << "Physics"  << "";
    PropertiesItem *physicsProperty = new PropertiesItem(physicsData,mScene);
    mRootItem->appendChild(physicsProperty);
    {
        QList<QVariant> gravityData;
        gravityData << "Gravity" << "";
        PropertiesItem *gravityProperty = new PropertiesItem(gravityData,mScene);
        physicsProperty->appendChild(gravityProperty);
        {
            QList<QVariant> gravityDataX;
            gravityDataX << "X" << mScene->getRuntime()->getGravity()[Constants::X_INDEX];
            PropertiesItem *gravityPropertyX = new PropertiesItem(gravityDataX,mScene);
            gravityProperty->appendChild(gravityPropertyX);

            QList<QVariant> gravityDataY;
            gravityDataY << "Y" << mScene->getRuntime()->getGravity()[Constants::Y_INDEX];
            PropertiesItem *gravityPropertyY = new PropertiesItem(gravityDataY,mScene);
            gravityProperty->appendChild(gravityPropertyY);

            QList<QVariant> gravityDataZ;
            gravityDataZ << "Z" << mScene->getRuntime()->getGravity()[Constants::Z_INDEX];
            PropertiesItem *gravityPropertyZ = new PropertiesItem(gravityDataZ,mScene);
            gravityProperty->appendChild(gravityPropertyZ);
        }
    }

    QList<QVariant> debugData;
    debugData << "Debug" << mScene->getPhysicsDebug();
    PropertiesItem *debugProperty = new PropertiesItem(debugData,mScene);
    physicsProperty->appendChild(debugProperty);
}

