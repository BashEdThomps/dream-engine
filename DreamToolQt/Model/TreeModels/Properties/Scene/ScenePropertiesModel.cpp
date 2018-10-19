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

#include <spdlog/spdlog.h>
#include <DreamCore.h>

#include "ScenePropertiesItem.h"
#include "ScenePropertiesTreeDelegate.h"

using Dream::Transform3D;
using Dream::Constants;

ScenePropertiesModel::ScenePropertiesModel
(SceneDefinition* scene, QTreeView* parent)
    : AbstractPropertiesModel(new ScenePropertiesTreeDelegate(this,parent), parent),
      mSceneDefinitionHandle(scene)
{
    auto log = spdlog::get("ScenePropertiesModel");
    if (log == nullptr)
    {
        log = spdlog::stderr_color_mt("ScenePropertiesModel");
    }
    log->debug("Constructor called");
    createRoot();
    createProperties();
    createDelegateConnections();
}

ScenePropertiesModel::~ScenePropertiesModel
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("Desstructor called");
}

void
ScenePropertiesModel::createRoot
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("craeteRoot");
    mRootItem.reset
    (
        new ScenePropertiesItem
        (
            QString::fromStdString(mSceneDefinitionHandle->getName()),
            mSceneDefinitionHandle
        )
    );
}

void
ScenePropertiesModel::createProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createProperties");
    createNameProperties();
    createNotesProperties();
    createCameraProperties();
    createRenderingProperties();
    createPhysicsProperties();
}

void
ScenePropertiesModel::createDelegateConnections
()
{
    ScenePropertiesTreeDelegate *delegate;
    delegate = static_cast<ScenePropertiesTreeDelegate*>(mDelegateHandle);

    // Camera Translation
    connect
    (
        delegate,
        SIGNAL(notifyButton_CaptureCameraTranslation()),
        this,
        SLOT(onButton_CaptureCameraTranslation())
    );

    // Camera LookAt
    connect
    (
        delegate,
        SIGNAL(notifyButton_CaptureCameraLookAt()),
        this,
        SLOT(onButton_CaptureCameraLookAt())
    );

    // Clear Colour Chooser
    connect(
        delegate, SIGNAL(notifyButton_ChooseClearColour()),
        this, SLOT(onButton_ChooseClearColour())
    );

    // Ambient Colour Chooser
    connect(
        delegate, SIGNAL(notifyButton_ChooseAmbientColour()),
        this, SLOT(onButton_ChooseAmbientColour())
    );
}

void
ScenePropertiesModel::createNameProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createNameProperties");
    mRootItem->appendChild
    (
        new ScenePropertiesItem
        (
            "Name",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_NAME
        )
    );
}

void
ScenePropertiesModel::createNotesProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createNotesProperties");
    mRootItem->appendChild
    (
        new ScenePropertiesItem
        (
            "Notes",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_NOTES
        )
    );
}

void
ScenePropertiesModel::createCameraProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createCameraProperties");
    ScenePropertiesItem *cameraProperty = new ScenePropertiesItem("Camera",mSceneDefinitionHandle,SCENE_PROPERTY_CAMERA);
    mRootItem->appendChild(cameraProperty);
    {
        log->debug("createCameraProperties translation");
        ScenePropertiesItem* cameraTranslationProperty = new ScenePropertiesItem
        (
            "Translation",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_CAMERA_TRANSLATION_CAPTURE
        );

        cameraProperty->appendChild(cameraTranslationProperty);
        {
            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "X",
                    mSceneDefinitionHandle,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_X
                )
            );

            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Y",
                    mSceneDefinitionHandle,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Y
                )
            );

            cameraTranslationProperty->appendChild
            (
                new ScenePropertiesItem
                (
                    "Z",
                    mSceneDefinitionHandle,
                    SCENE_PROPERTY_CAMERA_TRANSLATION_Z
                )
            );
        }

        log->debug("createCameraProperties LookAt");
        ScenePropertiesItem *cameraLookAtProperty = new ScenePropertiesItem
        (
            "Orientation",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_CAMERA_LOOK_AT_CAPTURE

        );

        cameraProperty->appendChild(cameraLookAtProperty);

        /*
        cameraLookAtProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "X",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_LOOK_AT_X
            )
        );

        cameraLookAtProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Y",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_LOOK_AT_Y
            )
        );

        cameraLookAtProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Z",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_LOOK_AT_Z
            )
        );
        */

        cameraLookAtProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Pitch",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_PITCH
            )
        );

        cameraLookAtProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Yaw",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_YAW
            )
        );


        log->debug("createCameraProperties speed");

        cameraProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Speed",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CAMERA_SPEED
            )
        );

        cameraProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Mesh Cull",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_MESH_CULL_DISTANCE
            )
        );

        auto frustum =  new ScenePropertiesItem(
            "Frustum",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_NONE
        );

        frustum->appendChild
        (
            new ScenePropertiesItem
            (
                "Minimum Draw",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_MINIMUM_DRAW
            )
        );

        frustum->appendChild
        (
            new ScenePropertiesItem
            (
                "Maximum Draw",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_MAXIMUM_DRAW
            )
        );
        cameraProperty->appendChild(frustum);
    }
}

