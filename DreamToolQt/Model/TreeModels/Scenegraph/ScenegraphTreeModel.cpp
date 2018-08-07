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
#include <spdlog/spdlog.h>
#include <memory>

using Dream::SceneDefinition;

ScenegraphTreeModel::ScenegraphTreeModel
(ProjectDefinition *project, QObject *parent)
    : QAbstractItemModel(parent)
{
    auto log = spdlog::get("ScenegraphTreeModel");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("ScenegraphTreeModel");
    }

    mProjectDefinitionHandle = project;

    log->info("Constructing for {}",mProjectDefinitionHandle->getNameAndUuidString());

    mProjectIcon = unique_ptr<QIcon>(new QIcon(":svg/noun_boxes.svg"));
    mSceneIcon = unique_ptr<QIcon>(new QIcon(":svg/noun_clapper.svg"));
    mSceneObjectIcon = unique_ptr<QIcon>(new QIcon(":svg/noun_Bowling.svg"));
    setupModelData();
}

ScenegraphTreeModel::~ScenegraphTreeModel
()
{
    auto log = spdlog::get("ScenegraphTreeModel");

    log->info("Destructing");
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

    if (role == Qt::DecorationRole )
    {
        auto data = static_cast<ScenegraphTreeItem*>(index.internalPointer());
        switch (data->getType())
        {
            case SCENEGRAPH_PROJECT:
                return QVariant(*mProjectIcon.get());
            case SCENEGRAPH_SCENE:
                return QVariant(*mSceneIcon.get());
            case SCENEGRAPH_SCENE_OBJECT:
                return QVariant(*mSceneObjectIcon.get());
            case SCENEGRAPH_TREE_NODE:
                return QVariant();

        }
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

    ScenegraphTreeItem *childItem = static_cast<ScenegraphTreeItem*>(parentItem->child(row));

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
    ScenegraphTreeItem *parentItem = static_cast<ScenegraphTreeItem*>(childItem->parentItem());

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
    auto log = spdlog::get("ScenegraphTreeModel");
    emit beginResetModel();
    log->info("Setting up project {}",mProjectDefinitionHandle->getNameAndUuidString());

    mRootItem.reset
    (
        new ScenegraphTreeItem
        (
            QString::fromStdString(mProjectDefinitionHandle->getName()),
            ScenegraphTreeItemType::SCENEGRAPH_TREE_NODE,
            mProjectDefinitionHandle
        )
    );

    auto project = new ScenegraphTreeItem
    (
        QString::fromStdString(mProjectDefinitionHandle->getName()),
        ScenegraphTreeItemType::SCENEGRAPH_PROJECT,
        mProjectDefinitionHandle,
        mRootItem.get()
    );

    mRootItem->appendChild(project);

    for (SceneDefinition *sceneHandle : mProjectDefinitionHandle->getSceneDefinitionsHandleList())
    {
        log->info("Adding Scene {}",sceneHandle->getNameAndUuidString());

        ScenegraphTreeItem *nextScene = new ScenegraphTreeItem
        (
            QString::fromStdString(sceneHandle->getName()),
            ScenegraphTreeItemType::SCENEGRAPH_SCENE,
            sceneHandle,
            project
        );
        project->appendChild(nextScene);

        // Setup SceneObjects
        SceneObjectDefinition *rootSceneObject = sceneHandle->getRootSceneObjectDefinitionHandle();
        ScenegraphTreeItem *rootSceneObjectItem = new ScenegraphTreeItem
        (
            QString::fromStdString(rootSceneObject->getName()),
            ScenegraphTreeItemType::SCENEGRAPH_SCENE_OBJECT,
            rootSceneObject,
            nextScene
        );

        nextScene->appendChild(rootSceneObjectItem);
        appendSceneObjects(rootSceneObject,rootSceneObjectItem);
    }
    emit endResetModel();
}

void ScenegraphTreeModel::forceDataChanged()
{
   setupModelData();
   emit dataChanged(QModelIndex(),QModelIndex());
}

void
ScenegraphTreeModel::appendSceneObjects
(SceneObjectDefinition *parentSceneObject, ScenegraphTreeItem* parentTreeNode)
{
    for (SceneObjectDefinition *sceneObject : parentSceneObject->getChildDefinitionsHandleList())
    {
        // Setup Child
        ScenegraphTreeItem *sceneObjectItem = new ScenegraphTreeItem
        (
            QString::fromStdString(sceneObject->getName()),
            ScenegraphTreeItemType::SCENEGRAPH_SCENE_OBJECT,
            sceneObject,
            parentTreeNode
        );
        appendSceneObjects(sceneObject,sceneObjectItem);
        parentTreeNode->appendChild(sceneObjectItem);
    }
}
