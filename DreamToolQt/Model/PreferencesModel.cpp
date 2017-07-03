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
    qDebug() << "PreferencesModel: Constructing";

    createPreferencesDirectory();

    if(!loadPreferencesFile())
    {
        if(savePreferenecsFile())
        {
            if (loadPreferencesFile())
            {
                qDebug() << "PreferencesModel: Created Initial Preferences file";
            }
        }
        else
        {
            qWarning() << "PreferencesModel: WARNING: Initialisation Failed!";
        }
    }
    else
    {
        qDebug() << "PreferencesModel: Initialised from existing preferences file";
    }
}

bool
PreferencesModel::createPreferencesDirectory
()
{
    QDir dir(getPreferencesDirectoryPath());
    if (!dir.exists())
    {
        qDebug() << "PreferencesModel: Creating Preferences Directory"
                 << getPreferencesDirectoryPath();
        return QDir().mkdir(getPreferencesDirectoryPath());
    }
    else
    {
        qDebug() << "PreferencesModel: Found Preferences Directory"
                 << getPreferencesDirectoryPath();
    }
    return true;
}

QString
PreferencesModel::getPreferencesDirectoryPath
()
{
    QString preferencesDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    preferencesDir = QDir(preferencesDir).filePath(QString::fromStdString(PREFERENCES_DIRECTORY_NAME));
    return preferencesDir;
}

QString
PreferencesModel::getPreferencesFilePath
()
{
    QString prefDir = getPreferencesDirectoryPath();
    QString prefFile = QDir(prefDir).filePath(QString::fromStdString(PREFERENCES_FILE_NAME));
    return prefFile;
}

void
PreferencesModel::setDefaultPreferences
()
{
    mJson = json::object();
    setDefaultProjectDirectory("~");
    setExternalTextEditorPath("/Applications/Atom.app");
}

bool
PreferencesModel::savePreferenecsFile
()
{
    qDebug() << "PreferencesModel: Saving Preferences To" << getPreferencesFilePath();
    if (mJson.is_null())
    {
        setDefaultPreferences();
    }
    QFile prefFile(getPreferencesFilePath());
    prefFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    bool writeResult = prefFile.write(mJson.dump(1).c_str());
    prefFile.close();
    return writeResult;
}

bool
PreferencesModel::loadPreferencesFile
()
{
    qDebug() << "PreferencesModel: Loading Preferences From" << getPreferencesFilePath();
    QFile prefFile(getPreferencesFilePath());
    if (prefFile.exists())
    {
        prefFile.open(QIODevice::ReadOnly);
        QByteArray rawJsonData = prefFile.readAll();
        QString jsonString(rawJsonData);
        mJson = json::parse(jsonString.toStdString());
        prefFile.close();
        return true;
    }
    return false;
}

void
PreferencesModel::setDefaultProjectDirectory
(QString dir)
{
    mJson[JSON_DEFAULT_PROJECT_DIRECTORY] = dir.toStdString();
    emit notifyDefaultProjectDirectoryChanged(dir);
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
    emit notifyExternalTextEditorChanged(path);
}

QString
PreferencesModel::getExternalTextEditorPath
()
{
    return QString::fromStdString(mJson[JSON_EXTERNAL_TEXT_EDITOR_PATH]);
}
