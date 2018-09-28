#include "AudioToolsFormController.h"
#include <DreamCore.h>
#include <memory>
#include "../Model/ProjectDirectoryModel.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace std;
using namespace Dream;

AudioToolsFormController::AudioToolsFormController
(AudioDefinition* ad, ProjectDirectoryModel* dirModel, QWidget *parent)
    : AbstractEditorWidget(ad, dirModel, parent),
      mChannels(-1),
      mFrequency(-1)
{
    auto log = spdlog::get("AudioToolsFormController");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("AudioToolsFormController");
    }

    mUi.setupUi(this);

    if (ad->isFormatOgg())
    {
        mAudioInstance = make_shared<OggAudioInstance>(nullptr,ad,nullptr);
    }
    else if (ad->isFormatWav())
    {
        mAudioInstance = make_shared<WavAudioInstance>(nullptr,ad,nullptr);
    }

    mAudioInstance->load(dirModel->getProjectDirectoryAbsolutePath().toStdString());
    mAudioSamples = mAudioInstance->getAudioDataBuffer();
    mFrequency = mAudioInstance->getFrequency();
    mChannels = mAudioInstance->getChannels();
    mUi.waveformWidget->setData(mChannels,mFrequency,mAudioSamples);
    auto absPath = mAudioInstance->getAbsolutePath();
    log->trace("Loading audio from {}",absPath);
    mMediaPlayer.setMedia(QUrl::fromLocalFile(QString::fromStdString(absPath)));
    mMediaPlayer.setVolume(50);
    mMediaPlayer.play();
}

AudioToolsFormController::~AudioToolsFormController
()
{

}
