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
    mRootItem.reset
    (
        new ScenePropertiesItem
        (
            QString::fromStdString(mSceneDefinition->getName()),
            mSceneDefinition
        )
    );
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
    ScenePropertiesItem *nameProperty = new ScenePropertiesItem("Name",mSceneDefinition,SCENE_PROPERTY_NAME);
    mRootItem->appendChild(nameProperty);
}

void
ScenePropertiesModel::createNotesProperties
()
{
    qDebug() << "ScenePropertiesModel: createNotesProperties";
    ScenePropertiesItem *notesProperty = new ScenePropertiesItem("Notes",mSceneDefinition,SCENE_PROPERTY_NOTES);
    mRootItem->appendChild(notesProperty);
}

void
ScenePropertiesModel::createCameraProperties
()
{
    qDebug() << "ScenePropertiesModel: createCameraProperties";
    ScenePropertiesItem *cameraProperty = new ScenePropertiesItem("Camera",mSceneDefinition);
    mRootItem->appendChild(cameraProperty);
    {
        Transform3D defaultTransform = mSceneDefinition->getCameraTransform();

        qDebug() << "ScenePropertiesModel: createCameraProperties translation";
        ScenePropertiesItem* cameraTranslationProperty = new ScenePropertiesItem("Translation", mSceneDefinition);

        cameraProperty->appendChild(cameraTranslationProperty);
        {
            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "X",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_X,
                    new DoubleSpinBoxDelegate()
                )
            );

            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Y",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Y,
                    new DoubleSpinBoxDelegate()
                )
            );

            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Z",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Z,
                    new DoubleSpinBoxDelegate()
                )
            );
        }

        qDebug() << "ScenePropertiesModel: createCameraProperties rotation";
        ScenePropertiesItem *cameraRotationProperty = new ScenePropertiesItem
        (
            "Rotation",
            mSceneDefinition
        );

        cameraProperty->appendChild(cameraRotationProperty);
        {
            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "X",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_X,
                    new DoubleSpinBoxDelegate()
                )
            );

            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Y",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_Y,
                    new DoubleSpinBoxDelegate()
                )
            );

            cameraRotationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Z",
                    mSceneDefinition,
                    SCENE_PROPERTY_CAMERA_ROTATION_Z,
                    new DoubleSpinBoxDelegate()
                )
            );

            qDebug() << "ScenePropertiesModel: createCameraProperties speed";

            cameraProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Speed",
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

    ScenePropertiesItem *renderingProperty = new ScenePropertiesItem("Rendering",mSceneDefinition);
    mRootItem->appendChild(renderingProperty);
    {

        qDebug() << "ScenePropertiesModel: createRenderingProperties (ClearColour)";
        // Clear Color
        ScenePropertiesItem *clearColorProperty = new ScenePropertiesItem("Clear Colour", mSceneDefinition);
        renderingProperty->appendChild(clearColorProperty);

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Red",
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_RED,
                new DoubleSpinBoxDelegate()
            )
        );

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Green",
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_GREEN,
                new DoubleSpinBoxDelegate()
            )
        );

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Blue",
                mSceneDefinition,
                SCENE_PROPERTY_CLEAR_BLUE,
                new DoubleSpinBoxDelegate()
            )
        );
    }

    qDebug() << "ScenePropertiesModel: createRenderingProperties (AmbientLight)";
    vector<float> ambient = mSceneDefinition->getAmbientColour();

    ScenePropertiesItem *ambientLightProperty = new ScenePropertiesItem("Ambient Light", mSceneDefinition);
    renderingProperty->appendChild(ambientLightProperty);
    {
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Red",
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_RED,
                new DoubleSpinBoxDelegate()
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Green",
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_GREEN,
                new DoubleSpinBoxDelegate()
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Blue",
                mSceneDefinition,
                SCENE_PROPERTY_AMBIENT_BLUE,
                new DoubleSpinBoxDelegate()
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Alpha",
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
    ScenePropertiesItem *physicsProperty = new ScenePropertiesItem("Physics",mSceneDefinition);
    mRootItem->appendChild(physicsProperty);
    {
        ScenePropertiesItem *gravityProperty = new ScenePropertiesItem("Gravity",mSceneDefinition);
        physicsProperty->appendChild(gravityProperty);
        {
            vector<float> gravity = mSceneDefinition->getGravity();
            ScenePropertiesItem *gravityPropertyX = new ScenePropertiesItem
            (
                "X",
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_X,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyX);

            ScenePropertiesItem *gravityPropertyY = new ScenePropertiesItem
            (
                "Y",
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Y,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyY);

            ScenePropertiesItem *gravityPropertyZ = new ScenePropertiesItem
            (
                "Z",
                mSceneDefinition,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Z,
                new DoubleSpinBoxDelegate()
            );
            gravityProperty->appendChild(gravityPropertyZ);
        }
    }

    ScenePropertiesItem *debugProperty = new ScenePropertiesItem
    (
        "Debug",
        mSceneDefinition,
        SCENE_PROPERTY_PHYSICS_DEBUG,
        new CheckBoxDelegate()
    );
    physicsProperty->appendChild(debugProperty);
}
