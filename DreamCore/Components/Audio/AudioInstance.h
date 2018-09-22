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
        AudioComponent* mAudioComponent;

    public:
        AudioInstance(AudioComponent* comp, AudioDefinition*, SceneObjectRuntime*);

        void setLooping(bool);
        bool isLooping() const ;

        vector<ALchar> getAudioDataBuffer()const ;
        ALsizei getFrequency()const ;
        ALenum  getFormat()const ;
        AudioStatus getStatus() ;
        void setStatus(AudioStatus);
        void   setBuffer(ALuint buffer);
        ALuint getBuffer()const ;

        void setSource(ALuint source);
        ALuint getSource() const;

        void play();
        void pause();
        void stop();

        // IAssetInstance interface
    public:
        void loadExtraAttributes(nlohmann::json) override;
    };

} // End of Dream
