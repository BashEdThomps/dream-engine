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
#include <QLayout>
#include <QVBoxLayout>

#include "../Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesModel.h"
#include "../Model/TreeModels/Properties/Project/ProjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/Scene/ScenePropertiesModel.h"

#include <iostream>
using std::cout;

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

    mDreamProjectModel.reset
    (
        new DreamProjectModel
        (
            this,
            mWindowComponentHandle
        )
    );

    mMacOsOpenModel.reset
    (
        new MacOSOpenModel
        (
            &mPreferencesDialogController.getPreferencesModel(),
            this
        )
    );

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
MainController::setupUI_AssetDefinitionPropertiesTreeViewModel
(AssetDefinitionTreeItem* item)
{
    QTreeView *propertiesTreeView = mMainWindowHandle->getPropertiesTreeView();

    mPropertiesModel.reset(nullptr);

    switch(item->getType())
    {
        case AssetDefinitionTreeItemType::ASSET_DEFINITION:
            qDebug() << "MainController: Selected an asset definition";
            mSelectedAssetDefinitionHandle = static_cast<AssetDefinitionTreeItem*>(item)->getAssetDefinition();
            mPropertiesModel.reset
            (
                new AssetDefinitionPropertiesModel
                (
                    mSelectedAssetDefinitionHandle,
                    &mTemplatesModel,
                    propertiesTreeView
                )
            );

            mMainWindowHandle->setPropertiesDockWidgetTitle("Asset Definition Properties");

            // Audio File
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_AudioFile(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_AudioFile(AssetDefinition*))
            );

            // Font File
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_FontFile(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_FontFile(AssetDefinition*))
            );

            // Model File
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_ModelFile(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_ModelFile(AssetDefinition*))
            );

            // Model Additional Files
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_ModelAdditionalFiles(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_ModelAdditionalFiles(AssetDefinition*))
            );

            // Remove Files
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_RemoveFiles(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_RemoveFiles(AssetDefinition*))
            );

            // Edit Script
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_EditScript(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_EditScript(AssetDefinition*))
            );

            // Edit Vertex Shader
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_EditVertexShader(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_EditVertexShader(AssetDefinition*))
            );

            // Edit Fragment Shader
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_EditFragmentShader(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_EditFragmentShader(AssetDefinition*))
            );

            // Script Template
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyCombo_ScriptTemplateChanged(AssetDefinition*,QString)),
                this,
                SLOT(onAssetDefinitionProperty_ScriptTemplateChanged(AssetDefinition*,QString))
            );

            // Shader Template
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyCombo_ShaderTemplateChanged(AssetDefinition*,QString)),
                this,
                SLOT(onAssetDefinitionProperty_ShaderTemplateChanged(AssetDefinition*,QString))
            );

            // PhysicsObject Mesh File
            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_PhysicsBvhTriangleMeshFile(AssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_PhysicsBvhTriangleMeshFile(AssetDefinition*))
            );
            break;

        case AssetDefinitionTreeItemType::ASSET_TREE_NODE:
            qDebug() << "MainController: Selected an asset tree node";
            break;
    }

    if (mPropertiesModel)
    {
        propertiesTreeView->setModel(mPropertiesModel.get());
    }
}

