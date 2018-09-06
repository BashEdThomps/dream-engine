/*
 * MainController.h
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

#pragma once

#include <memory>

#include <QObject>
#include <QErrorMessage>
#include <QStringList>
#include <QStringListModel>
#include <QItemSelection>
#include <QSurfaceFormat>

#include "PreferencesDialogController.h"
#include "ScriptEditorController.h"

#include "../Model/DreamProjectModel.h"
#include "../Model/ProjectDirectoryModel.h"
#include "../Model/TreeModels/Scenegraph/ScenegraphTreeModel.h"
#include "../Model/TreeModels/AssetDefinition/AssetDefinitionTreeModel.h"
#include "../Model/TreeModels/Properties/AbstractPropertiesModel.h"
#include "../Model/MacOSOpenModel.h"
#include "../Model/TemplatesModel.h"

#include "../Controller/MainWindowController.h"
#include "../Controller/PathEditorFormController.h"
#include "../Controller/MaterialShaderFormController.h"
#include "../View/GLView/Grid.h"
#include "../View/GLView/SelectionHighlighter.h"
#include "../View/GLView/RelationshipTree.h"
#include "../View/GLView/PathPointViewer.h"

using std::string;
using Dream::SceneDefinition;
using Dream::SceneRuntime;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController(MainWindowController* parent);
    ~MainController();

    Grid *getGridHandle();
    SelectionHighlighter *getSelectionHighlighterHandle();
    RelationshipTree *getRelationshipTreeHandle();
    PathPointViewer* getPathPointViewerHandle();
    void forceScenegraphTreeDataChanged();

signals:
    void notifyProjectDirectoryChanged(QString projectDir);
    void notifyInvalidProjectDirectory(QString projectDir);
    void notifyProjectNameChanged(QString name);
    void notifyProjectAuthorChanged(QString author);
    void notifyProjectDescriptionChanged(QString description);
    void notifyProjectWindowWidthChanged(int width);
    void notifyProjectWindowHeightChanged(int height);
    void notifyProjectStartupSceneChanged(QString startupScene);
    void notifyProjectSceneListChanged(QStringListModel *model);
    void notifyProjectWidgetsEnabledChanged(bool enabled);
    void notifyStatusBarProjectLoaded(QString);
    void notifyNoSceneSelected();
    void notifyPlayingScene(shared_ptr<SceneRuntime> scene);
    //void notifyStoppedScene(SceneDefinition* scene);

    void notifyScenegraphUpdated();
    void notifyProjectDefinitionChanged(shared_ptr<ProjectDefinition>);

public slots:
    void onScenegraphTreeDataChanged();
    void onAssetDefinitionTreeDataChanged();
    void onPropertiesTreeDataChanged();
    void onAction_Preferences();
    void onAction_File_New();
    void onAction_File_Open();
    void onAction_File_Save();
    void onAction_File_Close();

    void onAction_Scene_Reload();
    void onAction_Scene_Play();
    void onAction_Scene_Stop();
    void onAction_ControlSceneTriggered(bool);

    void onUI_ProjectNameChanged(QString name);
    void onUI_ProjectAuthorChanged(QString author);
    void onUI_ProjectDescriptionChanged(QString desc);
    void onUI_ProjectWindowWidthChanged(QString width);
    void onUI_ProjectWindowHeightChanged(QString height);
    void onUI_ProjectStartupSceneChanged(QString startupScene);

    void onUI_ScenegraphTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&);
    void onUI_AssetDefinitionTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&);
    void onUI_SelectedSceneChanged(shared_ptr<SceneDefinition> scene);

    void onAction_View_ToggleGrid(bool enabled);
    void onAction_View_ToggleDebug(bool enabled);
    void onAction_View_TogglePhysicsDebug(bool enable);
    void onAction_View_ToggleSelectionHighlighter(bool enabled);

    void onAction_Scene_NewScene();
    void onAction_Scene_NewSceneObject();

    void onUI_ScenegraphUpdated();
    void onUI_AssetDefinitionsUpdated();

    void onAction_Asset_AddToSelectedSceneObjectDefinition();

    void onAction_Asset_NewDefinition_Path();
    void onAction_Asset_NewDefinition_Audio();
    void onAction_Asset_NewDefinition_Font();
    void onAction_Asset_NewDefinition_Light();
    void onAction_Asset_NewDefinition_Model();
    void onAction_Asset_NewDefinition_PhysicsObject();
    void onAction_Asset_NewDefinition_Script();
    void onAction_Asset_NewDefinition_Shader();
    void onAction_Asset_NewDefinition_Sprite();
    void onAction_Debug_DumpProjectDefinitionJson(bool toggled);

    void onAssetDefinitionProperty_AudioFile(shared_ptr<IAssetDefinition> adHandle);

    void onAssetDefinitionProperty_FontFile(shared_ptr<IAssetDefinition> adHandle);

    void onAssetDefinitionProperty_ModelFile(shared_ptr<IAssetDefinition> adHandle);
    void onAssetDefinitionProperty_ModelAdditionalFiles(shared_ptr<IAssetDefinition> adHandle);
    void onAssetDefinitionProperty_RemoveFiles(shared_ptr<IAssetDefinition>adHandle);
    void onAssetDefinitionProperty_ModelMaterialShaderMap(shared_ptr<IAssetDefinition> adHandle);

    void onAssetDefinitionProperty_EditScript(shared_ptr<IAssetDefinition> adHandle);
    void onAssetDefinitionProperty_EditShader(shared_ptr<IAssetDefinition> adHandle);

    void onAssetDefinitionProperty_ScriptTemplateChanged(shared_ptr<IAssetDefinition>, const QString&);
    void onAssetDefinitionProperty_ShaderTemplateChanged(shared_ptr<IAssetDefinition>, const QString&);

    void onAssetDefinitionProperty_LightChooseAmbient(shared_ptr<IAssetDefinition>);
    void onAssetDefinitionProperty_LightChooseDiffuse(shared_ptr<IAssetDefinition>);
    void onAssetDefinitionProperty_LightChooseSpecular(shared_ptr<IAssetDefinition>);
    void onAssetDefinitionProperty_PathList(shared_ptr<IAssetDefinition>);

    void onSceneObjectProperty_CaptureTranslation(shared_ptr<SceneObjectDefinition>);
    void onSceneObjectProperty_CaptureOrientation(shared_ptr<SceneObjectDefinition>);
    void onSceneObjectProperty_CaptureScale(shared_ptr<SceneObjectDefinition>);

    void onSceneObjectProperty_RemoveAsset(shared_ptr<SceneObjectDefinition>, shared_ptr<IDefinition>);
    void onSceneObjectProperty_RemoveChild(shared_ptr<SceneObjectDefinition>, shared_ptr<IDefinition>);

    void onAssetDefinitionProperty_PhysicsBvhTriangleMeshFile(shared_ptr<IAssetDefinition> adHandle);

    void onSceneProperty_CaptureCameraTranslation(shared_ptr<SceneDefinition>);
    void onSceneProperty_CaptureCameraLookAt(shared_ptr<SceneDefinition>);
    void onSceneProperty_ChooseAmbientColour(shared_ptr<SceneDefinition>);
    void onSceneProperty_ChooseClearColour(shared_ptr<SceneDefinition>);

    void onCreateNewAssetDefinition(QString type);
    void onMainVolumeChanged(int);
    void onPathEditorClosed();
    void onPathVisibilityChanged(bool visible);


protected:
    void showImportResultDialog(bool success, shared_ptr<IAssetDefinition> adHandle, QString source);
    void forceAssetDefinitionTreeDataChanged();
private: // Methods
    // Setup
    void setupUI();
    void setupUI_GLWidgets();
    void setupUI_ScenegraphPropertiesTreeViewModel(ScenegraphTreeItem *item);
    void setupUI_AssetDefinitionPropertiesTreeViewModel(AssetDefinitionTreeItem *item);

    void setupConnections();

    // Misc
    void openProject();
    void updateWindowTitle(QString msg);

    void setActionsEnabled_ValidProject(bool enabled);

    QStringListModel* getSceneNamesListModel(vector<shared_ptr<SceneDefinition>> sceneList);

    // Signal/Slot Connections
    void connectMenus();

    void connectPreferences();
    void connectFileMenu();
    void connectSceneMenu();
    void connectAssetMenu();
    void connectViewMenu();
    void connectDebugMenu();

    void connectUI();
    void connectUI_TreeViewModels();

    string getSceneNameFromModelIndex(int index);

private: // Variables
    MainWindowController *mMainWindowHandle;
    shared_ptr<QOpenGLWindowComponent> mWindowComponent;

    shared_ptr<ProjectDefinition> mSelectedProjectDefinitionHandle;
    shared_ptr<IAssetDefinition> mSelectedAssetDefinitionHandle;
    shared_ptr<SceneDefinition> mSelectedSceneDefinitionHandle;
    shared_ptr<SceneObjectDefinition> mSelectedSceneObjectDefinitionHandle;

    QString mProjectDirectory;
    ProjectDirectoryModel mProjectDirectoryModel;
    PreferencesDialogController mPreferencesDialogController;
    TemplatesModel mTemplatesModel;

    unique_ptr<QErrorMessage> mInvalidProjectDirectoryError;
    unique_ptr<QStringListModel> mSceneListModel;

    unique_ptr<AssetDefinitionTreeModel> mAssetDefinitionTreeModel;
    unique_ptr<ScenegraphTreeModel> mScenegraphTreeModel;
    unique_ptr<AbstractPropertiesModel> mPropertiesModel;

    unique_ptr<Grid> mGrid;
    unique_ptr<SelectionHighlighter> mSelectionHighlighter;
    unique_ptr<RelationshipTree> mRelationshipTree;
    unique_ptr<PathPointViewer> mPathPointViewer;

    // Delete anything that relies on project before DreamModel
    unique_ptr<DreamProjectModel> mDreamProjectModel;
    unique_ptr<MacOSOpenModel> mMacOsOpenModel;

    ScriptEditorController mScriptEditor;
    QDir mLastDirectory;
    MaterialShaderFormController mMaterialShaderTableController;
    PathEditorFormController mPathEditorFormController;
    void connectPathEditorSignals();
};
