#pragma once

#include <QAbstractTableModel>
#include <memory>

namespace Dream
{
    class ModelDefinition;
    class ShaderDefinition;
}

using Dream::ModelDefinition;
using Dream::ShaderDefinition;
using std::shared_ptr;
using std::vector;

class MaterialShaderTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MaterialShaderTableModel(QObject* parent = nullptr);
    ~MaterialShaderTableModel() override;

    int rowCount(const QModelIndex& parent) const  override;
    int columnCount(const QModelIndex& parent) const  override;
    QVariant data(const QModelIndex& index, int role) const  override;
    void setModelDefinition(ModelDefinition* def);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const  override;
    bool setData(const QModelIndex& index,const  QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setShaderDefinitions(vector<ShaderDefinition*>& shaderDefinitions);
protected:
    ModelDefinition* mModelDefinitionHandle;
    vector<ShaderDefinition*> mShaderDefinitions;
    QString getShaderNameFromUuid(std::string uuid) const;
};
