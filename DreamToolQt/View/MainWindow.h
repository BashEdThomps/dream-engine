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

#include <GL/glew.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QStringListModel>
#include <QTreeView>
#include <QAction>
#include <QKeyEvent>

#include "../View/QOpenGLWindowComponent.h"
#include "../Model/TreeModels/AssetDefinition/AssetDefinitionTreeItem.h"
#include "../Model/TreeModels/Scenegraph/ScenegraphTreeItem.h"

using std::unique_ptr;
using std::map;
using Dream::AssetType;

namespace Ui
{
    class MainWindow;
}

class CreateAssetAction : public QAction
{
public:
    CreateAssetAction(QString type,QObject *parent = nullptr);
    CreateAssetAction(QString type,const QString &text, QObject *parent = nullptr);
    CreateAssetAction(QString type,const QIcon &icon, const QString &text, QObject *parent = nullptr);

    QString getType() const;

private:
    QString mType;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow( ) override;
    Ui::MainWindow *ui;

    QOpenGLWindowComponent* getWindowComponent();

    QTreeView* getScenegraphTreeView();
    QTreeView* getPropertiesTreeView();
    QTreeView* getAssetDefinitionTreeView();

    QAction* getAction_Preferences();

    QAction* getAction_File_New();
    QAction* getAction_File_Save();
    QAction* getAction_File_Open();
    QAction* getAction_File_CloseProject();
    QAction* getAction_File_OpenTestProject();

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

    QAction* getAction_Debug_DumpProjectDefinitionJson();
    void setActionEnabled_Debug_DumpProjectDefinitionJson(bool enabled);

    void setupMenu_Asset_NewDefinition();

    void clearOpenGLComponentRuntime();
private: // Methods
    bool shouldPassKey(int key);

signals:
    void notifyActionNew(QString);
    void notifyActionOpen(QString);
    void notifyActionSave(QString);
    void notifyCreateNewAssetDefinition(QString type);

public slots:
    void onInvalidProjectDirectory(QString directory);
    void onNoSceneSelected();
    void showStatusBarMessage(QString msg);
    void onSceneStopped(SceneDefinition* scene);

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

protected:
    void setupUiFeatures();
private slots:
    void onScenegraphContextMenuRequested(const QPoint& point);
    void onAsseetDefinitionContextMenuRequested(const QPoint& point);
    void onCreateAssetDefinitionAction();
private:
    void setupGL(QWidget *parent);
    QOpenGLWindowComponent* mWindowComponentHandle;
    const static vector<int> mKeysPassedToWindow;
    map<AssetType,QAction*> mActionMap_Asset_NewDefinition;
    unique_ptr<QMenu> mMenu_Asset_NewDefinition;
    shared_ptr<QMenu> createAssetDefinitionTreeContextMenu(AssetDefinitionTreeItem*);
    shared_ptr<QMenu> createScenegraphTreeContextMenu(ScenegraphTreeItem*);
    void createAssetsMenu(QMenu* menu);
};

