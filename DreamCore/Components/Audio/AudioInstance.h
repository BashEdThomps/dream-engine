#pragma once

#include <vector>
#include "AudioStatus.h"
#include "../IAssetInstance.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

using std::vector;

namespace Dream
{
    class AudioDefinition;
    class AudioComponent;

    class AudioInstance : public IAssetInstance
    {
    protected:
        bool mLooping;
        ALenum mFormat;
        ALsizei mFrequency;
        vector<char> mAudioDataBuffer;
        ALuint mSource;
        ALuint mBuffer;
        AudioStatus mStatus;
        shared_ptr<AudioComponent> mAudioComponent;

    public:
        AudioInstance(shared_ptr<AudioComponent> comp, shared_ptr<AudioDefinition>, shared_ptr<SceneObjectRuntime>);

        void setLooping(bool);
        bool isLooping();

        vector<ALchar> getAudioDataBuffer();
        ALsizei getFrequency();
        ALenum  getFormat();
        AudioStatus getStatus();
        void setStatus(AudioStatus);
        void   setBuffer(ALuint buffer);
        ALuint getBuffer();

        void setSource(ALuint source);
        ALuint getSource();

        void play();
        void pause();
        void stop();
    };

} // End of Dream
