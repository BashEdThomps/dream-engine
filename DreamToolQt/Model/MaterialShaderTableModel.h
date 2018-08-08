#pragma once

#include <QAbstractTableModel>

namespace Dream
{
    class ModelDefinition;
}

using Dream::ModelDefinition;

class MaterialShaderTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MaterialShaderTableModel(QObject* parent = nullptr);
    ~MaterialShaderTableModel() override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void setModelDefinition(ModelDefinition* def);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

protected:
    ModelDefinition* mModelDefinitionHandle;

};
