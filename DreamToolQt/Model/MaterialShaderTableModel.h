#pragma once

#include <QAbstractTableModel>
#include <memory>

namespace Dream
{
    class ModelDefinition;
}

using Dream::ModelDefinition;
using std::shared_ptr;

class MaterialShaderTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MaterialShaderTableModel(QObject* parent = nullptr);
    ~MaterialShaderTableModel() override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void setModelDefinition(shared_ptr<ModelDefinition> def);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

protected:
    shared_ptr<ModelDefinition> mModelDefinitionHandle;

};
