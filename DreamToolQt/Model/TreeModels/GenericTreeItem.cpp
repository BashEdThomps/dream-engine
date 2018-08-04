/*
 * GenericGenericTreeItem.cpp
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

#include "GenericTreeItem.h"
#include <QStringList>
#include <QDebug>

GenericTreeItem::GenericTreeItem
(QString title, GenericTreeItem *parent)
    : mParentItem(parent),
      mTitle(title)
{
    //qDebug() << "GenericTreeItem: Constructing";
}

GenericTreeItem::~GenericTreeItem
()
{
    //qDebug() << "GenericTreeItem: Destructing";
    qDeleteAll(mChildItems);
}

void
GenericTreeItem::appendChild
(GenericTreeItem *item)
{
    mChildItems.append(item);
}

GenericTreeItem*
GenericTreeItem::child
(int row)
{
    return mChildItems.value(row);
}

int
GenericTreeItem::childCount
() const
{
    return mChildItems.count();
}

int
GenericTreeItem::columnCount
() const
{
    return 1;
}

QVariant
GenericTreeItem::data
(int column) const
{
    switch (column)
    {
        case 0:
            return QVariant(mTitle);
    }

    return QVariant();
}

GenericTreeItem*
GenericTreeItem::parentItem
()
{
    return mParentItem;
}

QString GenericTreeItem::getTitle() const
{
    return mTitle;
}

int
GenericTreeItem::row
() const
{
    if (mParentItem)
        return mParentItem->mChildItems.indexOf(const_cast<GenericTreeItem*>(this));

    return 0;
}
