#pragma once

#include <vector>
#include <deque>
#include "AudioStatus.h"
#include "../IAssetInstance.h"
#include "../Event.h"

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <glm/glm.hpp>

using std::vector;
using std::deque;
#include "SpectrumAnalyser.h"

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
        long long mStartTime;
        unique_ptr<SpectrumAnalyser> mSpectrumAnalyser;
        int mLastSampleOffset;
        int mChannels;
        deque<Event> mMarkerEvents;

        void generateEventList();
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

        void loadExtraAttributes(nlohmann::json) override;
        void loadSpectrumAnalyser();
        void setSourcePosision(glm::vec3 pos);
        long long getStartTime() const;
        void setStartTime(long long startTime);
        void setVolume(float volume);
        void updateFFT();
        int getChannels() const;
        void updateMarkers();
    };

} // End of Dream
