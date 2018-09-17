/*
 * TemplatesModel.h
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

class TemplatesModel : public QObject
{
    Q_OBJECT
private:
    static  QString TEMPLATE_ROOT_PATH;
    static  QString TEMPLATE_SCRIPT_PATH;
    static  QString TEMPLATE_SHADER_PATH;
public:
    explicit TemplatesModel(QObject *parent = nullptr);
    ~TemplatesModel() override;

    QString getScriptTemplate(QString templateName,QString filename);
    QList<QString> getScriptTemplateNames();

    QString getShaderTemplate(QString templateName, QString fileName);
    QList<QString> getShaderTemplateNames();
};
