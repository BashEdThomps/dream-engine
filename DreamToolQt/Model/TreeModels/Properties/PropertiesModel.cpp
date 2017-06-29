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
#include "PropertiesModel.h"
#include <QDebug>
#include "ItemDelegateProxy.h"

using std::pair;

PropertiesModel::PropertiesModel(QTreeView* parent)
    : QAbstractItemModel (parent)
{
    qDebug() << "PropertiesModel: Constructing";
    mTreeViewHandle = parent;
    mItemDelegateProxy.reset(new ItemDelegateProxy());
    mTreeViewHandle->setItemDelegateForColumn(1,mItemDelegateProxy.get());
}

PropertiesModel::~PropertiesModel
()
{
    qDebug() << "PropertiesModel: Destructing";
}

AbstractPropertiesItem *PropertiesModel::getItem(const QModelIndex &index) const
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

bool PropertiesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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

/*
bool PropertiesModel::insertColumns(int position, int columns,const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = mRootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}
*/

/*
bool PropertiesModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = mRootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (mRootItem->columnCount() == 0)
    {
        removeRows(0, rowCount());
    }

    return success;
}
*/

/*
bool PropertiesModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    AbstractPropertiesItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, mRootItem->columnCount());
    endInsertRows();

    return success;
}
*/

/*
bool PropertiesModel::removeRows(int position, int rows,  const QModelIndex &parent)
{
    AbstractPropertiesItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}
*/

bool PropertiesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    /*if (role != Qt::EditRole)
    {
        return false;
    }
    */

    AbstractPropertiesItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
    {
        emit dataChanged(index, index);
    }

    return result;
}

QModelIndex PropertiesModel::parent(const QModelIndex &index) const
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

int PropertiesModel::columnCount(const QModelIndex &parent) const
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


QVariant PropertiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::SizeHintRole)
    {
        return QSize(100,24);
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    AbstractPropertiesItem *item = static_cast<AbstractPropertiesItem*>(index.internalPointer());

    return item->data(index.column());
}


Qt::ItemFlags PropertiesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    if (index.column() != 0)
    {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }

    return QAbstractItemModel::flags(index);
}

QVariant PropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }

    return QVariant();
}


QModelIndex PropertiesModel::index(int row, int column, const QModelIndex &parent) const
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

int PropertiesModel::rowCount(const QModelIndex &parent) const
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
