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
    qDebug() << "ProjectPropertiesItem: Constructing";
}

ProjectPropertiesItem::~ProjectPropertiesItem
()
{
    qDebug() << "ProjectPropertiesItem: Destructing";
}

QVariant
ProjectPropertiesItem::data
(int column)
{
    if (column == 0)
    {
        return QVariant(mTitle);
    }

    switch(getProperty())
    {

        case PROJECT_PROPERTY_WINDOW_WIDTH:
            return QVariant(mProjectDefinitionHandle->getWindowWidth());
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            return QVariant(mProjectDefinitionHandle->getWindowHeight());

        case PROJECT_PROPERTY_DESCRIPTION:
            return QVariant(QString::fromStdString(mProjectDefinitionHandle->getDescription()));

        case PROJECT_PROPERTY_STARTUP_SCENE:
            return QVariant(QString::fromStdString(mProjectDefinitionHandle->getStartupSceneUuid()));

        case PROJECT_PROPERTY_NAME:
            return QVariant(QString::fromStdString(mProjectDefinitionHandle->getName()));

        case PROJECT_PROPERTY_AUTHOR:
            return QVariant(QString::fromStdString(mProjectDefinitionHandle->getAuthor()));

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

    switch (getProperty())
    {
        case PROJECT_PROPERTY_DESCRIPTION:
            mProjectDefinitionHandle->setDescription(value.toString().toStdString());
            break;

        case PROJECT_PROPERTY_WINDOW_WIDTH:
            mProjectDefinitionHandle->setWindowWidth(value.toInt());
            break;
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            mProjectDefinitionHandle->setWindowHeight(value.toInt());
            break;

        case PROJECT_PROPERTY_STARTUP_SCENE:
            mProjectDefinitionHandle->setStartupSceneUuid(value.toString().toStdString());
            break;
        case PROJECT_PROPERTY_NAME:
            mProjectDefinitionHandle->setName(value.toString().toStdString());
            break;
        case PROJECT_PROPERTY_AUTHOR:
            mProjectDefinitionHandle->setAuthor(value.toString().toStdString());
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
ProjectPropertiesItem::getProjectDefinitionHandle
()
{
   return mProjectDefinitionHandle;
}
