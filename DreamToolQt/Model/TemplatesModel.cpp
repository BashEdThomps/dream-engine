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

const QString TemplatesModel::TEMPLATE_ROOT_PATH = ":/templates/";
const QString TemplatesModel::TEMPLATE_SCRIPT_PATH = "script/";
const QString TemplatesModel::TEMPLATE_SHADER_PATH = "shader/";

TemplatesModel::TemplatesModel
(QObject *parent)
    : QObject(parent)
{
    auto log = spdlog::get("TemplatesModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("TemplatesModel");
    }
    log->info("Constructing");
    log->info("Scripts found: {}", getScriptTemplateNames().join(":").toStdString());
    log->info("Shaders found: {}", getShaderTemplateNames().join(":").toStdString());
}

TemplatesModel::~TemplatesModel
()
{
    auto log = spdlog::get("TemplatesModel");
    log->info("Destructing");
}

QString
TemplatesModel::getScriptTemplate
(QString templateName)
{
    auto log = spdlog::get("TemplatesModel");
    log->info("Getting script template {}" , templateName.toStdString());

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SCRIPT_PATH);
    QString scriptAbsPath = QDir(templatesDir.filePath(templateName)).filePath
        (
            QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA)
        );

    log->info("from {}" , scriptAbsPath.toStdString());

    QFile scriptFile(scriptAbsPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        log->info("Got Script {}",templateContent.toStdString());
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
    log->info("Getting shader template {} file {}" , templateName.toStdString(), fileName.toStdString());

    QDir templatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SHADER_PATH);
    QString absPath = QDir(templatesDir.filePath(templateName)).filePath(fileName);

    log->info("from {}", absPath.toStdString());

    QFile scriptFile(absPath);
    if (scriptFile.exists())
    {
        scriptFile.open(QIODevice::ReadOnly);
        QString templateContent = scriptFile.readAll();
        scriptFile.close();
        log->info("TemplateModel: Got {}\n{}",fileName.toStdString(), templateContent.toStdString());
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