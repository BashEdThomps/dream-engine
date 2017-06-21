/*
 * MainController.cpp
 *
 * Created: 14 2017 by Ashley
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
#include "MainController.h"

#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>
#include <QModelIndexList>
#include <DreamCore.h>
#include <QOpenGLContext>

#include "../Model/TreeModels/Properties/AssetDefinitionPropertiesModel.h"
#include "../Model/TreeModels/Properties/PropertiesModel.h"
#include "../Model/TreeModels/Properties/ProjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/SceneObjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/ScenePropertiesModel.h"
#include "../Model/TreeModels/Properties/AssetDefinitionTypeComboDelegate.h"

MainController::MainController
(MainWindow* parent)
    : QObject(parent)
{
    qDebug() << "MainController: Constructing Object";
    mMainWindowHandle = parent;
    mWindowComponentHandle = parent->getWindowComponent();
    mDreamModel.reset(new DreamProjectModel(this,mWindowComponentHandle));

    mGrid.reset(new Grid(this));
    mWindowComponentHandle->setGrid(getGrid());

    mSelectionHighlighter.reset(new SelectionHighlighter(this));
    mWindowComponentHandle->setSelectionHighlighter(getSelectionHighlighter());

    mRelationshipTree.reset(new RelationshipTree(this));
    mWindowComponentHandle->setRelationshipTree(getRelationshipTree());

    createConnections();
}

MainController::~MainController
()
{
    qDebug() << "MainController: Destroying Object";
}

void
MainController::createConnections
()
{
    // actionNew
    connect
    (
        mMainWindowHandle->getActionNew(), SIGNAL(triggered()),
        this, SLOT(onProjectNewAction())
    );
    // actionOpen
    connect
    (
        mMainWindowHandle->getActionOpen(), SIGNAL(triggered()),
        this, SLOT(onProjectOpenAction())
    );
    // actionSave
    connect
    (
        mMainWindowHandle->getActionSave(), SIGNAL(triggered()),
        this, SLOT(onProjectSaveAction())
    );
    // actionReload
    connect
    (
        mMainWindowHandle->getActionReload(), SIGNAL(triggered()),
        this, SLOT(onProjectReloadAction())
    );
    // actionPlay
    connect
    (
        mMainWindowHandle->getActionPlay(), SIGNAL(triggered()),
        this, SLOT(onProjectPlayAction())
    );
    // actionStop
    connect
    (
        mMainWindowHandle->getActionStop(), SIGNAL(triggered()),
        this, SLOT(onProjectStopAction())
    );
    // actionCloseProject
    connect
    (
        mMainWindowHandle->getActionCloseProject(), SIGNAL(triggered()),
        this, SLOT(onProjectClosedAction())
    );
    // Action Toggle Grid
    connect
    (
        mMainWindowHandle->getActionToggleGrid(),SIGNAL(triggered(bool)),
        this,SLOT(onGridToggleAction(bool))
    );
    // Action Toggle Debug
    connect
    (
        mMainWindowHandle->getActionToggleDebug(),SIGNAL(triggered(bool)),
        this,SLOT(onToggleDebugAction(bool))
    );
    // Scene Stopped
    connect
    (
        this,SIGNAL(notifyStoppedScene(SceneDefinition*)),
        mMainWindowHandle,SLOT(onSceneStopped(SceneDefinition*))
    );
    // Open Default Project
    connect
    (
        mMainWindowHandle->getActionOpenTestProject(), SIGNAL(triggered()),
        this, SLOT(onProjectOpenTestProjectAction())
    );
    // Invalid Project Directory
    connect
    (
        this, SIGNAL(notifyInvalidProjectDirectory(QString)),
        mMainWindowHandle, SLOT(onInvalidProjectDirectory(QString))
    );
    // No Scene Selected
    connect
    (
        this,SIGNAL(notifyNoSceneSelected()),
        mMainWindowHandle, SLOT(onNoSceneSelected())
    );
    // Valid Scene Selected
    connect
    (
        mDreamModel.get(), SIGNAL(notifySelectedSceneChanged(SceneDefinition*)),
        this, SLOT(onSelectedSceneChanged(SceneDefinition*))
    );
    // Project Directory Changed
    connect
    (
        this, SIGNAL(notifyProjectDirectoryChanged(QString)),
        mMainWindowHandle, SLOT(setWindowTitle(QString))
     );
    // Status Bar
    connect
    (
        this, SIGNAL(notifyStatusBarProjectLoaded(QString)),
        mMainWindowHandle, SLOT(showStatusBarMessage(QString))
    );

}

void
MainController::onProjectNewAction
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);

    if(openDialog.exec())
    {
    }
}

void
MainController::updateWindowTitle
(QString msg)
{
    emit notifyProjectDirectoryChanged("Dream Tool :: " + msg);
}

void
MainController::onProjectOpenAction
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();
    }

    if (mProjectDirectory.size() == 0)
    {
        return;
    }

    bool loadResult = mDreamModel->loadProject(mProjectDirectory);
    qDebug() << "MainController: LoadResult " << loadResult;

    if (!loadResult)
    {
        emit notifyInvalidProjectDirectory(mProjectDirectory);
        emit notifyProjectWidgetsEnabledChanged(false);
        return;
    }
    updateWindowTitle(mProjectDirectory);
    Project *project = mDreamModel->getProject();
    ProjectDefinition* pdHandle = project->getProjectDefinitionHandle();

    if (pdHandle)
    {
        emit notifyProjectNameChanged(QString::fromStdString(pdHandle->getName()));
        emit notifyProjectAuthorChanged(QString::fromStdString(pdHandle->getAuthor()));
        emit notifyProjectDescriptionChanged(QString::fromStdString(pdHandle->getDescription()));
        emit notifyProjectWindowWidthChanged(pdHandle->getWindowWidth());
        emit notifyProjectWindowHeightChanged(pdHandle->getWindowHeight());
        emit notifyProjectSceneListChanged(getSceneNamesListModel(pdHandle->getSceneDefinitionsHandleList()));
        emit notifyProjectStartupSceneChanged(QString::fromStdString(pdHandle->getName()));

        emit notifyProjectWidgetsEnabledChanged(true);

        mProjectTreeModel.reset(
            new ProjectTreeModel(pdHandle,mMainWindowHandle->getProjectTreeView())
        );
        mMainWindowHandle->getProjectTreeView()->setModel(mProjectTreeModel.get());

        mAssetDefinitionTreeModel.reset
        (
            new AssetDefinitionTreeModel
            (
                pdHandle,mMainWindowHandle->getAssetDefinitionTreeView()
            )
        );
        mMainWindowHandle->getAssetDefinitionTreeView()->setModel(mAssetDefinitionTreeModel.get());

        emit notifyStatusBarProjectLoaded(QString::fromStdString(pdHandle->getNameAndUuidString()));
        connectTreeViewModel();
    }
}

void
MainController::connectTreeViewModel
()
{
    // projectTreeView
    connect
    (
        mMainWindowHandle->getProjectTreeView()->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this, SLOT(onTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    // assetDefinitionTreeView
    connect
    (
        mMainWindowHandle->getAssetDefinitionTreeView()->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this, SLOT(onTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    mMainWindowHandle->getProjectTreeView()->expandAll();
    mMainWindowHandle->getAssetDefinitionTreeView()->expandAll();
}

void
MainController::onProjectSaveAction
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);

    if(openDialog.exec())
    {
    }
}

QStringListModel*
MainController::getSceneNamesListModel
(vector<SceneDefinition*> sceneList)
{
    QStringList sceneNameList;
    mSceneListModel.reset(new QStringListModel(mMainWindowHandle));

    for (auto scene : sceneList)
    {
        sceneNameList.push_back(QString::fromStdString(scene->getName()));
    }

    mSceneListModel->setStringList(sceneNameList);
    return mSceneListModel.get();
}

void
MainController::onProjectNameChanged
(QString name)
{
    qDebug() << "MainController: Name set to " << name;
    mDreamModel->setProjectName(name.toStdString());
}

void
MainController::onProjectAuthorChanged
(QString author)
{
    qDebug() << "MainController: Author set to " << author;
    mDreamModel->setProjectAuthor(author.toStdString());
}

void
MainController::onProjectDescriptionChanged
(QString desc)
{
    qDebug() << "MainController: Description set to " << desc;
    mDreamModel->setProjectDescription(desc.toStdString());
}

void
MainController::onProjectWindowWidthChanged
(QString width)
{
    qDebug() << "MainController: Window Width set to " << width;
    mDreamModel->setProjectWindowWidth(width.toInt());
}

void
MainController::onProjectWindowHeightChanged
(QString height)
{
    qDebug() << "MainController: Window Height set to " << height;
    mDreamModel->setProjectWindowHeight(height.toInt());
}

void
MainController::onProjectStartupSceneChanged
(QString startupSceneIndex)
{
    string sceneName = getSceneNameFromModelIndex(startupSceneIndex.toInt());
    qDebug() << "MainController: startupScene set to " << startupSceneIndex << " " << QString::fromStdString(sceneName);
    SceneDefinition* sceneHandle = mDreamModel->getProject()
        ->getProjectDefinitionHandle()
        ->getSceneDefinitionHandleByUuid(sceneName);
    mDreamModel->setProjectStartupSceneByUuid(sceneHandle->getUuid());
}

void
MainController::onProjectPlayAction
()
{
    qDebug() << "MainController: onReloadProject Clicked";
    SceneDefinition *sceneDefinitionHandle = mDreamModel->getSelectedSceneDefinitionHandle();
    if (sceneDefinitionHandle)
    {
        mDreamModel->startSceneRuntimeFromDefinition(sceneDefinitionHandle);
    }
    else
    {
        emit notifyNoSceneSelected();
    }
}

void
MainController::onProjectReloadAction
()
{
   onProjectStopAction();
   onProjectPlayAction();
}

void
MainController::onProjectStopAction
()
{
    SceneRuntime* SceneRuntime = mDreamModel->stopActiveSceneRuntime();
}

void
MainController::onProjectOpenTestProjectAction
()
{
    mProjectDirectory = "/Users/Ashley/.dreamtool/de60-75ff-5cb7-c4a9";

    bool loadResult = mDreamModel->loadProject(mProjectDirectory);
    qDebug() << "MainController: Load Test Project Result " << loadResult;
    if (!loadResult)
    {
        emit notifyInvalidProjectDirectory(mProjectDirectory);
        emit notifyProjectWidgetsEnabledChanged(false);
        return;
    }
    updateWindowTitle(mProjectDirectory);
    ProjectDefinition *currentProject = mDreamModel->getProject()->getProjectDefinitionHandle();

    emit notifyProjectNameChanged(QString::fromStdString(currentProject->getName()));
    emit notifyProjectAuthorChanged(QString::fromStdString(currentProject->getAuthor()));
    emit notifyProjectDescriptionChanged(QString::fromStdString(currentProject->getDescription()));
    emit notifyProjectWindowWidthChanged(currentProject->getWindowWidth());
    emit notifyProjectWindowHeightChanged(currentProject->getWindowHeight());
    emit notifyProjectSceneListChanged(getSceneNamesListModel(currentProject->getSceneDefinitionsHandleList()));
    emit notifyProjectStartupSceneChanged(QString::fromStdString(currentProject->getStartupSceneDefinitionHandle()->getName()));
    emit notifyProjectWidgetsEnabledChanged(true);

    mProjectTreeModel.reset(new ProjectTreeModel(currentProject,mMainWindowHandle->getProjectTreeView()));
    mMainWindowHandle->getProjectTreeView()->setModel(mProjectTreeModel.get());

    mAssetDefinitionTreeModel.reset(new AssetDefinitionTreeModel(currentProject,mMainWindowHandle->getAssetDefinitionTreeView()));
    mMainWindowHandle->getAssetDefinitionTreeView()->setModel(mAssetDefinitionTreeModel.get());

    emit notifyStatusBarProjectLoaded(
        QString::fromStdString(
            "Successfuly Loaded Project: " +
            currentProject->getName() + " (" +
            currentProject->getUuid() + ")"
        )
    );
    connectTreeViewModel();
}

string
MainController::getSceneNameFromModelIndex
(int index)
{
    return mSceneListModel->stringList().at(index).toStdString();
}

void
MainController::onTreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        GenericTreeItem *selected = static_cast<GenericTreeItem*>(indexes.at(0).internalPointer());
        setupPropertiesTreeViewModel(selected);
        mMainWindowHandle->getPropertiesTreeView()->expandAll();
    }
}

void
MainController::setupPropertiesTreeViewModel
(GenericTreeItem *item)
{
    QTreeView *propertiesTreeView = mMainWindowHandle->getPropertiesTreeView();
    PropertiesModel *model = nullptr;
    ProjectDefinition *projectDefinition;
    AssetDefinition *assetDefinition = nullptr;
    SceneDefinition *sceneDefinition = nullptr;
    SceneObjectRuntime *sceneObjectRuntime = nullptr;
    SceneObjectDefinition *sceneObjectDefinition = nullptr;

    switch(item->getItemType())
    {
        case GenericTreeItemType::PROJECT:
            qDebug() << "MainController: Selected a project";
            projectDefinition = mDreamModel->getProject()->getProjectDefinitionHandle();
            model = new ProjectPropertiesModel(projectDefinition,propertiesTreeView);
            break;
        case GenericTreeItemType::ASSET_DEFINITION:
            qDebug() << "MainController: Selected an asset definition";
            assetDefinition = static_cast<AssetDefinitionTreeItem*>(item)->getAssetDefinition();
            model = new AssetDefinitionPropertiesModel(assetDefinition,propertiesTreeView);
            // Set Type Delegate
            break;
        case GenericTreeItemType::SCENE:
            sceneDefinition = static_cast<SceneDefinition*>(static_cast<ProjectTreeItem*>(item)->getItem());
            mDreamModel->setSelectedSceneDefinitionHandle(sceneDefinition);
            if (sceneDefinition)
            {
                model = new ScenePropertiesModel(sceneDefinition,propertiesTreeView);
                qDebug() << "MainController: Selected a scene";
            }
            break;
        case GenericTreeItemType::SCENE_OBJECT:
            qDebug() << "MainController: Selected a scene object";
            sceneObjectDefinition = static_cast<SceneObjectDefinition*>(static_cast<ProjectTreeItem*>(item)->getItem());
            model = new SceneObjectPropertiesModel(sceneObjectDefinition,propertiesTreeView);

            if (mSelectionHighlighter)
            {
                if (mDreamModel->getProject()->hasProjectRuntime())
                {
                    ProjectRuntime* prHandle = mDreamModel->getProject()->getProjectRuntimeHandle();
                    if (prHandle->hasActiveSceneRuntime())
                    {
                        sceneObjectRuntime = mDreamModel
                            ->getProject()
                            ->getProjectRuntimeHandle()
                            ->getActiveSceneRuntimeHandle()
                            ->getSceneObjectRuntimeHandleByUuid(sceneObjectDefinition->getUuid());
                        mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(sceneObjectRuntime);
                    }
                }
            }

            break;
        case GenericTreeItemType::TREE_NODE:
            qDebug() << "MainController: Selected a tree node";
            break;
    }

    if (model)
    {
        propertiesTreeView->setModel(model);
    }
}

void
MainController::onSelectedSceneChanged
(SceneDefinition *scene)
{
    mDreamModel->setSelectedSceneDefinitionHandle(scene);
    mMainWindowHandle->showStatusBarMessage(
        QString("Selected Scene: %1").
            arg(QString::fromStdString(scene->getName())
        )
    );
}

void
MainController::onSceneStopped
(SceneDefinition* scene)
{

}

Grid*
MainController::getGrid
()
{
    return mGrid.get();
}

SelectionHighlighter*
MainController::getSelectionHighlighter
()
{
    return mSelectionHighlighter.get();
}

void
MainController::onGridToggleAction
(bool enabled)
{
    mWindowComponentHandle->setGridEnabled(enabled);
}

void
MainController::onToggleDebugAction
(bool enabled)
{
    mDreamModel->setDebug(enabled);
}

RelationshipTree*
MainController::getRelationshipTree
()
{
    return mRelationshipTree.get();
}

void
MainController::onProjectClosedAction
()
{
    mDreamModel->closeProject();
}
