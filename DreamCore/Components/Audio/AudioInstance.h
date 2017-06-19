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

    public:
        AudioInstance(AssetDefinition*,SceneObjectRuntime*);

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
    };

} // End of Dream
