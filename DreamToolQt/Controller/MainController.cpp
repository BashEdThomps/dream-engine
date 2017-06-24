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
#include "../Model/TreeModels/Properties/ProjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/SceneObjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/ScenePropertiesModel.h"
#include "../Model/TreeModels/Properties/AssetDefinitionTypeComboDelegate.h"

// Constructors/Destructors =====================================================

MainController::MainController
(MainWindow* parent)
    : QObject(parent),
      mMainWindowHandle(parent),
      mWindowComponentHandle(parent->getWindowComponent()),
      mSelectedProjectDefinitionHandle(nullptr),
      mSelectedAssetDefinitionHandle(nullptr),
      mSelectedSceneDefinitionHandle(nullptr),
      mSelectedSceneObjectDefinitionHandle(nullptr)
{
    qDebug() << "MainController: Constructing Object";
    mDreamProjectModel.reset(new DreamProjectModel(this,mWindowComponentHandle));
    setupUI();
    setupConnections();
}

MainController::~MainController
()
{
    qDebug() << "MainController: Destroying Object";
}

// Setup ========================================================================

void
MainController::setupUI
()
{
   setupUI_GLWidgets();
}

void
MainController::setupUI_GLWidgets
()
{
    mGrid.reset(new Grid(this));
    mWindowComponentHandle->setGridHandle(getGridHandle());

    mSelectionHighlighter.reset(new SelectionHighlighter(this));
    mWindowComponentHandle->setSelectionHighlighterHandle(getSelectionHighlighterHandle());

    mRelationshipTree.reset(new RelationshipTree(this));
    mWindowComponentHandle->setRelationshipTreeHandle(getRelationshipTreeHandle());
}

void
MainController::setupUI_PropertiesTreeViewModel
(GenericTreeItem *item)
{
    QTreeView *propertiesTreeView = mMainWindowHandle->getPropertiesTreeView();



    SceneObjectRuntime *sceneObjectRuntime = nullptr;

    // Reset the selected object
    if (mSelectionHighlighter)
    {
        mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(nullptr);
    }

    switch(item->getItemType())
    {
        case GenericTreeItemType::PROJECT:
            qDebug() << "MainController: Selected a project";
            mSelectedProjectDefinitionHandle = mDreamProjectModel->getProject()->getProjectDefinitionHandle();
            mPropertiesModel.reset(new ProjectPropertiesModel(mSelectedProjectDefinitionHandle,propertiesTreeView));
            break;
        case GenericTreeItemType::ASSET_DEFINITION:
            qDebug() << "MainController: Selected an asset definition";
            mSelectedAssetDefinitionHandle = static_cast<AssetDefinitionTreeItem*>(item)->getAssetDefinition();
            mPropertiesModel.reset(new AssetDefinitionPropertiesModel(mSelectedAssetDefinitionHandle,propertiesTreeView));
            // Set Type Delegate
            break;
        case GenericTreeItemType::SCENE:
            mSelectedSceneDefinitionHandle = static_cast<SceneDefinition*>(static_cast<ScenegraphTreeItem*>(item)->getItem());
            mDreamProjectModel->setSelectedSceneDefinitionHandle(mSelectedSceneDefinitionHandle);
            if (mSelectedSceneDefinitionHandle)
            {
                mPropertiesModel.reset(new ScenePropertiesModel(mSelectedSceneDefinitionHandle,propertiesTreeView));
                qDebug() << "MainController: Selected a scene";
            }
            break;
        case GenericTreeItemType::SCENE_OBJECT:
            qDebug() << "MainController: Selected a scene object";
            mSelectedSceneObjectDefinitionHandle = static_cast<SceneObjectDefinition*>(static_cast<ScenegraphTreeItem*>(item)->getItem());
            mPropertiesModel.reset(new SceneObjectPropertiesModel(mSelectedSceneObjectDefinitionHandle,propertiesTreeView));

            if (mSelectionHighlighter)
            {
                if (mDreamProjectModel->getProject()->hasProjectRuntime())
                {
                    ProjectRuntime* prHandle = mDreamProjectModel->getProject()->getProjectRuntimeHandle();
                    if (prHandle->hasActiveSceneRuntime())
                    {
                        sceneObjectRuntime = mDreamProjectModel
                            ->getProject()
                            ->getProjectRuntimeHandle()
                            ->getActiveSceneRuntimeHandle()
                            ->getSceneObjectRuntimeHandleByUuid(mSelectedSceneObjectDefinitionHandle->getUuid());
                        mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(sceneObjectRuntime);
                    }
                }
            }

            break;
        case GenericTreeItemType::TREE_NODE:
            qDebug() << "MainController: Selected a tree node";
            break;
    }

    if (mPropertiesModel)
    {
        propertiesTreeView->setModel(mPropertiesModel.get());
    }
}

