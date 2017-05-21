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

PropertiesModel::PropertiesModel(QObject* parent)
    : QAbstractItemModel (parent)
{}

PropertiesModel::~PropertiesModel()
{

}

PropertiesItem *PropertiesModel
::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        PropertiesItem *item = static_cast<PropertiesItem*>(index.internalPointer());
        if (item)
        {
            return item;
        }
    }
    return mRootItem;
}

bool PropertiesModel
::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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

bool PropertiesModel
::insertColumns(int position, int columns,const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = mRootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool PropertiesModel
::removeColumns(int position, int columns, const QModelIndex &parent)
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

bool PropertiesModel
::insertRows(int position, int rows, const QModelIndex &parent)
{
    PropertiesItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, mRootItem->columnCount());
    endInsertRows();

    return success;
}

bool PropertiesModel
::removeRows(int position, int rows,  const QModelIndex &parent)
{
    PropertiesItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

bool PropertiesModel
::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
    {
        return false;
    }

    PropertiesItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
    {
        emit dataChanged(index, index);
    }

    return result;
}

QModelIndex PropertiesModel
::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    PropertiesItem *childItem = static_cast<PropertiesItem*>(index.internalPointer());
    PropertiesItem *parentItem = childItem->parentItem();

    if (parentItem == nullptr)
    {
        return QModelIndex();
    }

    if (parentItem == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int PropertiesModel
::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<PropertiesItem*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return mRootItem->columnCount();
    }
}


QVariant PropertiesModel
::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    PropertiesItem *item = static_cast<PropertiesItem*>(index.internalPointer());

    return item->data(index.column());
}


Qt::ItemFlags PropertiesModel
::flags(const QModelIndex &index) const
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

QVariant PropertiesModel
::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return mRootItem->data(section);
    }

    return QVariant();
}


QModelIndex PropertiesModel
::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    PropertiesItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<PropertiesItem*>(parent.internalPointer());
    }

    PropertiesItem *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

int PropertiesModel
::rowCount(const QModelIndex &parent) const
{
    PropertiesItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = mRootItem;
    }
    else
    {
        parentItem = static_cast<PropertiesItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}
