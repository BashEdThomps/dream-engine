/*
 * ApplicationController.cpp
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

#include "ApplicationController.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>
#include <QModelIndexList>
#include <DreamCore.h>
#include <QOpenGLContext>
#include <QLayout>
#include <QVBoxLayout>
#include <QColorDialog>

#include "../Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesModel.h"
#include "../Model/TreeModels/Properties/Project/ProjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesModel.h"
#include "../Model/TreeModels/Properties/Scene/ScenePropertiesModel.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using std::cout;

// Constructors/Destructors =====================================================

ApplicationController::ApplicationController
(QObject* parent)
    : QObject(parent),
      mSelectedProjectDefinitionHandle(nullptr),
      mSelectedAssetDefinitionHandle(nullptr),
      mSelectedSceneDefinitionHandle(nullptr),
      mSelectedSceneObjectDefinitionHandle(nullptr),
      mLastDirectory(QDir::home()),
      mLastVolume(100)
{
    auto log = spdlog::get("ApplicationController");

    if (!log)
    {
        log = spdlog::stdout_color_mt("ApplicationController");
    }

    log->debug( "Constructing Object");

    mDreamProjectModel.reset(new DreamProjectModel(this,mMainWindowHandle.getWindowComponent()));

    mMacOsOpenModel.reset(
        new MacOSOpenModel(
            &mPreferencesDialogController.getPreferencesModel(),
            this
        )
    );

    setupUI();
    setupConnections();
    mMainWindowHandle.setTemplatesModelHandle(&mTemplatesModel);
    mMainWindowHandle.show();
}

ApplicationController::~ApplicationController
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Destroying Object");
}

// Setup ========================================================================

void
ApplicationController::setupUI
()
{
    setupUI_GLWidgets();
    connect(
        &mMainWindowHandle,
        SIGNAL(notifyCreateNewAssetDefinition(QString)),
        this,
        SLOT(onCreateNewAssetDefinition(QString))
    );
    connect(
        &mMainWindowHandle,
        SIGNAL(notifyScenegraphTreeDataChanged()),
        this,
        SLOT(onScenegraphTreeDataChanged())
    );

     connect(
        &mMainWindowHandle,
        SIGNAL(notifyAssetDefinitionTreeDataChanged()),
        this,
        SLOT(onAssetDefinitionTreeDataChanged())
    );
    connect(
        &mMainWindowHandle,
        SIGNAL(notifyPropertiesTreeDataChanged()),
        this,
        SLOT(onPropertiesTreeDataChanged())
    );
    connect(
        this,
        SIGNAL(notifyProjectDefinitionChanged(ProjectDefinition*)),
        &mMainWindowHandle,
        SLOT(onProjectDefinitionChanged(ProjectDefinition*))
    );
    connect
    (
        &mMainWindowHandle,
        SIGNAL(notifyMainVolumeChanged(int)),
        this,
        SLOT(onMainVolumeChanged(int))
    );
    // Path List Form
    connectUI_PathEditorSignals();
}

void ApplicationController::connectUI_PathEditorSignals()
{
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifyTableChanged()),
        mPathPointViewer.get(),
        SLOT(onUpdateRequested())
    );
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifySelectedRowChanged(int)),
        mPathPointViewer.get(),
        SLOT(onSelectionChanged(int))
    );
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifyCloseEvent()),
        this,
        SLOT(onPathEditorClosed())
    );
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifyPathVisibilityChanged(bool)),
        this,
        SLOT(onPathVisibilityChanged(bool))
    );
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifyTangentVisibilityChanged(bool)),
        mPathPointViewer.get(),
        SLOT(onTangentVisibilityChanged(bool))
    );
    connect
    (
        &mPathEditorFormController,
        SIGNAL(notifyTangentIndexChanged(int)),
        mPathPointViewer.get(),
        SLOT(onTangentIndexChanged(int))
    );
    connect(
        mPathPointViewer.get(),
        SIGNAL(notifyNumberOfTangentsChanged(int)),
        &mPathEditorFormController,
        SLOT(onNumberOfTangentsChanged(int))
    );
}

void
ApplicationController::setupUI_GLWidgets
()
{
    auto windowComp =  mMainWindowHandle.getWindowComponent();
    mGrid.reset(new Grid(this));
    windowComp->setGridHandle(getGridHandle());

    mSelectionHighlighter.reset(new SelectionHighlighter(this));
    windowComp->setSelectionHighlighterHandle(getSelectionHighlighterHandle());

    mRelationshipTree.reset(new RelationshipTree(this));
    windowComp->setRelationshipTreeHandle(getRelationshipTreeHandle());

    mPathPointViewer.reset(new PathPointViewer(this));
    windowComp->setPathPointViewerHandle(getPathPointViewerHandle());
}

void
ApplicationController::setupUI_AssetDefinitionPropertiesTreeViewModel
(AssetDefinitionTreeItem* item)
{
    auto log = spdlog::get("ApplicationController");
    QTreeView *propertiesTreeView = mMainWindowHandle.getPropertiesTreeView();

    mPropertiesTreeModel.reset(nullptr);

    switch(item->getType())
    {
        case AssetDefinitionTreeItemType::ASSET_DEFINITION:
            log->debug( "Selected an asset definition");
            mSelectedAssetDefinitionHandle = static_cast<AssetDefinitionTreeItem*>(item)->getAssetDefinition();
            mPropertiesTreeModel.reset(
                new AssetDefinitionPropertiesModel(
                    mSelectedAssetDefinitionHandle,
                    propertiesTreeView
                )
            );

            // Audio File
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_AudioFile(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_AudioFile(IAssetDefinition* ))
            );
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_AudioEvents(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_AudioEvents(IAssetDefinition* ))
            );

            // Font File
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_FontFile(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_FontFile(IAssetDefinition* ))
                        );

            // Material Editor
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_MaterialEditor(IAssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_MaterialEditor(IAssetDefinition*))
            );

            // Model File
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_ModelFile(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_ModelFile(IAssetDefinition* ))
                        );

            // Model Additional Files
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_ModelAdditionalFiles(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_ModelAdditionalFiles(IAssetDefinition* ))
                        );

            // Remove Files
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_RemoveFiles(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_RemoveFiles(IAssetDefinition* ))
                        );
            // Model Material Map
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_ModelMaterialShaderMap(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_ModelMaterialShaderMap(IAssetDefinition* ))
            );

            // Edit Script
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_EditScript(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_EditScript(IAssetDefinition* ))
                        );

            // Edit Vertex Shader
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_EditShader(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_EditShader(IAssetDefinition* ))
                        );

            // Script Template
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyCombo_ScriptTemplateChanged(IAssetDefinition* ,QString&)),
                        this,
                        SLOT(onAssetDefinitionProperty_ScriptTemplateChanged(IAssetDefinition* ,QString&))
                        );

            // Shader Template
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyCombo_ShaderTemplateChanged(IAssetDefinition* ,QString&)),
                        this,
                        SLOT(onAssetDefinitionProperty_ShaderTemplateChanged(IAssetDefinition* ,QString&))
                        );

            // PhysicsObject Mesh File
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_PhysicsBvhTriangleMeshFile(IAssetDefinition* )),
                        this,
                        SLOT(onAssetDefinitionProperty_PhysicsBvhTriangleMeshFile(IAssetDefinition* ))
                        );

            // Light Colour
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_LightChooseAmbient(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_LightChooseAmbient(IAssetDefinition* ))
            );
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_LightChooseDiffuse(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_LightChooseDiffuse(IAssetDefinition* ))
            );
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_LightChooseSpecular(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_LightChooseSpecular(IAssetDefinition* ))
            );

            // Path List
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_PathList(IAssetDefinition* )),
                this,
                SLOT(onAssetDefinitionProperty_PathList(IAssetDefinition* ))
            );

            // Texture
            connect
            (
                mPropertiesTreeModel.get(),
                SIGNAL(notifyButton_TextureFile(IAssetDefinition*)),
                this,
                SLOT(onAssetDefinitionProperty_TextureFile(IAssetDefinition*))
            );
            break;

        case AssetDefinitionTreeItemType::ASSET_TREE_NODE:
            log->debug( "Selected an asset tree node");
            break;
    }

    if (mPropertiesTreeModel)
    {
        propertiesTreeView->setModel(mPropertiesTreeModel.get());
    }
}

void
ApplicationController::setupUI_ScenegraphPropertiesTreeViewModel
(ScenegraphTreeItem *item)
{
    auto log = spdlog::get("ApplicationController");
    QTreeView *propertiesTreeView = mMainWindowHandle.getPropertiesTreeView();

    mPropertiesTreeModel.reset(nullptr);

    switch(item->getType())
    {
        case ScenegraphTreeItemType::SCENEGRAPH_PROJECT:
            log->debug( "Selected a project");
            mSelectedProjectDefinitionHandle = mDreamProjectModel->getProject()->getProjectDefinition();
            mPropertiesTreeModel.reset(new ProjectPropertiesModel(mSelectedProjectDefinitionHandle,propertiesTreeView));
            emit notifyProjectDefinitionChanged(mSelectedProjectDefinitionHandle);
            break;

        case ScenegraphTreeItemType::SCENEGRAPH_SCENE:
            log->debug( "Selected a scene");
            mSelectedSceneDefinitionHandle = dynamic_cast<SceneDefinition*>(
                    static_cast<ScenegraphTreeItem*>(item)
                        ->getItem()
            );
            mDreamProjectModel->setSelectedSceneDefinition(mSelectedSceneDefinitionHandle);

            if (mSelectedSceneDefinitionHandle != nullptr)
            {
                mPropertiesTreeModel.reset(new ScenePropertiesModel(mSelectedSceneDefinitionHandle,propertiesTreeView));
            }


            // Capture Camera Translation
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_CaptureCameraTranslation(SceneDefinition* )),
                        this,
                        SLOT(onSceneProperty_CaptureCameraTranslation(SceneDefinition* ))
                        );

            // Capture Camera Orientation
            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_CaptureCameraLookAt(SceneDefinition* )),
                        this,
                        SLOT(onSceneProperty_CaptureCameraLookAt(SceneDefinition* ))
                        );

            // Choose Clear Colour
            connect
                    (
                        mPropertiesTreeModel.get(), SIGNAL(notifyButton_ChooseClearColour(SceneDefinition* )),
                        this, SLOT(onSceneProperty_ChooseClearColour(SceneDefinition* ))
                        );
            // Choose Ambient Colour
            connect
                    (
                        mPropertiesTreeModel.get(), SIGNAL(notifyButton_ChooseAmbientColour(SceneDefinition* )),
                        this, SLOT(onSceneProperty_ChooseAmbientColour(SceneDefinition* ))
                        );
            break;

        case ScenegraphTreeItemType::SCENEGRAPH_SCENE_OBJECT:
            log->debug( "Selected a scene object");
            mSelectedSceneObjectDefinitionHandle = dynamic_cast<SceneObjectDefinition*>(
                static_cast<ScenegraphTreeItem*>(item)->getItem()
            );

            mPropertiesTreeModel.reset(
                new SceneObjectPropertiesModel(mSelectedSceneObjectDefinitionHandle,propertiesTreeView)
            );

            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_CaptureTranslation(SceneObjectDefinition*)),
                        this,
                        SLOT(onSceneObjectProperty_CaptureTranslation(SceneObjectDefinition*))
                        );

            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_CaptureOrientation(SceneObjectDefinition*)),
                        this,
                        SLOT(onSceneObjectProperty_CaptureOrientation(SceneObjectDefinition*))
                        );

            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_CaptureScale(SceneObjectDefinition*)),
                        this,
                        SLOT(onSceneObjectProperty_CaptureScale(SceneObjectDefinition*))
                        );

            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_RemoveAsset(SceneObjectDefinition*,IDefinition* )),
                        this,
                        SLOT(onSceneObjectProperty_RemoveAsset(SceneObjectDefinition*,IDefinition* ))
                        );

            connect
                    (
                        mPropertiesTreeModel.get(),
                        SIGNAL(notifyButton_RemoveChild(SceneObjectDefinition*,IDefinition*)),
                        this,
                        SLOT(onSceneObjectProperty_RemoveChild(SceneObjectDefinition*,IDefinition*))
                        );

            if (mSelectionHighlighter)
            {
                if (mDreamProjectModel->getProject()->hasProjectRuntime())
                {
                    ProjectRuntime* prHandle = mDreamProjectModel->getProject()->getProjectRuntime();
                    if (prHandle != nullptr && prHandle->hasActiveSceneRuntime())
                    {
                        SceneRuntime* sceneRuntime = mDreamProjectModel
                                ->getProject()
                                ->getProjectRuntime()
                                ->getActiveSceneRuntime();

                        if (sceneRuntime != nullptr)
                        {
                            auto selected = sceneRuntime->getSceneObjectRuntimeByUuid(mSelectedSceneObjectDefinitionHandle->getUuid());
                            mSelectionHighlighter->setSelectedSceneObjectRuntimeHandle(selected);
                        }
                    }
                }
            }
            else
            {
                log->error("Selection Highlighter is nullptr");
            }
            break;

        case ScenegraphTreeItemType::SCENEGRAPH_TREE_NODE:
            log->debug( "Selected a tree node");
            break;
    }

    if (mPropertiesTreeModel)
    {
        propertiesTreeView->setModel(mPropertiesTreeModel.get());
    }
}

void
ApplicationController::setupConnections
()
{
    connectMenus();
    connectUI();
}

// Signal/Slot Connectors =======================================================

void
ApplicationController::connectMenus
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
ApplicationController::connectPreferences
()
{
    connect
            (
                mMainWindowHandle.getAction_Preferences(),SIGNAL(triggered()),
                this, SLOT(onAction_Preferences())
                );
}

void
ApplicationController::connectFileMenu
()
{
    // actionNew
    connect
            (
                mMainWindowHandle.getAction_File_New(), SIGNAL(triggered()),
                this, SLOT(onAction_File_New())
                );

    // actionOpen
    connect
            (
                mMainWindowHandle.getAction_File_Open(), SIGNAL(triggered()),
                this, SLOT(onAction_File_Open())
                );

    // actionSave
    connect
            (
                mMainWindowHandle.getAction_File_Save(), SIGNAL(triggered()),
                this, SLOT(onAction_File_Save())
                );

    // actionCloseProject
    connect
            (
                mMainWindowHandle.getAction_File_CloseProject(), SIGNAL(triggered()),
                this, SLOT(onAction_File_Close())
                );
}

void
ApplicationController::connectSceneMenu
()
{
    // actionPlay
    connect
            (
                mMainWindowHandle.getAction_Scene_Play(), SIGNAL(triggered()),
                this, SLOT(onAction_Scene_Play())
                );

    // actionReload
    connect
            (
                mMainWindowHandle.getAction_Scene_Reload(), SIGNAL(triggered()),
                this, SLOT(onAction_Scene_Reload())
                );

    // actionStop
    connect
            (
                mMainWindowHandle.getAction_Scene_Stop(), SIGNAL(triggered()),
                this, SLOT(onAction_Scene_Stop())
                );

    // New Scene
    connect
            (
                mMainWindowHandle.getAction_Scene_NewScene(),
                SIGNAL(triggered()),
                this,
                SLOT(onAction_Scene_NewScene())
                );

    // New Scene Object
    connect
            (
                mMainWindowHandle.getAction_Scene_NewSceneObject(),
                SIGNAL(triggered()),
                this,
                SLOT(onAction_Scene_NewSceneObject())

                );
    // Connect Control Scene
    connect
    (
        mMainWindowHandle.getAction_ControlScene(),
        SIGNAL(toggled(bool)),
        this,
        SLOT(onAction_ControlSceneTriggered(bool))
    );
}

void
ApplicationController::connectAssetMenu
()
{
    // Assets Menu -> Add To Selected SceneObject
    connect
    (
        mMainWindowHandle.getAction_Asset_AddToSelectedSceneObject(),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_AddToSelectedSceneObjectDefinition())
    );

    // Asset Menu -> New Definition -> Audio
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::AUDIO),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Audio())
    );

    // Asset Menu -> New Definition -> Font
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::FONT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Font())
    );

    // Asset Menu -> New Definition -> Light
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::LIGHT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Light())
    );

    // Asset Menu -> New Definition -> Material
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::MATERIAL),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Material())
    );

    // Asset Menu -> New Definition -> Model
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::MODEL),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Model())
    );

    // Asset Menu -> New Definition -> ParticleEmitter
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::PARTICLE_EMITTER),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_ParticleEmitter())
    );

    // Asset Menu -> New Definition -> Path
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::PATH),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Path())
    );

    // Asset Menu -> New Definition -> Physics Object
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::PHYSICS_OBJECT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_PhysicsObject())
    );

    // Asset Menu -> New Definition -> Script
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::SCRIPT),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Script())
    );

    // Asset Menu -> New Definition -> Shader
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::SHADER),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Shader())
    );

    // Asset Menu -> New Definition -> Texture
    connect
    (
        mMainWindowHandle.getAction_Asset_NewDefinition(AssetType::TEXTURE),
        SIGNAL(triggered()),
        this,
        SLOT(onAction_Asset_NewDefinition_Texture())
    );

    connect
    (
        &mMainWindowHandle,
        SIGNAL(notifyNewTextureCreated()),
        this,
        SLOT(onAction_Asset_NewDefinition_Texture())
    );
}

void
ApplicationController::connectViewMenu
()
{
    // Action Toggle Grid
    connect
            (
                mMainWindowHandle.getAction_View_ToggleGrid(),SIGNAL(triggered(bool)),
                this,SLOT(onAction_View_ToggleGrid(bool))
                );

    // Action Toggle Debug
    connect
            (
                mMainWindowHandle.getAction_View_ToggleDebug(),SIGNAL(triggered(bool)),
                this,SLOT(onAction_View_ToggleDebug(bool))
                );

    // Action Toggle Physics Debug
    connect
            (
                mMainWindowHandle.getAction_View_TogglePhysicsDebug(),SIGNAL(triggered(bool)),
                this,SLOT(onAction_View_TogglePhysicsDebug(bool))
                );

}

void
ApplicationController::connectDebugMenu
()
{
    connect
            (
                mMainWindowHandle.getAction_Debug_DumpProjectDefinitionJson(),
                SIGNAL(triggered(bool)),
                this,
                SLOT(onAction_Debug_DumpProjectDefinitionJson(bool))
                );
}

void
ApplicationController::connectUI
()
{
    // Status Bar
    connect
    (
        this,
        SIGNAL(notifyStatusBarProjectLoaded(QString)),
        &mMainWindowHandle,
        SLOT(showStatusBarMessage(QString))
    );
    // Project Directory Changed
    connect
            (
                this, SIGNAL(notifyProjectDirectoryChanged(QString)),
                &mMainWindowHandle, SLOT(setWindowTitle(QString))
                );
    // Valid Scene Selected
    connect
            (
                mDreamProjectModel.get(), SIGNAL(notifySelectedSceneChanged(SceneDefinition* )),
                this, SLOT(onUI_SelectedSceneChanged(SceneDefinition* ))
                );
    // Invalid Project Directory
    connect
            (
                this, SIGNAL(notifyInvalidProjectDirectory(QString)),
                &mMainWindowHandle, SLOT(onInvalidProjectDirectory(QString))
                );
    // No Scene Selected
    connect
            (
                this,SIGNAL(notifyNoSceneSelected()),
                &mMainWindowHandle, SLOT(onNoSceneSelected())
                );
    connect
    (
        &mMainWindowHandle,
        SIGNAL(notifyActionScriptingTriggered(bool)),
        this,
        SLOT(onActionScriptingTriggered(bool))
    );
}

void
ApplicationController::connectUI_TreeViewModels
()
{
    // projectTreeView
    connect
            (
                mMainWindowHandle.getScenegraphTreeView()->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                this, SLOT(onUI_ScenegraphTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
                );
     connect(
        mScenegraphTreeModel.get(),
        SIGNAL(notifyExpandRequested()),
        &mMainWindowHandle,
        SLOT(onScenegraphTreeExpandRequested())
    );

    // assetDefinitionTreeView
    connect
            (
                mMainWindowHandle.getAssetDefinitionTreeView()->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
                this, SLOT(onUI_AssetDefinitionTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
                );

   mMainWindowHandle.getScenegraphTreeView()->expandToDepth(1);
   mMainWindowHandle.getAssetDefinitionTreeView()->expandAll();
}

void
ApplicationController::updateWindowTitle
(QString msg)
{
    emit notifyProjectDirectoryChanged("Dream Tool :: " + msg);
}

void
ApplicationController::setActionsEnabled_ValidProject
(bool enabled)
{
    mMainWindowHandle.setActionsEnabled_Scene_Playback(enabled);
    mMainWindowHandle.setActionsEnabled_Scene_Modification(enabled);
    mMainWindowHandle.setActionEnabled_File_Save(enabled);
}

QStringListModel*
ApplicationController::getSceneNamesListModel
(vector<SceneDefinition*> sceneList)
{
    QStringList sceneNameList;
    mSceneListModel.reset(new QStringListModel(&mMainWindowHandle));

    for (auto scene : sceneList)
    {
        sceneNameList.push_back(QString::fromStdString(scene->getName()));
    }

    mSceneListModel->setStringList(sceneNameList);
    return mSceneListModel.get();
}

void
ApplicationController::onAction_Scene_Play
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "onReloadProject Clicked");
    SceneDefinition* sceneDefinitionHandle = mDreamProjectModel->getSelectedSceneDefinition();
    if (sceneDefinitionHandle)
    {
        auto result = mDreamProjectModel->startSceneRuntimeFromDefinition(sceneDefinitionHandle);
        if(result)
        {

            onMainVolumeChanged(mLastVolume);
        }
    }
    else
    {
        emit notifyNoSceneSelected();
    }
}

void
ApplicationController::onAction_Scene_Reload
()
{
    onAction_Scene_Stop();
    onAction_Scene_Play();
}

void
ApplicationController::onAction_Scene_Stop
()
{
    auto windowComp = mMainWindowHandle.getWindowComponent();
    windowComp->clearRuntime();
    windowComp->resetFBO();
    mDreamProjectModel->stopActiveSceneRuntime();
}

void
ApplicationController::onAction_ControlSceneTriggered
(bool activated)
{
   auto log = spdlog::get("ApplicationController");
   auto windowComp = mMainWindowHandle.getWindowComponent();
   log->critical("Scene Control Enabled {}",activated);
   windowComp->setControlScene(activated);
}

void
ApplicationController::onUI_ScenegraphTreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        ScenegraphTreeItem *selected = static_cast<ScenegraphTreeItem*>(indexes.at(0).internalPointer());
        setupUI_ScenegraphPropertiesTreeViewModel(selected);
        mMainWindowHandle.getPropertiesTreeView()->expandAll();
    }
}

void
ApplicationController::onUI_AssetDefinitionTreeViewSelectionChanged
(const QItemSelection& selected,const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    QModelIndexList indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        AssetDefinitionTreeItem *selected = static_cast<AssetDefinitionTreeItem*>(indexes.at(0).internalPointer());
        setupUI_AssetDefinitionPropertiesTreeViewModel(selected);
        mMainWindowHandle.getPropertiesTreeView()->expandAll();
    }
}

// Menu Action Handlers =========================================================

// File Menu

void
ApplicationController::onAction_File_New
()
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setLabelText(QFileDialog::Accept, "Create Project");
    openDialog.setDirectory(mPreferencesDialogController.getPreferencesModel().getDefaultProjectDirectory());

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();
        mLastDirectory = (mProjectDirectory);

        if (mProjectDirectory.size() == 0)
        {
            return;
        }

        if(!QDir(mProjectDirectory).isEmpty())
        {
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Directory not empty",
                "The chosen directory is not empty.\n\n Are you sure you want to create a project here?"
            );

            if (result != QMessageBox::Yes)
            {
                log->debug( "Project creation was was cancelled for ", mProjectDirectory.toStdString());
                return;
            }
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
ApplicationController::onAction_File_Open
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setLabelText(QFileDialog::Accept, "Open Project");
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

    mLastDirectory = QDir(mProjectDirectory);

    openProject();
}

void
ApplicationController::onAction_File_Save
()
{
    bool saveResult = mProjectDirectoryModel.writeProjectFile();
    if(saveResult)
    {
        emit notifyStatusBarProjectLoaded (
            QString::fromStdString (
                "Successfuly Saved Project: " +
                mDreamProjectModel->getProject()->getProjectDefinition()->getNameAndUuidString()
            )
        );
    }
    else
    {
        emit notifyStatusBarProjectLoaded (
            QString::fromStdString (
                "Error Saving Project: " +
                mDreamProjectModel->getProject()->getProjectDefinition()->getNameAndUuidString()
            )
        );
    }
}

void
ApplicationController::onAction_File_Close
()
{
    mMainWindowHandle.closeProject();
    mSelectedProjectDefinitionHandle = nullptr;
    mSelectedAssetDefinitionHandle = nullptr;
    mSelectedSceneDefinitionHandle = nullptr;
    mSelectedSceneObjectDefinitionHandle = nullptr;
    mScenegraphTreeModel.reset();
    mAssetDefinitionTreeModel.reset();
    mDreamProjectModel->closeProject();
    setActionsEnabled_ValidProject(false);
}

// View Menu

void
ApplicationController::onAction_View_ToggleGrid
(bool enabled)
{
    mMainWindowHandle.getWindowComponent()->setGridEnabled(enabled);
}

void
ApplicationController::onAction_View_ToggleDebug
(bool enabled)
{
    mDreamProjectModel->setDebug(enabled);
}

void
ApplicationController::onAction_View_TogglePhysicsDebug
(bool enable)
{
    mDreamProjectModel->setPhysicsDebug(enable);
}

void
ApplicationController::onAction_View_ToggleSelectionHighlighter
(bool enabled)
{
    mMainWindowHandle.getWindowComponent()->setSelectionHighlighterEnabled(enabled);
}

// Scene Menu

void
ApplicationController::onAction_Scene_NewScene
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "onScenegraphAddSceneAction");
    Project* pHandle = mDreamProjectModel->getProject();
    if (pHandle)
    {
        ProjectDefinition* pdHandle = pHandle->getProjectDefinition();
        if (pdHandle)
        {
            SceneDefinition* sdHandle = pdHandle->createNewSceneDefinition();
            if (sdHandle)
            {
                onUI_ScenegraphUpdated();
            }
        }
    }
}

void
ApplicationController::onAction_Scene_NewSceneObject
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "onScenegraphAddSceneObjectAction");
    if (mSelectedSceneObjectDefinitionHandle)
    {
        mSelectedSceneObjectDefinitionHandle->createNewChildSceneObjectDefinition();
        onUI_ScenegraphUpdated();
    }
}

// Asset Menu

void
ApplicationController::onAction_Asset_AddToSelectedSceneObjectDefinition
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "onAction_Asset_AddToSelectedSceneObjectDefinition ");
    if (mSelectedSceneObjectDefinitionHandle && mSelectedAssetDefinitionHandle)
    {
        mSelectedSceneObjectDefinitionHandle->addAssetDefinitionUuidToLoadQueue
        (
            mSelectedAssetDefinitionHandle->getUuid()
        );
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Audio
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Audio Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(AUDIO))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Font
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Font Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(FONT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Light
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Light Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(LIGHT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Material
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Material Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(MATERIAL))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Model
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Model Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(MODEL))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_ParticleEmitter
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new ParticleEmitter Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(PARTICLE_EMITTER))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Path
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Path Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(PATH))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_PhysicsObject
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Physics Object Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(PHYSICS_OBJECT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Script
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Script Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(SCRIPT))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Shader
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Shader Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(SHADER))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAction_Asset_NewDefinition_Texture
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Creating new Texture Asset Definition");
    if (mDreamProjectModel->createNewAssetDefinition(TEXTURE))
    {
        onUI_AssetDefinitionsUpdated();
    }
}

void
ApplicationController::onAssetDefinitionProperty_ModelFile
(IAssetDefinition* adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(mLastDirectory);

    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);
        mLastDirectory.setPath(sourceFilePath);

        log->debug( "Using Assimp file {}", sourceFilePath.toStdString());

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );
            if (result != QMessageBox::Yes)
            {
                log->debug( "Copy of {} was cancelled", fileName.toStdString());
                return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                log->debug( "Error, unable to delete main asset file for {}", adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        if (copyResult)
        {
            QString fileName = QFileInfo(sourceFile).baseName();
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Use File Name for Asset?",
                "Do you want to give this Asset the same name as it's source file?"
            );

            if (result == QMessageBox::Yes)
            {
                adHandle->setName(fileName.toStdString());
            }
        }

        dynamic_cast<ModelDefinition*>(adHandle)->clearModelMaterialList();

        log->debug( "Copy {} ", (copyResult ? "Success":"Failed"));
    }
}

void
ApplicationController::onAssetDefinitionProperty_PhysicsBvhTriangleMeshFile
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(mLastDirectory);

    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);
        mLastDirectory.setPath(sourceFilePath);

        log->debug( "Using BvhTriangleMesh file {}", sourceFilePath.toStdString());

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
                    (
                        &mMainWindowHandle,
                        "Overwrite Existing File?",
                        "An asset file all ready exists. Do you want to replace it?"
                        );
            if (result != QMessageBox::Yes)
            {
                log->debug( "Copy of {} was cancelled." , fileName.toStdString());
                return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                log->debug( "Error, unable to delete main asset file for {}",
                           adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        log->debug( "Copy {}", (copyResult ? "Success":"Failed"));
    }
}

void
ApplicationController::onSceneProperty_CaptureCameraTranslation
(SceneDefinition*  sdHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "CaptureCameraTranslation");
    Project* pHandle = mDreamProjectModel->getProject();

    if (pHandle != nullptr)
    {
        ProjectRuntime* prHandle = pHandle->getProjectRuntime();
        if (prHandle != nullptr)
        {
            auto camera = prHandle->getCamera();
            if ( camera == nullptr)
            {
                return;
            }

            auto translation = camera->getTranslation();
            sdHandle->setCameraTranslation(translation);
            mMainWindowHandle.showStatusBarMessage(
                QString("Captured Camera Translation x:%1, y:%2, z:%3")
                        .arg(QString::number(translation.x, 'G', 5))
                        .arg(QString::number(translation.y, 'G', 5))
                        .arg(QString::number(translation.z, 'G', 5))
            );
            return;
        }
        log->debug( "CaptureCameraTranslation - No ProjectRuntime");
        return;
    }
    log->debug( "CaptureCameraTranslation - No Project");
    return;
}

void
ApplicationController::onSceneProperty_CaptureCameraLookAt
(SceneDefinition* sdHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "CaptureCameraLookAt");
    Project* pHandle = mDreamProjectModel->getProject();

    if (pHandle != nullptr)
    {
        ProjectRuntime* prHandle = pHandle->getProjectRuntime();
        if (prHandle == nullptr)
        {
            auto camera = prHandle->getCamera();
            if ( camera == nullptr)
            {
                return;
            }
            auto pitch = camera->getPitch();
            auto yaw   = camera->getYaw();

            sdHandle->setCameraPitch(pitch);
            sdHandle->setCameraYaw(yaw);
            mMainWindowHandle.showStatusBarMessage(
                QString("Captured Camera LookAt pitch: %1, yaw:%2")
                        .arg(QString::number(pitch, 'G', 5))
                        .arg(QString::number(yaw, 'G', 5))
            );
            return;
        }
        log->debug( "CaptureCameraLookAt - No ProjectRuntime");
        return;
    }
    log->debug( "CaptureCameraLookAt - No Project");
    return;
}



void ApplicationController::onSceneProperty_ChooseAmbientColour(SceneDefinition*  sceneDef)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Choose Ambient Colour");

    QColor currentColour;

    auto colour = sceneDef->getAmbientColour();
    currentColour.setRgbF(
                static_cast<double>(colour[0]), // r
            static_cast<double>(colour[1]), // g
            static_cast<double>(colour[2]), // b
            static_cast<double>(colour[3])  // a

            );

    auto chosenColour = QColorDialog::getColor (
        currentColour,
        &mMainWindowHandle,
        "Ambient Colour",
        QColorDialog::ShowAlphaChannel
    );

    if (chosenColour.isValid())
    {
        sceneDef->setAmbientColourR(static_cast<float>(chosenColour.redF()));
        sceneDef->setAmbientColourG(static_cast<float>(chosenColour.greenF()));
        sceneDef->setAmbientColourB(static_cast<float>(chosenColour.blueF()));
        sceneDef->setAmbientColourA(static_cast<float>(chosenColour.alphaF()));
    }

}

void
ApplicationController::onSceneProperty_ChooseClearColour
(SceneDefinition*  sceneDef)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Choose Clear Colour");
    QColor currentColour;
    auto colour = sceneDef->getClearColour();
    currentColour.setRgbF(
                static_cast<double>(colour[0]), // r
            static_cast<double>(colour[1]), // g
            static_cast<double>(colour[2]) // b
            );

    auto chosenColour = QColorDialog::getColor (
        currentColour,
        &mMainWindowHandle,
        "Clear Colour"
    );

    if (chosenColour.isValid())
    {
        sceneDef->setClearColourR(static_cast<float>(chosenColour.redF()));
        sceneDef->setClearColourG(static_cast<float>(chosenColour.greenF()));
        sceneDef->setClearColourB(static_cast<float>(chosenColour.blueF()));
    }
}

void
ApplicationController::onCreateNewAssetDefinition
(QString type)
{
    auto log = spdlog::get("ApplicationController");
    log->debug("Creating new asset definition: {}",type.toStdString());
    AssetType assetType = Constants::getAssetTypeEnumFromString(type.toStdString());

    switch(assetType)
    {
        case AUDIO:
            onAction_Asset_NewDefinition_Audio();
            break;

        case FONT:
            onAction_Asset_NewDefinition_Font();
            break;

        case LIGHT:
            onAction_Asset_NewDefinition_Light();
            break;

        case MATERIAL:
            onAction_Asset_NewDefinition_Material();
            break;

        case MODEL:
            onAction_Asset_NewDefinition_Model();
            break;

        case PARTICLE_EMITTER:
            onAction_Asset_NewDefinition_ParticleEmitter();
            break;

        case PATH:
            onAction_Asset_NewDefinition_Path();
            break;

        case PHYSICS_OBJECT:
            onAction_Asset_NewDefinition_PhysicsObject();
            break;

        case SCRIPT:
            onAction_Asset_NewDefinition_Script();
            break;

        case SHADER:
            onAction_Asset_NewDefinition_Shader();
            break;

        case TEXTURE:
            onAction_Asset_NewDefinition_Texture();
            break;

        case NONE:
            break;
    }
}

void
ApplicationController::onMainVolumeChanged
(int vol)
{
   auto log = spdlog::get("ApplicationController");
   mLastVolume = vol;
   if (mDreamProjectModel != nullptr)
   {
       auto project = mDreamProjectModel->getProject();
       if (project != nullptr)
       {
           ProjectRuntime* runtime = project->getProjectRuntime();
           if (runtime != nullptr)
           {
               AudioComponent* audio = runtime->getAudioComponent();
               if (audio != nullptr)
               {
                   auto volume = vol/100.0f;
                   log->debug("Setting volume to {}",volume);
                   audio->setVolume(volume);
               }
           }
       }
   }
}

void
ApplicationController::onPathEditorClosed
()
{
    auto log = spdlog::get("ApplicationController");
    log->critical("Path editor was closed");
    mPathPointViewer->setPathDefinition(nullptr);
    mMainWindowHandle.setPathEditorFormControllerHandle(nullptr);
}

void
ApplicationController::onPathVisibilityChanged
(bool visible)
{
    mPathPointViewer->setPathVisible(visible);
}

void
ApplicationController::onActionScriptingTriggered
(bool checked)
{
    if (mDreamProjectModel)
    {
        mDreamProjectModel->setScriptingEnabled(checked);
    }
}

void
ApplicationController::onAssetDefinitionProperty_ModelAdditionalFiles
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFiles);
    openDialog.setDirectory(mLastDirectory);

    if(openDialog.exec())
    {
        for (QString sourceFilePath : openDialog.selectedFiles())
        {
            QFile sourceFile(sourceFilePath);
            mLastDirectory.setPath(sourceFilePath);

            log->debug( "Using Additional file {}",
                       sourceFilePath.toStdString());

            bool copyResult = mProjectDirectoryModel.copyAdditionalFile(adHandle,sourceFile);

            log->debug( "Copy ",
                       (copyResult ? "Success":"Failed"));
        }
    }
}

// UI Signal/Slot Handlers ======================================================
void
ApplicationController::onUI_SelectedSceneChanged
(SceneDefinition* scene)
{
    auto log = spdlog::get("ApplicationController");
    mDreamProjectModel->setSelectedSceneDefinition(scene);
    mMainWindowHandle.showStatusBarMessage(
                QString("Selected Scene: %1").
                arg(QString::fromStdString(scene->getName())
                    )
                );
}

void
ApplicationController::onUI_ScenegraphUpdated
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "updating scenegraph tree model");
    mScenegraphTreeModel->setupModelData();
    mMainWindowHandle.getScenegraphTreeView()->expandAll();
}

void
ApplicationController::onUI_AssetDefinitionsUpdated
()
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "updating scenegraph tree model");
    mAssetDefinitionTreeModel->setupModelData();
    mMainWindowHandle.getAssetDefinitionTreeView()->expandAll();
}

void
ApplicationController::onUI_ProjectNameChanged
(QString name)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Name set to {}" , name.toStdString());
    mDreamProjectModel->setProjectName(name.toStdString());
}

void
ApplicationController::onUI_ProjectAuthorChanged
(QString author)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Author set to " , author.toStdString());
    mDreamProjectModel->setProjectAuthor(author.toStdString());
}

void
ApplicationController::onUI_ProjectDescriptionChanged
(QString desc)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Description set to " , desc.toStdString());
    mDreamProjectModel->setProjectDescription(desc.toStdString());
}

void
ApplicationController::onUI_ProjectWindowWidthChanged
(QString width)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Window Width set to " , width.toStdString());
    mDreamProjectModel->setProjectWindowWidth(width.toInt());
}

void
ApplicationController::onUI_ProjectWindowHeightChanged
(QString height)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Window Height set to {}" , height.toStdString());
    mDreamProjectModel->setProjectWindowHeight(height.toInt());
}

void
ApplicationController::onUI_ProjectStartupSceneChanged
(QString startupSceneIndex)
{
    auto log = spdlog::get("ApplicationController");
    string sceneName = getSceneNameFromModelIndex(startupSceneIndex.toInt());
    log->debug( "startupScene set to {} {}" , startupSceneIndex.toStdString() ,  sceneName);
    SceneDefinition*  sceneHandle = mDreamProjectModel->getProject()
            ->getProjectDefinition()
            ->getSceneDefinitionByUuid(sceneName);
    mDreamProjectModel->setProjectStartupSceneByUuid(sceneHandle->getUuid());
}

// Object Handle Getters ========================================================

RelationshipTree*
ApplicationController::getRelationshipTreeHandle
()
{
    return mRelationshipTree.get();
}

PathPointViewer*
ApplicationController::getPathPointViewerHandle
()
{
    return mPathPointViewer.get();
}

PathEditorFormController*
ApplicationController::getPathEditorFormControllerHandle
()
{
   return &mPathEditorFormController;
}

void
ApplicationController::forceScenegraphTreeDataChanged
()
{
    mScenegraphTreeModel->forceDataChanged(QModelIndex());
    mMainWindowHandle.getScenegraphTreeView()->expandAll();
}

void
ApplicationController::forceAssetDefinitionTreeDataChanged
()
{
    mAssetDefinitionTreeModel->forceDataChanged();
    mMainWindowHandle.getAssetDefinitionTreeView()->expandAll();
}

void
ApplicationController::onScenegraphTreeDataChanged
()
{
    forceScenegraphTreeDataChanged();
}

void
ApplicationController::onAssetDefinitionTreeDataChanged
()
{
    forceAssetDefinitionTreeDataChanged();
}

void
ApplicationController::onPropertiesTreeDataChanged
()
{
    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::onAction_Preferences
()
{
    mPreferencesDialogController.showDialog();
}

Grid*
ApplicationController::getGridHandle
()
{
    return mGrid.get();
}

SelectionHighlighter*
ApplicationController::getSelectionHighlighterHandle
()
{
    return mSelectionHighlighter.get();
}

// Misc =========================================================================

void
ApplicationController::openProject
()
{
    auto log = spdlog::get("ApplicationController");
    bool loadResult = mDreamProjectModel->loadProject(mProjectDirectory);

    mProjectDirectoryModel.setProjectDefinitionHandle
            (
                mDreamProjectModel->getProject()->getProjectDefinition()
                );

    mProjectDirectoryModel.inflateFromDirectory(mProjectDirectory);

    log->debug( "Load Project Result {}", loadResult);
    if (!loadResult)
    {
        emit notifyInvalidProjectDirectory(mProjectDirectory);
        emit notifyProjectWidgetsEnabledChanged(false);
        return;
    }

    updateWindowTitle(mProjectDirectory);

    ProjectDefinition* currentProject = mDreamProjectModel->getProject()->getProjectDefinition();

    emit notifyProjectNameChanged(QString::fromStdString(currentProject->getName()));
    emit notifyProjectAuthorChanged(QString::fromStdString(currentProject->getAuthor()));
    emit notifyProjectDescriptionChanged(QString::fromStdString(currentProject->getDescription()));
    emit notifyProjectWindowWidthChanged(currentProject->getWindowWidth());
    emit notifyProjectWindowHeightChanged(currentProject->getWindowHeight());
    emit notifyProjectSceneListChanged(getSceneNamesListModel(currentProject->getSceneDefinitionsList()));
    emit notifyProjectWidgetsEnabledChanged(true);
    emit notifyProjectDefinitionChanged(currentProject);

    mScenegraphTreeModel.reset(new ScenegraphTreeModel(currentProject,mMainWindowHandle.getScenegraphTreeView()));
    mMainWindowHandle.getScenegraphTreeView()->setModel(mScenegraphTreeModel.get());
    mMainWindowHandle.setProjectDirectoryModel(&mProjectDirectoryModel);

    mAssetDefinitionTreeModel.reset(new AssetDefinitionTreeModel(currentProject,mMainWindowHandle.getAssetDefinitionTreeView()));
    mMainWindowHandle.getAssetDefinitionTreeView()->setModel(mAssetDefinitionTreeModel.get());
    mSelectedSceneDefinitionHandle = currentProject->getStartupSceneDefinition();
    mDreamProjectModel->setSelectedSceneDefinition(mSelectedSceneDefinitionHandle);

    if (mSelectedSceneDefinitionHandle != nullptr)
    {
        log->debug("Set selected scene to startup scene {}",mSelectedSceneDefinitionHandle->getNameAndUuidString());
    }
    else
    {
        log->error("Could not find startup scene");
    }

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
ApplicationController::getSceneNameFromModelIndex
(int index)
{
    return mSceneListModel->stringList().at(index).toStdString();
}

void
ApplicationController::onAction_Debug_DumpProjectDefinitionJson
(bool toggled)
{
    Q_UNUSED(toggled)
    cout << mDreamProjectModel
            ->getProject()
            ->getProjectDefinition()
            ->getJson().dump(1) << endl;
}

void
ApplicationController::onAssetDefinitionProperty_AudioEvents
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    mMainWindowHandle.openAudioEventEditor(dynamic_cast<AudioDefinition*>(adHandle));
}


void
ApplicationController::onAssetDefinitionProperty_AudioFile
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(mLastDirectory);

    QStringList filters;
    filters << "*.wav" << "*.ogg";
    openDialog.setNameFilters(filters);

    if(openDialog.exec())
    {
        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);
        mLastDirectory.setPath(sourceFilePath);

        log->debug( "Using Audio file", sourceFilePath.toStdString());

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
                    (
                        &mMainWindowHandle,
                        "Overwrite Existing File?",
                        "An asset file all ready exists. Do you want to replace it?"
                        );
            if (result != QMessageBox::Yes)
            {
                log->debug( "Copy of {} was cancelled" , fileName.toStdString());
                return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                log->debug( "Error, unable to delete main asset file for {}",
                           adHandle->getNameAndUuidString());
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        if (copyResult)
        {
            QString fileName = QFileInfo(sourceFile).baseName();
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Use File Name for Asset?",
                "Do you want to give this Asset the same name as it's source file?"
            );

            if (result == QMessageBox::Yes)
            {
                adHandle->setName(fileName.toStdString());
            }
        }


        log->debug( "Copy ",
                   (copyResult ? "Success":"Failed"));

        //showImportResultDialog(copyResult, adHandle, sourceFile.fileName());

    }

    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::showImportResultDialog
(bool success, IAssetDefinition*  adHandle, QString source)
{
    if (!success)
    {
        ;
        QMessageBox::critical
                (
                    &mMainWindowHandle,
                    QString("Import Failed"),
                    QString("Unable to import\n\n%1\n\nnto\n\n%2")
                    .arg(source,QString::fromStdString(adHandle->getAssetPath()))
                    );
    }
    else
    {
        QMessageBox::information
                (
                    &mMainWindowHandle,
                    QString("Import Success"),
                    QString("Successfuly imported\n\n%1\n\nto\n\n%2")
                    .arg(source,QString::fromStdString(adHandle->getAssetPath()))
                    );
    }
}

void
ApplicationController::onAssetDefinitionProperty_FontFile
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(mLastDirectory);

    QStringList filters;
    filters << "*.ttf";
    openDialog.setNameFilters(filters);


    if(openDialog.exec())
    {

        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);
        mLastDirectory.setPath(sourceFilePath);

        log->debug( "Using Font file {}", sourceFilePath.toStdString());

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
                    (
                        &mMainWindowHandle,
                        "Overwrite Existing File?",
                        "An asset file all ready exists. Do you want to replace it?"
                        );
            if (result != QMessageBox::Yes)
            {
                log->debug( "Copy of {} was cancelled" , fileName.toStdString());
                return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                log->debug( "Error, unable to delete main asset file for {}"
                           ,adHandle->getNameAndUuidString()
                           );
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        log->debug( "Copy {} ",
                   (copyResult ? "Success":"Failed")
                   );
    }
}

void
ApplicationController::onAssetDefinitionProperty_MaterialEditor
(IAssetDefinition* adHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug("Edit Material");
    if (mSelectedProjectDefinitionHandle == nullptr)
    {
        log->debug("SelectedProjectDefinition Handle is nullptr, setting from directory model");
        mSelectedProjectDefinitionHandle = mProjectDirectoryModel.getProjectDefinition();
    }
    mMainWindowHandle.openMaterialEditor
    (
        dynamic_cast<MaterialDefinition*>(adHandle),
        mSelectedProjectDefinitionHandle->getShaderAssetDefinitionVector(),
        mSelectedProjectDefinitionHandle->getTextureAssetDefinitionVector()
    );
}

void
ApplicationController::onAssetDefinitionProperty_RemoveFiles
(IAssetDefinition*  adHandle)
{
    auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Remove Asset Data?",
                "Are you sure you want to delete the asset data?"
                );

    if (result == QMessageBox::Yes)
    {
        mProjectDirectoryModel.deleteAssetDataDirectory(adHandle);
        return;
    }

    mPropertiesTreeModel->forceDataChanged();
}

void ApplicationController::onAssetDefinitionProperty_ModelMaterialShaderMap
(IAssetDefinition* adHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug("Opening Material Map Widget");
    mModelMaterialTableController.setProjectPath(mProjectDirectoryModel.getProjectDirectoryAbsolutePath());
    mModelMaterialTableController.setModelDefinition(dynamic_cast<ModelDefinition*>(adHandle));
    if (mSelectedProjectDefinitionHandle == nullptr)
    {
        log->debug("SelectedProjectDefinition Handle is nullptr, setting from directory model");
        mSelectedProjectDefinitionHandle = mProjectDirectoryModel.getProjectDefinition();
    }
    mModelMaterialTableController.setMaterialHandlesVector(mSelectedProjectDefinitionHandle->getMaterialAssetDefinitionVector());
    auto dw = mMainWindowHandle.addRightDockWidget(&mModelMaterialTableController);
    auto labelText = QString("Material Map (%1)").arg(QString::fromStdString(adHandle->getName()));
    auto title = new QLabel(labelText);
    title->setMinimumHeight(24);
    dw->setTitleBarWidget(title);
    dw->setWindowTitle(labelText);
}

void
ApplicationController::onAssetDefinitionProperty_EditScript
(IAssetDefinition* adHandle)
{
    mMainWindowHandle.openScriptEditor(dynamic_cast<ScriptDefinition*>(adHandle));
}

void
ApplicationController::onAssetDefinitionProperty_EditShader
(IAssetDefinition* adHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "EditShader");
    mMainWindowHandle.openShaderEditor(dynamic_cast<ShaderDefinition*>(adHandle));
}

void
ApplicationController::onAssetDefinitionProperty_ScriptTemplateChanged
(IAssetDefinition* adHandle,  QString& templateName)
{
    // TODO - Move this to the script editor window
    auto log = spdlog::get("ApplicationController");
    log->debug( "Script Template Changed");
    auto result = QMessageBox::Yes;

    if (mProjectDirectoryModel.assetMainFileExists(adHandle))
    {
        result = QMessageBox::question
        (
            &mMainWindowHandle,
            "Load Script Template",
            "Are you sure you want to load this template? Any existing data will be lost."
        );
    }

    if (result == QMessageBox::Yes)
    {
        QString scriptContent = mTemplatesModel.getScriptTemplate(templateName,QString::fromStdString(adHandle->getFormat()));
        log->debug(
            "Proceeding to write template {} to {} \n {}",
            templateName.toStdString(),
            adHandle->getName(),
            scriptContent.toStdString()
        );
        bool writeOK = mProjectDirectoryModel.writeAssetData(scriptContent,adHandle,"",true);
        log->debug("Write template success? {}",writeOK);
        return;
    }
}

void
ApplicationController::onAssetDefinitionProperty_ShaderTemplateChanged
(IAssetDefinition* adHandle,  QString& templateName)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Shader Template Changed");

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

void ApplicationController::onAssetDefinitionProperty_LightChooseAmbient
(IAssetDefinition* def)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Choose Diffuse Colour");

    QColor currentColour;
    auto lightDef = dynamic_cast<LightDefinition*>(def);

    auto colour = lightDef->getAmbient();
    currentColour.setRgbF(
        static_cast<double>(colour.r),
        static_cast<double>(colour.g),
        static_cast<double>(colour.b)
    );

    auto chosenColour = QColorDialog::getColor (
        currentColour,
        &mMainWindowHandle,
        "Ambient Colour"
    );

    if (chosenColour.isValid())
    {
        lightDef->setAmbient(
            vec3(
                static_cast<float>(chosenColour.redF()),
                static_cast<float>(chosenColour.greenF()),
                static_cast<float>(chosenColour.blueF())
            )
        );
    }

    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::onAssetDefinitionProperty_LightChooseDiffuse
(IAssetDefinition*  def)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Choose Diffuse Colour");

    QColor currentColour;
    auto lightDef = dynamic_cast<LightDefinition*>(def);

    auto colour = lightDef->getDiffuse();
    currentColour.setRgbF(
        static_cast<double>(colour.r),
        static_cast<double>(colour.g),
        static_cast<double>(colour.b)
    );

    auto chosenColour = QColorDialog::getColor
    (
        currentColour,
        &mMainWindowHandle,
        "Diffuse Colour"
    );

    if (chosenColour.isValid())
    {
        lightDef->setDiffuse(
            vec3(
                static_cast<float>(chosenColour.redF()),
                static_cast<float>(chosenColour.greenF()),
                static_cast<float>(chosenColour.blueF())
            )
        );
    }

    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::onAssetDefinitionProperty_LightChooseSpecular
(IAssetDefinition*  def)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "Choose Specular Colour");

    QColor currentColour;
    auto lightDef = dynamic_cast<LightDefinition*>(def);

    auto colour = lightDef->getSpecular();
    currentColour.setRgbF(
        static_cast<double>(colour.r),
        static_cast<double>(colour.g),
        static_cast<double>(colour.b)
    );

    auto chosenColour = QColorDialog::getColor (
        currentColour,
        &mMainWindowHandle,
        "Specular Colour"
    );

    if (chosenColour.isValid())
    {
        lightDef->setSpecular(
            vec3(
                static_cast<float>(chosenColour.redF()),
                static_cast<float>(chosenColour.greenF()),
                static_cast<float>(chosenColour.blueF())
            )
        );
    }

    mPropertiesTreeModel->forceDataChanged();
}


void
ApplicationController::onAssetDefinitionProperty_PathList
(IAssetDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug("Opening PathList Widget");
    mPathEditorFormController.setPathDefinition(dynamic_cast<PathDefinition*>(adHandle));
    mPathPointViewer->setPathDefinition(dynamic_cast<PathDefinition*>(adHandle));
    mMainWindowHandle.setPathEditorFormControllerHandle(&mPathEditorFormController);
    auto dw = mMainWindowHandle.addRightDockWidget(&mPathEditorFormController);
    auto title = new QLabel(QString("Path Editor (%1)").arg(QString::fromStdString(adHandle->getName())));
    title->setMinimumHeight(24);
    dw->setTitleBarWidget(title);
}

void
ApplicationController::onAssetDefinitionProperty_TextureFile
(IAssetDefinition* adHandle)
{
    auto log = spdlog::get("ApplicationController");
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(mLastDirectory);

    QStringList filters;
    filters << "*.png" << "*.bmp" << "*.tga";
    openDialog.setNameFilters(filters);


    if(openDialog.exec())
    {
        QString sourceFilePath = openDialog.selectedFiles().first();
        QFile sourceFile(sourceFilePath);
        mLastDirectory.setPath(sourceFilePath);

        log->debug( "Using Texture file {}", sourceFilePath.toStdString());

        bool fileExists = mProjectDirectoryModel.assetMainFileExists(adHandle);

        if (fileExists)
        {
            QString fileName = QFileInfo(sourceFile).fileName();
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Overwrite Existing File?",
                "An asset file all ready exists. Do you want to replace it?"
            );

            if (result != QMessageBox::Yes)
            {
                log->debug( "Copy of {} was cancelled" , fileName.toStdString());
                return;
            }

            if (!mProjectDirectoryModel.deleteMainAssetFile(adHandle))
            {
                log->debug
                (
                    "Error, unable to delete main asset file for {}",
                    adHandle->getNameAndUuidString()
                );
                return;
            }
        }

        bool copyResult = mProjectDirectoryModel.copyMainAssetFile(adHandle,sourceFile);

        if (copyResult)
        {
            QString fileName = QFileInfo(sourceFile).baseName();
            auto result = QMessageBox::question
            (
                &mMainWindowHandle,
                "Use File Name for Asset?",
                "Do you want to give this Asset the same name as it's source file?"
            );

            if (result == QMessageBox::Yes)
            {
                adHandle->setName(fileName.toStdString());
            }
        }


        log->debug
        (
            "Copy {} ",
            (copyResult ? "Success":"Failed")
        );
    }
}

void
ApplicationController::onSceneObjectProperty_CaptureTranslation
(SceneObjectDefinition* sodHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "CaptureTranslation");
    Project* pHandle = mDreamProjectModel->getProject();
    if (pHandle != nullptr)
    {
        ProjectRuntime* prHandle = pHandle->getProjectRuntime();
        if (prHandle != nullptr)
        {
            SceneRuntime* srHandle = prHandle->getActiveSceneRuntime();
            if (srHandle != nullptr)
            {
                SceneObjectRuntime* sorHandle = srHandle->getSceneObjectRuntimeByUuid(sodHandle->getUuid());
                if (sorHandle != nullptr)
                {
                    sodHandle->getTransform()->setTranslation(sorHandle->getTranslation());
                }
            }
        }
    }

    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::onSceneObjectProperty_CaptureOrientation
(SceneObjectDefinition* sodHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "CaptureOrientation");
    Project* pHandle = mDreamProjectModel->getProject();
    if (pHandle)
    {
        ProjectRuntime* prHandle = pHandle->getProjectRuntime();
        if (prHandle != nullptr)
        {
            SceneRuntime* srHandle = prHandle->getActiveSceneRuntime();
            if (srHandle != nullptr)
            {
                SceneObjectRuntime* sorHandle = srHandle->getSceneObjectRuntimeByUuid(sodHandle->getUuid());
                if (sorHandle != nullptr)
                {
                    sodHandle->getTransform()->setOrientation(sorHandle->getOrientation());
                }
            }
        }
    }

    mPropertiesTreeModel->forceDataChanged();
}

void
ApplicationController::onSceneObjectProperty_CaptureScale
(SceneObjectDefinition* sodHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug( "CaptureScale");
    Project* pHandle = mDreamProjectModel->getProject();
    if (pHandle)
    {
        ProjectRuntime* prHandle = pHandle->getProjectRuntime();
        if (prHandle != nullptr)
        {
            SceneRuntime* srHandle = prHandle->getActiveSceneRuntime();
            if (srHandle != nullptr)
            {
                SceneObjectRuntime* sorHandle = srHandle->getSceneObjectRuntimeByUuid(sodHandle->getUuid());
                if (sorHandle != nullptr)
                {
                    sodHandle->getTransform()->setScale(sorHandle->getScale());
                }
            }
        }
    }

   mPropertiesTreeModel->forceDataChanged();

}

void
ApplicationController::onSceneObjectProperty_RemoveAsset
(SceneObjectDefinition* sodHandle,IDefinition*  adHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug(
                "RemoveAsset {} from {}",
                adHandle->getNameAndUuidString(),
                sodHandle->getNameAndUuidString()
                );
    sodHandle->removeAssetDefinitionFromLoadQueue(dynamic_cast<IAssetDefinition*>(adHandle));
    mPropertiesTreeModel->forceDataChanged();
    mMainWindowHandle.getPropertiesTreeView()->update();
}

void
ApplicationController::onSceneObjectProperty_RemoveChild
(SceneObjectDefinition* sodHandle, IDefinition* sodChildHandle)
{
    auto log = spdlog::get("ApplicationController");
    log->debug(
                "RemoveChild {} from {}",
                sodChildHandle->getNameAndUuidString(),
                sodHandle->getNameAndUuidString()
                );
    sodHandle->removeChildSceneObjectDefinition(dynamic_cast<SceneObjectDefinition*>(sodChildHandle));
    onUI_ScenegraphUpdated();
    mPropertiesTreeModel->forceDataChanged();
}
