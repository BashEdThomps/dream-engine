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
#include <QDebug>

SceneObjectPropertiesModel::SceneObjectPropertiesModel
(SceneObject *sceneObject, QTreeView *parent)
    : PropertiesModel(parent)
{
    qDebug() << "SceneObjectPropertiesModel: Constructor called";
    mSceneObject = sceneObject;
    createRoot();
    createProperties();

}

SceneObjectPropertiesModel::~SceneObjectPropertiesModel
()
{
    qDebug() << "SceneObjectPropertiesModel: Destructor called";
    delete mRootItem;
}

void
SceneObjectPropertiesModel::createRoot
()
{
    qDebug() << "SceneObjectPropertiesModel: CreateRoot";
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mSceneObject->getName())
             << QString::fromStdString(mSceneObject->getUuid());
    mRootItem = new PropertiesItem(rootData,mSceneObject);
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


void SceneObjectPropertiesModel::createNameProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createName";
    QList<QVariant> nameData;
    nameData << "Name" << QString::fromStdString(mSceneObject->getName());
    mRootItem->appendChild(new PropertiesItem(nameData,mSceneObject));
}

void SceneObjectPropertiesModel::createTranslationProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createTranslationProperty";
    QList<QVariant> translationData;
    translationData << "Translation" << "";
    PropertiesItem *translationItem = new PropertiesItem(translationData,mSceneObject);
    mRootItem->appendChild(translationItem);

    QList<QVariant> translationDataX;
    translationDataX << "X" << mSceneObject->getTranslation().x;
    translationItem->appendChild(new PropertiesItem(translationDataX,mSceneObject));

    QList<QVariant> translationDataY;
    translationDataY << "Y" << mSceneObject->getTranslation().y;
    translationItem->appendChild(new PropertiesItem(translationDataY,mSceneObject));

    QList<QVariant> translationDataZ;
    translationDataZ << "Z" << mSceneObject->getTranslation().z;
    translationItem->appendChild(new PropertiesItem(translationDataZ,mSceneObject));
}

void SceneObjectPropertiesModel::createRotationProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createRotationProperty";
    QList<QVariant> rotationData;
    rotationData << "Rotation" << "";
    PropertiesItem *rotationItem = new PropertiesItem(rotationData,mSceneObject);
    mRootItem->appendChild(rotationItem);

    QList<QVariant> rotationDataX;
    rotationDataX << "X" << mSceneObject->getRotation().x;
    rotationItem->appendChild(new PropertiesItem(rotationDataX,mSceneObject));

    QList<QVariant> rotationDataY;
    rotationDataY << "Y" << mSceneObject->getRotation().y;
    rotationItem->appendChild(new PropertiesItem(rotationDataY,mSceneObject));

    QList<QVariant> rotationDataZ;
    rotationDataZ << "Z" << mSceneObject->getRotation().z;
    rotationItem->appendChild(new PropertiesItem(rotationDataZ,mSceneObject));
}

void SceneObjectPropertiesModel::createScaleProperty()
{

    qDebug() << "SceneObjectPropertiesModel: createScaleProperty";
    QList<QVariant> scaleData;
    scaleData << "Scale" << "";
    PropertiesItem *scaleItem = new PropertiesItem(scaleData,mSceneObject);
    mRootItem->appendChild(scaleItem);

    QList<QVariant> scaleDataX;
    scaleDataX << "X" << mSceneObject->getScale().x;
    scaleItem->appendChild(new PropertiesItem(scaleDataX,mSceneObject));

    QList<QVariant> scaleDataY;
    scaleDataY << "Y" << mSceneObject->getScale().y;
    scaleItem->appendChild(new PropertiesItem(scaleDataY,mSceneObject));

    QList<QVariant> scaleDataZ;
    scaleDataZ << "Z" << mSceneObject->getScale().z;
    scaleItem->appendChild(new PropertiesItem(scaleDataZ,mSceneObject));
}

void SceneObjectPropertiesModel::createTransformTypeProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createTransformTypeProperty";
    QList<QVariant> transformTypeData;
    transformTypeData << "Transform Type" << QString::fromStdString(mSceneObject->getTransformType());
    PropertiesItem *transformTypeItem = new PropertiesItem(transformTypeData,mSceneObject);
    mRootItem->appendChild(transformTypeItem);
}

void SceneObjectPropertiesModel::createHasFocusProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createHasFocusProperty";
    QList<QVariant> hasFocusData;
    hasFocusData << "Has Focus" << mSceneObject->hasFocus();
    PropertiesItem *hasFocusItem = new PropertiesItem(hasFocusData,mSceneObject);
    mRootItem->appendChild(hasFocusItem);
}

void SceneObjectPropertiesModel::createAssetInstancesProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createAssetInstancesProperty";
    QList<QVariant> assetDefData;
    assetDefData << "Asset Definitions" << QString::number(mSceneObject->getAssetDefUuidsToLoad().size());
    PropertiesItem *assetDefItem = new PropertiesItem(assetDefData,mSceneObject);
    mRootItem->appendChild(assetDefItem);

    for (std::string adUuid : mSceneObject->getAssetDefUuidsToLoad())
    {
       QList<QVariant> assetInstanceData;
       AssetDefinition* definition = mSceneObject->getSceneHandle()
                                                 ->getProjectHandle()
                                                 ->getAssetDefinitionByUuid(adUuid);
       assetInstanceData << QString::fromStdString(definition->getName()) << QString::fromStdString(definition->getType());
       assetDefItem->appendChild(new PropertiesItem(assetInstanceData,mSceneObject));
    }
}

void SceneObjectPropertiesModel::createChildrenProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createChildrenProperty";
    QList<QVariant> childrenData;
    childrenData << "Children" << QString::number(mSceneObject->getChildren().size());
    PropertiesItem *childrenItem = new PropertiesItem(childrenData,mSceneObject);
    mRootItem->appendChild(childrenItem);

    for (SceneObject* child : mSceneObject->getChildren())
    {
       QList<QVariant> childData;
       childData << QString::fromStdString(child->getName()) << QString::fromStdString(child->getUuid());
       childrenItem->appendChild(new PropertiesItem(childData,mSceneObject));
    }
}

void SceneObjectPropertiesModel::createDelegates()
{

}