void
MainController::setupConnections
()
{
    connectMenus();
    connectUI();
}

// Signal/Slot Connectors =======================================================

void
MainController::connectMenus
()
{
    connectFileMenu();
    connectSceneMenu();
    connectAssetMenu();
    connectViewMenu();
}

void
MainController::connectFileMenu
()
{
    // actionNew
    connect
    (
        mMainWindowHandle->getAction_File_New(), SIGNAL(triggered()),
        this, SLOT(onAction_File_New())
    );

    // actionOpen
    connect
    (
        mMainWindowHandle->getAction_File_Open(), SIGNAL(triggered()),
        this, SLOT(onAction_File_Open())
    );

    // actionSave
    connect
    (
        mMainWindowHandle->getAction_File_Save(), SIGNAL(triggered()),
        this, SLOT(onAction_File_Save())
    );

    // actionCloseProject
    connect
    (
        mMainWindowHandle->getAction_File_CloseProject(), SIGNAL(triggered()),
        this, SLOT(onAction_File_Close())
    );

    // Open Default Project
    connect
    (
        mMainWindowHandle->getAction_File_OpenTestProject(), SIGNAL(triggered()),
        this, SLOT(onAction_File_OpenTestProject())
    );
}

void
MainController::connectSceneMenu
()
{
    // actionPlay
    connect
    (
        mMainWindowHandle->getAction_Scene_Play(), SIGNAL(triggered()),
        this, SLOT(onAction_Scene_Play())
    );

    // actionReload
    connect
    (
        mMainWindowHandle->getAction_Scene_Reload(), SIGNAL(triggered()),
        this, SLOT(onAction_Scene_Reload())
    );

    // actionStop
    connect
    (
        mMainWindowHandle->getAction_Scene_Stop(), SIGNAL(triggered()),
        this, SLOT(onAction_Scene_Stop())
    );

    // New Scene
    connect
    (
        mMainWindowHandle->getAction_Scene_NewScene(),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Scene_NewScene())
    );

    // New Scene Object
    connect
    (
        mMainWindowHandle->getAction_Scene_NewSceneObject(),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Scene_NewSceneObject())

    );
}


void
MainController::connectAssetMenu
()
{
    // Assets Menu -> Add To Selected SceneObject
    connect
    (
        mMainWindowHandle->getAction_Asset_AddToSelectedSceneObject(),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_AddToSelectedSceneObjectDefinition())
    );

    // Asset Menu -> New Definition -> Animation
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::ANIMATION),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Animation())
    );

    // Asset Menu -> New Definition -> Audio
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::AUDIO),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Audio())
    );

    // Asset Menu -> New Definition -> Font
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::FONT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Font())
    );

    // Asset Menu -> New Definition -> Light
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::LIGHT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Light())
    );

    // Asset Menu -> New Definition -> Model
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::MODEL),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Model())
    );

    // Asset Menu -> New Definition -> Physics Object
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::PHYSICS_OBJECT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_PhysicsObject())
    );

    // Asset Menu -> New Definition -> Script
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::SCRIPT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Script())
    );

    // Asset Menu -> New Definition -> Shader
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::SHADER),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Shader())
    );

    // Asset Menu -> New Definition -> Sprite
    connect
    (
        mMainWindowHandle->getAction_Asset_NewDefinition(AssetType::SPRITE),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Sprite())
    );
}

void
MainController::connectViewMenu
()
{
    // Action Toggle Grid
    connect
    (
        mMainWindowHandle->getAction_View_ToggleGrid(),SIGNAL(triggered(bool)),
        this,SLOT(onAction_View_ToggleGrid(bool))
    );

    // Action Toggle Debug
    connect
    (
        mMainWindowHandle->getAction_View_ToggleDebug(),SIGNAL(triggered(bool)),
        this,SLOT(onAction_View_ToggleDebug(bool))
    );

    // Action Toggle Physics Debug
    connect
    (
        mMainWindowHandle->getAction_View_TogglePhysicsDebug(),SIGNAL(triggered(bool)),
        this,SLOT(onAction_View_TogglePhysicsDebug(bool))
    );

}

