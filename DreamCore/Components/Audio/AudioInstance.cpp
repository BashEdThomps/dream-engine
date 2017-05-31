
#include "AudioInstance.h"

namespace Dream
{
    AudioInstance::AudioInstance
    (AssetDefinition* definition, Transform3D* transform)
        : IAssetInstance(definition, transform)
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
    {
        return mLooping;
    }

    std::vector<ALchar>
    AudioInstance::getAudioDataBuffer
    ()
    {
        return mAudioDataBuffer;
    }

    ALsizei
    AudioInstance::getFrequency
    ()
    {
        return mFrequency;
    }

    ALenum
    AudioInstance::getFormat
    ()
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
    {
        return mSource;
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
