#include "PathEditorTableModel.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "DreamCore.h"

using Dream::PathDefinition;
using Dream::Constants;

PathEditorTableModel::PathEditorTableModel
(QObject* parent)
    : QAbstractTableModel(parent),
      mPathDefinitionHandle(nullptr)
{
    auto log=spdlog::get("PathEditorTableModel");
    if (log==nullptr)
    {
        log=spdlog::stdout_color_mt("PathEditorTableModel");
    }
    log->trace("Constructing");
}

PathEditorTableModel::~PathEditorTableModel()
{
    auto log = spdlog::get("PathEditorTableModel");
    log->trace("Destructing");
}

int
PathEditorTableModel::rowCount
(const QModelIndex& parent)
const
{
    Q_UNUSED(parent)
    auto log = spdlog::get("PathEditorTableModel");
    log->trace("rowCount");


    if (mPathDefinitionHandle == nullptr)
    {
        return 0;
    }

    return mPathDefinitionHandle->getControlPoints()->size();
}

int
PathEditorTableModel::columnCount
(const  QModelIndex& parent)
const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant
PathEditorTableModel::data
(const QModelIndex& index, int role)
const
{
    auto log = spdlog::get("PathEditorTableModel");

    if (mPathDefinitionHandle == nullptr)
    {
        log->error("PathDefinition is null, i was looking for row {}",index.row());
        return QVariant();
    }

    if (!index.isValid())
        return QVariant();

    if (index.row() >= mPathDefinitionHandle->getControlPoints()->size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        json* row = mPathDefinitionHandle->getControlPoint(index.row());

        float retval = 0.0f;
        switch(index.column())
        {
            case 0:
                retval =  index.row();//(*row)[Constants::ASSET_ATTR_INDEX];
                break;
            case 1:
                retval =  (*row)[Constants::X];
                break;
            case 2:
                retval = (*row)[Constants::Y];
                break;
            case 3:
                retval = (*row)[Constants::Z];
                break;
        }
        return retval;
    }
    return QVariant();
}

void
PathEditorTableModel::setPathDefinition
(PathDefinition* def)
{
    auto log = spdlog::get("PathEditorTableModel");
    log->trace("setPathDefinition");

    beginResetModel();
    mPathDefinitionHandle = def;
    endResetModel();
}

QVariant
PathEditorTableModel::headerData
(int section, Qt::Orientation orientation, int role)
const
{
    if (role != Qt::DisplayRole)
             return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString("");
            case 1:
                return QString("X");
            case 2:
                return QString("Y");
            case 3:
                return QString("Z");
        }
    }
    return QVariant();
}

bool
PathEditorTableModel::setData
(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(role)
    auto log = spdlog::get("PathEditorTableModel");
    log->trace("SetData");
    json* cp = mPathDefinitionHandle->getControlPoint(index.row());
    switch(index.column())
    {
        case 0: // Index
            (*cp)[Constants::ASSET_ATTR_INDEX] = value.toInt();
            break;
        case 1: // X
            (*cp)[Constants::X] = value.toFloat();
            break;
        case 2: // Y
            (*cp)[Constants::Y] = value.toFloat();
            break;
        case 3: // Z
            (*cp)[Constants::Z] = value.toFloat();
            break;
    }
    emit dataChanged(index,index);
    emit changed();
    return true;
}

Qt::ItemFlags
PathEditorTableModel::flags
(const  QModelIndex &index)
const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0)
    {
        return QAbstractTableModel::flags(index) & !(Qt::ItemIsEditable);
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool
PathEditorTableModel::insertRows
(int row, int count,const   QModelIndex& index)
{
    Q_UNUSED(index)
    auto log = spdlog::get("PathEditorTableModel");
    log->debug("insertRows");

    if (mPathDefinitionHandle != nullptr)
    {
        log->debug("inserting {} rows at {}", count, row);
        beginInsertRows(QModelIndex(),row, row+count-1);
        for (int i=0; i<count; i++)
        {
            auto newRow = mPathDefinitionHandle->createControlPoint(row+i);
            mPathDefinitionHandle->getControlPoints()->push_back(newRow);
        }
        endInsertRows();
    }
    else
    {
        log->debug("No PathDefinition Pointer");
    }
    emit changed();
    return true;
}

bool
PathEditorTableModel::removeRows
(int row, int count,const   QModelIndex& parent)
{
    Q_UNUSED(parent)
    if (row < 0 || count < 1)
    {
        return true;
    }

    auto log = spdlog::get("PathEditorTableModel");
    log->debug("Removing row(s) from {} count {}",row,count);
    if (mPathDefinitionHandle != nullptr)
    {
        if (mPathDefinitionHandle->getControlPoints()->empty())
        {
            return true;
        }

        beginRemoveRows(QModelIndex(),row, row+count-1);
        auto start = mPathDefinitionHandle->getControlPoints()->begin();
        for (int i=0; i<count; i++)
        {
            mPathDefinitionHandle->getControlPoints()->erase(start+row+i);
        }
        endRemoveRows();
    }
    emit changed();
    return true;
}
