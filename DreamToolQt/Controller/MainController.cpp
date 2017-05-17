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

MainController::MainController(MainWindow* parent)
    : QObject(parent)
{
    mMainWindow = parent;
    mAudioComponent = new QTDreamAudioComponent();
    mWindowComponent = new QTDreamWindowComponent(parent->getOpenGLWidget());
    mDreamModel = new DreamModel(this,mAudioComponent,mWindowComponent);
    createConnections();
}

MainController::~MainController()
{

}

void MainController::createConnections()
{
    // actionNew
    connect(
                mMainWindow->getActionNew(), SIGNAL(triggered()),
                this, SLOT(onNewProjectButtonClicked())
                );
    // actionOpen
    connect(
                mMainWindow->getActionOpen(), SIGNAL(triggered()),
                this, SLOT(onOpenProjectButtonClicked())
                );
    // Invalid Project Directory
    connect(
                this, SIGNAL(notifyInvalidProjectDirectory(QString)),
                mMainWindow, SLOT(onInvalidProjectDirectory(QString))
                );
    // Project Directory Changed
    connect(
                this, SIGNAL(notifyProjectDirectoryChanged(QString)),
                mMainWindow, SLOT(setWindowTitle(QString))
                );
    // Status Bar
    connect(
                this, SIGNAL(notifyStatusBarProjectLoaded(QString)),
                mMainWindow, SLOT(showStatusBarMessage(QString))
                );
}

void MainController::onNewProjectButtonClicked()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();
        updateWindowTitle(mProjectDirectory);
    }
}

void MainController::updateWindowTitle(QString msg)
{
    emit notifyProjectDirectoryChanged("Dream Tool :: " + msg);
}

void MainController::onOpenProjectButtonClicked()
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
    cout << "LoadResult " << loadResult << endl;
    if (!loadResult)
    {
        emit notifyInvalidProjectDirectory(mProjectDirectory);
        emit notifyProjectWidgetsEnabledChanged(false);
        return;
    }
    updateWindowTitle(mProjectDirectory);
    Dream::Project *currentProject = mDreamModel->getProject();

    emit notifyProjectNameChanged(QString::fromStdString(currentProject->getName()));
    emit notifyProjectAuthorChanged(QString::fromStdString(currentProject->getAuthor()));
    emit notifyProjectDescriptionChanged(QString::fromStdString(currentProject->getDescription()));
    emit notifyProjectWindowWidthChanged(currentProject->getWindowWidth());
    emit notifyProjectWindowHeightChanged(currentProject->getWindowHeight());
    emit notifyProjectSceneListChanged(getSceneNamesListModel(currentProject->getSceneList()));
    emit notifyProjectStartupSceneChanged(QString::fromStdString(currentProject->getStartupScene()->getName()));
    emit notifyProjectWidgetsEnabledChanged(true);
    Dream::Project *project = mDreamModel->getProject();
    mProjectTreeModel = new ProjectTreeModel(project,mMainWindow->getProjectTreeView());
    mMainWindow->getProjectTreeView()->setModel(mProjectTreeModel);
    emit notifyStatusBarProjectLoaded(
                QString::fromStdString(
                    "Successfuly Loaded Project: " +
                    project->getName() + " (" +
                    project->getUuid() + ")"
                    )
                );
    connectTreeViewModel();
}

void MainController::connectTreeViewModel()
{
    // Tree View
    connect(
                mMainWindow->getProjectTreeView()->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
                this, SLOT(onTreeViewSelectionChanged(const QItemSelection&,const QItemSelection&))
                );

}

void MainController::onSaveProjectButtonClicked()
{

}

QStringListModel* MainController::getSceneNamesListModel(vector<Dream::Scene*> sceneList)
{
    QStringList sceneNameList;
    mSceneListModel = new QStringListModel(mMainWindow);

    for (auto scene : sceneList)
    {
        sceneNameList.push_back(QString::fromStdString(scene->getName()));
    }

    mSceneListModel->setStringList(sceneNameList);
    return mSceneListModel;
}

void MainController::onProjectNameChanged(QString name)
{
    qDebug() << "Name set to " << name;
    mDreamModel->setProjectName(name.toStdString());
}

void MainController::onProjectAuthorChanged(QString author)
{
    qDebug() << "Author set to " << author;
    mDreamModel->setProjectAuthor(author.toStdString());
}

void MainController::onProjectDescriptionChanged(QString desc)
{
    qDebug() << "Description set to " << desc;
    mDreamModel->setProjectDescription(desc.toStdString());
}

void MainController::onProjectWindowWidthChanged(QString width)
{
    qDebug() << "Window Width set to " << width;
    mDreamModel->setProjectWindowWidth(width.toInt());
}

void MainController::onProjectWindowHeightChanged(QString height)
{
    qDebug() << "Window Height set to " << height;
    mDreamModel->setProjectWindowHeight(height.toInt());
}

void MainController::onProjectStartupSceneChanged(QString startupSceneIndex)
{
    string sceneName = getSceneNameFromModelIndex(startupSceneIndex.toInt());
    qDebug() << "startupScene set to " << startupSceneIndex << " " << QString::fromStdString(sceneName);
    mDreamModel->setProjectStartupSceneByName(sceneName);
}

string MainController::getSceneNameFromModelIndex(int index)
{
    return mSceneListModel->stringList().at(index).toStdString();
}

void MainController::onTreeViewSelectionChanged(const QItemSelection& selected,const QItemSelection& deselected)
{
    qDebug() << "Selected tree item changed!";
    QModelIndexList indexes = selected.indexes();
    for (QModelIndex index : indexes)
    {
        ProjectTreeItem *selected = static_cast<ProjectTreeItem*>(index.internalPointer());
        switch(selected->getItemType())
        {
            case ProjectItemType::PROJECT:
                qDebug() << "Selected a project";
                break;
            case ProjectItemType::ASSET_DEFINITION:
                qDebug() << "Selected an asset definition";
                break;
            case ProjectItemType::SCENE:
                qDebug() << "Selected a scene";
                break;
            case ProjectItemType::SCENE_OBJECT:
                qDebug() << "Selected a scene object";
                break;
            case ProjectItemType::TREE_NODE:
                qDebug() << "Selected a tree node";
                break;
        }
    }
}
