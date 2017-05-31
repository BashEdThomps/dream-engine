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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GL/glew.h>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringListModel>
#include <QTreeView>
#include <QAction>
#include "../Model/QOpenGLWindowComponent.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

    QTreeView* getProjectTreeView();
    QTreeView* getPropertiesTreeView();
    QTreeView* getAssetDefinitionTreeView();

    QAction* getActionNew();
    QAction* getActionSave();
    QAction* getActionOpen();
    QAction* getActionReload();
    QAction* getActionStop();
    QAction* getActionPlay();

    QOpenGLWindowComponent* getWindowComponent();

signals:
    void notifyActionNew(QString);
    void notifyActionOpen(QString);
    void notifyActionSave(QString);

public slots:
    void onInvalidProjectDirectory(QString directory);
    void onNoSceneSelected();
    void showStatusBarMessage(QString msg);
    void onSceneStopped(Dream::Scene* scene);

private:
    void setupGL(QWidget *parent);
    QOpenGLWindowComponent* mWindowComponent;
};

#endif // MAINWINDOW_H
