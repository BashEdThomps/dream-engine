/*
 * PropertiesModel.cpp
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
#include "AbstractPropertiesModel.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using std::pair;

AbstractPropertiesModel::AbstractPropertiesModel(QItemDelegate* delegate, QTreeView* parent)
    : QAbstractItemModel (parent),
      mTreeViewHandle(parent),
      mDelegateHandle(delegate)
{
    auto log = spdlog::get("AbstractPropertiesModel");
    if (log==nullptr)
    {
        log = spdlog::stderr_color_mt("AbstractPropertiesModel");
    }
    log->debug("PropertiesModel: Constructing");
    mTreeViewHandle->setItemDelegateForColumn(1,delegate);
}

AbstractPropertiesModel::~AbstractPropertiesModel
()
{
    auto log = spdlog::get("AbstractPropertiesModel");
    log->debug("PropertiesModel: Destructing");
}

AbstractPropertiesItem*
AbstractPropertiesModel::getItem
(const QModelIndex &index)
{
    if (index.isValid())
    {
        AbstractPropertiesItem *item = static_cast<AbstractPropertiesItem*>(index.internalPointer());
        if (item)
        {
            return item;
        }
    }
    return mRootItem.get();
}

bool AbstractPropertiesModel::setHeaderData
(int section, Qt::Orientation orientation,const  QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
    {
        return false;
    }

    bool result = mRootItem->setData(section, value);

    if (result)
    {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool
AbstractPropertiesModel::setData
(const QModelIndex &index,const   QVariant &value, int role)
{
    AbstractPropertiesItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
    {
        emit dataChanged(index, index);
    }

    return result;
}

QModelIndex
AbstractPropertiesModel::parent
(const  QModelIndex &index)
const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    AbstractPropertiesItem *childItem = static_cast<AbstractPropertiesItem*>(index.internalPointer());
    AbstractPropertiesItem *parentItem = childItem->parentItem();

    if (parentItem == nullptr)
    {
        return QModelIndex();
    }

    if (parentItem == mRootItem.get())
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int
AbstractPropertiesModel::columnCount
(const  QModelIndex &parent)
const
{
    if (parent.isValid())
    {
        return static_cast<AbstractPropertiesItem*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return mRootItem->columnCount();
    }
}


QVariant
AbstractPropertiesModel::data
(const  QModelIndex &index, int role)
const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::SizeHintRole)
    {
        return QSize(200,24);
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    AbstractPropertiesItem *item = static_cast<AbstractPropertiesItem*>(index.internalPointer());

    return item->data(index.column());
}


Qt::ItemFlags
AbstractPropertiesModel::flags
(const  QModelIndex &index)
const
{
    if (!index.isValid())
    {
        return nullptr;
    }

    if (index.column() != 0)
    {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }

    return QAbstractItemModel::flags(index);
}

QVariant
AbstractPropertiesModel::headerData
(int section, Qt::Orientation orientation, int role)
const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:
                return mRootItem->data(section);
        }
    }

    return QVariant();
}


QModelIndex
AbstractPropertiesModel::index
(int row, int column,const   QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    AbstractPropertiesItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem.get();
    }
    else
    {
        parentItem = static_cast<AbstractPropertiesItem*>(parent.internalPointer());
    }

    AbstractPropertiesItem *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

int
AbstractPropertiesModel::rowCount
(const  QModelIndex &parent)
const
{
    AbstractPropertiesItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = mRootItem.get();
    }
    else
    {
        parentItem = static_cast<AbstractPropertiesItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

void AbstractPropertiesModel::forceDataChanged()
{
   auto log = spdlog::get("AbstractPropertiesModel");
   log->debug("Force Data Changed");
   beginResetModel();
   createRoot();
   createProperties();
   createDelegateConnections();
   endResetModel();
   //mTreeViewHandle->expandAll();
   emit dataChanged(QModelIndex(), QModelIndex());
}
