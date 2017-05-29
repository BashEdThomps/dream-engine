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
#include "Model/QOpenGLWindowComponent.h"
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGL(parent);
}

void MainWindow::setupGL(QWidget *parent)
{
    QVBoxLayout *glVerticalLayout = new QVBoxLayout(ui->centralWidget);
    glVerticalLayout->setSpacing(6);
    glVerticalLayout->setContentsMargins(11, 11, 11, 11);
    glVerticalLayout->setObjectName(QStringLiteral("glVerticalLayout"));

    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSamples(4);
    mWindowComponent = new QOpenGLWindowComponent(glFormat,parent);

    glVerticalLayout->addWidget(mWindowComponent);
    ui->centralWidget->setLayout(glVerticalLayout);
}

MainWindow::~MainWindow()
{
    delete mWindowComponent;
    delete ui;
}

QAction* MainWindow::getActionNew()
{
    return ui->actionNew;
}

QAction* MainWindow::getActionOpen()
{
    return ui->actionOpen;
}

QAction* MainWindow::getActionSave()
{
    return ui->actionSave;
}

QAction* MainWindow::getActionPlay()
{
   return ui->actionPlay;
}

QAction* MainWindow::getActionStop()
{
    return ui->actionStop;
}

void MainWindow::onInvalidProjectDirectory(QString directory)
{
    QMessageBox::critical(
                this,
                tr("Invalid Project Directory"),
                tr("\"%1\"\n\nDoes not contain a vaild Dream project.").arg(directory)
                );
}

void MainWindow::onNoSceneSelected()
{
    QMessageBox::critical(
                this,
                tr("No Scene Selected"),
                tr("Please select a Scene to run from the Project Tree.")
                );
}

QTreeView* MainWindow::getProjectTreeView()
{
    return ui->projectTreeView;
}

QTreeView* MainWindow::getPropertiesTreeView()
{
    return ui->propertiesTreeView;
}

QTreeView* MainWindow::getAssetDefinitionTreeView()
{
    return ui->assetDefinitionTreeView;
}

void MainWindow::showStatusBarMessage(QString msg)
{
    ui->statusBar->showMessage(msg);
}

QAction* MainWindow::getActionReload()
{
    return ui->actionReload;
}

QOpenGLWindowComponent* MainWindow::getWindowComponent()
{
    return mWindowComponent;
}
