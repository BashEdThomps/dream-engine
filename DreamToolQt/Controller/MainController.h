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
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QErrorMessage>
#include <QStringList>
#include <QStringListModel>
#include <QItemSelection>
#include "../View/MainWindow.h"
#include "../Model/DreamModel.h"
#include "../Model/ProjectTreeModel.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController(MainWindow* parent);
    ~MainController();
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

public slots:
    void onProjectNewButtonClicked();
    void onProjectOpenButtonClicked();
    void onProjectSaveButtonClicked();
    void onProjectReloadButtonClicked();

    void onProjectNameChanged(QString name);
    void onProjectAuthorChanged(QString author);
    void onProjectDescriptionChanged(QString desc);
    void onProjectWindowWidthChanged(QString width);
    void onProjectWindowHeightChanged(QString height);
    void onProjectStartupSceneChanged(QString startupScene);
    void onProjectTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&);

private: // Variables
    MainWindow *mMainWindow;
    QTDreamWindowComponent *mWindowComponent;
    QTDreamAudioComponent  *mAudioComponent;
    DreamModel *mDreamModel;
    QString mProjectDirectory;
    QErrorMessage *mInvalidProjectDirectoryError;
    QStringListModel *mSceneListModel;
    ProjectTreeModel *mProjectTreeModel;
private: // Methods
    void setupPropertiesTreeViewModel(ProjectTreeItem *item);
    void createConnections();
    void updateWindowTitle(QString msg);
    void connectTreeViewModel();
    QStringListModel* getSceneNamesListModel(vector<Dream::Scene*> sceneList);
    string getSceneNameFromModelIndex(int index);
};

#endif // MAINCONTROLLER_H
