/*
 * ProjectPropertiesItem.h
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

#pragma once

#include "../AbstractPropertiesItem.h"

namespace Dream
{
    class ProjectDefinition;
}

using Dream::ProjectDefinition;

enum ProjectProperty
{
    PROJECT_PROPERTY_NAME,
    PROJECT_PROPERTY_AUTHOR,
    PROJECT_PROPERTY_DESCRIPTION,
    PROJECT_PROPERTY_WINDOW_SIZE,
    PROJECT_PROPERTY_WINDOW_WIDTH,
    PROJECT_PROPERTY_WINDOW_HEIGHT,
    PROJECT_PROPERTY_STARTUP_SCENE,
    PROJECT_PROPERTY_NONE
};

class ProjectPropertiesItem : public AbstractPropertiesItem
{
public:
    ProjectPropertiesItem
    (
        QString title,
        ProjectDefinition* pdHandle,
        ProjectProperty property = PROJECT_PROPERTY_NONE,
        QItemDelegate* delegate = nullptr,
        AbstractPropertiesItem *parent = nullptr
    );
    ~ProjectPropertiesItem();

    QVariant data(int column) override;
    bool setData(int column, const QVariant &value) override;

    ProjectProperty getProperty();
    ProjectDefinition* getProjectDefinitionHandle();

private:
    ProjectDefinition *mProjectDefinitionHandle;
    ProjectProperty mProperty;
};
