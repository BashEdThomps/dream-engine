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
    setupAssetDefinitionAddRemoveButtonMenus();
    setupScenegraphAddRemoveButtonMenus();
    setupPropertiesAddRemoveButtonMenus();

    setPlaybackActionsEnabled(false);
    setSaveActionEnabled(false);
    setAddRemoveButtonsEnabled(false);
}

void
MainWindow::setAddRemoveButtonsEnabled
(bool enabled)
{
   ui->assetDefinitionAddButton->setEnabled(enabled);
   ui->assetDefinitionRemoveButton->setEnabled(enabled);

   ui->scenegraphAddButton->setEnabled(enabled);
   ui->scenegraphRemoveButton->setEnabled(enabled);

   ui->propertiesAddButton->setEnabled(enabled);
   ui->propertiesRemoveButton->setEnabled(enabled);
}

void
MainWindow::setPlaybackActionsEnabled
(bool enabled)
{
    ui->actionPlay->setEnabled(enabled);
    ui->actionStop->setEnabled(enabled);
    ui->actionReload->setEnabled(enabled);
}

void
MainWindow::setOpenActionEnabled
(bool enabled)
{
    ui->actionOpen->setEnabled(enabled);
}

QAction*
MainWindow::getScenegraphAddSceneAction
()
{
    return mScenegraphAddSceneAction.get();
}

QAction*
MainWindow::getScenegraphAddSceneObjectAction
()
{
    return mScenegraphAddSceneObjectAction.get();
}

QAction*
MainWindow::getPropertiesAddAssetAction
()
{
    return mPropertiesAddAssetAction.get();
}

QAction*
MainWindow::getPropertiesAddSceneObjectChildAction
()
{
    return mPropertiesAddSceneObjectChildAction.get();
}

QAction*
MainWindow::getAssetDefinitionAddAction
(AssetType type, string format)
{
   return mAssetDefinitionAddActionsMap.at(type).at(format);
}

void
MainWindow::setSaveActionEnabled
(bool enabled)
{
    ui->actionSave->setEnabled(enabled);
}

void
MainWindow::setNewActionEnabled
(bool enabled)
{
    ui->actionNew->setEnabled(enabled);
}

void
MainWindow::setupAssetDefinitionAddRemoveButtonMenus
()
{
    mAssetDefinitionAddMenu.reset(new QMenu());
    mAssetDefinitionAddMenu->setWindowTitle("New Definition");

    for (pair<AssetType,string> typePair : Constants::DREAM_ASSET_TYPES_MAP)
    {
        QMenu *typeMenu = mAssetDefinitionAddMenu->addMenu(QString::fromStdString(Constants::getAssetTypeReadableName(typePair.second)));

        map<string,QAction*> formatActions;

        for (string format : Constants::DREAM_ASSET_FORMATS_MAP[typePair.first])
        {
            QAction *action = new QAction(QString::fromStdString(Constants::getAssetFormatReadableName(format)));
            typeMenu->addAction(action);
            formatActions.insert(pair<string,QAction*>(format,action));
        }

        mAssetDefinitionAddActionsMap.insert(pair<AssetType,map<string,QAction*>>(typePair.first,formatActions));
    }
    ui->assetDefinitionAddButton->setMenu(mAssetDefinitionAddMenu.get());
}


void
MainWindow::setupScenegraphAddRemoveButtonMenus
()
{
    mScenegraphAddMenu.reset(new QMenu());

    mScenegraphAddSceneAction.reset(new QAction(QString("Scene")));
    mScenegraphAddSceneObjectAction.reset(new QAction(QString("Scene Object")));

    mScenegraphAddMenu->addAction(mScenegraphAddSceneAction.get());
    mScenegraphAddMenu->addAction(mScenegraphAddSceneObjectAction.get());

    ui->scenegraphAddButton->setMenu(mScenegraphAddMenu.get());
}

void
MainWindow::setupPropertiesAddRemoveButtonMenus
()
{
    mPropertiesAddMenu.reset(new QMenu());

    mPropertiesAddAssetAction.reset(new QAction(QString("Asset")));
    mPropertiesAddSceneObjectChildAction.reset(new QAction(QString("Child Scene Object")));

    mPropertiesAddMenu->addAction(mPropertiesAddAssetAction.get());
    mPropertiesAddMenu->addAction(mPropertiesAddSceneObjectChildAction.get());

    ui->propertiesAddButton->setMenu(mPropertiesAddMenu.get());

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
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
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
MainWindow::getActionNew
()
{
    return ui->actionNew;
}

QAction*
MainWindow::getActionOpen
()
{
    return ui->actionOpen;
}

QAction*
MainWindow::getActionCloseProject
()
{
    return ui->actionCloseProject;
}

QAction*
MainWindow::getActionTogglePhysicsDebug
()
{
   return ui->actionTogglePhysicsDebug;
}


QAction*
MainWindow::getActionSave
()
{
    return ui->actionSave;
}

QAction*
MainWindow::getActionPlay
()
{
    return ui->actionPlay;
}

QAction*
MainWindow::getActionStop
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
MainWindow::getProjectTreeView
()
{
    return ui->projectTreeView;
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
MainWindow::getActionReload
()
{
    return ui->actionReload;
}

QAction*
MainWindow::getActionOpenTestProject
()
{
    return ui->actionOpen_Test_Project;
}

QAction*
MainWindow::getActionToggleGrid
()
{
    return ui->actionToggleGrid;
}

QAction*
MainWindow::getActionToggleDebug
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
