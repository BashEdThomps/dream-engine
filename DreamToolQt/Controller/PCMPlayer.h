#pragma once

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <QObject>
#include <QTimer>
#include <QAudioOutput>
#include <DreamCore.h>
#include "WavFile.h"

using namespace Dream;

class PCMPlayer : public QObject
{
    Q_OBJECT

public:
    PCMPlayer(QObject *parent = nullptr);
    ~PCMPlayer() override;

    void play();
    void pause();
    void stop();

    void loadAudioInstance(AudioInstance* ai);

    ALint getSampleOffset() const;
    void setSampleOffset(ALint offset);

    static void initAL();
    static void destroyAL();
    static void loadClick();
    static void click();

    void getStatus();
public slots:
    void onSampleOffsetChanged(int);
    void onSampleOffsetTimer();
    void onVolumeChanged(int);

signals:
    void notifySampleOffsetChanged(int);

protected:
    AudioInstance* mAudioInstanceHandle;
    char* mPcmDataHandle;
    ALuint mSource;
    ALuint mBuffer;
    ALenum mFormat;
    ALsizei mFrequency;
    ALsizei mPcmDataSize;
    QTimer mSampleOffsetTimer;
    int mSampleOffsetInterval;
    int mStatus;

    static ALCdevice*  AlDevice;
    static ALCcontext* AlContext;
    static ALuint      AlClickSource;
    static ALuint      AlClickBuffer;
    static WavFile     AlClickFile;
    static bool        AlInitialised;
};