void
MainController::connectUI
()
{
    // Status Bar
    connect
    (
        this, SIGNAL(notifyStatusBarProjectLoaded(QString)),
        mMainWindowHandle, SLOT(showStatusBarMessage(QString))
    );
    // Project Directory Changed
    connect
    (
        this, SIGNAL(notifyProjectDirectoryChanged(QString)),
        mMainWindowHandle, SLOT(setWindowTitle(QString))
    );
    // Valid Scene Selected
    connect
    (
        mDreamProjectModel.get(), SIGNAL(notifySelectedSceneChanged(SceneDefinition*)),
        this, SLOT(onUI_SelectedSceneChanged(SceneDefinition*))
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
}

void
MainController::connectUI_TreeViewModels
()
{
    // projectTreeView
    connect
    (
        mMainWindowHandle->getScenegraphTreeView()->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this, SLOT(onUI_TreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    // assetDefinitionTreeView
    connect
    (
        mMainWindowHandle->getAssetDefinitionTreeView()->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this, SLOT(onUI_TreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    mMainWindowHandle->getScenegraphTreeView()->expandAll();
    mMainWindowHandle->getAssetDefinitionTreeView()->expandAll();
}

void
MainController::updateWindowTitle
(QString msg)
{
    emit notifyProjectDirectoryChanged("Dream Tool :: " + msg);
}

void
MainController::setActionsEnabled_ValidProject
(bool enabled)
{
    mMainWindowHandle->setActionsEnabled_Scene_Playback(enabled);
    mMainWindowHandle->setActionsEnabled_Scene_Modification(enabled);
    mMainWindowHandle->setActionEnabled_File_Save(enabled);
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
MainController::onAction_Scene_Play
()
{
    qDebug() << "MainController: onReloadProject Clicked";
    SceneDefinition *sceneDefinitionHandle = mDreamProjectModel->getSelectedSceneDefinitionHandle();
    if (sceneDefinitionHandle)
    {
        mDreamProjectModel->startSceneRuntimeFromDefinition(sceneDefinitionHandle);
    }
    else
    {
        emit notifyNoSceneSelected();
    }
}

void
MainController::onAction_Scene_Reload
()
{
   onAction_Scene_Stop();
   onAction_Scene_Play();
}

void
MainController::onAction_Scene_Stop
()
{
    mDreamProjectModel->stopActiveSceneRuntime();
}

void
MainController::onUI_TreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        GenericTreeItem *selected = static_cast<GenericTreeItem*>(indexes.at(0).internalPointer());
        setupUI_PropertiesTreeViewModel(selected);
        mMainWindowHandle->getPropertiesTreeView()->expandAll();
    }
}

// Menu Action Handlers =========================================================

// File Menu

void
MainController::onAction_File_New
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();

        if (mProjectDirectory.size() == 0)
        {
            return;
        }

        bool createResult = mProjectDirectoryModel.createNewProjectTree(mProjectDirectory);

        if (createResult && mProjectDirectoryModel.isValidProject())
        {
            qDebug() << "MainController: Project is valid";
            setActionsEnabled_ValidProject(true);
            openProject();

        }
        else
        {
            qDebug() << "MainController: Project is NOT valid";
            setActionsEnabled_ValidProject(false);
        }
    }
}

void
MainController::onAction_File_Open
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setDirectory(QDir::home());
    openDialog.setAcceptMode(QFileDialog::AcceptOpen);

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();
    }

    if (mProjectDirectory.size() == 0)
    {
        return;
    }

    openProject();
}

void
MainController::onAction_File_OpenTestProject
()
{
    mProjectDirectory = "/Users/Ashley/.dreamtool/de60-75ff-5cb7-c4a9";
    openProject();
}

void
MainController::onAction_File_Save
()
{
    QFileDialog saveDialog;
    saveDialog.setDirectory(QDir::home());
    saveDialog.setFileMode(QFileDialog::Directory);
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);

    if(saveDialog.exec())
    {
    }
}

