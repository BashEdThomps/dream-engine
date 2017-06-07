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

#include "ProjectTreeModel.h"
#include <QStringList>

ProjectTreeModel::ProjectTreeModel
(Project *project, QObject *parent)
    : QAbstractItemModel(parent)
{
    mProject = project;
    QList<QVariant> rootData;
    rootData << QString::fromStdString(project->getName());
    mRootItem = new ProjectTreeItem(rootData,GenericTreeItemType::PROJECT,nullptr);
    setupModelData(mRootItem);
}

ProjectTreeModel::~ProjectTreeModel
()
{
    delete mRootItem;
}

int
ProjectTreeModel::columnCount
(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<ProjectTreeItem*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return mRootItem->columnCount();
    }
}

QVariant
ProjectTreeModel::data
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

    ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags
ProjectTreeModel::flags
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }
    return QAbstractItemModel::flags(index);
}

QVariant
ProjectTreeModel::headerData
(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }
    return QVariant();
}

QModelIndex
ProjectTreeModel::index
(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    ProjectTreeItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());
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
ProjectTreeModel::parent
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    ProjectTreeItem *childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    GenericTreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int
ProjectTreeModel::rowCount
(const QModelIndex &parent) const
{
    ProjectTreeItem *parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

void
ProjectTreeModel::setupModelData
(ProjectTreeItem *parent)
{
    QList<QVariant> sceneNodeData;
    sceneNodeData << QString("Scenes");

    ProjectTreeItem *scenesNode = new ProjectTreeItem(sceneNodeData,GenericTreeItemType::TREE_NODE,nullptr,parent);
    parent->appendChild(scenesNode);

    for (Scene *scene : mProject->getSceneList())
    {
        QList<QVariant> nextSceneData;
        nextSceneData << QString::fromStdString(scene->getName());

        ProjectTreeItem *nextScene = new ProjectTreeItem(nextSceneData,GenericTreeItemType::SCENE,scene,scenesNode);
        scenesNode->appendChild(nextScene);

        // Setup SceneObjects
        SceneObject *rootSceneObject = scene->getRootSceneObject();
        QList<QVariant> rootSceneObjectData;
        rootSceneObjectData << QString::fromStdString(rootSceneObject->getName());
        ProjectTreeItem *rootSceneObjectItem;
        rootSceneObjectItem = new ProjectTreeItem(
            rootSceneObjectData,
            GenericTreeItemType::SCENE_OBJECT,
            rootSceneObject,
            nextScene
        );
        nextScene->appendChild(rootSceneObjectItem);
        appendSceneObjects(rootSceneObject,rootSceneObjectItem);
    }
}

void
ProjectTreeModel::appendSceneObjects
(SceneObject *parentSceneObject, ProjectTreeItem* parentTreeNode)
{
    for (SceneObject *sceneObject : parentSceneObject->getChildren())
    {
        // Setup Child
        QList<QVariant> sceneObjectData;
        sceneObjectData << QString::fromStdString(sceneObject->getName());
        ProjectTreeItem *sceneObjectItem = new ProjectTreeItem(
            sceneObjectData,
            GenericTreeItemType::SCENE_OBJECT,
            sceneObject,
            parentTreeNode
        );
        appendSceneObjects(sceneObject,sceneObjectItem);
        parentTreeNode->appendChild(sceneObjectItem);
    }
}