void
MainController::setupUI_ScenegraphPropertiesTreeViewModel
(ScenegraphTreeItem *item)
{
    QTreeView *propertiesTreeView = mMainWindowHandle->getPropertiesTreeView();
    SceneObjectRuntime *selectedSceneObjectRuntime = nullptr;

    // Reset the selected object
    if (mSelectionHighlighter)
    {
        mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(nullptr);
    }

    mPropertiesModel.reset(nullptr);

    switch(item->getType())
    {
        case ScenegraphTreeItemType::SCENEGRAPH_PROJECT:
            qDebug() << "MainController: Selected a project";
            mSelectedProjectDefinitionHandle = mDreamProjectModel->getProject()->getProjectDefinitionHandle();
            mPropertiesModel.reset(new ProjectPropertiesModel(mSelectedProjectDefinitionHandle,propertiesTreeView));
            mMainWindowHandle->setPropertiesDockWidgetTitle("Project Properties");


            break;

        case ScenegraphTreeItemType::SCENEGRAPH_SCENE:
            qDebug() << "MainController: Selected a scene";
            mSelectedSceneDefinitionHandle = static_cast<SceneDefinition*>(static_cast<ScenegraphTreeItem*>(item)->getItem());
            mDreamProjectModel->setSelectedSceneDefinitionHandle(mSelectedSceneDefinitionHandle);

            if (mSelectedSceneDefinitionHandle)
            {
                mPropertiesModel.reset(new ScenePropertiesModel(mSelectedSceneDefinitionHandle,propertiesTreeView));
            }

            mMainWindowHandle->setPropertiesDockWidgetTitle("Scene Properties");
            break;

        case ScenegraphTreeItemType::SCENEGRAPH_SCENE_OBJECT:
            qDebug() << "MainController: Selected a scene object";
            mSelectedSceneObjectDefinitionHandle = static_cast<SceneObjectDefinition*>(static_cast<ScenegraphTreeItem*>(item)->getItem());
            mPropertiesModel.reset(new SceneObjectPropertiesModel(mSelectedSceneObjectDefinitionHandle,propertiesTreeView));

            mMainWindowHandle->setPropertiesDockWidgetTitle("Scene Object Properties");

            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_CaptureTranslation(SceneObjectDefinition*)),
                this,
                SLOT(onSceneObjectProperty_CaptureTranslation(SceneObjectDefinition*))
            );

            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_CaptureRotation(SceneObjectDefinition*)),
                this,
                SLOT(onSceneObjectProperty_CaptureRotation(SceneObjectDefinition*))
            );

            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_CaptureScale(SceneObjectDefinition*)),
                this,
                SLOT(onSceneObjectProperty_CaptureScale(SceneObjectDefinition*))
            );

            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_RemoveAsset(SceneObjectDefinition*,AssetDefinition*)),
                this,
                SLOT(onSceneObjectProperty_RemoveAsset(SceneObjectDefinition*,AssetDefinition*))
            );

            connect
            (
                mPropertiesModel.get(),
                SIGNAL(notifyButton_RemoveChild(SceneObjectDefinition*,SceneObjectDefinition*)),
                this,
                SLOT(onSceneObjectProperty_RemoveChild(SceneObjectDefinition*,SceneObjectDefinition*))
            );

            if (mSelectionHighlighter)
            {
                if (mDreamProjectModel->getProject()->hasProjectRuntime())
                {
                    ProjectRuntime* prHandle = mDreamProjectModel->getProject()->getProjectRuntimeHandle();
                    if (prHandle->hasActiveSceneRuntime())
                    {
                        selectedSceneObjectRuntime = mDreamProjectModel
                            ->getProject()
                            ->getProjectRuntimeHandle()
                            ->getActiveSceneRuntimeHandle()
                            ->getSceneObjectRuntimeHandleByUuid(mSelectedSceneObjectDefinitionHandle->getUuid());
                        mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(selectedSceneObjectRuntime);
                    }
                }
            }
            break;

        case ScenegraphTreeItemType::SCENEGRAPH_TREE_NODE:
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
    connectPreferences();
    connectFileMenu();
    connectSceneMenu();
    connectAssetMenu();
    connectViewMenu();
    connectDebugMenu();
}

void
MainController::connectPreferences
()
{
    connect
    (
        mMainWindowHandle->getAction_Preferences(),SIGNAL(triggered()),
        this, SLOT(onAction_Preferences())
    );
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
MainController::connectDebugMenu
()
{
    connect
    (
        mMainWindowHandle->getAction_Debug_DumpProjectDefinitionJson(),
        SIGNAL(triggered(bool)),
        this,
        SLOT(onAction_Debug_DumpProjectDefinitionJson(bool))
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
        this, SLOT(onUI_ScenegraphTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    // assetDefinitionTreeView
    connect
    (
        mMainWindowHandle->getAssetDefinitionTreeView()->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this, SLOT(onUI_AssetDefinitionTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
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
MainController::onUI_ScenegraphTreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        ScenegraphTreeItem *selected = static_cast<ScenegraphTreeItem*>(indexes.at(0).internalPointer());
        setupUI_ScenegraphPropertiesTreeViewModel(selected);
        mMainWindowHandle->getPropertiesTreeView()->expandAll();
    }
}

void
MainController::onUI_AssetDefinitionTreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        AssetDefinitionTreeItem *selected = static_cast<AssetDefinitionTreeItem*>(indexes.at(0).internalPointer());
        setupUI_AssetDefinitionPropertiesTreeViewModel(selected);
        mMainWindowHandle->getPropertiesTreeView()->expandAll();
    }
}

// Menu Action Handlers =========================================================

// File Menu

void
MainController::onAction_File_New ()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setDirectory(mPreferencesDialogController.getPreferencesModel().getDefaultProjectDirectory());

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();

        if (mProjectDirectory.size() == 0)
        {
            return;
        }

        bool createResult = mProjectDirectoryModel.createNewProjectTree(mProjectDirectory);
        ProjectDefinition* newDef = Project::createNewProjectDefinition(mProjectDirectoryModel.getProjectDirectoryName().toStdString());
        mProjectDirectoryModel.setProjectDefinitionHandle(newDef);
        bool projectFileResult = mProjectDirectoryModel.writeProjectFile();

        if (createResult && projectFileResult)
        {
            openProject();
        }
    }
}

