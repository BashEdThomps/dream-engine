#pragma once

#include <QAbstractTableModel>

namespace Dream
{
    class AudioDefinition;
    class AudioEventMarker;
}

using namespace Dream;

class AudioMarkersTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AudioMarkersTableModel(AudioDefinition* def, QObject* parent = nullptr);
    ~AudioMarkersTableModel() override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void update();

    AudioDefinition* getAudioDefinitionHandle() const;

protected:
    AudioDefinition* mAudioDefinitionHandle;
};