void
MainController::onAction_File_Close
()
{
    mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(nullptr);

    mMainWindowHandle->getScenegraphTreeView()->setModel(nullptr);
    mScenegraphTreeModel.reset();

    mMainWindowHandle->getAssetDefinitionTreeView()->setModel(nullptr);
    mAssetDefinitionTreeModel.reset();

    mMainWindowHandle->getPropertiesTreeView()->setModel(nullptr);

    mDreamProjectModel->closeProject();

    setActionsEnabled_ValidProject(false);
}

// View Menu

void
MainController::onAction_View_ToggleGrid
(bool enabled)
{
    mWindowComponentHandle->setGridEnabled(enabled);
}

void
MainController::onAction_View_ToggleDebug
(bool enabled)
{
    mDreamProjectModel->setDebug(enabled);
}

void
MainController::onAction_View_TogglePhysicsDebug
(bool enable)
{
    mDreamProjectModel->setPhysicsDebug(enable);
}

void
MainController::onAction_View_ToggleSelectionHighlighter
(bool enabled)
{
   mWindowComponentHandle->setSelectionHighlighterEnabled(enabled);
}

// Scene Menu

void
MainController::onAction_Scene_NewScene
()
{
    qDebug() << "MainController: onScenegraphAddSceneAction";
    Project* pHandle = mDreamProjectModel->getProject();
    if (pHandle)
    {
        ProjectDefinition* pdHandle = pHandle->getProjectDefinitionHandle();
        if (pdHandle)
        {
            SceneDefinition *sdHandle = pdHandle->createNewSceneDefinition();
            if (sdHandle)
            {
                onUI_ScenegraphUpdated();
            }
        }
    }
}

void
MainController::onAction_Scene_NewSceneObject
()
{
    qDebug() << "MainController: onScenegraphAddSceneObjectAction";
}

// Asset Menu

void
MainController::onAction_Asset_AddToSelectedSceneObjectDefinition
()
{
    qDebug() << "MainController: onAction_Asset_AddToSelectedSceneObjectDefinition ";
    if (mSelectedSceneObjectDefinitionHandle && mSelectedAssetDefinitionHandle)
    {
        mSelectedSceneObjectDefinitionHandle->addAssetDefinitionUuidToLoadQueue
        (
            mSelectedAssetDefinitionHandle->getUuid()
        );
    }
}

