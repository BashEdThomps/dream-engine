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

#include <spdlog/spdlog.h>

AbstractPropertiesItem::AbstractPropertiesItem
(QString title, QItemDelegate* delegate, AbstractPropertiesItem *parent)
    : mTitle(title),
      mParentItemHandle(parent)
{
    auto log = spdlog::get("AbstractPropertiesItem");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("AbstractPropertiesItem");
    }
    log->info("Constructing");
    setDelegate(delegate);
}

AbstractPropertiesItem::~AbstractPropertiesItem
()
{

    auto log = spdlog::get("AbstractPropertiesItem");
    log->info("Destructing");
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
    return 2;
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