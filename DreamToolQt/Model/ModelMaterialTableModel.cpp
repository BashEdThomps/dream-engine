#include "ModelMaterialTableModel.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "DreamCore.h"

using Dream::ModelDefinition;
using Dream::Constants;

ModelMaterialTableModel::ModelMaterialTableModel
(QObject* parent)
    : QAbstractTableModel(parent),
      mModelDefinitionHandle(nullptr)
{
    auto log=spdlog::get("ModelMaterialTableModel");
    if (log==nullptr)
    {
        log=spdlog::stdout_color_mt("ModelMaterialTableModel");
    }
    log->debug("Constructing");
}

ModelMaterialTableModel::~ModelMaterialTableModel()
{
    auto log = spdlog::get("ModelMaterialTableModel");
    log->debug("Destructing");
}

int
ModelMaterialTableModel::rowCount
(const QModelIndex& parent)
const
{
    Q_UNUSED(parent)
    auto log = spdlog::get("ModelMaterialTableModel");
    log->debug("rowCount");


    if (mModelDefinitionHandle == nullptr)
    {
        return 0;
    }

    int rows = mModelDefinitionHandle->getModelMaterials()->size();
    log->debug("{} rows",rows);
    return rows;

}

int
ModelMaterialTableModel::columnCount
(const  QModelIndex& parent)
const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant
ModelMaterialTableModel::data
(const  QModelIndex& index, int role)
const
{
    auto log = spdlog::get("ModelMaterialTableModel");

    if (mModelDefinitionHandle == nullptr)
    {
        log->error("ModelDefinition is null, i was looking for row {}",index.row());
        return QVariant();
    }

    if (!index.isValid())
        return QVariant();

    if (index.row() >= mModelDefinitionHandle->getModelMaterials()->size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        json pair = mModelDefinitionHandle->getModelMaterials()->at(index.row());
        if (pair.is_null())
        {
            log->error("Row {} is null pair",index.row());
            return QVariant();
        }

        string valStr;

        switch (index.column())
        {
            case 0:
                valStr = pair[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL];;
                log->debug("Getting col 0 (material) {}",valStr);
                return QString::fromStdString(valStr);
            case 1:
                valStr = pair[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL];
                log->debug("Getting col 0 (shader) {}", valStr);
                return getMaterialNameFromUuid(valStr);
        }
    }
    return QVariant();
}

void
ModelMaterialTableModel::setModelDefinition
(ModelDefinition* def)
{
    auto log = spdlog::get("ModelMaterialTableModel");
    log->debug("setModelDefinition");

    beginResetModel();
    mModelDefinitionHandle = def;
    endResetModel();
}

QVariant
ModelMaterialTableModel::headerData
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
                return QString("Material");
            case 1:
                return QString("Shader");
        }
    }
    return QVariant();
}

bool
ModelMaterialTableModel::setData
(const QModelIndex& index,const  QVariant& value, int role)
{
    Q_UNUSED(role)
    auto log = spdlog::get("ModelMaterialTableModel");
    log->debug("SetData");

    if (mModelDefinitionHandle != nullptr)
    {
        string valStr = value.toString().toStdString();
        json* list = mModelDefinitionHandle->getModelMaterials();

        switch(index.column())
        {
            case 0:
                log->debug("setting row {} col 0 material to {}",index.row(),valStr);
                list->at(index.row())[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL] = valStr;
                break;
            case 1:
                log->debug("setting row {} col 1 shader to {}",index.row(),valStr);
                list->at(index.row())[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL] = valStr;
                break;
        }
        emit dataChanged(index,index);
    }
    return true;
}

Qt::ItemFlags
ModelMaterialTableModel::flags
(const QModelIndex &index)
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

void
ModelMaterialTableModel::setMaterialDefinitions
(vector<MaterialDefinition*>& shaderDefinitions)
{
    mMaterialDefinitions = shaderDefinitions;
}

QString
ModelMaterialTableModel::getMaterialNameFromUuid
(string uuid)
const
{
  for (auto sDef : mMaterialDefinitions)
  {
      if (sDef->getUuid().compare(uuid) == 0)
      {
          return QString::fromStdString(sDef->getName());
      }
  }
  return QString();
}
