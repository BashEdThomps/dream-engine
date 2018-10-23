#include "PCMPlayer.h"

#include <QDebug>
#include <thread>

PCMPlayer::PCMPlayer(QObject *parent)
    : QObject(parent),
      mAudioInstanceHandle(nullptr),
      mSource(0),
      mBuffer(0),
      mSampleOffsetInterval(floor(1000.f/60.f)),
      mStatus(AL_STOPPED)
{
    auto log = spdlog::get("PCMPlayer");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("PCMPlayer");
    }

    if (!AlInitialised)
    {
        initAL();
    }

    connect(
        &mSampleOffsetTimer,
        SIGNAL(timeout()),
        this,
        SLOT(onSampleOffsetTimer())
    );
}

void
PCMPlayer::loadAudioInstance
(AudioInstance* ai)
{
    mAudioInstanceHandle = ai;
    mPcmDataHandle = &mAudioInstanceHandle->getAudioDataBuffer()[0];
    mPcmDataSize = mAudioInstanceHandle->getAudioDataBuffer().size();
    mFrequency = mAudioInstanceHandle->getFrequency();
    mFormat = mAudioInstanceHandle->getFormat();

    ALenum error;
    do
    {
        alGenBuffers(1, &mBuffer);
        error = alGetError();
        qDebug() << "Error creating buffer";
        std::this_thread::yield();
    }
    while (error != AL_NO_ERROR);

    alGenSources(1, &mSource);
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        qDebug() << "Error creating source";
        return;
    }

    alBufferData(
        mBuffer,
        mFormat,
        mPcmDataHandle,
        mPcmDataSize,
        mFrequency
    );

    alSourcei(mSource,AL_BUFFER,static_cast<ALint>(mBuffer));
    alSource3f(mSource, AL_POSITION, 0.f,0.f,0.f);
    qDebug() << "Successfully loaded audio file";
}


void
PCMPlayer::initAL
()
{
    qDebug() << "Initialising OpenAL";
    AlDevice = alcOpenDevice(nullptr);
    AlContext  = alcCreateContext(AlDevice, nullptr);
    alcMakeContextCurrent(AlContext);
    alListener3f(AL_POSITION, 0.f,0.f,0.f);
    loadClick();
}

void
PCMPlayer::loadClick
()
{
    if (!AlClickFile.open(":audio/click.wav"))
    {
        qDebug() << "Could not open click file";
        return;
    }
    auto format = AlClickFile.getFormat();
    auto pcmData = AlClickFile.getAudioData() ;
    auto pcmDataSize = AlClickFile.getAudioDataSize();
    auto frequency = AlClickFile.getFrequency();
    ALenum error;
    do
    {
        alGenBuffers(1, &AlClickBuffer);
        error = alGetError();
        qDebug() << "Error creating buffer";
        std::this_thread::yield();
    }
    while (error != AL_NO_ERROR);

    alGenSources(1, &AlClickSource);
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        qDebug() << "Error creating source";
        return;
    }

    alBufferData(
        AlClickBuffer,
        format,
        pcmData,
        pcmDataSize,
        frequency
    );

    alSourcei(AlClickSource,AL_BUFFER,static_cast<ALint>(AlClickBuffer));
    alSource3f(AlClickSource, AL_POSITION, 0.f,0.f,0.f);
    alSourcef(AlClickSource, AL_GAIN, 1.0f);
    qDebug() << "Successfully loaded click file";
}

void PCMPlayer::click()
{
    qDebug() << "*CLICK*";
    alSourcePlay(AlClickSource);
}

PCMPlayer::~PCMPlayer
()
{
    alSourceStop(mSource);
    mSampleOffsetTimer.stop();

    alDeleteSources(1,&mSource);
    mSource = 0;

    alDeleteBuffers(1,&mBuffer);
    mBuffer = 0;
}

void
PCMPlayer::destroyAL
()
{
    alSourceStop(AlClickSource);

    alDeleteSources(1,&AlClickSource);
    AlClickSource = 0;

    alDeleteBuffers(1,&AlClickBuffer);
    AlClickBuffer = 0;

    if (AlContext != nullptr)
    {
        alcDestroyContext(AlContext);
        AlContext = nullptr;
    }

    if (AlDevice != nullptr)
    {
        alcCloseDevice(AlDevice);
        AlDevice = nullptr;
    }
    AlInitialised = false;
}

void
PCMPlayer::onSampleOffsetChanged
(int offset)
{
    setSampleOffset(offset);
}

void
PCMPlayer::onSampleOffsetTimer
()
{
    auto offset = getSampleOffset();
    emit notifySampleOffsetChanged(offset);
}

void PCMPlayer::onVolumeChanged(int vol)
{
   alSourcef(mSource,AL_GAIN,(1.0f/100.0f)*vol);
}

void
PCMPlayer::play
()
{
    getStatus();
    if (mStatus == AL_PLAYING)
    {
        pause();
    }
    else
    {
        qDebug() << "Playing source: " << mSource;
        alSourcePlay(mSource);
        mSampleOffsetTimer.start(mSampleOffsetInterval);
    }
}

void
PCMPlayer::stop
()
{
    getStatus();
    if (mStatus == AL_PLAYING)
    {
        qDebug() << "Stopping source: " << mSource;
        alSourceStop(mSource);
        mSampleOffsetTimer.stop();
    }
    else
    {
        qDebug() << "Source is not playing: " << mSource;
    }
}

void
PCMPlayer::pause
()
{

    qDebug() << "Pausing source: " << mSource;
    alSourcePause(mSource);
    mSampleOffsetTimer.stop();
}

ALint
PCMPlayer::getSampleOffset
()
const
{
    ALint sampleOffset;
    alGetSourcei(mSource, AL_SAMPLE_OFFSET, &sampleOffset);
    return sampleOffset;
}

void
PCMPlayer::setSampleOffset
(ALint offset)
{
    alSourcei(mSource, AL_SAMPLE_OFFSET, offset);
}

void
PCMPlayer::getStatus
()
{
    alGetSourcei(mSource, AL_SOURCE_STATE, &mStatus);
}

bool        PCMPlayer::AlInitialised = false;
ALCdevice*  PCMPlayer::AlDevice      = nullptr;
ALCcontext* PCMPlayer::AlContext     = nullptr;
ALuint      PCMPlayer::AlClickSource = 0;
ALuint      PCMPlayer::AlClickBuffer = 0;
WavFile     PCMPlayer::AlClickFile;


