#include "AudioInstance.h"

#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"
#include "SpectrumAnalyser.h"

namespace Dream
{


    AudioInstance::AudioInstance
    (AudioComponent* component,
     AudioDefinition* definition,
     SceneObjectRuntime* transform)
        : IAssetInstance(definition, transform),
          mAudioComponent(component),
          mStartTime(-1),
          mSpectrumAnalyser(nullptr),
          mLastSampleOffset(0),
          mChannels(-1)
    {
        setStatus(UNKNOWN);
        setLooping(false);
        setBuffer(0);
        setSource(0);
        loadExtraAttributes(json::object());
        generateEventList();
    }

    AudioInstance::~AudioInstance()
    {
       if (mAudioComponent != nullptr)
       {
          mAudioComponent->deleteBuffers(1,mBuffer);
          mAudioComponent->deleteSources(1,mSource);
       }
    }

    void
    AudioInstance::setLooping
    (bool looping)
    {
        mLooping = looping;
        alSourcei(mSource, AL_LOOPING, mLooping ? 1 : 0);
    }

    bool
    AudioInstance::isLooping
    ()
    const
    {
        return mLooping;
    }

    std::vector<ALchar>
    AudioInstance::getAudioDataBuffer
    ()
    const
    {
        return mAudioDataBuffer;
    }

    ALsizei
    AudioInstance::getFrequency
    ()
    const
    {
        return mFrequency;
    }

    ALenum
    AudioInstance::getFormat
    ()
    const
    {
        return mFormat;
    }

    void
    AudioInstance::setBuffer
    (ALuint buffer)
    {
        mBuffer = buffer;
    }

    ALuint
    AudioInstance::getBuffer
    ()
    const
    {
        return mBuffer;
    }

    void
    AudioInstance::setSource
    (ALuint source)
    {
        mSource = source;
    }

    ALuint
    AudioInstance::getSource
    ()
    const
    {
        return mSource;
    }

    void AudioInstance::play()
    {
        if (mAudioComponent != nullptr)
            mAudioComponent->pushToPlayQueue(this);
    }

    void AudioInstance::pause()
    {
        if (mAudioComponent != nullptr)
            mAudioComponent->pushToPauseQueue(this);
    }

    void AudioInstance::stop()
    {
        if (mAudioComponent != nullptr)
            mAudioComponent->pushToStopQueue(this);
    }

    void
    AudioInstance::setStatus
    (AudioStatus status)
    {
        mStatus = status;
    }

    AudioStatus
    AudioInstance::getStatus
    ()
    {
        return mStatus;
    }

    void
    AudioInstance::loadExtraAttributes
    (nlohmann::json)
    {
    }

    void AudioInstance::loadSpectrumAnalyser()
    {
        auto log = getLog();
        auto def = dynamic_cast<AudioDefinition*>(mDefinition);
        if (def->getSpectrumAnalyser())
        {
            mSpectrumAnalyser.reset(new SpectrumAnalyser(this));
            mSpectrumAnalyser->setSampleRate(mFrequency);
            mSpectrumAnalyser->setSourceStereo(mFormat == AL_FORMAT_STEREO16);
            // Currently Arbitrary, from example
            mSpectrumAnalyser->setParameters(mFrequency/9,1024);
            log->critical("Creating Spectrum Analyser for {} with sample rate {}",getNameAndUuidString(),mFrequency);
            if (mAudioComponent != nullptr)
            {
                mAudioComponent->pushToUpdateQueue(this);
            }
        }
    }

    void
    AudioInstance::setSourcePosision
    (glm::vec3 pos)
    {
        alSource3f(mSource, AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioInstance::setVolume
    (float volume)
    {
        alSourcef(mSource, AL_GAIN,volume);
    }

    void
    AudioInstance::updateFFT
    ()
    {
        if (mSpectrumAnalyser != nullptr)
        {
            auto currentSample = mAudioComponent->getSampleOffset(mSource);
            // TODO - be less of a clown about this
            mSpectrumAnalyser->onData(&mAudioDataBuffer[mLastSampleOffset],currentSample-mLastSampleOffset);
            mLastSampleOffset = currentSample;
        }
    }

    void
    AudioInstance::generateEventList
    ()
    {
        auto ad = dynamic_cast<AudioDefinition*>(mDefinition);
        auto nMarkers = ad->countMarkers();

        if (nMarkers == 0)
        {
           return;
        }

        mMarkerEvents.clear();

        for (int markerIndex = 0; markerIndex< nMarkers; markerIndex++)
        {
            auto log = getLog();
            log->debug("Generating events for marker {}", markerIndex);
            auto markerStart = ad->getMarkerSampleIndex(markerIndex);
            auto count = ad->getMarkerRepeat(markerIndex);
            auto step = ad->getMarkerRepeatPeriod(markerIndex);
            auto markerName = ad->getMarkerName(markerIndex);

            auto next = markerStart;
            log->debug("Marker {}'s is : ", markerIndex, next);
            Event e(mSceneObjectRuntime,"audio");
            e.setString("name",markerName);
            e.setNumber("time",markerStart);
            mMarkerEvents.push_back(e);

            for (int i=0; i<count; i++)
            {
                auto repeatIndex = i+1;
                auto next = markerStart + (repeatIndex*step);
                log->debug("Marker {}'s {}th step is : {}", markerIndex, repeatIndex, next);
                Event e(mSceneObjectRuntime,"audio");
                e.setString("name",markerName);
                e.setNumber("time",next);
                mMarkerEvents.push_back(e);
            }
        }

        std::sort(
            mMarkerEvents.begin(),
            mMarkerEvents.end(),
            [](Event& first, Event& second)
            {
                return first.getNumber("time") < second.getNumber("time");
            }
        );

        mMarkerEventsCache = mMarkerEvents;

        if (mAudioComponent != nullptr)
        {
            mAudioComponent->pushToUpdateQueue(this);
        }
    }

    void
    AudioInstance::updateMarkers
    ()
    {
        auto log = getLog();
        auto currentSample = mAudioComponent->getSampleOffset(mSource);
        // has just looped, restore cached events
        if (mLooping && mLastSampleOffset > currentSample)
        {
            log->critical("Just Looped");
           mMarkerEvents = mMarkerEventsCache;
        }

        for (auto it = mMarkerEvents.begin(); it != mMarkerEvents.end();)
        {
            auto time = (*it).getNumber("time");
            if (currentSample > time)
            {
                mSceneObjectRuntime->addEvent((*it));
                it++;
                mMarkerEvents.pop_front();
            }
            else
            {
                break;
            }
        }
        mLastSampleOffset = currentSample;
    }

    long long AudioInstance::getStartTime() const
    {
        return mStartTime;
    }

    void AudioInstance::setStartTime(long long startTime)
    {
        mStartTime = startTime;
    }

    int AudioInstance::getChannels() const
    {
        return mChannels;
    }
} // End of Dream
