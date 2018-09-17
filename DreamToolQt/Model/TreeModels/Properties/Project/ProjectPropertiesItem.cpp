/*
 * ProjectPropertiesItem.cpp
 *
 * Created: 29 2017 by Ashley
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
#include "ProjectPropertiesItem.h"

#include <DreamCore.h>
#include <QDebug>

ProjectPropertiesItem::ProjectPropertiesItem
(
    QString title,
    ProjectDefinition* pdHandle,
    ProjectProperty property,
    QItemDelegate* delegate,
    AbstractPropertiesItem *parent
)
    : AbstractPropertiesItem (title,delegate,parent),
      mProjectDefinitionHandle(pdHandle),
      mProperty(property)
{
   // qDebug() << "ProjectPropertiesItem: Constructing";
}

ProjectPropertiesItem::~ProjectPropertiesItem
()
{
   // qDebug() << "ProjectPropertiesItem: Destructing";
}

QVariant
ProjectPropertiesItem::data
(int column)
{
    if (column == 0)
    {
        return QVariant(mTitle);
    }

    auto proj = mProjectDefinitionHandle;

    switch(getProperty())
    {

        case PROJECT_PROPERTY_WINDOW_WIDTH:
            return QVariant(proj->getWindowWidth());
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            return QVariant(proj->getWindowHeight());

        case PROJECT_PROPERTY_DESCRIPTION:
            return QVariant(QString::fromStdString(proj->getDescription()));

        case PROJECT_PROPERTY_STARTUP_SCENE:
            return QVariant(QString::fromStdString(proj->getStartupSceneUuid()));

        case PROJECT_PROPERTY_NAME:
            return QVariant(QString::fromStdString(proj->getName()));

        case PROJECT_PROPERTY_AUTHOR:
            return QVariant(QString::fromStdString(proj->getAuthor()));

        case PROJECT_PROPERTY_CAPTURE_JOYSTICK:
            return QVariant(proj->getCaptureJoystick());

        case PROJECT_PROPERTY_CAPTURE_KEYBOARD:
            return QVariant(proj->getCaptureKeyboard());

        case PROJECT_PROPERTY_CAPTURE_MOUSE:
            return QVariant(proj->getCaptureMouse());

        // Not Used
        case PROJECT_PROPERTY_WINDOW_SIZE:
        case PROJECT_PROPERTY_NONE:
            return QVariant();
    }

    return QVariant();
}

bool
ProjectPropertiesItem::setData
(int column, const QVariant &value)
{
    if (column == 0)
    {
        return true;
    }

    auto proj = mProjectDefinitionHandle;

    switch (getProperty())
    {
        case PROJECT_PROPERTY_CAPTURE_KEYBOARD:
            proj->setCaptureKeyboard(value.toBool());
            break;

        case PROJECT_PROPERTY_CAPTURE_MOUSE:
            proj->setCaptureMouse(value.toBool());
            break;

        case PROJECT_PROPERTY_CAPTURE_JOYSTICK:
            proj->setCaptureJoystick(value.toBool());
            break;

        case PROJECT_PROPERTY_DESCRIPTION:
            proj->setDescription(value.toString().toStdString());
            break;

        case PROJECT_PROPERTY_WINDOW_WIDTH:
            proj->setWindowWidth(value.toInt());
            break;
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            proj->setWindowHeight(value.toInt());
            break;

        case PROJECT_PROPERTY_STARTUP_SCENE:
            proj->setStartupSceneUuid(value.toString().toStdString());
            break;
        case PROJECT_PROPERTY_NAME:
            proj->setName(value.toString().toStdString());
            break;
        case PROJECT_PROPERTY_AUTHOR:
            proj->setAuthor(value.toString().toStdString());
            break;

        case PROJECT_PROPERTY_WINDOW_SIZE:
        case PROJECT_PROPERTY_NONE:
            break;

    }

    return true;
}

ProjectProperty
ProjectPropertiesItem::getProperty
()
{
    return mProperty;
}

ProjectDefinition*
ProjectPropertiesItem::getProjectDefinition
()
{
   return mProjectDefinitionHandle;
}
