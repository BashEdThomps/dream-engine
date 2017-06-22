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

#include "../Model/DreamProjectModel.h"
#include "../Model/ProjectDirectoryModel.h"
#include "../Model/PreferencesModel.h"
#include "../Model/TreeModels/ProjectTreeModel.h"
#include "../Model/TreeModels/AssetDefinitionTreeModel.h"
#include "../Model/TreeModels/Properties/PropertiesModel.h"

#include "../View/MainWindow.h"
#include "../View/GLView/Grid.h"
#include "../View/GLView/SelectionHighlighter.h"
#include "../View/GLView/RelationshipTree.h"

using std::string;
using Dream::SceneDefinition;
using Dream::SceneRuntime;

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController(MainWindow* parent);
    ~MainController();

    Grid *getGrid();
    SelectionHighlighter *getSelectionHighlighter();
    RelationshipTree *getRelationshipTree();

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
    void notifyPlayingScene(SceneRuntime* scene);
    void notifyStoppedScene(SceneDefinition* scene);

public slots:
    void onProjectNewAction();
    void onProjectOpenAction();
    void onProjectSaveAction();
    void onProjectReloadAction();
    void onProjectPlayAction();
    void onProjectStopAction();
    void onProjectOpenTestProjectAction();
    void onProjectClosedAction();

    void onProjectNameChanged(QString name);
    void onProjectAuthorChanged(QString author);
    void onProjectDescriptionChanged(QString desc);
    void onProjectWindowWidthChanged(QString width);
    void onProjectWindowHeightChanged(QString height);
    void onProjectStartupSceneChanged(QString startupScene);
    void onTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&);

    void onSelectedSceneChanged(SceneDefinition *scene);
    void onSceneStopped(SceneDefinition* scene);

    void onGridToggleAction(bool enabled);
    void onToggleDebugAction(bool enabled);
    void onTogglePhysicsDebugAction(bool enable);

    void onScenegraphAddSceneAction();
    void onScenegraphAddSceneObjectAction();
    void onPropertiesAddAssetAction();
    void onPropertiesAddSceneObjectChildAction();

private: // Methods
    void openProject();
    void updateWindowTitle(QString msg);

    void setValidProjectActionsEnabled(bool enabled);

    void createConnections();
    void setupMenuActionConnections();
    void setupPropertiesTreeViewModel(GenericTreeItem *item);

    QStringListModel* getSceneNamesListModel(vector<SceneDefinition*> sceneList);
    void connectTreeViewModel();
    string getSceneNameFromModelIndex(int index);


private: // Variables
    MainWindow *mMainWindowHandle;
    QOpenGLWindowComponent *mWindowComponentHandle;
    QString mProjectDirectory;
    ProjectDirectoryModel mProjectDirectoryModel;
    PreferencesModel mPreferencesModel;
    unique_ptr<QErrorMessage> mInvalidProjectDirectoryError;
    unique_ptr<QStringListModel> mSceneListModel;
    unique_ptr<ProjectTreeModel> mProjectTreeModel;
    unique_ptr<AssetDefinitionTreeModel> mAssetDefinitionTreeModel;
    unique_ptr<PropertiesModel> mPropertiesModel;
    unique_ptr<DreamProjectModel> mDreamModel;
    unique_ptr<Grid> mGrid;
    unique_ptr<SelectionHighlighter> mSelectionHighlighter;
    unique_ptr<RelationshipTree> mRelationshipTree;
};
