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
#include "../DoubleSpinBoxDelegate.h"
#include "../CheckBoxDelegate.h"

using Dream::Transform3D;
using Dream::AssetDefinition;
using Dream::SceneObjectDefinition;

SceneObjectPropertiesModel::SceneObjectPropertiesModel
(SceneObjectDefinition *sceneObject, QTreeView *parent)
    : PropertiesModel(parent)
{
    qDebug() << "SceneObjectPropertiesModel: Constructor called";
    mSceneObjectDefinitionHandle = sceneObject;
    createRoot();
    createProperties();

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
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mSceneObjectDefinitionHandle->getName())
             << QString::fromStdString(mSceneObjectDefinitionHandle->getUuid());
    mRootItem.reset(new SceneObjectPropertiesItem(rootData,mSceneObjectDefinitionHandle));
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
    createAssetInstancesProperty();
    createChildrenProperty();
}


void
SceneObjectPropertiesModel::createNameProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createName";
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mSceneObjectDefinitionHandle->getName());
    mRootItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            nameData,
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
    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();

    QList<QVariant> translationData;
    translationData << "Translation" << "";
    SceneObjectPropertiesItem *translationItem = new SceneObjectPropertiesItem
    (
        translationData,
        mSceneObjectDefinitionHandle
    );
    mRootItem->appendChild(translationItem);

    QList<QVariant> translationDataX;
    translationDataX << "X" << transform.getTranslation().x;
    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            translationDataX,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_X,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> translationDataY;
    translationDataY << "Y" << transform.getTranslation().y;
    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            translationDataY,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_Y,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> translationDataZ;
    translationDataZ << "Z" << transform.getTranslation().z;
    translationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            translationDataZ,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_TRANSLATION_Z,
            new DoubleSpinBoxDelegate()
        )
    );
}

void
SceneObjectPropertiesModel::createRotationProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createRotationProperty";
    QList<QVariant> rotationData;
    rotationData << "Rotation" << "";
    SceneObjectPropertiesItem *rotationItem = new SceneObjectPropertiesItem(rotationData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(rotationItem);

    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();

    QList<QVariant> rotationDataX;
    rotationDataX << "X" << transform.getRotation().x;
    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            rotationDataX,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_X,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> rotationDataY;
    rotationDataY << "Y" << transform.getRotation().y;
    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            rotationDataY,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_Y,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> rotationDataZ;
    rotationDataZ << "Z" << transform.getRotation().z;
    rotationItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            rotationDataZ,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_ROTATION_Z,
            new DoubleSpinBoxDelegate()
        )
    );
}

void
SceneObjectPropertiesModel::createScaleProperty
()
{

    qDebug() << "SceneObjectPropertiesModel: createScaleProperty";
    QList<QVariant> scaleData;
    scaleData << "Scale" << "";
    SceneObjectPropertiesItem *scaleItem = new SceneObjectPropertiesItem(scaleData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(scaleItem);

    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();

    QList<QVariant> scaleDataX;
    scaleDataX << "X" << transform.getScale().x;
    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            scaleDataX,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_X,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> scaleDataY;
    scaleDataY << "Y" << transform.getScale().y;
    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            scaleDataY,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_Y,
            new DoubleSpinBoxDelegate()
        )
    );

    QList<QVariant> scaleDataZ;
    scaleDataZ << "Z" << transform.getScale().z;
    scaleItem->appendChild
    (
        new SceneObjectPropertiesItem
        (
            scaleDataZ,
            mSceneObjectDefinitionHandle,
            SCENE_OBJECT_PROPERTY_SCALE_Z,
            new DoubleSpinBoxDelegate()
        )
    );
}

void
SceneObjectPropertiesModel::createTransformTypeProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createTransformTypeProperty";
    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();
    QList<QVariant> transformTypeData;
    transformTypeData << "Transform Type" << QString::fromStdString(transform.getTransformType());
    SceneObjectPropertiesItem *transformTypeItem = new SceneObjectPropertiesItem
    (
        transformTypeData,
        mSceneObjectDefinitionHandle
    );
    mRootItem->appendChild(transformTypeItem);
}

void
SceneObjectPropertiesModel::createHasFocusProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createHasFocusProperty";
    QList<QVariant> hasFocusData;
    hasFocusData << "Has Focus" << mSceneObjectDefinitionHandle->hasFocus();
    SceneObjectPropertiesItem *hasFocusItem = new SceneObjectPropertiesItem
    (
        hasFocusData,
        mSceneObjectDefinitionHandle,
        SCENE_OBJECT_PROPERTY_HAS_FOCUS,
        new CheckBoxDelegate()
    );
    mRootItem->appendChild(hasFocusItem);
}

void
SceneObjectPropertiesModel::createAssetInstancesProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createAssetInstancesProperty";
    QList<QVariant> assetDefData;
    vector<string> definitionsToLoad = mSceneObjectDefinitionHandle->getAssetDefinitionLoadQueue();
    assetDefData << "Asset Definitions" << QString::number(definitionsToLoad.size());
    SceneObjectPropertiesItem *assetDefItem = new SceneObjectPropertiesItem
    (
        assetDefData,
        mSceneObjectDefinitionHandle
    );
    mRootItem->appendChild(assetDefItem);

    for (std::string adUuid : definitionsToLoad)
    {
       QList<QVariant> assetInstanceData;
       AssetDefinition* definition = mSceneObjectDefinitionHandle->getSceneDefinitionHandle()
               ->getProjectDefinitionHandle()
               ->getAssetDefinitionHandleByUuid(adUuid);
       assetInstanceData << QString::fromStdString(definition->getName()) << QString::fromStdString(definition->getType());
       assetDefItem->appendChild(new SceneObjectPropertiesItem(assetInstanceData,mSceneObjectDefinitionHandle));
    }
}

void
SceneObjectPropertiesModel::createChildrenProperty
()
{
    qDebug() << "SceneObjectPropertiesModel: createChildrenProperty";
    QList<QVariant> childrenData;
    childrenData << "Children" << QString::number(mSceneObjectDefinitionHandle->getChildDefinitionsHandleList().size());
    SceneObjectPropertiesItem *childrenItem = new SceneObjectPropertiesItem(childrenData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(childrenItem);

    for (SceneObjectDefinition* child : mSceneObjectDefinitionHandle->getChildDefinitionsHandleList())
    {
       QList<QVariant> childData;
       childData << QString::fromStdString(child->getName()) << QString::fromStdString(child->getUuid());
       childrenItem->appendChild(new SceneObjectPropertiesItem(childData,mSceneObjectDefinitionHandle));
    }
}

bool
SceneObjectPropertiesModel::setData
(const QModelIndex &index, const QVariant &value, int role)
{
    SceneObjectPropertiesItem *item = static_cast<SceneObjectPropertiesItem*>(getItem(index));
    SceneObjectDefinition *sdHandle = item->getSceneObjectDefinitionHandle();

    switch(item->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_NAME:
            sdHandle->setName(value.toString().toStdString());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
            sdHandle->getTransform().setTranslationX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
            sdHandle->getTransform().setTranslationY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
            sdHandle->getTransform().setTranslationZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ROTATION_X:
            sdHandle->getTransform().setRotationX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ROTATION_Y:
            sdHandle->getTransform().setRotationY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ROTATION_Z:
            sdHandle->getTransform().setRotationZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_X:
            sdHandle->getTransform().setScaleX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
            sdHandle->getTransform().setScaleY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            sdHandle->getTransform().setScaleZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            sdHandle->getTransform().setTransformType(value.toString().toStdString());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            sdHandle->setHasFocus(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_NONE:
            break;

    }

    return false;
}
