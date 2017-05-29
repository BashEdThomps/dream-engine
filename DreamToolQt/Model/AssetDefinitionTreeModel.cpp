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

AssetDefinitionTreeModel::AssetDefinitionTreeModel(Dream::Project *project, QObject *parent)
    : QAbstractItemModel(parent)
{
    mProject = project;
    QList<QVariant> rootData;
    rootData << QString::fromStdString(project->getName());
    mRootItem = new AssetDefinitionTreeItem(rootData,nullptr,nullptr);
    setupModelData();
}

AssetDefinitionTreeModel::~AssetDefinitionTreeModel()
{
    delete mRootItem;
}

int AssetDefinitionTreeModel::columnCount(const QModelIndex &parent) const
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

QVariant AssetDefinitionTreeModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags AssetDefinitionTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }
    return QAbstractItemModel::flags(index);
}

QVariant AssetDefinitionTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }
    return QVariant();
}

QModelIndex AssetDefinitionTreeModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex AssetDefinitionTreeModel::parent(const QModelIndex &index) const
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

int AssetDefinitionTreeModel::rowCount(const QModelIndex &parent) const
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

void AssetDefinitionTreeModel::setupModelData()
{
    QList<QVariant> assetDefinitionsNodeData;
    assetDefinitionsNodeData << QString("Asset Definitions");
    AssetDefinitionTreeItem *assetDefinitionsTreeItem = new AssetDefinitionTreeItem(
        assetDefinitionsNodeData,
        nullptr,
        mRootItem
    );

    mRootItem->appendChild(assetDefinitionsTreeItem);

    // Create Asset Type Nodes
    QList<QVariant> animationNode;
    animationNode << QString("Animation");
    AssetDefinitionTreeItem* animationTreeItem =
            new AssetDefinitionTreeItem(animationNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(animationTreeItem);

    QList<QVariant> audioNode;
    audioNode << QString("Audio");
    AssetDefinitionTreeItem* audioTreeItem =
            new AssetDefinitionTreeItem(audioNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(audioTreeItem);

    QList<QVariant> fontNode;
    fontNode << QString("Font");
    AssetDefinitionTreeItem* fontTreeItem =
            new AssetDefinitionTreeItem(fontNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(fontTreeItem);

    QList<QVariant> lightNode;
    lightNode << QString("Light");
    AssetDefinitionTreeItem* lightTreeItem =
            new AssetDefinitionTreeItem(lightNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(lightTreeItem);

    QList<QVariant> modelNode;
    modelNode << QString("Model");
    AssetDefinitionTreeItem* modelTreeItem =
            new AssetDefinitionTreeItem(modelNode,nullptr,assetDefinitionsTreeItem);;
    assetDefinitionsTreeItem->appendChild(modelTreeItem);

    QList<QVariant> physicsObjectNode;
    physicsObjectNode << QString("Physics Object");
    AssetDefinitionTreeItem* physicsObjectTreeItem =
            new AssetDefinitionTreeItem(physicsObjectNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(physicsObjectTreeItem);

    QList<QVariant> scriptNode;
    scriptNode << QString("Script");
    AssetDefinitionTreeItem* scriptTreeItem =
            new AssetDefinitionTreeItem(scriptNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(scriptTreeItem);

    QList<QVariant> shaderNode;
    shaderNode << QString("Shader");
    AssetDefinitionTreeItem* shaderTreeItem =
            new AssetDefinitionTreeItem(shaderNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(shaderTreeItem);

    QList<QVariant> spriteNode;
    spriteNode << QString("Sprite");
    AssetDefinitionTreeItem* spriteTreeItem =
            new AssetDefinitionTreeItem(spriteNode,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(spriteTreeItem);

    for (Dream::AssetDefinition *definition : mProject->getAssetDefinitions())
    {
        QList<QVariant> nextDefinitionData;
        nextDefinitionData << QString::fromStdString(definition->getName());

        if (definition->getType().compare(ASSET_TYPE_ANIMATION) == 0)
        {
            animationTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    animationTreeItem
                 )
            );
        }
        else if (definition->getType().compare(ASSET_TYPE_AUDIO) == 0)
        {
            audioTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    audioTreeItem
                 )
            );
        }
        else if (definition->getType().compare(ASSET_TYPE_FONT) == 0)
        {
           fontTreeItem->appendChild(
               new AssetDefinitionTreeItem(
               nextDefinitionData,
               definition,
               fontTreeItem
               )
           );
        }
        else if (definition->getType().compare(ASSET_TYPE_LIGHT) == 0)
        {
            lightTreeItem->appendChild(
                new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    lightTreeItem
                 )
             );
        }
        else if (definition->getType().compare(ASSET_TYPE_MODEL) == 0)
        {
            modelTreeItem->appendChild(
                        new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    modelTreeItem
                 )
                        );
        }
        else if (definition->getType().compare(ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
                physicsObjectTreeItem->appendChild(
                         new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    physicsObjectTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SCRIPT) == 0)
        {
                scriptTreeItem->appendChild(
                            new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    scriptTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SHADER) == 0)
        {
                shaderTreeItem->appendChild(
                            new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    shaderTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SPRITE) == 0)
        {
                spriteTreeItem->appendChild(
                            new AssetDefinitionTreeItem(
                    nextDefinitionData,
                    definition,
                    spriteTreeItem
                            ));
        }
        else {
            cerr << "AssetDefinitionTreeModel: Unable to add asset definition to tree with type "
                 << definition->getType() << endl;
        }

    }
}
