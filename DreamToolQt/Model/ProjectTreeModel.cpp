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

ProjectTreeModel::ProjectTreeModel(Dream::Project *project, QObject *parent)
    : QAbstractItemModel(parent)
{
    mProject = project;
    QList<QVariant> rootData;
    rootData << QString::fromStdString(project->getName());
    mRootItem = new ProjectTreeItem(rootData,ProjectItemType::PROJECT,nullptr);
    setupModelData(mRootItem);
}

ProjectTreeModel::~ProjectTreeModel()
{
    delete mRootItem;
}

int ProjectTreeModel::columnCount(const QModelIndex &parent) const
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

QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }
    return QAbstractItemModel::flags(index);
}

QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }
    return QVariant();
}

QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
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

    ProjectTreeItem *childItem = parentItem->child(row);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    ProjectTreeItem *childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    ProjectTreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectTreeModel::rowCount(const QModelIndex &parent) const
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

void ProjectTreeModel::setupModelData(ProjectTreeItem *parent)
{
    QList<QVariant> sceneNodeData;
    sceneNodeData << QString("Scenes");

    ProjectTreeItem *scenesNode = new ProjectTreeItem(sceneNodeData,ProjectItemType::TREE_NODE,nullptr,parent);
    parent->appendChild(scenesNode);

    for (Dream::Scene *scene : mProject->getSceneList())
    {
        QList<QVariant> nextSceneData;
        nextSceneData << QString::fromStdString(scene->getName());

        ProjectTreeItem *nextScene = new ProjectTreeItem(nextSceneData,ProjectItemType::SCENE,scene,scenesNode);
        scenesNode->appendChild(nextScene);

        // Setup SceneObjects
        Dream::SceneObject *rootSceneObject = scene->getRootSceneObject();
        QList<QVariant> rootSceneObjectData;
        rootSceneObjectData << QString::fromStdString(rootSceneObject->getName());
        ProjectTreeItem *rootSceneObjectItem;
        rootSceneObjectItem = new ProjectTreeItem(
            rootSceneObjectData,
            ProjectItemType::SCENE_OBJECT,
            rootSceneObject,
            nextScene
        );
        nextScene->appendChild(rootSceneObjectItem);
        appendSceneObjects(rootSceneObject,rootSceneObjectItem);
    }

    QList<QVariant> assetDefinitionsNodeData;
    assetDefinitionsNodeData << QString("Asset Definitions");
    ProjectTreeItem *assetDefinitionsTreeItem = new ProjectTreeItem(
        assetDefinitionsNodeData,
        ProjectItemType::TREE_NODE,
        nullptr,
        parent
    );

    parent->appendChild(assetDefinitionsTreeItem);

    // Create Asset Type Nodes
    QList<QVariant> animationNode;
    animationNode << QString("Animation");
    ProjectTreeItem* animationTreeItem =
            new ProjectTreeItem(animationNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(animationTreeItem);

    QList<QVariant> audioNode;
    audioNode << QString("Audio");
    ProjectTreeItem* audioTreeItem =
            new ProjectTreeItem(audioNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(audioTreeItem);

    QList<QVariant> fontNode;
    fontNode << QString("Font");
    ProjectTreeItem* fontTreeItem =
            new ProjectTreeItem(fontNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(fontTreeItem);

    QList<QVariant> lightNode;
    lightNode << QString("Light");
    ProjectTreeItem* lightTreeItem =
            new ProjectTreeItem(lightNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(lightTreeItem);

    QList<QVariant> modelNode;
    modelNode << QString("Model");
    ProjectTreeItem* modelTreeItem =
            new ProjectTreeItem(modelNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);;
    assetDefinitionsTreeItem->appendChild(modelTreeItem);

    QList<QVariant> physicsObjectNode;
    physicsObjectNode << QString("Physics Object");
    ProjectTreeItem* physicsObjectTreeItem =
            new ProjectTreeItem(physicsObjectNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(physicsObjectTreeItem);

    QList<QVariant> scriptNode;
    scriptNode << QString("Script");
    ProjectTreeItem* scriptTreeItem =
            new ProjectTreeItem(scriptNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(scriptTreeItem);

    QList<QVariant> shaderNode;
    shaderNode << QString("Shader");
    ProjectTreeItem* shaderTreeItem =
            new ProjectTreeItem(shaderNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(shaderTreeItem);

    QList<QVariant> spriteNode;
    spriteNode << QString("Sprite");
    ProjectTreeItem* spriteTreeItem =
            new ProjectTreeItem(spriteNode,ProjectItemType::TREE_NODE,nullptr,assetDefinitionsTreeItem);
    assetDefinitionsTreeItem->appendChild(spriteTreeItem);

    for (Dream::AssetDefinition *definition : mProject->getAssetDefinitions())
    {
        QList<QVariant> nextDefinitionData;
        nextDefinitionData << QString::fromStdString(definition->getName());

        if (definition->getType().compare(ASSET_TYPE_ANIMATION) == 0)
        {
            animationTreeItem->appendChild(
                new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    animationTreeItem
                 )
            );
        }
        else if (definition->getType().compare(ASSET_TYPE_AUDIO) == 0)
        {
            audioTreeItem->appendChild(
                new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    audioTreeItem
                 )
            );
        }
        else if (definition->getType().compare(ASSET_TYPE_FONT) == 0)
        {
           fontTreeItem->appendChild(
               new ProjectTreeItem(
               nextDefinitionData,
               ProjectItemType::ASSET_DEFINITION,
               definition,
               fontTreeItem
               )
           );
        }
        else if (definition->getType().compare(ASSET_TYPE_LIGHT) == 0)
        {
            lightTreeItem->appendChild(
                new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    lightTreeItem
                 )
             );
        }
        else if (definition->getType().compare(ASSET_TYPE_MODEL) == 0)
        {
            modelTreeItem->appendChild(
                        new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    modelTreeItem
                 )
                        );
        }
        else if (definition->getType().compare(ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
                physicsObjectTreeItem->appendChild(
                         new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    physicsObjectTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SCRIPT) == 0)
        {
                scriptTreeItem->appendChild(
                            new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    scriptTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SHADER) == 0)
        {
                shaderTreeItem->appendChild(
                            new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    shaderTreeItem
                 )
                            );
        }
        else if (definition->getType().compare(ASSET_TYPE_SPRITE) == 0)
        {
                spriteTreeItem->appendChild(
                            new ProjectTreeItem(
                    nextDefinitionData,
                    ProjectItemType::ASSET_DEFINITION,
                    definition,
                    spriteTreeItem
                            ));
        }
        else {
            cerr << "ProjectTreeModel: Unable to add asset definition to tree with type "
                 << definition->getType() << endl;
        }

    }
}

void ProjectTreeModel::appendSceneObjects(Dream::SceneObject *parentSceneObject, ProjectTreeItem* parentTreeNode)
{
    for (Dream::SceneObject *sceneObject : parentSceneObject->getChildren())
    {
        // Setup Child
        QList<QVariant> sceneObjectData;
        sceneObjectData << QString::fromStdString(sceneObject->getName());
        ProjectTreeItem *sceneObjectItem = new ProjectTreeItem(
            sceneObjectData,
            ProjectItemType::SCENE_OBJECT,
            sceneObject,
            parentTreeNode
        );
        appendSceneObjects(sceneObject,sceneObjectItem);
        parentTreeNode->appendChild(sceneObjectItem);
    }
}
