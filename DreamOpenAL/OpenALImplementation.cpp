/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenALImplementation.h"

namespace octronic::dream::open_al
{
    OpenALImplementation::OpenALImplementation
    (const shared_ptr<AudioLoader>& loader)
        : AudioRuntimeImplementation(loader),
          mALFormat(0),
          mALSource(0),
          mALBuffer(0),
          mALDurationInSamples(0)
    {
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
    }

    OpenALImplementation::~OpenALImplementation()
    {
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
        if (mALBuffer!=0) alDeleteBuffers(1,&mALBuffer);
        if (mALSource!=0) alDeleteSources(1,&mALSource);
    }

    bool OpenALImplementation::loadFromDefinition(ProjectRuntime* pr, AudioDefinition* ad)
	{
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
        if (!mLoader->loadIntoBuffer(ad, pr)) return false;
        if (!loadIntoAL()) return false;
        return true;
	}

    ALenum
    OpenALImplementation::getFormat
    ()
    const
    {
        return mALFormat;
    }

    void
    OpenALImplementation::setBuffer
    (ALuint buffer)

    {
        mALBuffer = buffer;
    }

    ALuint
    OpenALImplementation::getBuffer
    ()
    const
    {
        return mALBuffer;
    }

    void
    OpenALImplementation::setSource
    (ALuint source)
    {
        mALSource = source;
    }

    ALuint
    OpenALImplementation::getSource
    ()
    const
    {
        return mALSource;
    }

    void
    OpenALImplementation::play
    ()
    {
        LOG_DEBUG(  "OpenALImplementation: Playing source {}" , mALSource);
        alSourcePlay(mALSource);
    }

    void
    OpenALImplementation::stop
    ()
    {
        LOG_DEBUG(  "OpenALImplementation: Stopping source {}" , mALSource);
        alSourceStop(mALSource);
    }

    void
    OpenALImplementation::pause
    ()
    {
        LOG_DEBUG(  "OpenALImplementation: Pausing source {}" , mALSource);
        alSourcePause(mALSource);
    }

    void
    OpenALImplementation::setSourcePosision
    (const vec3& pos)
    {
        LOG_DEBUG(  "OpenALImplementation: SetSource Position {} {},{},{}" ,
                    mALSource, pos.x,pos.y,pos.z);
        alSource3f(mALSource, AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    OpenALImplementation::setVolume
    (float volume)
    {
        LOG_DEBUG(  "OpenALImplementation: SetVolume src:{} vol:{},{},{}" , mALSource, volume);
        alSourcef(mALSource, AL_GAIN,volume);
    }

    AudioStatus
    OpenALImplementation::getState
    ()
    {
        LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
        int state = 0;
        alGetSourcei(mALSource, AL_SOURCE_STATE, &state);
        switch (state)
        {
            case AL_STOPPED:
                return AUDIO_STATUS_STOPPED;
            case AL_PLAYING:
                return AUDIO_STATUS_PLAYING;
            case AL_PAUSED:
                return AUDIO_STATUS_PAUSED;
            default:
                LOG_ERROR("OpenALImplementation: Unknown Audio State");
                return AUDIO_STATUS_UNKNOWN;
        }
    }

    int
    OpenALImplementation::getDurationInSamples
    ()
    {
        LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
        if (mALDurationInSamples == 0 && mALBuffer != 0)
        {
            ALint sizeInBytes;
            ALint channels;
            ALint bits;

            alGetBufferi(mALBuffer, AL_SIZE, &sizeInBytes);
            alGetBufferi(mALBuffer, AL_CHANNELS, &channels);
            alGetBufferi(mALBuffer, AL_BITS, &bits);

            mALDurationInSamples = sizeInBytes * 8 / (channels * bits);
        }
        return mALDurationInSamples;
    }

    unsigned int
    OpenALImplementation::getSampleOffset
    ()
    const
    {
        LOG_TRACE("OpenALImplementation: {}", __FUNCTION__);
        ALint sampleOffset;
        alGetSourcei(mALSource, AL_SAMPLE_OFFSET, &sampleOffset);
        return sampleOffset;
    }

    void
    OpenALImplementation::setSampleOffset
    (unsigned int offset)
    {
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
        alSourcei(mALSource,AL_SAMPLE_OFFSET, (ALint)offset);
    }

    ALuint
    OpenALImplementation::generateSource
    ()
    {
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
        alGetError();
        alGenSources(1, &mALSource);
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            mALSource = static_cast<ALuint>(-1);
        }
        return mALSource;
    }

    ALuint
    OpenALImplementation::generateBuffer
    ()
    {
        LOG_DEBUG("OpenALImplementation: {}", __FUNCTION__);
        alGetError();
        alGenBuffers(1, &mALBuffer);
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            mALBuffer = static_cast<ALuint>(-1);
        }
        return mALBuffer;
    }

    bool
    OpenALImplementation::loadIntoAL
    ()
    {
        LOG_DEBUG("OpenALImplementation: Loading Into AL {}",mParent->getNameAndUuidString());

        if (mALSource == 0 && mALBuffer == 0)
        {
            generateBuffer();
            generateSource();

            if (mLoader->getAudioBuffer() == nullptr || mLoader->getAudioBufferSize() == 0)
            {
                LOG_ERROR("OpenALImplementation: Unable to load audio data: Empty Buffer");
            	mParent->setLoadError(true);
                return false;
            }

            switch(mLoader->getChannels())
			{
                case 1:
					mALFormat = AL_FORMAT_MONO16;
                    break;
                case 2:
                default:
					mALFormat = AL_FORMAT_STEREO16;
                    break;
			}

            alBufferData(mALBuffer, mALFormat, &mLoader->getAudioBuffer()[0],
                    static_cast<ALsizei>(mLoader->getAudioBufferSize()),
                    mLoader->getSampleRate());

            alSourcei(mALSource, AL_BUFFER, static_cast<ALint>(mALBuffer));
            alSourcei(mALSource, AL_LOOPING, mParent->isLooping() ? 1 : 0 );
        }
        else
        {
            LOG_ERROR("OpenALImplementation: Unable to load audio, source or buffer is empty");
            mParent->setLoadError(true);
            return false;
        }

        LOG_DEBUG("OpenALImplementation: Pushed audio asset to play queue");
        mParent->setLoaded(true);
        return true;
    }
}


