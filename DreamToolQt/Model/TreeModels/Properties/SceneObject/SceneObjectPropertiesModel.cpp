/*
 * SceneObjectPropertiesModel.cpp
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

#include "SceneObjectPropertiesModel.h"

#include <DreamCore.h>
#include <spdlog/spdlog.h>

#include "SceneObjectPropertiesItem.h"
#include "SceneObjectPropertiesTreeDelegate.h"

using Dream::Transform3D;
using Dream::IAssetDefinition;
using Dream::SceneObjectDefinition;

SceneObjectPropertiesModel::SceneObjectPropertiesModel
(SceneObjectDefinition* sceneObject, QTreeView *parent)
    : AbstractPropertiesModel(new SceneObjectPropertiesTreeDelegate(this,parent), parent)
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("SceneObjectPropertiesModel");
    }

    log->info("Constructor called");
    mSceneObjectDefinitionHandle = sceneObject;
    createRoot();
    createProperties();
    createDelegateConnections();
}

SceneObjectPropertiesModel::~SceneObjectPropertiesModel
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("Destructor called");
}

void
SceneObjectPropertiesModel::createRoot
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("CreateRoot");
    QString name = QString::fromStdString(mSceneObjectDefinitionHandle->getName());
    mRootItem.reset(new SceneObjectPropertiesItem(name,mSceneObjectDefinitionHandle));
}

void
SceneObjectPropertiesModel::createProperties
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("CreateProperties");
    createNameProperty();
    createTranslationProperty();
    createOrientationProperty();
    createScaleProperty();
    createTransformTypeProperty();
    createHasFocusProperty();
    createAlwaysDrawProperty();
    createFollowsCameraProperty();
    createStaticProperty();
    createAssetInstancesProperty();
    createChildrenProperty();
}

void
SceneObjectPropertiesModel::createDelegateConnections
()
{
    SceneObjectPropertiesTreeDelegate *delegate;
    delegate = static_cast<SceneObjectPropertiesTreeDelegate*>(mDelegateHandle);

    connect
    (
        delegate,
        SIGNAL(notifyButton_CaptureTranslation()),
        this,
        SLOT(onButton_CaptureTranslation())
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_CaptureOrientation()),
        this,
        SLOT(onButton_CaptureOrientation())
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_CaptureScale()),
        this,
        SLOT(onButton_CaptureScale())
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_RemoveAsset(IDefinition*)),
        this,
        SLOT(onButton_RemoveAsset(IDefinition*))
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_RemoveChild(IDefinition*)),
        this,
        SLOT(onButton_RemoveChild(IDefinition*))
    );
}


void
SceneObjectPropertiesModel::createNameProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createName");
    mRootItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Name",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_NAME
        )
    );
}

void
SceneObjectPropertiesModel::createTranslationProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createTranslationProperty");
    SceneObjectPropertiesItem *translationItem = new SceneObjectPropertiesItem
    (
        "Translation",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE
    );
    mRootItem->appendChild(translationItem);

    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "X",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_X
        )
    );

    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Y",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_Y
        )
    );

    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Z",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_Z
        )
    );
}

void
SceneObjectPropertiesModel::createOrientationProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("create Orientation Property");
    SceneObjectPropertiesItem *rotationItem = new SceneObjectPropertiesItem
    (
        "Orientation",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE
    );
    mRootItem->appendChild(rotationItem);

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "W",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ORIENTATION_W
        )
    );

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "X",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ORIENTATION_X
        )
    );

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Y",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ORIENTATION_Y
        )
    );

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Z",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ORIENTATION_Z
        )
    );
}

void
SceneObjectPropertiesModel::createScaleProperty
()
{

    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createScaleProperty");
    SceneObjectPropertiesItem *scaleItem = new SceneObjectPropertiesItem
    (
        "Scale",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_SCALE_CAPTURE
    );
    mRootItem->appendChild(scaleItem);

    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "X",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_X
        )
    );

    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Y",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_Y
        )
    );

    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Z",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_Z
        )
    );
}

void
SceneObjectPropertiesModel::createTransformTypeProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createTransformTypeProperty");
    SceneObjectPropertiesItem *transformTypeItem = new SceneObjectPropertiesItem
    (
        "Transform Type",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE
    );
    mRootItem->appendChild(transformTypeItem);
}

void
SceneObjectPropertiesModel::createFollowsCameraProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createFollowsCameraProperty");
    mRootItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Follows Camera",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA
        )
    );
}

void
SceneObjectPropertiesModel::createHasFocusProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createHasFocusProperty");
    SceneObjectPropertiesItem *hasFocusItem = new SceneObjectPropertiesItem
    (
        "Has Focus",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_HAS_FOCUS
    );
    mRootItem->appendChild(hasFocusItem);
}

void SceneObjectPropertiesModel::createAlwaysDrawProperty()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createAlwaysDrawProperty");
    SceneObjectPropertiesItem *hasFocusItem = new SceneObjectPropertiesItem
    (
        "Always Draw",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_ALWAYS_DRAW
    );
    mRootItem->appendChild(hasFocusItem);

}

void
SceneObjectPropertiesModel::createAssetInstancesProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createAssetInstancesProperty");
    vector<string> definitionsToLoad = mSceneObjectDefinitionHandle->getAssetDefinitionLoadQueue();
    SceneObjectPropertiesItem *assetDefItem = new SceneObjectPropertiesItem
    (
        "Asset Definitions",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST
    );
    mRootItem->appendChild(assetDefItem);

    for (std::string adUuid : definitionsToLoad)
    {
       IAssetDefinition* definition = mSceneObjectDefinitionHandle
           ->getSceneDefinition()
               ->getProjectDefinition()
                ->getAssetDefinitionByUuid(adUuid);

       SceneObjectPropertiesItem *adItem = new SceneObjectPropertiesItem
       (
            QString::fromStdString
            (
                definition->getName()
            ),
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ASSET_DEFINITION
       );
       adItem->setTargetAssetDefinitionHandle(definition);
       assetDefItem->appendChild(adItem);
    }
}

void
SceneObjectPropertiesModel::createChildrenProperty
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createChildrenProperty");
    SceneObjectPropertiesItem *childrenItem = new SceneObjectPropertiesItem(
        "Children",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_CHILD_LIST
    );
    mRootItem->appendChild(childrenItem);

    for (SceneObjectDefinition* child : mSceneObjectDefinitionHandle->getChildDefinitionsList())
    {
        SceneObjectPropertiesItem *childItem = new SceneObjectPropertiesItem
        (
            QString::fromStdString(child->getName()),
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_CHILD
        );
        childItem->setTargetSceneObjectDefinitionHandle(child);
        childrenItem->appendChild(childItem);
    }
}

void SceneObjectPropertiesModel::createStaticProperty()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("createStaticProperty");
    SceneObjectPropertiesItem *hasFocusItem = new SceneObjectPropertiesItem
    (
        "Static",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_STATIC
    );
    mRootItem->appendChild(hasFocusItem);

}

void
SceneObjectPropertiesModel::onButton_CaptureTranslation
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("CaptureTranslation");
    emit notifyButton_CaptureTranslation(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_CaptureOrientation
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("Capture Orientation");
    emit notifyButton_CaptureOrientation(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_CaptureScale
()
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("CaptureScale");
    emit notifyButton_CaptureScale(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_RemoveAsset
(IDefinition* adHandle)
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("RemoveAsset");
    emit notifyButton_RemoveAsset(mSceneObjectDefinitionHandle,adHandle);
}

void
SceneObjectPropertiesModel::onButton_RemoveChild
(IDefinition* sodHandle)
{
    auto log = spdlog::get("SceneObjectPropertiesModel");
    log->info("RemoveChild");
    emit notifyButton_RemoveChild(mSceneObjectDefinitionHandle,sodHandle);
}
