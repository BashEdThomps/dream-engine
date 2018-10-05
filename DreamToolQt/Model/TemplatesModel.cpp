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
#include <DreamCore.h>
#include <spdlog/spdlog.h>


using Dream::Constants;

 QString TemplatesModel::TEMPLATE_ROOT_PATH = ":/templates/";
 QString TemplatesModel::TEMPLATE_SCRIPT_PATH = "script/";
 QString TemplatesModel::TEMPLATE_SHADER_PATH = "shader/";

TemplatesModel::TemplatesModel
(QObject *parent)
    : QObject(parent)
{
    auto log = spdlog::get("TemplatesModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("TemplatesModel");
    }
    log->debug("Constructing");
    log->debug("Scripts found: {}", getScriptTemplateNames().join(":").toStdString());
    log->debug("Shaders found: {}", getShaderTemplateNames().join(":").toStdString());
}

TemplatesModel::~TemplatesModel
()
{
    auto log = spdlog::get("TemplatesModel");
    log->debug("Destructing");
}

QString
TemplatesModel::getScriptTemplate
(QString templateName, QString filename)
{
    auto log = spdlog::get("TemplatesModel");
    log->debug("Getting script template {}" , templateName.toStdString());

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SCRIPT_PATH);
    QString scriptAbsPath = QDir(templatesDir.filePath(templateName)).filePath(filename);

    log->debug("from {}" , scriptAbsPath.toStdString());

    QFile scriptFile(scriptAbsPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        log->debug("Got Script {}",templateContent.toStdString());
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
    auto log = spdlog::get("TemplatesModel");
    log->debug("Getting shader template {} file {}" , templateName.toStdString(), fileName.toStdString());

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SHADER_PATH);
    QString absPath = QDir(templatesDir.filePath(templateName)).filePath(fileName);

    log->debug("from {}", absPath.toStdString());

    QFile scriptFile(absPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        log->debug("TemplateModel: Got {}\n{}",fileName.toStdString(), templateContent.toStdString());
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
