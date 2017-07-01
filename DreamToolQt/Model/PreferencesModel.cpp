/*
 * PreferencesModel.cpp
 *
 * Created: 16 2017 by Ashley
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
#include "PreferencesModel.h"

#include <QStandardPaths>
#include <QDebug>
#include <QDir>

const string PreferencesModel::PREFERENCES_DIRECTORY_NAME = "DreamTool";
const string PreferencesModel::PREFERENCES_FILE_NAME = "preferences.json";

const string PreferencesModel::JSON_DEFAULT_PROJECT_DIRECTORY = "default_project_directory";
const string PreferencesModel::JSON_EXTERNAL_TEXT_EDITOR_PATH = "external_text_editor_path";

PreferencesModel::PreferencesModel
(QObject *parent)
    : QObject(parent)
{

}

QString
PreferencesModel::getPreferencesDirectoryPath
()
{
    QString preferencesDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    preferencesDir = QDir(preferencesDir).filePath(QString::fromStdString(PREFERENCES_DIRECTORY_NAME));
    qDebug() << "PreferencesModel: Using path" << preferencesDir;
    return preferencesDir;
}

QString
PreferencesModel::getPreferencesFilePath
()
{
    QString prefDir = getPreferencesDirectoryPath();
    QString prefFile = QDir(prefDir).filePath(QString::fromStdString(PREFERENCES_FILE_NAME));
    qDebug() << "PreferencesModel: Using File" << prefFile;
    return prefFile;
}

bool
PreferencesModel::savePreferenecsFile
()
{
   QString path = getPreferencesFilePath();
   return false;
}

bool
PreferencesModel::loadPreferencesFile
()
{
    return false;
}

void
PreferencesModel::setDefaultProjectDirectory
(QString dir)
{
    mJson[JSON_DEFAULT_PROJECT_DIRECTORY] = dir.toStdString();
}

QString
PreferencesModel::getDefaultProjectDirectory
()
{
    return QString::fromStdString(mJson[JSON_DEFAULT_PROJECT_DIRECTORY]);
}

void
PreferencesModel::setExternalTextEditorPath
(QString path)
{
    mJson[JSON_EXTERNAL_TEXT_EDITOR_PATH] = path.toStdString();
}

QString
PreferencesModel::getExternalTextEditorPath
()
{
    return QString::fromStdString(mJson[JSON_EXTERNAL_TEXT_EDITOR_PATH]);
}
