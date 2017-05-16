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
#include "../ui_MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QErrorMessage>

MainController::MainController(MainWindow* parent) : QObject(parent)
{
    mMainWindow = parent;
    mDreamModel = new DreamModel(this);
    createConnections();
}

MainController::~MainController()
{

}

void MainController::createConnections()
{
    // Project Tab Widgets Enabled State
    connect(
                this,SIGNAL(notifyProjectWidgetsEnabledChanged(bool)),
                mMainWindow,SLOT(onProjectWidgetsEnabledChanged(bool)));
    // actionNew
    connect(
                mMainWindow->ui->actionNew, SIGNAL(triggered(bool)),
                this, SLOT(onNewProjectButtonClicked(bool))
                );
    // actionOpen
    connect(
                mMainWindow->ui->actionOpen, SIGNAL(triggered(bool)),
                this, SLOT(onOpenProjectButtonClicked(bool))
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
    // Project Name Line Edit
    connect(
                this, SIGNAL(notifyProjectNameChanged(QString)),
                mMainWindow->ui->nameEdit,SLOT(setText(QString))
                );
    connect(
                mMainWindow->ui->nameEdit,SIGNAL(textEdited(QString)),
                this,SLOT(onProjectNameChanged(QString))
                );
    // Project Author Line Edit
    connect(
                this, SIGNAL(notifyProjectAuthorChanged(QString)),
                mMainWindow->ui->authorEdit, SLOT(setText(QString))
                );
    connect(
                mMainWindow->ui->authorEdit, SIGNAL(textEdited(QString)),
                this,SLOT(onProjectAuthorChanged(QString))
                );
    // Project Description
    connect(
                this, SIGNAL(notifyProjectDescriptionChanged(QString)),
                mMainWindow->ui->descriptionEdit, SLOT(setText(QString))
                );
    connect(
                mMainWindow->ui->descriptionEdit,SIGNAL(textEdited(QString)),
                this,SLOT(onProjectDescriptionChanged(QString))
                );
    // Window Dimensions
    connect(
                this, SIGNAL(notifyProjectWindowWidthChanged(int)),
                mMainWindow->ui->windowWidthSpinBox, SLOT(setValue(int))
                );
    connect(
                this, SIGNAL(notifyProjectWindowHeightChanged(int)),
                mMainWindow->ui->windowHeightSpinBox, SLOT(setValue(int))
                );
    connect(
                mMainWindow->ui->windowWidthSpinBox,SIGNAL(valueChanged(QString)),
                this,SLOT(onProjectWindowWidthChanged(QString))
                );
    connect(
                mMainWindow->ui->windowHeightSpinBox,SIGNAL(valueChanged(QString)),
                this,SLOT(onProjectWindowHeightChanged(QString))
                );
    // Startup Scene
    connect(
                this, SIGNAL(notifyProjectSceneListChanged(QStringListModel*)),
                mMainWindow, SLOT(onStartupSceneModelChanged(QStringListModel*))
                );
    connect(
                mMainWindow->ui->startupSceneComboBox,SIGNAL(currentIndexChanged(QString)),
                this,SLOT(onProjectWindowHeightChanged(QString))
                );
    // Tab Changed
    connect(
                mMainWindow->ui->tabWidget, SIGNAL(currentChanged(int)),
                this, SLOT(onTabChanged(int))
    );
    // Status Bar
    connect(
                this, SIGNAL(notifyStatusBarProjectLoaded(QString)),
                mMainWindow->ui->statusBar, SLOT(showMessage(QString))
    );
}

void MainController::onNewProjectButtonClicked(bool checked)
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

void MainController::onOpenProjectButtonClicked(bool checked)
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);

    if(openDialog.exec())
    {
        mProjectDirectory = openDialog.selectedFiles().first();
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
    mProjectTreeModel = new ProjectTreeModel(project,mMainWindow->ui->treeView);
    mMainWindow->ui->treeView->setModel(mProjectTreeModel);
    emit notifyStatusBarProjectLoaded(
        QString::fromStdString(
            "Successfuly Loaded Project: " +
            project->getName() + " (" +
            project->getUuid() + ")"
        )
    );
}

void MainController::onSaveProjectButtonClicked(bool checked)
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


void MainController::onRefreshProjectButtonClicked(bool checked)
{

}

void MainController::onTabChanged(int tabIndex)
{
    qDebug() << "Selected tab: " << tabIndex;
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
