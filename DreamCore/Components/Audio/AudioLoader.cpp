#include "AudioLoader.h"

#include "Common/Logger.h"



namespace octronic::dream
{

    AudioLoader::AudioLoader()
        : LockableObject("AudioLoader"),
          mAudioBuffer(nullptr),
          mAudioBufferSize(0),
          mChannels(0),
          mSampleRate(0)
    {

    }

    AudioLoader::~AudioLoader()
    {
        if (mAudioBuffer != nullptr) delete mAudioBuffer;
    }

    bool AudioLoader::checkLoaded() const
    {
        LOG_INFO("AudioLoader: Checking\n"
				 "\tChannels:{}\n"
                 "\tSampleRate:{}\n"
                 "\tBufferSz:{}",
                 mChannels, mSampleRate, mAudioBufferSize);

        return mChannels        != 0 &&
                mSampleRate      != 0 &&
                mAudioBufferSize != 0 &&
                mAudioBuffer     != nullptr;

    }

    uint8_t* AudioLoader::getAudioBuffer() const
    {
        return mAudioBuffer;
    }

    size_t AudioLoader::getAudioBufferSize() const
    {
        return mAudioBufferSize;
    }

    uint8_t AudioLoader::getChannels() const
    {
        return mChannels;
    }

    long AudioLoader::getSampleRate() const
    {
        return mSampleRate;
    }
}
