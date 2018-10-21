/*
 * PreferencesModel.h
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
#pragma once

#include <QObject>
#include <json.hpp>
#include <string>

using nlohmann::json;
using std::string;

class PreferencesModel : public QObject
{
    Q_OBJECT
public:
    explicit PreferencesModel(QObject *parent = nullptr);

    bool savePreferenecsFile();
    bool loadPreferencesFile();

    void setDefaultProjectDirectory(QString dir);
    QString getDefaultProjectDirectory();

    void setExternalTextEditorPath(QString path);
    QString getExternalTextEditorPath();

protected:
    void initPreferencesModel();
    bool createPreferencesDirectory();

    void setDefaultPreferences();
private: // Member Functions
    QString getPreferencesFilePath();
    QString getPreferencesDirectoryPath();

private: // Variables
     json mJson;
     static string PREFERENCES_FILE_NAME;
     static string PREFERENCES_DIRECTORY_NAME;
     static string JSON_DEFAULT_PROJECT_DIRECTORY;
     static string JSON_EXTERNAL_TEXT_EDITOR_PATH;

signals:
    void notifyDefaultProjectDirectoryChanged(QString directory);
    void notifyExternalTextEditorChanged(QString editorPath);
};
