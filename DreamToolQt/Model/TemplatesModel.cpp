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
    return QString();
}

QList<QString>
TemplatesModel::getScriptTemplateNames
()
{
    QDir scriptTenplatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SCRIPT_PATH);
    return scriptTenplatesDir.entryList(QDir::AllDirs);
}

QString
TemplatesModel::getShaderTemplate
(QString templateName, QString fileName)
{
    return QString();
}

QList<QString>
TemplatesModel::getShaderTemplateNames
()
{
    QDir shaderTenplatesDir(TEMPLATE_ROOT_PATH+TEMPLATE_SHADER_PATH);
    return shaderTenplatesDir.entryList(QDir::AllDirs);

}