void
MainController::onAction_File_Open
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setDirectory(mPreferencesDialogController.getPreferencesModel().getDefaultProjectDirectory());
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
    bool saveResult = mProjectDirectoryModel.writeProjectFile();
    if(saveResult)
    {
        emit notifyStatusBarProjectLoaded(
            QString::fromStdString(
                "Successfuly Saved Project: " +
                mDreamProjectModel->getProject()->getProjectDefinitionHandle()->getNameAndUuidString()
            )
        );
    }
    else
    {
        emit notifyStatusBarProjectLoaded(
            QString::fromStdString(
                "Error Saving Project: " +
                mDreamProjectModel->getProject()->getProjectDefinitionHandle()->getNameAndUuidString()
            )
        );
    }
}

void
MainController::onAction_File_Close
()
{
    mSelectedProjectDefinitionHandle = nullptr;
    mSelectedAssetDefinitionHandle = nullptr;
    mSelectedSceneDefinitionHandle = nullptr;
    mSelectedSceneObjectDefinitionHandle = nullptr;

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
    if (mSelectedSceneObjectDefinitionHandle)
    {
        mSelectedSceneObjectDefinitionHandle->createNewChildSceneObjectDefinition();
                onUI_ScenegraphUpdated();

    }
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

void
MainController::onAssetDefinitionProperty_ModelFile
(AssetDefinition* adHandle)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);

        qDebug() << "MainController: Using Assimp file"
                 << sourceFilePath;

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );
            if (result != QMessageBox::Yes)
            {
                qDebug() << "MainController: Copy of" << fileName
                         <<"was cancelled.";
               return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                qDebug() << "MainController: Error, unable to delete main asset file for"
                         << QString::fromStdString(adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        qDebug() << "MainController: Copy "
                 << (copyResult ? "Success":"Failed");
    }
}

void
MainController::onAssetDefinitionProperty_PhysicsBvhTriangleMeshFile
(AssetDefinition* adHandle)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);

        qDebug() << "MainController: Using BvhTriangleMesh file"
                 << sourceFilePath;

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );
            if (result != QMessageBox::Yes)
            {
                qDebug() << "MainController: Copy of" << fileName
                         <<"was cancelled.";
               return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                qDebug() << "MainController: Error, unable to delete main asset file for"
                         << QString::fromStdString(adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        qDebug() << "MainController: Copy "
                 << (copyResult ? "Success":"Failed");
    }
}

void
MainController::onAssetDefinitionProperty_ModelAdditionalFiles
(AssetDefinition* adHandle)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFiles);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {
        for (QString sourceFilePath : openDialog.selectedFiles())
        {
            QFile sourceFile(sourceFilePath);

            qDebug() << "MainController: Using Additional file"
                     << sourceFilePath;

            bool copyResult = mProjectDirectoryModel.copyAdditionalFile(adHandle,sourceFile);

            qDebug() << "MainController: Copy "
                     << (copyResult ? "Success":"Failed");
        }
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
    mMainWindowHandle->getScenegraphTreeView()->expandAll();
}

void
MainController::onUI_AssetDefinitionsUpdated
()
{
    qDebug() << "MainController: updating scenegraph tree model";
    mAssetDefinitionTreeModel->setupModelData();
    mMainWindowHandle->getAssetDefinitionTreeView()->expandAll();
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

void
MainController::onAction_Preferences
()
{
    mPreferencesDialogController.showDialog();
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

    mProjectDirectoryModel.setProjectDefinitionHandle
    (
        mDreamProjectModel->getProject()->getProjectDefinitionHandle()
    );

    mProjectDirectoryModel.inflateFromDirectory(mProjectDirectory);

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

void
MainController::onAction_Debug_DumpProjectDefinitionJson
(bool toggled)
{
    cout << mDreamProjectModel
            ->getProject()
            ->getProjectDefinitionHandle()
            ->getJson().dump(1) << endl;
    ;
}

void
MainController::onAssetDefinitionProperty_AudioFile
(AssetDefinition* adHandle)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(QDir::home());

    QStringList filters;
    filters << "*.wav" << "*.ogg";
    openDialog.setNameFilters(filters);

    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);

        qDebug() << "MainController: Using Audio file"
                 << sourceFilePath;

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );
            if (result != QMessageBox::Yes)
            {
                qDebug() << "MainController: Copy of" << fileName
                         <<"was cancelled.";
               return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                qDebug() << "MainController: Error, unable to delete main asset file for"
                         << QString::fromStdString(adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        qDebug() << "MainController: Copy "
                 << (copyResult ? "Success":"Failed");
    }
}

