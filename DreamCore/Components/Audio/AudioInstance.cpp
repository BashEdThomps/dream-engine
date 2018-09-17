#include "AudioInstance.h"

#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"

namespace Dream
{
    AudioInstance::AudioInstance
    (AudioComponent* component,
     AudioDefinition* definition,
     SceneObjectRuntime* transform)
        : IAssetInstance(definition, transform),
          mAudioComponent(component)
    {
        setStatus(UNKNOWN);
        setLooping(false);
        setBuffer(0);
        setSource(0);
    }

    void
    AudioInstance::setLooping
    (bool looping)
    {
        mLooping = looping;
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
} // End of Dream
