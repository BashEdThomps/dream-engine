/*
 * TemplatesModel.cpp
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

#include "TemplatesModel.h"

#include <QDir>
#include <QDebug>
#include <DreamCore.h>

using Dream::Constants;

const QString TemplatesModel::TEMPLATE_ROOT_PATH = ":/templates/";
const QString TemplatesModel::TEMPLATE_SCRIPT_PATH = "script/";
const QString TemplatesModel::TEMPLATE_SHADER_PATH = "shader/";

TemplatesModel::TemplatesModel
(QObject *parent)
    : QObject(parent)
{
    qDebug() << "TemplatesModel: Constructing";

    qDebug() << "TemplatesModel: Scripts:"
             << getScriptTemplateNames();

    qDebug() << "TemplatesModel: Shaders:"
             << getShaderTemplateNames();
}

TemplatesModel::~TemplatesModel
()
{
    qDebug() << "TemplatesModel: Destructing";
}

QString
TemplatesModel::getScriptTemplate
(QString templateName)
{
    qDebug() << "TemplatesModel: Getting script template" << templateName;

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SCRIPT_PATH);
    QString scriptAbsPath = QDir(templatesDir.filePath(templateName)).filePath
        (
            QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA)
        );

    qDebug() << "TemplatesModel: from " << scriptAbsPath;

    QFile scriptFile(scriptAbsPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        qDebug() << "TemplatesModel: Got Script"
                 << endl
                 << templateContent;
        return templateContent;
    }

    return QString();
}

QList<QString>
TemplatesModel::getScriptTemplateNames
()
{
    QDir scriptTemplatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SCRIPT_PATH);
    return scriptTemplatesDir.entryList(QDir::AllDirs);
}

QString
TemplatesModel::getShaderTemplate
(QString templateName, QString fileName)
{
    qDebug() << "TemplatesModel: Getting shader template" << templateName
             << "file" << fileName;

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SHADER_PATH);
    QString absPath = QDir(templatesDir.filePath(templateName)).filePath(fileName);

    qDebug() << "TemplatesModel: from " << absPath;

    QFile scriptFile(absPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        qDebug() << "TemplateModel: Got " << fileName
                 << endl
                 << templateContent;
        return templateContent;
    }

    return QString();
}

QList<QString>
TemplatesModel::getShaderTemplateNames
()
{
    QDir shaderTenplatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SHADER_PATH);
    return shaderTenplatesDir.entryList(QDir::AllDirs);

}
