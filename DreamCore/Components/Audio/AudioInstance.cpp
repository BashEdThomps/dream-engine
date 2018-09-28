#include "AudioInstance.h"

#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"
#include "SpectrumAnalyser.h"

namespace Dream
{
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
       mAudioComponent->pushToPlayQueue(this);
    }

    void AudioInstance::pause()
    {
       mAudioComponent->pushToPauseQueue(this);
    }

    void AudioInstance::stop()
    {
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
        mLooping = def->getLoop();
        if (def->getSpectrumAnalyser())
        {
            mSpectrumAnalyser.reset(new SpectrumAnalyser(this));
            mSpectrumAnalyser->setSampleRate(mFrequency);
            mSpectrumAnalyser->setSourceStereo(mFormat == AL_FORMAT_STEREO16);
            // Currently Arbitrary, from example
            mSpectrumAnalyser->setParameters(mFrequency/9,1024);
            log->critical("Creating Spectrum Analyser for {} with sample rate {}",getNameAndUuidString(),mFrequency);
        }
        mAudioComponent->pushToFFTQueue(this);
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
} // End of Dream