void
MainController::onAssetDefinitionProperty_FontFile
(AssetDefinition* adHandle)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(QDir::home());

    QStringList filters;
    filters << "*.ttf";
    openDialog.setNameFilters(filters);


    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);

        qDebug() << "MainController: Using Font file"
                 << sourceFilePath;

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );
            if (result != QMessageBox::Yes)
            {
                qDebug() << "MainController: Copy of" << fileName
                         <<"was cancelled.";
               return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                qDebug() << "MainController: Error, unable to delete main asset file for"
                         << QString::fromStdString(adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        qDebug() << "MainController: Copy "
                 << (copyResult ? "Success":"Failed");
    }
}

void
MainController::onAssetDefinitionProperty_RemoveFiles
(AssetDefinition* adHandle)
{
    auto result = QMessageBox::question
    (
        mMainWindowHandle,
        "Remove Asset Data?",
        "Are you sure you want to remove Asset data?"
    );

    if (result == QMessageBox::Yes)
    {
        mProjectDirectoryModel.deleteAssetDataDirectory(adHandle);
        return;
    }
}

void
MainController::onAssetDefinitionProperty_EditScript
(AssetDefinition* adHandle)
{
    qDebug() << "MainController: EditScript";
    QString filePath = mProjectDirectoryModel.createAssetTargetPath(adHandle);
    mProjectDirectoryModel.touchFile(filePath);
    mMacOsOpenModel->openInExternalEditor(filePath);
}

void
MainController::onAssetDefinitionProperty_EditVertexShader
(AssetDefinition* adHandle)
{
    qDebug() << "MainController: EditVertexShader";
    QString filePath = mProjectDirectoryModel.createAssetTargetPath
    (
        adHandle,
        QString::fromStdString
        (
            Constants::ASSET_FORMAT_SHADER_GLSL +
            Constants::SHADER_VERTEX
        )
    );
    mProjectDirectoryModel.touchFile(filePath);
    mMacOsOpenModel->openInExternalEditor(filePath);
}

void
MainController::onAssetDefinitionProperty_EditFragmentShader
(AssetDefinition* adHandle)
{
    qDebug() << "MainController: EditFragmentShader";
    QString filePath = mProjectDirectoryModel.createAssetTargetPath
    (
        adHandle,
        QString::fromStdString
        (
            Constants::ASSET_FORMAT_SHADER_GLSL +
            Constants::SHADER_FRAGMENT
        )
    );
    mProjectDirectoryModel.touchFile(filePath);
    mMacOsOpenModel->openInExternalEditor(filePath);
}

void
MainController::onAssetDefinitionProperty_ScriptTemplateChanged
(AssetDefinition* adHandle, const QString& templateName)
{
    qDebug() << "MainController: Script Template Changed";

    QString scriptContent = mTemplatesModel.getScriptTemplate(templateName);

    mProjectDirectoryModel.writeAssetData(scriptContent,adHandle);
}

void
MainController::onAssetDefinitionProperty_ShaderTemplateChanged
(AssetDefinition* adHandle, const QString& templateName)
{
    qDebug() << "MainController: Shader Template Changed";

    QString fragmentContent = mTemplatesModel.getShaderTemplate
    (
        templateName,
        QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME)
    );

    mProjectDirectoryModel.writeAssetData
    (
        fragmentContent,
        adHandle,
        QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME),
        true
    );

    QString vertexContent = mTemplatesModel.getShaderTemplate
    (
        templateName,
        QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME)
    );

    mProjectDirectoryModel.writeAssetData
    (
        vertexContent,
        adHandle,
        QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME),
        true
    );
}

void
MainController::onSceneObjectProperty_CaptureTranslation
(SceneObjectDefinition*)
{
    qDebug() << "MainController: CaptureTranslation";
}

void
MainController::onSceneObjectProperty_CaptureRotation
(SceneObjectDefinition*)
{
    qDebug() << "MainController: CaptureRotation";
}

void
MainController::onSceneObjectProperty_CaptureScale
(SceneObjectDefinition*)
{
    qDebug() << "MainController: CaptureScale";
}

void
MainController::onSceneObjectProperty_RemoveAsset
(SceneObjectDefinition* sodHandle,AssetDefinition* adHandle)
{
    qDebug() << "MainController: RemoveAsset"
             << QString::fromStdString(adHandle->getNameAndUuidString())
             << "from"
             << QString::fromStdString(sodHandle->getNameAndUuidString());
    sodHandle->removeAssetDefinitionFromLoadQueue(adHandle);
}

void
MainController::onSceneObjectProperty_RemoveChild
(SceneObjectDefinition* sodHandle, SceneObjectDefinition* sodChildHandle)
{
    qDebug() << "MainController: RemoveChild"
             << QString::fromStdString(sodChildHandle->getNameAndUuidString())
             << "from"
             << QString::fromStdString(sodHandle->getNameAndUuidString());
    sodHandle->removeChildSceneObjectDefinition(sodChildHandle);
    onUI_ScenegraphUpdated();
}
