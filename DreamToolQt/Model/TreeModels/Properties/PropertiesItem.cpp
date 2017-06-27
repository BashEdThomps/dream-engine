/*
 * PropertiesItem.cpp
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

#include "PropertiesItem.h"

#include <QDebug>

PropertiesItem::PropertiesItem(const QList<QVariant> &data, void* item, PropertiesItem *parent)
{
    qDebug() << "PropertiesItem: Constructing";
    mItemData = data;
    mParentItemHandle = parent;
    mItemHandle = item;
}

PropertiesItem::~PropertiesItem()
{
    qDebug() << "PropertiesItem: Destructing";
    qDeleteAll(mChildItems);
}

void PropertiesItem::setParent(PropertiesItem* parent)
{
    mParentItemHandle = parent;
}

void PropertiesItem::appendChild(PropertiesItem *item)
{
    item->setParent(this);
    mChildItems.append(item);
}

PropertiesItem *PropertiesItem::child(int row)
{
    return mChildItems.value(row);
}

int PropertiesItem::childCount() const
{
    return mChildItems.count();
}

int PropertiesItem::columnCount() const
{
    return mItemData.count();
}

QVariant PropertiesItem::data(int column) const
{
    return mItemData.value(column);
}

PropertiesItem *PropertiesItem::parentItem()
{
    return mParentItemHandle;
}

int PropertiesItem::row() const
{
    if (mParentItemHandle)
    {
        return mParentItemHandle->mChildItems.indexOf(const_cast<PropertiesItem*>(this));
    }
    return 0;
}

int PropertiesItem::childNumber() const
{
    if (mParentItemHandle)
    {
        return mParentItemHandle->mChildItems.indexOf(const_cast<PropertiesItem*>(this));
    }
    return 0;
}

bool PropertiesItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > mChildItems.size())
    {
        return false;
    }

    for (int row = 0; row < count; ++row)
    {
        QList<QVariant> data;
        PropertiesItem *item = new PropertiesItem(data, nullptr, this);
        mChildItems.insert(position, item);
    }
    return true;
}

bool PropertiesItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > mItemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        mItemData.insert(position, QVariant());
    }

    foreach (PropertiesItem *child, mChildItems)
    {
        child->insertColumns(position, columns);
    }

    return true;
}

bool PropertiesItem::removeChildren(int position, int count)
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

bool PropertiesItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > mItemData.size())
    {
        return false;
    }

    for (int column = 0; column < columns; ++column)
    {
        mItemData.removeAt(position);
    }

    foreach (PropertiesItem *child, mChildItems)
    {
        child->removeColumns(position, columns);
    }

    return true;
}

bool PropertiesItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= mItemData.size())
    {
        return false;
    }

    mItemData[column] = value;
    return true;
}
