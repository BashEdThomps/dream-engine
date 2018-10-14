#pragma once

#include <QAbstractTableModel>

namespace Dream
{
    class MaterialDefinition;
}

using Dream::MaterialDefinition;

class MaterialParametersTableModel : public QAbstractTableModel
{
public:
    MaterialParametersTableModel(MaterialDefinition* def, QObject* parent = nullptr);
    ~MaterialParametersTableModel() override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    MaterialDefinition* mMaterialDefinitionHandle;
    const static QStringList mRows;

};
