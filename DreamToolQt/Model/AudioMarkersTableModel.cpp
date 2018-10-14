#include "AudioMarkersTableModel.h"
#include <DreamCore.h>
#include <QDebug>

AudioMarkersTableModel::AudioMarkersTableModel
(AudioDefinition* definition, QObject* parent)
    : QAbstractTableModel (parent),
      mAudioDefinitionHandle(definition)
{
    auto log = spdlog::get("AudioMarkersTableModel");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("AudioMarkersTableModel");
    }

}

AudioMarkersTableModel::~AudioMarkersTableModel
()
{}

int
AudioMarkersTableModel::rowCount
(const QModelIndex&)
const
{
    auto log = spdlog::get("AudioMarkersTableModel");
    auto rc = static_cast<int>(mAudioDefinitionHandle->countMarkers());
    //log->critical("RowCount {}",rc);
    return rc;
}

int
AudioMarkersTableModel::columnCount
(const QModelIndex&)
const
{
    auto log = spdlog::get("AudioMarkersTableModel");
    //log->critical("ColumnCount 3");
    return 5;
}

QVariant
AudioMarkersTableModel::data
(const QModelIndex& index, int role)
const
{
    auto log = spdlog::get("AudioMarkersTableModel");
    //log->critical("Data {} {} {}",index.row(), index.column(), role);
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    if (mAudioDefinitionHandle == nullptr)
    {
        return QVariant();
    }

    auto markerIndex = index.row();
    switch (index.column())
    {
       case 0: // Index
            return markerIndex;
        case 1: // Name
            return QString::fromStdString(mAudioDefinitionHandle->getMarkerName(markerIndex));
        case 2: // Sample Index
            return QVariant::fromValue(mAudioDefinitionHandle->getMarkerSampleIndex(markerIndex));
        case 3: // Repeat
            return QVariant::fromValue(mAudioDefinitionHandle->getMarkerRepeat(markerIndex));
        case 4: // Period
            return QVariant::fromValue(mAudioDefinitionHandle->getMarkerRepeatPeriod(markerIndex));

    }

    return QVariant();
}

bool
AudioMarkersTableModel::setData
(const QModelIndex& index, const QVariant& value, int role)
{
    auto log = spdlog::get("AudioMarkersTableModel");
    log->critical("SetData {} {} {}",index.row(),index.column(),role);
    if (!index.isValid())
    {
        return false;
    }

    if (mAudioDefinitionHandle == nullptr)
    {
        return false;
    }

    auto markerIndex = index.row();

    switch (index.column())
    {
        case 0: // Index
            break;
        case 1: // Name
            mAudioDefinitionHandle->setMarkerName(markerIndex,value.toString().toStdString());
            break;
        case 2: // Sample Index
            mAudioDefinitionHandle->setMarkerSampleIndex(markerIndex,value.toInt());
            break;
        case 3: // Repeat
            mAudioDefinitionHandle->setMarkerRepeat(markerIndex,value.toInt());
            break;
        case 4: // Repeat Period
            mAudioDefinitionHandle->setMarkerRepeatPeriod(markerIndex,value.toInt());
            break;
    }

    emit dataChanged(index,index);
    return true;
}

QVariant
AudioMarkersTableModel::headerData
(int section, Qt::Orientation orientation, int role)
const
{
    //auto log = spdlog::get("AudioMarkersTableModel");
    //log->critical("HeaderData {} {}",section,role);
    if (role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Index";
            case 1:
                return "Name";
            case 2:
                return "Sample Index";
            case 3:
                return "Repeat";
            case 4:
                return "Repeat Period";
        }
    }
    return QVariant();
}


bool
AudioMarkersTableModel::insertRows
(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent,row,row+count);
    auto log = spdlog::get("AudioMarkersTableModel");
    log->critical("InsertRows {} {}",row,count);
    for (int i=0; i<count; i++)
    {
        log->critical("Inserting {}",i);
    }
    endInsertRows();
    return true;
}

bool
AudioMarkersTableModel::removeRows
(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent,row,row+count);
    auto log = spdlog::get("AudioMarkersTableModel");
    log->critical("RemoveRows {} {}",row,count);
    for (int i=0; i<count; i++)
    {
        log->critical("RemoveRows Removing {}",i);
        mAudioDefinitionHandle->removeMarker(row+i);
    }
    endRemoveRows();
    return true;
}


Qt::ItemFlags
AudioMarkersTableModel::flags
(const QModelIndex& index) const
{
    switch(index.column())
    {
        case 0:
            return QAbstractItemModel::flags(index);
        default:
            return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
}

void
AudioMarkersTableModel::update
()
{
    qDebug() << "Updating AudioMarkersTableModel";
    emit dataChanged(QModelIndex(),QModelIndex());
}

AudioDefinition* AudioMarkersTableModel::getAudioDefinitionHandle() const
{
    return mAudioDefinitionHandle;
}
