/*
 * ProjectPropertiesModel.cpp
 *
 * Created: 17 2017 by Ashley
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
#include "ProjectPropertiesModel.h"
#include "ProjectPropertiesItem.h"
#include "ProjectPropertiesTreeDelegate.h"

#include <QDebug>

ProjectPropertiesModel::ProjectPropertiesModel
(shared_ptr<ProjectDefinition> project,QTreeView* parent)
    : AbstractPropertiesModel(new ProjectPropertiesTreeDelegate(this,parent), parent),
       mProjectDefinitionHandle(project)
{
    auto log = spdlog::get("ProjectPropertiesModel");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("ProjectPropertiesModel");
    }
    log->trace("Contructing");
    createRoot();
    createProperties();
}

ProjectPropertiesModel::~ProjectPropertiesModel
()
{
    auto log = spdlog::get("ProjectPropertiesModel");
    log->trace("Destructing");
}

void
ProjectPropertiesModel::createRoot
()
{
    auto log = spdlog::get("ProjectPropertiesModel");
    log->trace("Constructing");
    mRootItem.reset
    (
        new ProjectPropertiesItem
        (
            QString::fromStdString(mProjectDefinitionHandle->getName()),
            mProjectDefinitionHandle
        )
    );
}

void
ProjectPropertiesModel::createProperties
()
{
    createNameProperty();
    createAuthorProperty();
    createDescriptionProperty();
    createStartupSceneProperty();
    createWindowSizeProperty();
    // TODO
}

void
ProjectPropertiesModel::createDelegateConnections
()
{
    // TODO
}

void
ProjectPropertiesModel::createNameProperty
()
{
    mRootItem->appendChild
    (
        new ProjectPropertiesItem
        (
            "Name",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_NAME
        )
    );
}

void
ProjectPropertiesModel::createAuthorProperty
()
{
    mRootItem->appendChild
    (
        new ProjectPropertiesItem
        (
            "Author",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_AUTHOR
        )
    );
}

void
ProjectPropertiesModel::createDescriptionProperty
()
{
    mRootItem->appendChild
    (
        new ProjectPropertiesItem
        (
            "Description",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_DESCRIPTION
        )
    );
}

void
ProjectPropertiesModel::createStartupSceneProperty
()
{
    mRootItem->appendChild
    (
        new ProjectPropertiesItem
        (
            "Startup Scene",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_STARTUP_SCENE
        )
    );
}

void
ProjectPropertiesModel::createWindowSizeProperty
()
{
    auto windowSize = new ProjectPropertiesItem
    (
        "Window Size",
        mProjectDefinitionHandle,
        PROJECT_PROPERTY_WINDOW_SIZE
    );

    windowSize->appendChild
    (
        new ProjectPropertiesItem
        (
            "Width",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_WINDOW_WIDTH
        )
    );

    windowSize->appendChild
    (
        new ProjectPropertiesItem
        (
            "Height",
            mProjectDefinitionHandle,
            PROJECT_PROPERTY_WINDOW_HEIGHT
        )
    );

    mRootItem->appendChild(windowSize);
}
