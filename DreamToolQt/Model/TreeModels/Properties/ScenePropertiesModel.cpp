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
    mRootItem.reset(new PropertiesItem(rootData,mSceneDefinition));
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
             << QString::fromStdString(mSceneDefinition->getName());
    PropertiesItem *nameProperty = new PropertiesItem(nameData,mSceneDefinition);
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
             << QString::fromStdString(mSceneDefinition->getNotes());
    PropertiesItem *notesProperty = new PropertiesItem(notesData,mSceneDefinition);
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
    PropertiesItem *cameraProperty = new PropertiesItem(cameraData,mSceneDefinition);
    mRootItem->appendChild(cameraProperty);

    qDebug() << "ScenePropertiesModel: createCameraProperties translation";
    // Camera Translation
    QList<QVariant> cameraTranslationData;
    cameraTranslationData << "Translation" << "";
    PropertiesItem* cameraTranslationProperty = new PropertiesItem(cameraTranslationData, mSceneDefinition);
    cameraProperty->appendChild(cameraTranslationProperty);

    Transform3D defaultTransform = mSceneDefinition->getCameraTransform();
    QList<QVariant> cameraTranslationX_Data;
    cameraTranslationX_Data << "X" << QString::number(static_cast<double>(defaultTransform.getTranslation().x));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationX_Data,mSceneDefinition));

    QList<QVariant> cameraTranslationY_Data;
    cameraTranslationY_Data << "Y" << QString::number(static_cast<double>(defaultTransform.getTranslation().y));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationY_Data,mSceneDefinition));

    QList<QVariant> cameraTranslationZ_Data;
    cameraTranslationZ_Data << "Z" << QString::number(static_cast<double>(defaultTransform.getTranslation().z));
    cameraTranslationProperty->appendChild(new PropertiesItem(cameraTranslationZ_Data,mSceneDefinition));

    qDebug() << "ScenePropertiesModel: createCameraProperties rotation";
    // Camera Rotation
    QList<QVariant> cameraRotationData;
    cameraRotationData << "Rotation" << "";
    PropertiesItem *cameraRotationProperty = new PropertiesItem(cameraRotationData, mSceneDefinition);
    cameraProperty->appendChild(cameraRotationProperty);

    QList<QVariant> cameraRotationX_Data;
    cameraRotationX_Data << "X" << QString::number(static_cast<double>(defaultTransform.getRotation().x));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationX_Data,mSceneDefinition));

    QList<QVariant> cameraRotationY_Data;
    cameraRotationY_Data << "Y" << QString::number(static_cast<double>(defaultTransform.getRotation().y));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationY_Data,mSceneDefinition));

    QList<QVariant> cameraRotationZ_Data;
    cameraRotationZ_Data << "Z" << QString::number(static_cast<double>(defaultTransform.getRotation().z));
    cameraRotationProperty->appendChild(new PropertiesItem(cameraRotationZ_Data,mSceneDefinition));

    qDebug() << "ScenePropertiesModel: createCameraProperties speed";
    // Camera Speed
    QList<QVariant> cameraSpeedData;
    cameraSpeedData << "Speed" << QString::number(static_cast<double>(mSceneDefinition->getCameraMovementSpeed()));
    cameraProperty->appendChild(new PropertiesItem(cameraSpeedData, mSceneDefinition));
}

