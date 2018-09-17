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
#include <QDir>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

 string PreferencesModel::PREFERENCES_DIRECTORY_NAME = "DreamTool";
 string PreferencesModel::PREFERENCES_FILE_NAME = "preferences.json";

 string PreferencesModel::JSON_DEFAULT_PROJECT_DIRECTORY = "default_project_directory";
 string PreferencesModel::JSON_EXTERNAL_TEXT_EDITOR_PATH = "external_text_editor_path";

PreferencesModel::PreferencesModel
(QObject *parent)
    : QObject(parent)
{
    auto log = spdlog::get("PreferencesModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("PreferencesModel");
    }
    log->info("Constructing");

    createPreferencesDirectory();

    if(!loadPreferencesFile())
    {
        if(savePreferenecsFile())
        {
            if (loadPreferencesFile())
            {
                log->info("Created Initial Preferences file");
            }
        }
        else
        {
            log->error("Initialisation Failed!");
        }
    }
    else
    {
        log->info("Initialised from existing preferences file");
    }
}

bool
PreferencesModel::createPreferencesDirectory
()
{
    auto log = spdlog::get("PreferencesModel");
    QDir dir(getPreferencesDirectoryPath());
    if (!dir.exists())
    {
        log->info("Creating Preferences Directory {}", getPreferencesDirectoryPath().toStdString());
        return QDir().mkdir(getPreferencesDirectoryPath());
    }
    else
    {
        log->info("Found Preferences Directory {}",  getPreferencesDirectoryPath().toStdString());
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
    auto log = spdlog::get("PreferencesModel");
    log->info("Saving Preferences To {}" , getPreferencesFilePath().toStdString());
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
    auto log = spdlog::get("PreferencesModel");
    log->info("Loading Preferences From {}",  getPreferencesFilePath().toStdString());
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
