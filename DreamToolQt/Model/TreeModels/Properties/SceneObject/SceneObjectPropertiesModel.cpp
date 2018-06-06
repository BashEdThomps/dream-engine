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
#include <QDebug>

#include "SceneObjectPropertiesItem.h"
#include "SceneObjectPropertiesTreeDelegate.h"

using Dream::Transform3D;
using Dream::IAssetDefinition;
using Dream::SceneObjectDefinition;

SceneObjectPropertiesModel::SceneObjectPropertiesModel
(SceneObjectDefinition *sceneObject, QTreeView *parent)
    : AbstractPropertiesModel(new SceneObjectPropertiesTreeDelegate(this,parent), parent)
{
    qDebug() << "SceneObjectPropertiesModel: Constructor called";
    mSceneObjectDefinitionHandle = sceneObject;
    createRoot();
    createProperties();
    createDelegateConnections();
}

SceneObjectPropertiesModel::~SceneObjectPropertiesModel
()
{
    qDebug() << "SceneObjectPropertiesModel: Destructor called";
}

void
SceneObjectPropertiesModel::createRoot
()
{
    qDebug() << "SceneObjectPropertiesModel: CreateRoot";
    QString name = QString::fromStdString(mSceneObjectDefinitionHandle->getName());
    mRootItem.reset(new SceneObjectPropertiesItem(name,mSceneObjectDefinitionHandle));
}

void
SceneObjectPropertiesModel::createProperties
()
{
    qDebug() << "SceneObjectPropertiesModel: CreateProperties";
    createNameProperty();
    createTranslationProperty();
    createRotationProperty();
    createScaleProperty();
    createTransformTypeProperty();
    createHasFocusProperty();
    createFollowsCameraProperty();
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
        SIGNAL(notifyButton_CaptureRotation()),
        this,
        SLOT(onButton_CaptureRotation())
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
        SIGNAL(notifyButton_RemoveAsset(IAssetDefinition*)),
        this,
        SLOT(onButton_RemoveAsset(IAssetDefinition*))
    );

    connect
    (
        delegate,
        SIGNAL(notifyButton_RemoveChild(SceneObjectDefinition*)),
        this,
        SLOT(onButton_RemoveChild(SceneObjectDefinition*))
    );
}


void
SceneObjectPropertiesModel::createNameProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createName";
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
    qDebug() << "SceneObjectPropertiesModel: createTranslationProperty";
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
SceneObjectPropertiesModel::createRotationProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createRotationProperty";
    SceneObjectPropertiesItem *rotationItem = new SceneObjectPropertiesItem
    (
        "Rotation",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_ROTATION_CAPTURE
    );
    mRootItem->appendChild(rotationItem);

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "X",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_X
        )
    );

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Y",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_Y
        )
    );

    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            "Z",
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_Z
        )
    );
}

void
SceneObjectPropertiesModel::createScaleProperty
()
{

    qDebug() << "SceneObjectPropertiesModel: createScaleProperty";
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
    qDebug() << "SceneObjectPropertiesModel: createTransformTypeProperty";
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
    qDebug() << "SceneObjectPropertiesModel: createFollowsCameraProperty";
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
    qDebug() << "SceneObjectPropertiesModel: createHasFocusProperty";
    SceneObjectPropertiesItem *hasFocusItem = new SceneObjectPropertiesItem
    (
        "Has Focus",
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_HAS_FOCUS
    );
    mRootItem->appendChild(hasFocusItem);
}

void
SceneObjectPropertiesModel::createAssetInstancesProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createAssetInstancesProperty";
    vector<string> definitionsToLoad = mSceneObjectDefinitionHandle->getAssetDefinitionLoadQueue();
    SceneObjectPropertiesItem *assetDefItem = new SceneObjectPropertiesItem
    (
        "Asset Definitions",
        mSceneObjectDefinitionHandle
    );
    mRootItem->appendChild(assetDefItem);

    for (std::string adUuid : definitionsToLoad)
    {
       IAssetDefinition* definition = mSceneObjectDefinitionHandle->getSceneDefinitionHandle()
               ->getProjectDefinitionHandle()
               ->getAssetDefinitionHandleByUuid(adUuid);

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
    qDebug() << "SceneObjectPropertiesModel: createChildrenProperty";
    SceneObjectPropertiesItem *childrenItem = new SceneObjectPropertiesItem("Children",mSceneObjectDefinitionHandle);
    mRootItem->appendChild(childrenItem);

    for (SceneObjectDefinition* child : mSceneObjectDefinitionHandle->getChildDefinitionsHandleList())
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

void
SceneObjectPropertiesModel::onButton_CaptureTranslation
()
{
    qDebug() << "SceneObjectPropertiesModel: CaptureTranslation";
    emit notifyButton_CaptureTranslation(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_CaptureRotation
()
{
    qDebug() << "SceneObjectPropertiesModel: CaptureRotation";
    emit notifyButton_CaptureRotation(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_CaptureScale
()
{
    qDebug() << "SceneObjectPropertiesModel: CaptureScale";
    emit notifyButton_CaptureScale(mSceneObjectDefinitionHandle);
}

void
SceneObjectPropertiesModel::onButton_RemoveAsset
(IAssetDefinition* adHandle)
{
    qDebug() << "SceneObjectPropertiesModel: RemoveAsset";
    emit notifyButton_RemoveAsset(mSceneObjectDefinitionHandle,adHandle);
}

void
SceneObjectPropertiesModel::onButton_RemoveChild
(SceneObjectDefinition* sodHandle)
{
    qDebug() << "SceneObjectPropertiesModel: RemoveChild";
    emit notifyButton_RemoveChild(mSceneObjectDefinitionHandle,sodHandle);
}
