/*
 * PropertiesItem.h
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

#pragma once

#include <QList>
#include <QVariant>

class PropertiesItem
{
public:
    PropertiesItem(const QList<QVariant> &data, void* item, PropertiesItem *parent =0);
    ~PropertiesItem();
    void appendChild(PropertiesItem *child);
    PropertiesItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PropertiesItem *parentItem();
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    void setParent(PropertiesItem* parent);
private:
    QList<PropertiesItem*> mChildItems;
    QList<QVariant> mItemData;
    PropertiesItem *mParentItemHandle;
    void           *mItemHandle;
};
