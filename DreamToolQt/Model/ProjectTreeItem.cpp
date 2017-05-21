/*
 * ProjectProjectTreeItem.cpp
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
#include "ProjectTreeItem.h"
#include <QStringList>

ProjectTreeItem::ProjectTreeItem(const QList<QVariant> &data, ProjectItemType type, void* item, ProjectTreeItem *parent)
{
    mType = type;
    mParentItem = parent;
    mItemData = data;
    mItem = item;
}

ProjectTreeItem::~ProjectTreeItem()
{
    qDeleteAll(mChildItems);
}

void ProjectTreeItem::appendChild(ProjectTreeItem *item)
{
    mChildItems.append(item);
}

ProjectTreeItem *ProjectTreeItem::child(int row)
{
    return mChildItems.value(row);
}

int ProjectTreeItem::childCount() const
{
    return mChildItems.count();
}

int ProjectTreeItem::columnCount() const
{
    return mItemData.count();
}

QVariant ProjectTreeItem::data(int column) const
{
    return mItemData.value(column);
}

ProjectTreeItem *ProjectTreeItem::parentItem()
{
    return mParentItem;
}

int ProjectTreeItem::row() const
{
    if (mParentItem)
        return mParentItem->mChildItems.indexOf(const_cast<ProjectTreeItem*>(this));

    return 0;
}

ProjectItemType ProjectTreeItem::getItemType()
{
   return mType;
}

void* ProjectTreeItem::getItem()
{
    return mItem;
}
