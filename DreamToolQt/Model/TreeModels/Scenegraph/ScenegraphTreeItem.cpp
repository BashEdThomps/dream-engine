/*
 * ProjectScenegraphTreeItem.cpp
 *
 * Created: 15 2017 by Ashley
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
#include "ScenegraphTreeItem.h"
#include <QDebug>
#include <DreamCore.h>

using Dream::ProjectDefinition;
using Dream::SceneDefinition;
using Dream::SceneObjectDefinition;

ScenegraphTreeItem::ScenegraphTreeItem
(QString title, ScenegraphTreeItemType type, void* item, ScenegraphTreeItem *parent)
    : GenericTreeItem(title, parent),
      mItem(item),
      mType(type)
{
   // qDebug() << "ScenegraphTreeItem: Constructing";
}

ScenegraphTreeItem::~ScenegraphTreeItem
()
{
   //qDebug() << "ScenegraphTreeItem: Destructing";
}

void*
ScenegraphTreeItem::getItem
()
const
{
    return mItem;
}

ScenegraphTreeItemType
ScenegraphTreeItem::getType
()
const
{
   return mType;
}

QVariant
ScenegraphTreeItem::data
(int column)
const
{
    switch (getType())
    {
        case SCENEGRAPH_PROJECT:
            return QVariant(QString::fromStdString(static_cast<ProjectDefinition*>(mItem)->getName()));
        case SCENEGRAPH_SCENE:
            return QVariant(QString::fromStdString(static_cast<SceneDefinition*>(mItem)->getName()));
        case SCENEGRAPH_SCENE_OBJECT:
            return QVariant(QString::fromStdString(static_cast<SceneObjectDefinition*>(mItem)->getName()));
        case SCENEGRAPH_TREE_NODE:
            return QVariant(mTitle);
    }
    return QVariant();
}
