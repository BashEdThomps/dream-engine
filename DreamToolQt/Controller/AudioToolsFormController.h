#pragma once

#include "AbstractEditorWidget.h"
#include <QMediaPlayer>
#include "ui_AudioToolsForm.h"

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
    Ui::AudioToolsForm mUi;
    shared_ptr<AudioInstance> mAudioInstance;
    vector<char> mAudioSamples;
    QMediaPlayer mMediaPlayer;
    int mChannels;
    int mFrequency;
};
