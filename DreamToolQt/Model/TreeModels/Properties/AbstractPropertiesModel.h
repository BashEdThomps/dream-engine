/*
 * PropertiesModel.h
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

#pragma once

#include "AbstractPropertiesItem.h"

#include <QAbstractItemModel>
#include <QTreeView>
#include <QItemDelegate>

#include <memory>

using std::unique_ptr;

class AbstractPropertiesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    AbstractPropertiesModel(QItemDelegate* delegate, QTreeView* parent = 0);
    ~AbstractPropertiesModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    AbstractPropertiesItem *getItem(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual void createRoot() = 0;
    virtual void createProperties() = 0;

protected:
    unique_ptr<AbstractPropertiesItem> mRootItem;
    QTreeView *mTreeViewHandle;
};
