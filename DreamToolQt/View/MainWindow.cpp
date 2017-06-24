/*
 * MainWindow.cpp
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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QWindow>
#include <QDebug>
#include <vector>
#include <algorithm>

#include "View/QOpenGLWindowComponent.h"

using std::pair;
using std::begin;
using std::find;
using std::end;

const vector<int> MainWindow::mKeysPassedToWindow =
{
    Qt::Key_W,
    Qt::Key_A,
    Qt::Key_S,
    Qt::Key_D,
    Qt::Key_Up,
    Qt::Key_Down,
    Qt::Key_Left,
    Qt::Key_Right,
    Qt::Key_Shift
};

MainWindow::MainWindow
(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGL(parent);
    setupMenu_Asset_NewDefinition();
    setActionsEnabled_Scene_Playback(false);
    setActionEnabled_File_Save(false);
}

void
MainWindow::setActionsEnabled_Scene_Playback
(bool enabled)
{
    ui->actionPlay->setEnabled(enabled);
    ui->actionStop->setEnabled(enabled);
    ui->actionReload->setEnabled(enabled);
}

void
MainWindow::setActionEnabled_File_Open
(bool enabled)
{
    ui->actionOpen->setEnabled(enabled);
}

QAction*
MainWindow::getAction_Asset_NewDefinition
(AssetType type)
{
    return mActionMap_Asset_NewDefinition.at(type);
}

QAction*
MainWindow::getAction_Scene_NewScene
()
{
    return ui->actionSceneMenuNewScene;
}

QAction*
MainWindow::getAction_Scene_NewSceneObject
()
{
    return ui->actionSceneNewSceneObject;
}

QAction*
MainWindow::getAction_Asset_AddToSelectedSceneObject
()
{
    return ui->actionAssetMenuAddAssetToSelectedSceneObject;
}

void
MainWindow::setActionEnabled_File_Save
(bool enabled)
{
    ui->actionSave->setEnabled(enabled);
}

void
MainWindow::setActionEnabled_File_New
(bool enabled)
{
    ui->actionNew->setEnabled(enabled);
}

void
MainWindow::setupMenu_Asset_NewDefinition
()
{
    mMenu_Asset_NewDefinition.reset(new QMenu());
    mMenu_Asset_NewDefinition->setTitle("New Definition");

    for (pair<AssetType,string> typePair : Constants::DREAM_ASSET_TYPES_MAP)
    {
        QAction *typeAction = new QAction
        (
            QString::fromStdString
            (
                Constants::getAssetTypeReadableName(typePair.second)
            )
        );
        mMenu_Asset_NewDefinition->addAction(typeAction);
        mActionMap_Asset_NewDefinition.insert(pair<AssetType,QAction*>(typePair.first, typeAction));
    }

    ui->menuAsset->addMenu(mMenu_Asset_NewDefinition.get());
}

void
MainWindow::setupGL
(QWidget *parent)
{
    QVBoxLayout *glVerticalLayout = new QVBoxLayout(ui->centralWidget);
    glVerticalLayout->setSpacing(0);
    glVerticalLayout->setContentsMargins(0,0,0,0);
    glVerticalLayout->setObjectName(QStringLiteral("glVerticalLayout"));

    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile );
    glFormat.setSamples(4);
    mWindowComponent.reset(new QOpenGLWindowComponent(glFormat,parent));

    glVerticalLayout->addWidget(mWindowComponent.get());
    ui->previewTab->setLayout(glVerticalLayout);
}

MainWindow::~MainWindow
()
{
    delete ui;
}

QAction*
MainWindow::getAction_File_New
()
{
    return ui->actionNew;
}

QAction*
MainWindow::getAction_File_Open
()
{
    return ui->actionOpen;
}

QAction*
MainWindow::getAction_File_CloseProject
()
{
    return ui->actionCloseProject;
}

QAction*
MainWindow::getAction_View_TogglePhysicsDebug
()
{
    return ui->actionTogglePhysicsDebug;
}


QAction*
MainWindow::getAction_File_Save
()
{
    return ui->actionSave;
}

QAction*
MainWindow::getAction_Scene_Play
()
{
    return ui->actionPlay;
}

QAction*
MainWindow::getAction_Scene_Stop
()
{
    return ui->actionStop;
}

void
MainWindow::onInvalidProjectDirectory
(QString directory)
{
    QMessageBox::critical(
                this,
                tr("Invalid Project Directory"),
                tr("\"%1\"\n\nDoes not contain a vaild Dream project.").arg(directory)
                );
}

void
MainWindow::onNoSceneSelected
()
{
    showStatusBarMessage(QString("No Scene Selected to Run!"));
}

QTreeView*
MainWindow::getScenegraphTreeView
()
{
    return ui->scenegraphTreeView;
}

QTreeView*
MainWindow::getPropertiesTreeView
()
{
    return ui->propertiesTreeView;
}

QTreeView*
MainWindow::getAssetDefinitionTreeView
()
{
    return ui->assetDefinitionTreeView;
}

void
MainWindow::showStatusBarMessage
(QString msg)
{
    ui->statusBar->showMessage(msg);
}

QAction*
MainWindow::getAction_Scene_Reload
()
{
    return ui->actionReload;
}

QAction*
MainWindow::getAction_File_OpenTestProject
()
{
    return ui->actionOpen_Test_Project;
}

QAction*
MainWindow::getAction_View_ToggleGrid
()
{
    return ui->actionToggleGrid;
}

QAction*
MainWindow::getAction_View_ToggleDebug
()
{
    return ui->actionToggleDebug;
}

QOpenGLWindowComponent*
MainWindow::getWindowComponent
()
{
    return mWindowComponent.get();
}

void
MainWindow::onSceneStopped
(SceneDefinition* scene)
{
    if (scene)
    {
        showStatusBarMessage(
                    QString("Stopped Scene: %1")
                    .arg(QString::fromStdString(scene->getNameAndUuidString())
                         )
                    );
    }
    else
    {
        showStatusBarMessage(QString("No Scene Running"));
    }
}


void
MainWindow::keyPressEvent
(QKeyEvent* ke)
{
    qDebug() << "MainWindow:" << ke->key() << "pressed";

    if (shouldPassKey(ke->key()))
    {
        mWindowComponent->keyPressEvent(ke);
    }
    else
    {
        QMainWindow::keyPressEvent(ke);
    }
}

bool
MainWindow::shouldPassKey
(int key)
{
    return std::find
            (
                begin(mKeysPassedToWindow),
                end(mKeysPassedToWindow),
                key
                ) != end(mKeysPassedToWindow);
}

void
MainWindow::keyReleaseEvent
(QKeyEvent* ke)
{
    qDebug() << "MainWindow:" << ke->key() << " released";

    if (shouldPassKey(ke->key()))
    {
        mWindowComponent->keyReleaseEvent(ke);
    }
    else
    {
        QMainWindow::keyReleaseEvent(ke);
    }
}
