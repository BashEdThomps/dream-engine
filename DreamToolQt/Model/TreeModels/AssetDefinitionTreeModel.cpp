/*
 * AssetDefinitionTreeModel.cpp
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

#include "AssetDefinitionTreeModel.h"
#include <QStringList>

AssetDefinitionTreeModel::AssetDefinitionTreeModel
(ProjectDefinition* project, QObject *parent)
    : QAbstractItemModel(parent)
{
    mProject = project;

    setupModelData();
}

AssetDefinitionTreeModel::~AssetDefinitionTreeModel
()
{
    delete mRootItem;
}

int
AssetDefinitionTreeModel::columnCount
(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<AssetDefinitionTreeItem*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return mRootItem->columnCount();
    }
}

QVariant
AssetDefinitionTreeModel::data
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

    AssetDefinitionTreeItem *item = static_cast<AssetDefinitionTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags
AssetDefinitionTreeModel::flags
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }
    return QAbstractItemModel::flags(index);
}

QVariant
AssetDefinitionTreeModel::headerData
(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }
    return QVariant();
}

QModelIndex
AssetDefinitionTreeModel::index
(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GenericTreeItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<AssetDefinitionTreeItem*>(parent.internalPointer());
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
AssetDefinitionTreeModel::parent
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    GenericTreeItem *childItem = static_cast<GenericTreeItem*>(index.internalPointer());
    GenericTreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int
AssetDefinitionTreeModel::rowCount
(const QModelIndex &parent) const
{
    AssetDefinitionTreeItem *parentItem;

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
        parentItem = static_cast<AssetDefinitionTreeItem*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

void
AssetDefinitionTreeModel::setupModelData
()
{
    emit beginResetModel();

    // Create Root Node
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mProject->getName());
    mRootItem = new AssetDefinitionTreeItem(
        rootData,
        GenericTreeItemType::TREE_NODE,
        nullptr,
        nullptr
    );

    // Create Asset Type Nodes
    QList<QVariant> animationNode;
    animationNode << QString("Animation");
    AssetDefinitionTreeItem* animationTreeItem =
            new AssetDefinitionTreeItem(
                animationNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(animationTreeItem);

    QList<QVariant> audioNode;
    audioNode << QString("Audio");
    AssetDefinitionTreeItem* audioTreeItem =
            new AssetDefinitionTreeItem(
                audioNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(audioTreeItem);

    QList<QVariant> fontNode;
    fontNode << QString("Font");
    AssetDefinitionTreeItem* fontTreeItem =
            new AssetDefinitionTreeItem(
                fontNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(fontTreeItem);

    QList<QVariant> lightNode;
    lightNode << QString("Light");
    AssetDefinitionTreeItem* lightTreeItem =
            new AssetDefinitionTreeItem(
                lightNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(lightTreeItem);

    QList<QVariant> modelNode;
    modelNode << QString("Model");
    AssetDefinitionTreeItem* modelTreeItem =
            new AssetDefinitionTreeItem(
                modelNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(modelTreeItem);

    QList<QVariant> physicsObjectNode;
    physicsObjectNode << QString("Physics Object");
    AssetDefinitionTreeItem* physicsObjectTreeItem =
            new AssetDefinitionTreeItem(
                physicsObjectNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(physicsObjectTreeItem);

    QList<QVariant> scriptNode;
    scriptNode << QString("Script");
    AssetDefinitionTreeItem* scriptTreeItem =
            new AssetDefinitionTreeItem(
                scriptNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(scriptTreeItem);

    QList<QVariant> shaderNode;
    shaderNode << QString("Shader");
    AssetDefinitionTreeItem* shaderTreeItem =
            new AssetDefinitionTreeItem(
                shaderNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(shaderTreeItem);

    QList<QVariant> spriteNode;
    spriteNode << QString("Sprite");
    AssetDefinitionTreeItem* spriteTreeItem =
            new AssetDefinitionTreeItem(
                spriteNode,
                GenericTreeItemType::TREE_NODE,
                nullptr,
                mRootItem
            );
    mRootItem->appendChild(spriteTreeItem);

    for (AssetDefinition *definition : mProject->getAssetDefinitionsHandleList())
    {
        QList<QVariant> nextDefinitionData;
        nextDefinitionData << QString::fromStdString(definition->getName());

        if (definition->getType().compare(Constants::ASSET_TYPE_ANIMATION) == 0)
        {
            animationTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    animationTreeItem
                 )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_AUDIO) == 0)
        {
            audioTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    audioTreeItem
                 )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_FONT) == 0)
        {
           fontTreeItem->appendChild(
               new AssetDefinitionTreeItem(
               nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
               definition,
               fontTreeItem
               )
           );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_LIGHT) == 0)
        {
            lightTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    lightTreeItem
                 )
             );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_MODEL) == 0)
        {
            modelTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    modelTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
            physicsObjectTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    physicsObjectTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0)
        {
            scriptTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    scriptTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SHADER) == 0)
        {
            shaderTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    shaderTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SPRITE) == 0)
        {
            spriteTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    GenericTreeItemType::ASSET_DEFINITION,
                    definition,
                    spriteTreeItem
                )
            );
        }
        else
        {
            cerr << "AssetDefinitionTreeModel: Unable to add asset definition to tree with type "
                 << definition->getType() << endl;
        }

    }

    emit endResetModel();
}