void
ScenePropertiesModel::createRenderingProperties
()
{
    qDebug() << "ScenePropertiesModel: createRenderingProperties";

    // Rendering
    QList<QVariant> renderingData;
    renderingData << "Rendering" << "";
    PropertiesItem *renderingProperty = new PropertiesItem(renderingData,mSceneDefinition);
    mRootItem->appendChild(renderingProperty);

    qDebug() << "ScenePropertiesModel: createRenderingProperties (ClearColour)";

    // Clear Color
    QList<QVariant> clearColorData;
    clearColorData << "Clear Color" << "";
    PropertiesItem *clearColorProperty = new PropertiesItem(clearColorData, mSceneDefinition);
    renderingProperty->appendChild(clearColorProperty);

    vector<float> clear = mSceneDefinition->getClearColour();

    QList<QVariant> clearColor_Red_Data;
    clearColor_Red_Data << "Red" << clear[0];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Red_Data,mSceneDefinition));

    QList<QVariant> clearColor_Green_Data;
    clearColor_Green_Data << "Green" << clear[1];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Green_Data,mSceneDefinition));

    QList<QVariant> clearColor_Blue_Data;
    clearColor_Blue_Data << "Blue" << clear[2];
    clearColorProperty->appendChild(new PropertiesItem(clearColor_Blue_Data,mSceneDefinition));

    qDebug() << "ScenePropertiesModel: createRenderingProperties (AmbientLight)";

    vector<float> ambient = mSceneDefinition->getAmbientColour();
    // Ambient Light Color
    QList<QVariant> ambientLightData;
    ambientLightData << "Ambient Light" << "";
    PropertiesItem *ambientLightProperty = new PropertiesItem(ambientLightData, mSceneDefinition);
    renderingProperty->appendChild(ambientLightProperty);

    QList<QVariant> ambientLight_Red_Data;
    ambientLight_Red_Data
        << "Red"
        << QString::number(
               static_cast<double>(ambient[Constants::RED_INDEX]));
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Red_Data,mSceneDefinition));

    QList<QVariant> ambientLight_Green_Data;
    ambientLight_Green_Data
        << "Green"
        << QString::number(
              static_cast<double>( ambient[Constants::GREEN_INDEX]));
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Green_Data,mSceneDefinition));

    QList<QVariant> ambientLight_Blue_Data;
    ambientLight_Blue_Data
        << "Blue"
        << QString::number(
               static_cast<double>(ambient[Constants::BLUE_INDEX]));
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Blue_Data,mSceneDefinition));

    QList<QVariant> ambientLight_Alpha_Data;
    ambientLight_Alpha_Data
        << "Alpha"
        << QString::number(
               static_cast<double>(ambient[Constants::ALPHA_INDEX]));
    ambientLightProperty->appendChild(new PropertiesItem(ambientLight_Alpha_Data,mSceneDefinition));
}

void
ScenePropertiesModel::createPhysicsProperties
()
{
    qDebug() << "ScenePropertiesModel: createPhysicsProperties";
    // Physics
    QList<QVariant> physicsData;
    physicsData << "Physics"  << "";
    PropertiesItem *physicsProperty = new PropertiesItem(physicsData,mSceneDefinition);
    mRootItem->appendChild(physicsProperty);
    {
        QList<QVariant> gravityData;
        gravityData << "Gravity" << "";
        PropertiesItem *gravityProperty = new PropertiesItem(gravityData,mSceneDefinition);
        physicsProperty->appendChild(gravityProperty);
        {
            vector<float> gravity = mSceneDefinition->getGravity();
            QList<QVariant> gravityDataX;
            gravityDataX << "X" << gravity[Constants::X_INDEX];
            PropertiesItem *gravityPropertyX = new PropertiesItem(gravityDataX,mSceneDefinition);
            gravityProperty->appendChild(gravityPropertyX);

            QList<QVariant> gravityDataY;
            gravityDataY << "Y" << gravity[Constants::Y_INDEX];
            PropertiesItem *gravityPropertyY = new PropertiesItem(gravityDataY,mSceneDefinition);
            gravityProperty->appendChild(gravityPropertyY);

            QList<QVariant> gravityDataZ;
            gravityDataZ << "Z" << gravity[Constants::Z_INDEX];
            PropertiesItem *gravityPropertyZ = new PropertiesItem(gravityDataZ,mSceneDefinition);
            gravityProperty->appendChild(gravityPropertyZ);
        }
    }

    QList<QVariant> debugData;
    debugData << "Debug" << mSceneDefinition->getPhysicsDebug();
    PropertiesItem *debugProperty = new PropertiesItem(debugData,mSceneDefinition);
    physicsProperty->appendChild(debugProperty);
}

