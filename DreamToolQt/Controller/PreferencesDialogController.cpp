/*
 * PreferencesDialogController.cpp
 *
 * Created: 01 2017 by Ashley
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
#include "PreferencesDialogController.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <QVBoxLayout>
#include <QFileDialog>

PreferencesDialogController::PreferencesDialogController
(QObject *parent)
    : QObject(parent)
{
    auto log = spdlog::get("PreferencesDialogController");
    if (log==nullptr)
    {
        log = spdlog::stderr_color_mt("PreferencesDialogController");
    }
    log->debug("Constructing");
    setupLayout();
    setupDialog();
    setupValues();
    createConnections();
}

PreferencesDialogController::~PreferencesDialogController
()
{
    auto log = spdlog::get("PreferencesDialogController");
    log->debug("Destructing");
}

void
PreferencesDialogController::setupLayout
()
{
    mPreferencesDialogLayout.addWidget(&mPreferencesWidget);
    mPreferencesDialogLayout.setMargin(0);
}

void
PreferencesDialogController::setupDialog
()
{
    mPreferencesDialog.setLayout(&mPreferencesDialogLayout);
    mPreferencesDialog.setWindowTitle("Preferences");
    mPreferencesDialog.setSizeGripEnabled(false);
    mPreferencesDialog.setModal(true);
    mPreferencesDialog.setFixedSize(mPreferencesWidget.size());
}

void
PreferencesDialogController::setupValues
()
{
    mPreferencesWidget.onDefaultProjectDirectoryChanged
    (
        mPreferencesModel.getDefaultProjectDirectory()
    );

    mPreferencesWidget.onExternalTextEditorChanged
    (
        mPreferencesModel.getExternalTextEditorPath()
    );
}

void
PreferencesDialogController::showDialog
()
{
    mPreferencesDialog.show();
}

void
PreferencesDialogController::hideDialog
()
{
    mPreferencesDialog.hide();
}

PreferencesModel&
PreferencesDialogController::getPreferencesModel
()
{
    return mPreferencesModel;
}

void
PreferencesDialogController::createConnections
()
{
    // On OK Button
    connect
            (
                mPreferencesWidget.getButton_OK(),
                SIGNAL(pressed()),
                this,
                SLOT(onButton_OK())
                );

    // on Default Project Dir Browse Button
    connect
            (
                mPreferencesWidget.getButton_DefaultProjectDirectoryBrowse(),
                SIGNAL(pressed()),
                this,
                SLOT(onButton_DefaultProjectDirectoryBrowse())
                );

    // On External Text Editor Button
    connect
            (
                mPreferencesWidget.getButton_ExternalTextEditorBrowse(),
                SIGNAL(pressed()),
                this,
                SLOT(onButton_ExternalTextEditorBrowse())
                );
    // Set Default Project Dir Label
    connect
            (
                &mPreferencesModel,
                SIGNAL(notifyDefaultProjectDirectoryChanged(QString)),
                &mPreferencesWidget,
                SLOT(onDefaultProjectDirectoryChanged(QString))
                );
    // Set External Text Editor Label
    connect
            (
                &mPreferencesModel,
                SIGNAL(notifyExternalTextEditorChanged(QString)),
                &mPreferencesWidget,
                SLOT(onExternalTextEditorChanged(QString))

                );
}

void
PreferencesDialogController::onButton_OK
()
{
    mPreferencesModel.savePreferenecsFile();
    hideDialog();
}

void
PreferencesDialogController::onButton_DefaultProjectDirectoryBrowse
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::Directory);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {
        QString directory = openDialog.selectedFiles().first();

        if (directory.size() == 0)
        {
            return;
        }

        mPreferencesModel.setDefaultProjectDirectory(directory);
    }
}

void
PreferencesDialogController::onButton_ExternalTextEditorBrowse
()
{
    QFileDialog openDialog;
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setDirectory(QDir::home());

    if(openDialog.exec())
    {
        QString textEditorPath = openDialog.selectedFiles().first();

        if (textEditorPath.size() == 0)
        {
            return;
        }

        mPreferencesModel.setExternalTextEditorPath(textEditorPath);
    }
}


