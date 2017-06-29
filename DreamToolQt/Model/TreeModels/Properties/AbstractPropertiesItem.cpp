/*
 * AbstractPropertiesItem.cpp
 *
 * Created: 18 2017 by Ashley
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

#include "AbstractPropertiesItem.h"

#include <QDebug>

AbstractPropertiesItem::AbstractPropertiesItem
(const QList<QVariant> &data, QItemDelegate* delegate, AbstractPropertiesItem *parent)
    : mItemData(data),
      mParentItemHandle(parent)
{
    qDebug() << "AbstractPropertiesItem: Constructing";
    setDelegate(delegate);
}

void AbstractPropertiesItem::setParent(AbstractPropertiesItem* parent)
{
    mParentItemHandle = parent;
}

void
AbstractPropertiesItem::setDelegate
(QItemDelegate* delegate)
{
    mDelegate.reset(delegate);
}

QItemDelegate*
AbstractPropertiesItem::getDelegate
()
{
    return mDelegate.get();
}

void AbstractPropertiesItem::appendChild(AbstractPropertiesItem *item)
{
    item->setParent(this);
    mChildItems.append(item);
}

AbstractPropertiesItem *AbstractPropertiesItem::child(int row)
{
    return mChildItems.value(row);
}

int AbstractPropertiesItem::childCount() const
{
    return mChildItems.count();
}

int AbstractPropertiesItem::columnCount() const
{
    return mItemData.count();
}

QVariant AbstractPropertiesItem::data(int column) const
{
    return mItemData.value(column);
}

AbstractPropertiesItem *AbstractPropertiesItem::parentItem()
{
    return mParentItemHandle;
}

int AbstractPropertiesItem::row() const
{
    if (mParentItemHandle)
    {
        return mParentItemHandle->mChildItems.indexOf(const_cast<AbstractPropertiesItem*>(this));
    }
    return 0;
}

int AbstractPropertiesItem::childNumber() const
{
    if (mParentItemHandle)
    {
        return mParentItemHandle->mChildItems.indexOf(const_cast<AbstractPropertiesItem*>(this));
    }
    return 0;
}

/*
bool AbstractPropertiesItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > mChildItems.size())
    {
        return false;
    }

    for (int row = 0; row < count; ++row)
    {
        QList<QVariant> data;
        AbstractPropertiesItem *item = new AbstractPropertiesItem(data, nullptr, "", nullptr, this);
        mChildItems.insert(position, item);
    }
    return true;
}
*/

/*
bool AbstractPropertiesItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > mItemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        mItemData.insert(position, QVariant());
    }

    foreach (AbstractPropertiesItem *child, mChildItems)
    {
        child->insertColumns(position, columns);
    }

    return true;
}
*/

/*
bool AbstractPropertiesItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > mChildItems.size())
    {
        return false;
    }

    for (int row = 0; row < count; ++row)
    {
        delete mChildItems.takeAt(position);
    }

    return true;
}
*/

/*
bool AbstractPropertiesItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > mItemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        mItemData.removeAt(position);
    }

    foreach (AbstractPropertiesItem *child, mChildItems)
    {
        child->removeColumns(position, columns);
    }

    return true;
}
*/

bool AbstractPropertiesItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= mItemData.size())
    {
        return false;
    }

    mItemData[column] = value;
    return true;
}