void
ScenePropertiesModel::createRenderingProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createRenderingProperties");
    vector<float> clear = mSceneDefinitionHandle->getClearColour();

    auto renderingProperty = new ScenePropertiesItem("Rendering",mSceneDefinitionHandle);
    mRootItem->appendChild(renderingProperty);
    {

        log->debug("createRenderingProperties (ClearColour)");
        // Clear Color
        auto clearColorProperty = new ScenePropertiesItem("Clear Colour", mSceneDefinitionHandle,SCENE_PROPERTY_CLEAR_PARENT);
        renderingProperty->appendChild(clearColorProperty);

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Red",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CLEAR_RED
            )
        );

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Green",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CLEAR_GREEN
            )
        );

        clearColorProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Blue",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_CLEAR_BLUE
            )
        );

    }

    log->debug("createRenderingProperties (AmbientLight)");
    vector<float> ambient = mSceneDefinitionHandle->getAmbientColour();

    auto *ambientLightProperty = new ScenePropertiesItem(
        "Ambient Light", mSceneDefinitionHandle,SCENE_PROPERTY_AMBIENT_PARENT
    );
    renderingProperty->appendChild(ambientLightProperty);
    {
        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Red",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_AMBIENT_RED
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Green",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_AMBIENT_GREEN
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Blue",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_AMBIENT_BLUE
            )
        );

        ambientLightProperty->appendChild
        (
            new ScenePropertiesItem
            (
                "Alpha",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_AMBIENT_ALPHA
            )
        );
    }

    renderingProperty->appendChild
    (
        new ScenePropertiesItem
        (
            "Lighting Shader",
            mSceneDefinitionHandle,
            SCENE_PROPERTY_LIGHTING_SHADER
        )
    );
}

void
ScenePropertiesModel::createPhysicsProperties
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("createPhysicsProperties");
    ScenePropertiesItem *physicsProperty = new ScenePropertiesItem("Physics",mSceneDefinitionHandle);
    mRootItem->appendChild(physicsProperty);
    {
        ScenePropertiesItem *gravityProperty = new ScenePropertiesItem("Gravity",mSceneDefinitionHandle);
        physicsProperty->appendChild(gravityProperty);
        {
            vector<float> gravity = mSceneDefinitionHandle->getGravity();
            ScenePropertiesItem *gravityPropertyX = new ScenePropertiesItem
            (
                "X",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_PHYSICS_GRAVITY_X
            );
            gravityProperty->appendChild(gravityPropertyX);

            ScenePropertiesItem *gravityPropertyY = new ScenePropertiesItem
            (
                "Y",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Y
            );
            gravityProperty->appendChild(gravityPropertyY);

            ScenePropertiesItem *gravityPropertyZ = new ScenePropertiesItem
            (
                "Z",
                mSceneDefinitionHandle,
                SCENE_PROPERTY_PHYSICS_GRAVITY_Z
            );
            gravityProperty->appendChild(gravityPropertyZ);
        }
    }

    ScenePropertiesItem *debugProperty = new ScenePropertiesItem
    (
        "Debug",
        mSceneDefinitionHandle,
        SCENE_PROPERTY_PHYSICS_DEBUG
    );
    physicsProperty->appendChild(debugProperty);
}

void
ScenePropertiesModel::onButton_CaptureCameraTranslation
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("CaptureCameraTranslation");
    emit notifyButton_CaptureCameraTranslation(mSceneDefinitionHandle);
}

void
ScenePropertiesModel::onButton_CaptureCameraLookAt
()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("CaptureCameraLookAt");
    emit notifyButton_CaptureCameraLookAt(mSceneDefinitionHandle);
}

void ScenePropertiesModel::onButton_ChooseClearColour()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("Choose Clear Colour");
    emit notifyButton_ChooseClearColour(mSceneDefinitionHandle);
}

void ScenePropertiesModel::onButton_ChooseAmbientColour()
{
    auto log = spdlog::get("ScenePropertiesModel");
    log->debug("Choose Ambient Colour");
    emit notifyButton_ChooseAmbientColour(mSceneDefinitionHandle);

}
