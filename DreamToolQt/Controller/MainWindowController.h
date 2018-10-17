/*
 * MainWindow.h
 *
 * Created: 13 2017 by Ashley
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
#include <vector>
#include <map>

#include <QDockWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringListModel>
#include <QTreeView>
#include <QSlider>
#include <QAction>
#include <QKeyEvent>

#include "../View/QOpenGLWindowComponent.h"
#include "../Model/TreeModels/AssetDefinition/AssetDefinitionTreeItem.h"
#include "../Model/TreeModels/Scenegraph/ScenegraphTreeItem.h"
#include "AbstractEditorWidget.h"
#include "TextEditorTabController.h"
#include "UIActions.h"

using std::unique_ptr;
using std::map;
using Dream::AssetType;

namespace Ui
{
    class MainWindow;
}

class PathEditorFormController;

class MainWindowController : public QMainWindow, public LuaPrintListener
{
    Q_OBJECT
public:
    explicit MainWindowController(QWidget *parent = nullptr);
    ~MainWindowController( ) override;

    void onPrint(std::string) override;

    QOpenGLWindowComponent* getWindowComponent();

    void setProjectDirectoryModel(ProjectDirectoryModel* dirModel);
    QDockWidget* addRightDockWidget(QWidget* widget);

    QTreeView* getScenegraphTreeView();
    QTreeView* getPropertiesTreeView();
    QTreeView* getAssetDefinitionTreeView();

    QAction* getAction_Preferences();

    QAction* getAction_File_New();
    QAction* getAction_File_Save();
    QAction* getAction_File_Open();
    QAction* getAction_File_CloseProject();

    void setActionEnabled_File_New(bool enabled);
    void setActionEnabled_File_Save(bool enabled);
    void setActionEnabled_File_Open(bool enabled);

    QAction* getAction_Scene_Stop();
    QAction* getAction_Scene_Play();
    QAction* getAction_Scene_Reload();
    QAction* getAction_Scene_NewScene();
    QAction* getAction_Scene_NewSceneObject();

    void setActionsEnabled_Scene_Playback(bool enabled);
    void setActionsEnabled_Scene_Modification(bool enabled);

    QAction* getAction_View_ToggleGrid();
    QAction* getAction_View_ToggleDebug();
    QAction* getAction_View_TogglePhysicsDebug();
    QAction* getAction_View_ToggleHighlightSelected();
    QAction* getAction_Asset_NewDefinition(AssetType type);
    QAction* getAction_Asset_AddToSelectedSceneObject();
    QAction* getAction_ControlScene();
    QAction* getAction_Debug_DumpProjectDefinitionJson();

    void setActionEnabled_Debug_DumpProjectDefinitionJson(bool enabled);
    void setupMenu_Asset_NewDefinition();
    void openScriptEditor(ScriptDefinition* scriptDefinitionHandle);
    void openShaderEditor(ShaderDefinition* shaderDefinitionHandle);
    void openMaterialEditor(MaterialDefinition*, vector<ShaderDefinition*>, vector<TextureDefinition*>);

    void openAudioEventEditor(AudioDefinition* adHandle);
    void setTemplatesModelHandle(TemplatesModel* templatesModelHandle);

    void setPathEditorFormControllerHandle(PathEditorFormController* pathEditorFormControllerHandle);

signals:
    void notifyActionNew(QString);
    void notifyActionOpen(QString);
    void notifyActionSave(QString);
    void notifyCreateNewAssetDefinition(QString type);
    void notifyScenegraphTreeDataChanged();
    void notifyPropertiesTreeDataChanged();
    void notifyMainVolumeChanged(int);
    void notifyAssetDefinitionTreeDataChanged();

    void notifyActionScriptingTriggered(bool checked);
    void notifyRightTabWidgetClosed();
    void notifyNewTextureCreated();

public slots:
    void onInvalidProjectDirectory(QString directory);
    void onNoSceneSelected();
    void showStatusBarMessage(QString msg);
    void onSceneStopped(SceneDefinition* scene);
    void onProjectDefinitionChanged(ProjectDefinition*);
    void onScenegraphTreeExpandRequested();
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void onRightTabWidgetClosed(QCloseEvent*);
    void onNewTextureCreated();

protected:
    void setupUiFeatures();
    int  isEditorTabOpen(IAssetDefinition* definition);
    void setupEditorTabCloseButtonSignal();
    shared_ptr<QMenu> createAssetDefinitionTreeContextMenu(AssetDefinitionTreeItem*);
    shared_ptr<QMenu> createScenegraphTreeContextMenu(ScenegraphTreeItem*);
    void createAssetsMenu(QMenu* menu,ScenegraphTreeItem* item);
    void setupMenu_Debug();
    bool shouldPassKey(int key);
    void setupGL(QWidget *parent);

protected slots:
    void onMainVolumeChanged(int);
    void onEditorTabCloseRequested(int index);
    void onScenegraphContextMenuRequested(const QPoint& point);
    void onAsseetDefinitionContextMenuRequested(const QPoint& point);
    void onCreateAssetDefinitionAction();
    void onScenegraphMenuAddNewSceneTriggered();
    void onScenegraphMenuDeleteSceneTriggered();
    void onScenegraphMenuAddSceneObjectTriggered();
    void onScenegraphMenuDuplicateSceneObjectTriggered();
    void onScenegraphMenuDeleteSceneObjectTriggered();
    void onAssetDefinitionMenuDeleteTriggered();
    void onAddAssetToSceneObjectTriggered();
    void onScenegraphTreeViewActivated(const QModelIndex &index);
    void onPropertiesTreeViewActivated(const QModelIndex &index);
    void onAssetDefinitionTreeViewActivated(const QModelIndex &index);
    void onMenu_Debug_LogLevelChanged(bool);
    void onActionLuaLogTriggered(bool);
    void onActionScriptingTriggered(bool);

private:
    const static vector<int> mKeysPassedToWindow;

    Ui::MainWindow* mUi;
    ProjectDefinition* mProjectDefinitionHandle;
    ProjectDirectoryModel* mProjectDirectoryModelHandle;
    TemplatesModel* mTemplatesModelHandle;
    QOpenGLWindowComponent* mWindowComponentHandle;
    map<AssetType,QAction*> mActionMap_Asset_NewDefinition;
    unique_ptr<QMenu> mMenu_Asset_NewDefinition;
    QSlider mVolumeSlider;
    QDockWidget mRightDockWidget;
    vector<shared_ptr<AbstractEditorWidget>> mEditorTabForms;
    int mLastVolume;
    PathEditorFormController* mPathEditorFormControllerHandle;
};


