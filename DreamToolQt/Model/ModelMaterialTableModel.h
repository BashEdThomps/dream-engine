#pragma once

#include <QAbstractTableModel>
#include <memory>

namespace Dream
{
    class ModelDefinition;
    class MaterialDefinition;
}

using Dream::ModelDefinition;
using Dream::MaterialDefinition;
using std::shared_ptr;
using std::vector;

class ModelMaterialTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ModelMaterialTableModel(QObject* parent = nullptr);
    ~ModelMaterialTableModel() override;

    int rowCount(const QModelIndex& parent) const  override;
    int columnCount(const QModelIndex& parent) const  override;
    QVariant data(const QModelIndex& index, int role) const  override;
    void setModelDefinition(ModelDefinition* def);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const  override;
    bool setData(const QModelIndex& index,const  QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void setMaterialDefinitions(vector<MaterialDefinition*>& shaderDefinitions);

protected:
    ModelDefinition* mModelDefinitionHandle;
    vector<MaterialDefinition*> mMaterialDefinitions;
    QString getMaterialNameFromUuid(std::string uuid) const;
};
