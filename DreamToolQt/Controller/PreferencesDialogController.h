/*
 * PreferencesDialogController.h
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
#pragma once

#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include "../View/PreferencesWidget.h"
#include "../Model/PreferencesModel.h"

class PreferencesDialogController : public QObject
{
    Q_OBJECT
public:
    explicit PreferencesDialogController(QObject *parent = nullptr);
    ~PreferencesDialogController() override;
    void showDialog();
    void hideDialog();
    PreferencesModel& getPreferencesModel();

public slots:
    void onButton_OK();
    void onButton_DefaultProjectDirectoryBrowse();
    void onButton_ExternalTextEditorBrowse();

protected:
    void setupLayout();
    void setupDialog();
    void setupValues();
    void createConnections();

    PreferencesModel mPreferencesModel;
    QDialog mPreferencesDialog;
    PreferencesWidget mPreferencesWidget;
    QVBoxLayout mPreferencesDialogLayout;

};
