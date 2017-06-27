/*
 * ProjectTreeModel.cpp
 *
 * Created: 15 2017 by Ashley
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

#include "ScenegraphTreeModel.h"
#include <QStringList>
#include <QDebug>

using Dream::SceneDefinition;

ScenegraphTreeModel::ScenegraphTreeModel
(ProjectDefinition *project, QObject *parent)
    : QAbstractItemModel(parent)
{

    mProjectDefinitionHandle = project;

    qDebug() << "ScenegraphTreeModel: Constructing for "
             << QString::fromStdString(mProjectDefinitionHandle->getNameAndUuidString());

    setupModelData();
}

ScenegraphTreeModel::~ScenegraphTreeModel
()
{
    qDebug() << "ScenegraphTreeModel: Destructing";
    if (mRootItem)
    {
        mRootItem.release();
    }
}

int
ScenegraphTreeModel::columnCount
(const QModelIndex &parent) const
{
    return 1;
}

QVariant
ScenegraphTreeModel::data
(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    ScenegraphTreeItem *item = static_cast<ScenegraphTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags
ScenegraphTreeModel::flags
(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant
ScenegraphTreeModel::headerData
(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QVariant(QString::fromStdString(mProjectDefinitionHandle->getName()));
    }
    return QVariant();
}

QModelIndex
ScenegraphTreeModel::index
(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ScenegraphTreeItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem.get();
    }
    else
    {
        parentItem = static_cast<ScenegraphTreeItem*>(parent.internalPointer());
    }

    GenericTreeItem *childItem = parentItem->child(row);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex
ScenegraphTreeModel::parent
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    ScenegraphTreeItem *childItem = static_cast<ScenegraphTreeItem*>(index.internalPointer());
    GenericTreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem.get())
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int
ScenegraphTreeModel::rowCount
(const QModelIndex &parent) const
{
    ScenegraphTreeItem *parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = mRootItem.get();
    }
    else
    {
        parentItem = static_cast<ScenegraphTreeItem*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

void
ScenegraphTreeModel::setupModelData
()
{
    emit beginResetModel();
    qDebug() << "ScenegraphTreeModel: Setting up project "
             << QString::fromStdString(mProjectDefinitionHandle->getNameAndUuidString());

    QList<QVariant> rootData;
    rootData << QString::fromStdString(mProjectDefinitionHandle->getName());
    mRootItem.reset(new ScenegraphTreeItem(rootData,GenericTreeItemType::PROJECT,nullptr));

    for (SceneDefinition *sceneHandle : mProjectDefinitionHandle->getSceneDefinitionsHandleList())
    {
        qDebug() << "ScenegraphTreeModel: Adding Scene "
                 << QString::fromStdString(sceneHandle->getNameAndUuidString());

        QList<QVariant> nextSceneData;
        nextSceneData << QString::fromStdString(sceneHandle->getName());

        ScenegraphTreeItem *nextScene = new ScenegraphTreeItem(
            nextSceneData,GenericTreeItemType::SCENE,sceneHandle,mRootItem.get()
        );
        mRootItem->appendChild(nextScene);

        // Setup SceneObjects
        SceneObjectDefinition *rootSceneObject = sceneHandle->getRootSceneObjectDefinitionHandle();
        QList<QVariant> rootSceneObjectData;
        rootSceneObjectData << QString::fromStdString(rootSceneObject->getName());
        ScenegraphTreeItem *rootSceneObjectItem;

        rootSceneObjectItem = new ScenegraphTreeItem(
            rootSceneObjectData,
            GenericTreeItemType::SCENE_OBJECT,
            rootSceneObject,
            nextScene
        );

        nextScene->appendChild(rootSceneObjectItem);
        appendSceneObjects(rootSceneObject,rootSceneObjectItem);
    }
    emit endResetModel();
}

void
ScenegraphTreeModel::appendSceneObjects
(SceneObjectDefinition *parentSceneObject, ScenegraphTreeItem* parentTreeNode)
{
    for (SceneObjectDefinition *sceneObject : parentSceneObject->getChildDefinitionsHandleList())
    {
        // Setup Child
        QList<QVariant> sceneObjectData;
        sceneObjectData << QString::fromStdString(sceneObject->getName());
        ScenegraphTreeItem *sceneObjectItem = new ScenegraphTreeItem(
            sceneObjectData,
            GenericTreeItemType::SCENE_OBJECT,
            sceneObject,
            parentTreeNode
        );
        appendSceneObjects(sceneObject,sceneObjectItem);
        parentTreeNode->appendChild(sceneObjectItem);
    }
}
