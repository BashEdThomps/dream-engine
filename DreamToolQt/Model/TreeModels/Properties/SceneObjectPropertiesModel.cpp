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
    mRootItem.reset(new PropertiesItem(rootData,mSceneObjectDefinitionHandle));
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
    nameData << "Name" << QString::fromStdString(mSceneObjectDefinitionHandle->getName());
    mRootItem->appendChild(new PropertiesItem(nameData,mSceneObjectDefinitionHandle));
}

void SceneObjectPropertiesModel::createTranslationProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createTranslationProperty";
    QList<QVariant> translationData;
    translationData << "Translation" << "";
    PropertiesItem *translationItem = new PropertiesItem(translationData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(translationItem);

    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();
    QList<QVariant> translationDataX;
    translationDataX << "X" << transform.getTranslation().x;
    translationItem->appendChild(new PropertiesItem(translationDataX,mSceneObjectDefinitionHandle));

    QList<QVariant> translationDataY;
    translationDataY << "Y" << transform.getTranslation().y;
    translationItem->appendChild(new PropertiesItem(translationDataY,mSceneObjectDefinitionHandle));

    QList<QVariant> translationDataZ;
    translationDataZ << "Z" << transform.getTranslation().z;
    translationItem->appendChild(new PropertiesItem(translationDataZ,mSceneObjectDefinitionHandle));
}

void SceneObjectPropertiesModel::createRotationProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createRotationProperty";
    QList<QVariant> rotationData;
    rotationData << "Rotation" << "";
    PropertiesItem *rotationItem = new PropertiesItem(rotationData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(rotationItem);

    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();

    QList<QVariant> rotationDataX;
    rotationDataX << "X" << transform.getRotation().x;
    rotationItem->appendChild(new PropertiesItem(rotationDataX,mSceneObjectDefinitionHandle));

    QList<QVariant> rotationDataY;
    rotationDataY << "Y" << transform.getRotation().y;
    rotationItem->appendChild(new PropertiesItem(rotationDataY,mSceneObjectDefinitionHandle));

    QList<QVariant> rotationDataZ;
    rotationDataZ << "Z" << transform.getRotation().z;
    rotationItem->appendChild(new PropertiesItem(rotationDataZ,mSceneObjectDefinitionHandle));
}

void SceneObjectPropertiesModel::createScaleProperty()
{

    qDebug() << "SceneObjectPropertiesModel: createScaleProperty";
    QList<QVariant> scaleData;
    scaleData << "Scale" << "";
    PropertiesItem *scaleItem = new PropertiesItem(scaleData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(scaleItem);

    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();

    QList<QVariant> scaleDataX;
    scaleDataX << "X" << transform.getScale().x;
    scaleItem->appendChild(new PropertiesItem(scaleDataX,mSceneObjectDefinitionHandle));

    QList<QVariant> scaleDataY;
    scaleDataY << "Y" << transform.getScale().y;
    scaleItem->appendChild(new PropertiesItem(scaleDataY,mSceneObjectDefinitionHandle));

    QList<QVariant> scaleDataZ;
    scaleDataZ << "Z" << transform.getScale().z;
    scaleItem->appendChild(new PropertiesItem(scaleDataZ,mSceneObjectDefinitionHandle));
}

void SceneObjectPropertiesModel::createTransformTypeProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createTransformTypeProperty";
    Transform3D transform = mSceneObjectDefinitionHandle->getTransform();
    QList<QVariant> transformTypeData;
    transformTypeData << "Transform Type" << QString::fromStdString(transform.getTransformType());
    PropertiesItem *transformTypeItem = new PropertiesItem(transformTypeData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(transformTypeItem);
}

void SceneObjectPropertiesModel::createHasFocusProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createHasFocusProperty";
    QList<QVariant> hasFocusData;
    hasFocusData << "Has Focus" << mSceneObjectDefinitionHandle->hasFocus();
    PropertiesItem *hasFocusItem = new PropertiesItem(hasFocusData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(hasFocusItem);
}

void SceneObjectPropertiesModel::createAssetInstancesProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createAssetInstancesProperty";
    QList<QVariant> assetDefData;
    vector<string> definitionsToLoad = mSceneObjectDefinitionHandle->getAssetDefinitionLoadQueue();
    assetDefData << "Asset Definitions" << QString::number(definitionsToLoad.size());
    PropertiesItem *assetDefItem = new PropertiesItem(assetDefData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(assetDefItem);

    for (std::string adUuid : definitionsToLoad)
    {
       QList<QVariant> assetInstanceData;
       AssetDefinition* definition = mSceneObjectDefinitionHandle->getSceneDefinitionHandle()
               ->getProjectDefinitionHandle()
               ->getAssetDefinitionHandleByUuid(adUuid);
       assetInstanceData << QString::fromStdString(definition->getName()) << QString::fromStdString(definition->getType());
       assetDefItem->appendChild(new PropertiesItem(assetInstanceData,mSceneObjectDefinitionHandle));
    }
}

void SceneObjectPropertiesModel::createChildrenProperty()
{
    qDebug() << "SceneObjectPropertiesModel: createChildrenProperty";
    QList<QVariant> childrenData;
    childrenData << "Children" << QString::number(mSceneObjectDefinitionHandle->getChildDefinitionsHandleList().size());
    PropertiesItem *childrenItem = new PropertiesItem(childrenData,mSceneObjectDefinitionHandle);
    mRootItem->appendChild(childrenItem);

    for (SceneObjectDefinition* child : mSceneObjectDefinitionHandle->getChildDefinitionsHandleList())
    {
       QList<QVariant> childData;
       childData << QString::fromStdString(child->getName()) << QString::fromStdString(child->getUuid());
       childrenItem->appendChild(new PropertiesItem(childData,mSceneObjectDefinitionHandle));
    }
}

void SceneObjectPropertiesModel::createDelegates()
{

}
