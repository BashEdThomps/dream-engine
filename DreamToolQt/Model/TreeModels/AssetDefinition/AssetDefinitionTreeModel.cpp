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
#include <QDebug>
#include <QIcon>

using Dream::Constants;
using Dream::IAssetDefinition;

AssetDefinitionTreeModel::AssetDefinitionTreeModel
(ProjectDefinition* project, QObject *parent)
    : QAbstractItemModel(parent)
{
    qDebug() << "AssetDefinitionTreeModel: Constructing";
    mProjectHandle = project;
    setupModelData();
    mAssetDefinitionIcon = unique_ptr<QIcon>(new QIcon(":svg/noun_Object.svg"));
}

AssetDefinitionTreeModel::~AssetDefinitionTreeModel
()
{
    qDebug() << "AssetDefinitionTreeModel: Destructing";
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

    AssetDefinitionTreeItem *item = static_cast<AssetDefinitionTreeItem*>(index.internalPointer());

    if (role == Qt::DecorationRole )
    {
        switch (item->getType())
        {
            case ASSET_DEFINITION:
                return QVariant(*(mAssetDefinitionIcon.get()));
            case ASSET_TREE_NODE:
                return QVariant();
        }
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }


    return item->data(index.column());
}

Qt::ItemFlags
AssetDefinitionTreeModel::flags
(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return nullptr;
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

    AssetDefinitionTreeItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem.get();
    }
    else
    {
        parentItem = static_cast<AssetDefinitionTreeItem*>(parent.internalPointer());
    }

    AssetDefinitionTreeItem *childItem = static_cast<AssetDefinitionTreeItem*>(parentItem->child(row));

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

    AssetDefinitionTreeItem *childItem = static_cast<AssetDefinitionTreeItem*>(index.internalPointer());
    AssetDefinitionTreeItem *parentItem = nullptr;

    if (childItem)
    {
        parentItem = static_cast<AssetDefinitionTreeItem*>(childItem->parentItem());
    }

    if (parentItem == mRootItem.get())
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
        parentItem = mRootItem.get();
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
    mRootItem.reset
    (
        new AssetDefinitionTreeItem
        (
            QString::fromStdString(mProjectHandle->getName()),
            AssetDefinitionTreeItemType::ASSET_TREE_NODE,
            nullptr,
            nullptr
        )
    );

    // Create Asset Type Nodes
    AssetDefinitionTreeItem* animationTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Animation"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(animationTreeItem);

    AssetDefinitionTreeItem* audioTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Audio"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(audioTreeItem);

    AssetDefinitionTreeItem* fontTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Font"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(fontTreeItem);

    AssetDefinitionTreeItem* lightTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Light"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(lightTreeItem);

    AssetDefinitionTreeItem* modelTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Model"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(modelTreeItem);

    AssetDefinitionTreeItem* physicsObjectTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Physics Object"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(physicsObjectTreeItem);

    AssetDefinitionTreeItem* scriptTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Script"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(scriptTreeItem);

    AssetDefinitionTreeItem* shaderTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Shader"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(shaderTreeItem);

    AssetDefinitionTreeItem* spriteTreeItem =
    new AssetDefinitionTreeItem
    (
        QString("Sprite"),
        AssetDefinitionTreeItemType::ASSET_TREE_NODE,
        nullptr,
        mRootItem.get()
    );
    mRootItem->appendChild(spriteTreeItem);

    for (IAssetDefinition* definition : mProjectHandle->getAssetDefinitionsHandleList())
    {
        if (definition->getType().compare(Constants::ASSET_TYPE_ANIMATION) == 0)
        {
            animationTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    animationTreeItem
                 )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_AUDIO) == 0)
        {
            audioTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    audioTreeItem
                 )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_FONT) == 0)
        {
           fontTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    fontTreeItem
               )
           );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_LIGHT) == 0)
        {
            lightTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    lightTreeItem
                 )
             );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_MODEL) == 0)
        {
            modelTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    modelTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
            physicsObjectTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    physicsObjectTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0)
        {
            scriptTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    scriptTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SHADER) == 0)
        {
            shaderTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
                    definition,
                    shaderTreeItem
                )
            );
        }
        else if (definition->getType().compare(Constants::ASSET_TYPE_SPRITE) == 0)
        {
            spriteTreeItem->appendChild
            (
                new AssetDefinitionTreeItem
                (
                    QString::fromStdString(definition->getName()),
                    AssetDefinitionTreeItemType::ASSET_DEFINITION,
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
