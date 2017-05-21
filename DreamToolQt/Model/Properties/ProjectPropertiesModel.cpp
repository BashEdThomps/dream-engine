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

ProjectPropertiesModel::ProjectPropertiesModel(Dream::Project *project,QObject* parent)
    : PropertiesModel(parent)
{
    mProject = project;
    createRoot();
    createProperties();
}

void ProjectPropertiesModel
::createRoot()
{
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mProject->getName())
             << QString::fromStdString(mProject->getUuid());
    mRootItem = new PropertiesItem(rootData,mProject);
}

void ProjectPropertiesModel
::createProperties()
{
}

ProjectPropertiesModel::~ProjectPropertiesModel()
{
    delete mRootItem;
}
