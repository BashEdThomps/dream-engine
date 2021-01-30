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

#include "OpenALAudioRuntime.h"

namespace octronic::dream::open_al
{
    OpenALAudioRuntime::OpenALAudioRuntime
    (AudioLoader* loader, AudioDefinition* def, ProjectRuntime* project)
        : AudioRuntime(loader, def,project),
          mALFormat(0),
          mALSource(0),
          mALBuffer(0),
          mALDurationInSamples(0)
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
        setLooping(false);
        generateEventList();
    }

    OpenALAudioRuntime::~OpenALAudioRuntime()
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
        if (mALBuffer!=0) alDeleteBuffers(1,&mALBuffer);
        if (mALSource!=0) alDeleteSources(1,&mALSource);
    }

    bool OpenALAudioRuntime::useDefinition()
	{
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
        if (!mLoader->loadIntoBuffer(dynamic_cast<AudioDefinition*>(mDefinition), mProjectRuntime)) return false;
        if (!loadIntoAL()) return false;
        return true;
	}

    void
    OpenALAudioRuntime::setLooping
    (bool looping)
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
        mLooping = looping;
        alSourcei(mALSource, AL_LOOPING, mLooping ? 1 : 0);
    }

    ALenum
    OpenALAudioRuntime::getFormat
    ()
    const
    {
        return mALFormat;
    }

    void
    OpenALAudioRuntime::setBuffer
    (ALuint buffer)

    {
        mALBuffer = buffer;
    }

    ALuint
    OpenALAudioRuntime::getBuffer
    ()
    const
    {
        return mALBuffer;
    }

    void
    OpenALAudioRuntime::setSource
    (ALuint source)
    {
        mALSource = source;
    }

    ALuint
    OpenALAudioRuntime::getSource
    ()
    const
    {
        return mALSource;
    }

    void
    OpenALAudioRuntime::play
    ()
    {
        LOG_DEBUG(  "OpenALAudioRuntime: Playing source {}" , mALSource);
        alSourcePlay(mALSource);
    }

    void
    OpenALAudioRuntime::stop
    ()
    {
        LOG_DEBUG(  "OpenALAudioRuntime: Stopping source {}" , mALSource);
        alSourceStop(mALSource);
    }

    void
    OpenALAudioRuntime::pause
    ()
    {
        LOG_DEBUG(  "OpenALAudioRuntime: Pausing source {}" , mALSource);
        alSourcePause(mALSource);
    }

    void
    OpenALAudioRuntime::setSourcePosision
    (vec3 pos)
    {
        LOG_DEBUG(  "OpenALAudioRuntime: SetSource Position {} {},{},{}" , mALSource, pos.x,pos.y,pos.z);
        alSource3f(mALSource, AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    OpenALAudioRuntime::setVolume
    (float volume)
    {
        LOG_DEBUG(  "OpenALAudioRuntime: SetVolume src:{} vol:{},{},{}" , mALSource, volume);
        alSourcef(mALSource, AL_GAIN,volume);
    }

    AudioStatus
    OpenALAudioRuntime::getState
    ()
    {
        LOG_TRACE("OpenALAudioRuntime: {}", __FUNCTION__);
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
                LOG_ERROR("OpenALAudioRuntime: Unknown Audio State for {} " , getNameAndUuidString());
                return AUDIO_STATUS_UNKNOWN;
        }
    }

    int
    OpenALAudioRuntime::getDurationInSamples
    ()
    {
        LOG_TRACE("OpenALAudioRuntime: {}", __FUNCTION__);
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
    OpenALAudioRuntime::getSampleOffset
    ()
    const
    {
        LOG_TRACE("OpenALAudioRuntime: {}", __FUNCTION__);
        ALint sampleOffset;
        alGetSourcei(mALSource, AL_SAMPLE_OFFSET, &sampleOffset);
        return sampleOffset;
    }

    void
    OpenALAudioRuntime::setSampleOffset
    (unsigned int offset)
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
        alSourcei(mALSource,AL_SAMPLE_OFFSET, (ALint)offset);
    }

    ALuint
    OpenALAudioRuntime::generateSource
    ()
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
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
    OpenALAudioRuntime::generateBuffer
    ()
    {
        LOG_DEBUG("OpenALAudioRuntime: {}", __FUNCTION__);
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
    OpenALAudioRuntime::loadIntoAL
    ()
    {
        LOG_DEBUG("OpenALAudioRuntime: Loading Into AL {}",getNameAndUuidString());

        if (mALSource == 0 && mALBuffer == 0)
        {
            generateBuffer();
            generateSource();

            if (mLoader->getAudioBuffer() == nullptr || mLoader->getAudioBufferSize() == 0)
            {
                LOG_ERROR("OpenALAudioRuntime: Unable to load audio data: Empty Buffer");
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
            alSourcei(mALSource, AL_LOOPING, mLooping ? 1 : 0 );
            delete mLoader;
            mLoader = nullptr;
        }
        else
        {
            LOG_ERROR("OpenALAudioRuntime: Unable to load audio, source or buffer is empty");
            return false;
        }

        LOG_DEBUG("OpenALAudioRuntime: Pushed audio asset to play queue");
        return true;
    }
}


