#include "MaterialShaderTableModel.h"
#include <spdlog/spdlog.h>
#include "DreamCore.h"

using Dream::ModelDefinition;
using Dream::Constants;

MaterialShaderTableModel::MaterialShaderTableModel
(QObject* parent)
    : QAbstractTableModel(parent),
      mModelDefinitionHandle(nullptr)
{
    auto log=spdlog::get("MaterialShaderTableModel");
    if (log==nullptr)
    {
        log=spdlog::stdout_color_mt("MaterialShaderTableModel");
    }
    log->info("Constructing");
}

MaterialShaderTableModel::~MaterialShaderTableModel()
{
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("Destructing");
}

int
MaterialShaderTableModel::rowCount
(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("rowCount");


    if (mModelDefinitionHandle == nullptr)
    {
        return 0;
    }

    int rows = mModelDefinitionHandle->getMaterialShaders()->size();
    log->info("{} rows",rows);
    return rows;

}

int
MaterialShaderTableModel::columnCount
(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant
MaterialShaderTableModel::data
(const QModelIndex& index, int role) const
{
    auto log = spdlog::get("MaterialShaderTableModel");

    if (mModelDefinitionHandle == nullptr)
    {
        log->error("ModelDefinition is null, i was looking for row {}",index.row());
        return QVariant();
    }

    if (!index.isValid())
        return QVariant();

    if (index.row() >= mModelDefinitionHandle->getMaterialShaders()->size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        json pair = mModelDefinitionHandle->getMaterialShaders()->at(index.row());
        if (pair.is_null())
        {
            log->error("Row {} is null pair",index.row());
            return QVariant();
        }

        string valStr;

        switch (index.column())
        {
            case 0:
                valStr = pair[Constants::ASSET_ATTR_MODEL_MATERIAL];;
                log->info("Getting col 0 (material) {}",valStr);
                return QString::fromStdString(valStr);
            case 1:
                valStr = pair[Constants::ASSET_ATTR_MODEL_SHADER];
                log->info("Getting col 0 (shader) {}", valStr);
                return QString::fromStdString(valStr);
        }
    }
    return QVariant();
}

void
MaterialShaderTableModel::setModelDefinition
(ModelDefinition* def)
{
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("setModelDefinition");

    beginResetModel();
    mModelDefinitionHandle = def;
    endResetModel();
}

QVariant
MaterialShaderTableModel::headerData
(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
             return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString("Material");
            case 1:
                return QString("Shader");
        }
    }
    return QVariant();
}

bool
MaterialShaderTableModel::setData
(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(role)
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("SetData");

    if (mModelDefinitionHandle != nullptr)
    {
        string valStr = value.toString().toStdString();
        json* list = mModelDefinitionHandle->getMaterialShaders();

        switch(index.column())
        {
            case 0:
                log->info("setting row {} col 0 material to {}",index.row(),valStr);
                list->at(index.row())[Constants::ASSET_ATTR_MODEL_MATERIAL] = valStr;
                break;
            case 1:
                log->info("setting row {} col 1 shader to {}",index.row(),valStr);
                list->at(index.row())[Constants::ASSET_ATTR_MODEL_SHADER] = valStr;
                break;
        }
        emit dataChanged(index,index);
    }
    return true;
}

/*bool
MaterialShaderTableModel::insertRows
(int row, int count, const QModelIndex& index)
{
    Q_UNUSED(index)
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("insertRows");

    if (mModelDefinitionHandle != nullptr)
    {
        log->info("inserting {} rows at {}", count, row);
        beginInsertRows(QModelIndex(),row, row+count-1);
        for (int i=0; i<count; i++)
        {
            auto newRow = json::object();
            newRow[Constants::ASSET_ATTR_MODEL_MATERIAL] = "Unset Material";
            newRow[Constants::ASSET_ATTR_MODEL_SHADER] = "Unset Shader";
            mModelDefinitionHandle->getMaterialShaders()->push_back(newRow);
        }
        endInsertRows();
    }
    else
    {
        log->info("No ModelDefinition Pointer");
    }
    return true;
}
*/

/*
bool
MaterialShaderTableModel::removeRows
(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(parent)
    if (row < 0 || count < 1)
    {
        return true;
    }
    auto log = spdlog::get("MaterialShaderTableModel");
    log->info("Removing row(s) from {} count {}",row,count);
    if (mModelDefinitionHandle != nullptr)
    {
        if (mModelDefinitionHandle->getMaterialShaders()->empty())
        {
            return true;
        }

        beginRemoveRows(QModelIndex(),row, row+count-1);
        auto start = mModelDefinitionHandle->getMaterialShaders()->begin();
        for (int i=0; i<count; i++)
        {
            mModelDefinitionHandle->getMaterialShaders()->erase(start+row+i);
        }
        endRemoveRows();
    }
    return true;
}
*/

Qt::ItemFlags MaterialShaderTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0)
    {
        return QAbstractTableModel::flags(index) & !(Qt::ItemIsEditable);
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
