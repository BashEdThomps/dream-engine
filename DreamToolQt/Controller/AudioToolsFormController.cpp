#include "AudioToolsFormController.h"

#include <memory>


#include <DreamCore.h>
#include "../Model/ProjectDirectoryModel.h"

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

    connect(mUi.playButton, SIGNAL(clicked(bool)),this,SLOT(onPlayButtonClicked(bool)));
    connect(mUi.stopButton, SIGNAL(clicked(bool)),this,SLOT(onStopButtonClicked(bool)));

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
    mPlayer.loadAudioInstance(mAudioInstance.get());


    mTableModel = make_shared<AudioMarkersTableModel>(ad,mUi.eventsTableView);
    mUi.eventsTableView->setModel(mTableModel.get());
    connect(
        mUi.eventsTableView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
        this,
        SLOT(onTableSelectionChanged(const QItemSelection&,const QItemSelection&))
    );

    mUi.eventsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mUi.waveformWidget->setTableModel(mTableModel.get());

    connect(
        mUi.waveformWidget, SIGNAL(notifyScrubToSampleChanged(int)),
        &mPlayer, SLOT(onSampleOffsetChanged(int))
    );

    connect(
        &mPlayer, SIGNAL(notifySampleOffsetChanged(int)),
        mUi.waveformWidget, SLOT(onSampleOffsetChanged(int))
    );

    connect(
        &mPlayer, SIGNAL(notifySampleOffsetChanged(int)),
        this, SLOT(onSampleOffsetChanged(int))
    );

    connect(
        mUi.waveformWidget, SIGNAL(notifyMarkerSelectionChanged(int)),
        this, SLOT(onMarkerSelectionChanged(int))
    );

    connect(
        mUi.volumeSlider, SIGNAL(valueChanged(int)),
        &mPlayer, SLOT(onVolumeChanged(int))
    );
}

AudioToolsFormController::~AudioToolsFormController
()
{
    mPlayer.stop();
}

void
AudioToolsFormController::onPlayButtonClicked
(bool)
{
    qDebug() << "Starting audio";
    mPlayer.play();
}

void
AudioToolsFormController::onStopButtonClicked
(bool)
{
    qDebug() << "Stopping audio";
    mPlayer.stop();
}

void
AudioToolsFormController::onTableSelectionChanged
(const QItemSelection& selected, const QItemSelection&)
{
    qDebug() << "Table Selection Changed";
    auto indexes = selected.indexes();
    if (indexes.size() > 0)
    {
        mUi.waveformWidget->setSelectedMarker(indexes.first().row());
    }
    else
    {
        mUi.waveformWidget->setSelectedMarker(-1);
    }
    mUi.waveformWidget->update();
}

void
AudioToolsFormController::onMarkerSelectionChanged
(int marker)
{
    qDebug() << "Marker selection changed to " << marker;
    if (marker > -1) generateEventList(marker);
}

void
AudioToolsFormController::generateEventList
(int marker)
{
    qDebug() << "Generating events for marker " << marker;
    auto ad = dynamic_cast<AudioDefinition*>(mDefinitionHandle);
    auto markerStart = ad->getMarkerSampleIndex(marker);
    auto count = ad->getMarkerRepeat(marker);
    auto step = ad->getMarkerRepeatPeriod(marker);

    mMarkerEvents.clear();

    for (int i=0; i<count; i++)
    {
        auto next = markerStart + (i*step);
        qDebug() << "NeXTSTEP: " << next;
       mMarkerEvents.push_back(next);
    }
}

void
AudioToolsFormController::onSampleOffsetChanged
(int smpl)
{
    if (mMarkerEvents.empty()) return;
    auto next = mMarkerEvents.at(0);
    if (smpl >= next)
    {
        mMarkerEvents.erase(mMarkerEvents.begin());
        PCMPlayer::click();
    }
}
