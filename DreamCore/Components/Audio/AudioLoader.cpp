#include "AudioLoader.h"

#include "Common/Logger.h"



namespace octronic::dream
{

    AudioLoader::AudioLoader
    ()
        : mChannels(0),
          mSampleRate(0)
    {

    }

    AudioLoader::~AudioLoader
    ()
    {
    }

    bool
    AudioLoader::checkLoaded
    ()
    const
    {
        LOG_INFO("AudioLoader: Checking\n"
				 "\tChannels:{}\n"
                 "\tSampleRate:{}\n"
                 "\tBufferSz:{}",
                 mChannels, mSampleRate, mAudioBuffer.size());

        return mChannels        != 0 &&
               mSampleRate      != 0 &&
               !mAudioBuffer.empty();

    }

    vector<uint8_t>
    AudioLoader::getAudioBuffer
    ()
    const
    {
        return mAudioBuffer;
    }

    uint8_t
    AudioLoader::getChannels
    ()
    const
    {
        return mChannels;
    }

    long
    AudioLoader::getSampleRate
    ()
    const
    {
        return mSampleRate;
    }
}
