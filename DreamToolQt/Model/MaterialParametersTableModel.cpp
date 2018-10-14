#include "MaterialParametersTableModel.h"
#include <DreamCore.h>

MaterialParametersTableModel::MaterialParametersTableModel
(Dream::MaterialDefinition* def, QObject* parent)
    : QAbstractTableModel (parent),
      mMaterialDefinitionHandle(def)
{

}

MaterialParametersTableModel::~MaterialParametersTableModel
()
{

}

int
MaterialParametersTableModel::rowCount
(const QModelIndex&)
const
{
    return mRows.size();
}

int MaterialParametersTableModel::columnCount
(const QModelIndex&)
const
{
    return 2;
}

QVariant MaterialParametersTableModel::data
(const QModelIndex& index, int role)
const
{
    if (!index.isValid())
    {
        return  QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0:
                return mRows.at(index.row());
            case 1:
                switch(index.row())
                {
                    case 0:
                        return mMaterialDefinitionHandle->getOpacity();
                    case 1:
                        return mMaterialDefinitionHandle->getBumpScaling();
                    case 2:
                        return mMaterialDefinitionHandle->getHardness();
                    case 3:
                        return mMaterialDefinitionHandle->getReflectivity();
                    case 4:
                        return mMaterialDefinitionHandle->getShininessStrength();
                    case 5:
                        return mMaterialDefinitionHandle->getRefractionIndex();
                }
                break;
        }
    }
    return QVariant();
}

bool MaterialParametersTableModel::setData
(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
    {
        return true;
    }

    if (role == Qt::EditRole)
    {

        switch (index.column())
        {
            case 1:
                switch(index.row())
                {
                    case 0:
                        mMaterialDefinitionHandle->setOpacity(value.toFloat());
                        break;
                    case 1:
                        mMaterialDefinitionHandle->setBumpScaling(value.toFloat());
                        break;
                    case 2:
                        mMaterialDefinitionHandle->setHardness(value.toFloat());
                        break;
                    case 3:
                        mMaterialDefinitionHandle->setReflectivity(value.toFloat());
                        break;
                    case 4:
                        mMaterialDefinitionHandle->setShininessStrength(value.toFloat());
                        break;
                    case 5:
                        mMaterialDefinitionHandle->setRefractionIndex(value.toFloat());
                        break;
                }
                break;
        }
    }
    return true;
}

const QStringList MaterialParametersTableModel::mRows =
{
    "Opacity",
    "Bump Scaling",
    "Hardness",
    "Reflectivity",
    "Shininess Strength",
    "Refraction Index"
};


Qt::ItemFlags
MaterialParametersTableModel::flags
(const QModelIndex& index)
const
{
    switch (index.column())
    {
        case 1:
            return QAbstractTableModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
        default:
            return QAbstractTableModel::flags(index);
    }
}
