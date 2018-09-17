/*
 * AbstractPropertiesItem.h
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
#include <memory>

#include <QItemDelegate>

using std::string;
using std::unique_ptr;

class AbstractPropertiesItem
{
public:
    AbstractPropertiesItem
    (
        QString title,
        QItemDelegate* delegate = nullptr,
        AbstractPropertiesItem *parent = nullptr
    );

    virtual ~AbstractPropertiesItem();

    void appendChild(AbstractPropertiesItem *child);
    AbstractPropertiesItem *child(int row);
    int childCount() ;
    int childNumber() ;

    int row() ;
    int columnCount() ;

    AbstractPropertiesItem *parentItem();
    void setParent(AbstractPropertiesItem* parent);

    void setDelegate(QItemDelegate* delegate);
    QItemDelegate* getDelegate();

    virtual bool setData(int column,const  QVariant &value) = 0;
    virtual QVariant data(int column) = 0;

protected:
    QList<AbstractPropertiesItem*> mChildItems;
    QString mTitle;
    AbstractPropertiesItem *mParentItemHandle;
    unique_ptr<QItemDelegate> mDelegate;
};
