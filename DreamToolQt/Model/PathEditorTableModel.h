#pragma once

#include <QAbstractTableModel>
#include <memory>

namespace Dream
{
    class PathDefinition;
}

using Dream::PathDefinition;
using std::shared_ptr;

class PathEditorTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PathEditorTableModel(QObject* parent = nullptr);
    ~PathEditorTableModel() override;

    int rowCount(const QModelIndex& parent) const  override;
    int columnCount(const  QModelIndex& parent) const  override;
    QVariant data(const  QModelIndex& index, int role) const  override;
    void setPathDefinition(PathDefinition* def);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const  override;
    bool setData(const  QModelIndex& index, const  QVariant& value, int role) override;
    Qt::ItemFlags flags( const QModelIndex& index)const   override;
    bool insertRows(int row, int count, const  QModelIndex& parent) override;
    bool removeRows(int row, int count, const  QModelIndex& parent) override;

signals:
    void changed();

protected:
    PathDefinition* mPathDefinitionHandle;
};