void
MainController::onAction_Asset_NewDefinition_Animation
()
{
    qDebug() << "MainController: Creating new Animation Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(ANIMATION))
    {
        onUI_AssetDefinitionsUpdated();
    }
}
void
MainController::onAction_Asset_NewDefinition_Audio
()
{
    qDebug() << "MainController: Creating new Audio Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(AUDIO))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Font
()
{
    qDebug() << "MainController: Creating new Font Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(FONT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Light
()
{
    qDebug() << "MainController: Creating new Light Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(LIGHT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Model
()
{
    qDebug() << "MainController: Creating new Model Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(MODEL))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_PhysicsObject
()
{
    qDebug() << "MainController: Creating new Physics Object Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(PHYSICS_OBJECT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Script
()
{
    qDebug() << "MainController: Creating new Script Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(SCRIPT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Shader
()
{
    qDebug() << "MainController: Creating new Shader Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(SHADER))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
MainController::onAction_Asset_NewDefinition_Sprite
()
{
    qDebug() << "MainController: Creating new Sprite Asset Definition";
    if (mDreamProjectModel->createNewAssetDefinition(SPRITE))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

// UI Signal/Slot Handlers ======================================================
void
MainController::onUI_SelectedSceneChanged
(SceneDefinition *scene)
{
    mDreamProjectModel->setSelectedSceneDefinitionHandle(scene);
    mMainWindowHandle->showStatusBarMessage(
        QString("Selected Scene: %1").
            arg(QString::fromStdString(scene->getName())
        )
    );
}

void
MainController::onUI_ScenegraphUpdated
()
{
    qDebug() << "MainController: updating scenegraph tree model";
    mScenegraphTreeModel->setupModelData();
}

void
MainController::onUI_AssetDefinitionsUpdated
()
{
    qDebug() << "MainController: updating scenegraph tree model";
    mAssetDefinitionTreeModel->setupModelData();
}

void
MainController::onUI_ProjectNameChanged
(QString name)
{
    qDebug() << "MainController: Name set to " << name;
    mDreamProjectModel->setProjectName(name.toStdString());
}

void
MainController::onUI_ProjectAuthorChanged
(QString author)
{
    qDebug() << "MainController: Author set to " << author;
    mDreamProjectModel->setProjectAuthor(author.toStdString());
}

void
MainController::onUI_ProjectDescriptionChanged
(QString desc)
{
    qDebug() << "MainController: Description set to " << desc;
    mDreamProjectModel->setProjectDescription(desc.toStdString());
}

void
MainController::onUI_ProjectWindowWidthChanged
(QString width)
{
    qDebug() << "MainController: Window Width set to " << width;
    mDreamProjectModel->setProjectWindowWidth(width.toInt());
}

void
MainController::onUI_ProjectWindowHeightChanged
(QString height)
{
    qDebug() << "MainController: Window Height set to " << height;
    mDreamProjectModel->setProjectWindowHeight(height.toInt());
}

void
MainController::onUI_ProjectStartupSceneChanged
(QString startupSceneIndex)
{
    string sceneName = getSceneNameFromModelIndex(startupSceneIndex.toInt());
    qDebug() << "MainController: startupScene set to " << startupSceneIndex << " " << QString::fromStdString(sceneName);
    SceneDefinition* sceneHandle = mDreamProjectModel->getProject()
        ->getProjectDefinitionHandle()
        ->getSceneDefinitionHandleByUuid(sceneName);
    mDreamProjectModel->setProjectStartupSceneByUuid(sceneHandle->getUuid());
}

// Object Handle Getters ========================================================

RelationshipTree*
MainController::getRelationshipTreeHandle
()
{
    return mRelationshipTree.get();
}

Grid*
MainController::getGridHandle
()
{
    return mGrid.get();
}

SelectionHighlighter*
MainController::getSelectionHighlighterHandle
()
{
    return mSelectionHighlighter.get();
}

// Misc =========================================================================

void
MainController::openProject
()
{
    bool loadResult = mDreamProjectModel->loadProject(mProjectDirectory);
    qDebug() << "MainController: Load Project Result " << loadResult;
    if (!loadResult)
    {
        emit notifyInvalidProjectDirectory(mProjectDirectory);
        emit notifyProjectWidgetsEnabledChanged(false);
        return;
    }
    updateWindowTitle(mProjectDirectory);
    ProjectDefinition *currentProject = mDreamProjectModel->getProject()->getProjectDefinitionHandle();

    emit notifyProjectNameChanged(QString::fromStdString(currentProject->getName()));
    emit notifyProjectAuthorChanged(QString::fromStdString(currentProject->getAuthor()));
    emit notifyProjectDescriptionChanged(QString::fromStdString(currentProject->getDescription()));
    emit notifyProjectWindowWidthChanged(currentProject->getWindowWidth());
    emit notifyProjectWindowHeightChanged(currentProject->getWindowHeight());
    emit notifyProjectSceneListChanged(getSceneNamesListModel(currentProject->getSceneDefinitionsHandleList()));
    //emit notifyProjectStartupSceneChanged(QString::fromStdString(currentProject->getStartupSceneDefinitionHandle()->getName()));
    emit notifyProjectWidgetsEnabledChanged(true);

    mScenegraphTreeModel.reset(new ScenegraphTreeModel(currentProject,mMainWindowHandle->getScenegraphTreeView()));
    mMainWindowHandle->getScenegraphTreeView()->setModel(mScenegraphTreeModel.get());

    mAssetDefinitionTreeModel.reset(new AssetDefinitionTreeModel(currentProject,mMainWindowHandle->getAssetDefinitionTreeView()));
    mMainWindowHandle->getAssetDefinitionTreeView()->setModel(mAssetDefinitionTreeModel.get());

    emit notifyStatusBarProjectLoaded(
        QString::fromStdString(
            "Successfuly Loaded Project: " +
            currentProject->getName() + " (" +
            currentProject->getUuid() + ")"
        )
    );
    setActionsEnabled_ValidProject(true);
    connectUI_TreeViewModels();
}

string
MainController::getSceneNameFromModelIndex
(int index)
{
    return mSceneListModel->stringList().at(index).toStdString();
}

