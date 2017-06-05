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
    delete mRootItem;
}


void
ScenePropertiesModel::createRoot
()
{
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mScene->getName())
             << QString::fromStdString(mScene->getUuid());
    mRootItem = new PropertiesItem(rootData,mScene);
}

void
ScenePropertiesModel::createDelegates
()
{

}

void
ScenePropertiesModel::createProperties
()
{
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

    QList<QVariant> cameraTranslationX_Data;
    cameraTranslationX_Data << "X" << QString::number(static_cast<double>(mScene->getDefaultCameraTranslation().x));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationX_Data,mScene));

    QList<QVariant> cameraTranslationY_Data;
    cameraTranslationY_Data << "Y" << QString::number(static_cast<double>(mScene->getDefaultCameraTranslation().y));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationY_Data,mScene));

    QList<QVariant> cameraTranslationZ_Data;
    cameraTranslationZ_Data << "Z" << QString::number(static_cast<double>(mScene->getDefaultCameraTranslation().z));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationZ_Data,mScene));

    // Camera Rotation
    QList<QVariant> cameraRotationData;
    cameraRotationData << "Rotation" << "";
    PropertiesItem *cameraRotationProperty = new PropertiesItem(cameraRotationData, mScene);
    cameraProperty->appendChild(cameraRotationProperty);

    QList<QVariant> cameraRotationX_Data;
    cameraRotationX_Data << "X" << QString::number(static_cast<double>(mScene->getDefaultCameraRotation().x));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationX_Data,mScene));

    QList<QVariant> cameraRotationY_Data;
    cameraRotationY_Data << "Y" << QString::number(static_cast<double>(mScene->getDefaultCameraRotation().y));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationY_Data,mScene));

    QList<QVariant> cameraRotationZ_Data;
    cameraRotationZ_Data << "Z" << QString::number(static_cast<double>(mScene->getDefaultCameraRotation().z));
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
    // Rendering
    QList<QVariant> renderingData;
    renderingData << "Rendering" << "";
    PropertiesItem *renderingProperty = new PropertiesItem(renderingData,mScene);
    mRootItem->appendChild(renderingProperty);

    // Clear Color
    QList<QVariant> clearColorData;
    clearColorData << "Clear Color" << "";
    PropertiesItem *clearColorProperty = new PropertiesItem(clearColorData, mScene);
    renderingProperty->appendChild(clearColorProperty);

    QList<QVariant> clearColor_Red_Data;
    clearColor_Red_Data << "Red" << mScene->getClearColour().at(0);
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Red_Data,mScene));

    QList<QVariant> clearColor_Green_Data;
    clearColor_Green_Data << "Green" << mScene->getClearColour().at(1);
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Green_Data,mScene));

    QList<QVariant> clearColor_Blue_Data;
    clearColor_Blue_Data << "Blue" << mScene->getClearColour().at(2);
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Blue_Data,mScene));

    QList<QVariant> clearColor_Alpha_Data;
    clearColor_Alpha_Data << "Alpha" << mScene->getClearColour().at(3);
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Alpha_Data,mScene));

    // Ambient Light Color
    QList<QVariant> ambientLightData;
    ambientLightData << "Ambient Light" << "";
    PropertiesItem *ambientLightProperty = new PropertiesItem(ambientLightData, mScene);
    renderingProperty->appendChild(ambientLightProperty);

    QList<QVariant> ambientLight_Red_Data;
    ambientLight_Red_Data << "Red" << mScene->getClearColour().at(0);
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Red_Data,mScene));

    QList<QVariant> ambientLight_Green_Data;
    ambientLight_Green_Data << "Green" << mScene->getClearColour().at(1);
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Green_Data,mScene));

    QList<QVariant> ambientLight_Blue_Data;
    ambientLight_Blue_Data << "Blue" << mScene->getClearColour().at(2);
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Blue_Data,mScene));

    QList<QVariant> ambientLight_Alpha_Data;
    ambientLight_Alpha_Data << "Alpha" << mScene->getClearColour().at(3);
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Alpha_Data,mScene));
}

void
ScenePropertiesModel::createPhysicsProperties
()
{
    // Physics
    QList<QVariant> physicsData;
    physicsData << "Physics"  << "";
    PropertiesItem *physicsProperty= new PropertiesItem(physicsData,mScene);
    mRootItem->appendChild(physicsProperty);

    QList<QVariant> gravityData;
    gravityData << "Gravity" << "";
    PropertiesItem *gravityProperty = new PropertiesItem(gravityData,mScene);
    physicsProperty->appendChild(gravityProperty);

    QList<QVariant> gravityDataX;
    gravityDataX << "X" << mScene->getGravity().at(0);
    PropertiesItem *gravityPropertyX = new PropertiesItem(gravityDataX,mScene);
    gravityProperty->appendChild(gravityPropertyX);

    QList<QVariant> gravityDataY;
    gravityDataY << "Y" << mScene->getGravity().at(1);
    PropertiesItem *gravityPropertyY = new PropertiesItem(gravityDataY,mScene);
    gravityProperty->appendChild(gravityPropertyY);

    QList<QVariant> gravityDataZ;
    gravityDataZ << "Z" << mScene->getGravity().at(2);
    PropertiesItem *gravityPropertyZ = new PropertiesItem(gravityDataZ,mScene);
    gravityProperty->appendChild(gravityPropertyZ);

    QList<QVariant> debugData;
    debugData << "Debug" << mScene->getPhysicsDebug();
    PropertiesItem *debugProperty = new PropertiesItem(debugData,mScene);
    physicsProperty->appendChild(debugProperty);
}

