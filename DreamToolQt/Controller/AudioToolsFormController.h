#pragma once

#include "AbstractEditorWidget.h"
#include <QMediaPlayer>
#include "ui_AudioToolsForm.h"
#include <QBuffer>
#include <QAudioOutput>
#include <QThread>
#include "../Model/AudioMarkersTableModel.h"
#include "PCMPlayer.h"

namespace Dream
{
    class AudioDefinition;
    class AudioInstance;
}

class ProjectDirectoryModel;

using namespace Dream;

class AudioToolsFormController : public AbstractEditorWidget
{
    Q_OBJECT
public:
    explicit AudioToolsFormController(
        AudioDefinition* ad,
        ProjectDirectoryModel* dirModel,
        QWidget *parent = nullptr
    );
    ~AudioToolsFormController() override;

protected:
    void generateEventList(int);


    Ui::AudioToolsForm mUi;
    shared_ptr<AudioInstance> mAudioInstance;
    vector<char> mAudioSamples;
    int mChannels;
    int mFrequency;
    shared_ptr<AudioMarkersTableModel> mTableModel;
    PCMPlayer mPlayer;
    vector<int> mMarkerEvents;

protected slots:
    void onPlayButtonClicked(bool);
    void onStopButtonClicked(bool);
    void onTableSelectionChanged(const QItemSelection&,const QItemSelection&);
    void onMarkerSelectionChanged(int);
    void onSampleOffsetChanged(int smpl);
};




