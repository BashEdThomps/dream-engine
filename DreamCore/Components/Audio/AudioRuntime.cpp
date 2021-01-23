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

#include "AudioRuntime.h"

#include "Common/Logger.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"

namespace octronic::dream
{
    AudioRuntime::AudioRuntime
    (AudioDefinition* def, ProjectRuntime* project)
        : SharedAssetRuntime(def,project),
          mLooping(false),
          mFormat(0),
          mFrequency(0),
          mSource(0),
          mBuffer(0),
          mStartTime(-1),
          mLastSampleOffset(0),
          mChannels(-1),
          mDurationInSamples(-1),
          mMarkersUpdateTask(this)
    {
        setLooping(false);
        setBuffer(0);
        setSource(0);
        generateEventList();
    }

    AudioRuntime::~AudioRuntime()
    {
          alDeleteBuffers(1,&mBuffer);
          alDeleteSources(1,&mSource);
    }

    void
    AudioRuntime::setLooping
    (bool looping)
    {
        mLooping = looping;
        alSourcei(mSource, AL_LOOPING, mLooping ? 1 : 0);
    }

    bool
    AudioRuntime::isLooping
    ()
    const
    {
        return mLooping;
    }

    std::vector<ALchar>
    AudioRuntime::getAudioDataBuffer
    ()
    const
    {
        return mAudioDataBuffer;
    }

    ALsizei
    AudioRuntime::getFrequency
    ()
    const
    {
        return mFrequency;
    }

    ALenum
    AudioRuntime::getFormat
    ()
    const
    {
        return mFormat;
    }

    void
    AudioRuntime::setBuffer
    (ALuint buffer)
    {
        mBuffer = buffer;
    }

    ALuint
    AudioRuntime::getBuffer
    ()
    const
    {
        return mBuffer;
    }

    void
    AudioRuntime::setSource
    (ALuint source)
    {
        mSource = source;
    }

    ALuint
    AudioRuntime::getSource
    ()
    const
    {
        return mSource;
    }

    void
    AudioRuntime::play
    ()
    {
        LOG_DEBUG(  "AudioRuntime: Playing source {}" , mSource);
        alSourcePlay(mSource);
    }

    void
    AudioRuntime::stop
    ()
    {
        LOG_DEBUG(  "AudioRuntime: Stopping source {}" , mSource);
        alSourceStop(mSource);
    }

    void
    AudioRuntime::pause
    ()
    {
        LOG_DEBUG(  "AudioRuntime: Pausing source {}" , mSource);
        alSourcePause(mSource);
    }

    void
    AudioRuntime::setSourcePosision
    (glm::vec3 pos)
    {
        alSource3f(mSource, AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioRuntime::setVolume
    (float volume)
    {
        alSourcef(mSource, AL_GAIN,volume);
    }

    void
    AudioRuntime::generateEventList
    ()
    {
        // TODO = Rethink, SharedAssetRuntime has no EntityRuntime to send
        // Events to :thinking:

        /*
        auto ad = static_cast<AudioDefinition*>(mDefinition);
        auto nMarkers = ad->countMarkers();

        if (nMarkers == 0)
        {
           return;
        }

        mMarkerEvents.clear();

        for (int markerIndex = 0; markerIndex< nMarkers; markerIndex++)
        {
            auto log = getLog();
            LOG_trace("AudioRuntime: Generating events for marker {}", markerIndex);
            auto markerStart = ad->getMarkerSampleIndex(markerIndex);
            auto count = ad->getMarkerRepeat(markerIndex);
            auto step = ad->getMarkerRepeatPeriod(markerIndex);
            auto markerName = ad->getMarkerName(markerIndex);

            auto next = markerStart;
            LOG_trace("AudioRuntime: Marker {}'s is : ", markerIndex, next);
            Event e(mEntityRuntime,"audio");
            e.setString("name",markerName);
            e.setNumber("time",markerStart);
            mMarkerEvents.push_back(e);

            for (int i=0; i<count; i++)
            {
                auto repeatIndex = i+1;
                auto next = markerStart + (repeatIndex*step);
                LOG_trace("AudioRuntime: Marker {}'s {}th step is : {}", markerIndex, repeatIndex, next);
                Event e(mEntityRuntime,"audio");
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
        */
    }

    void
    AudioRuntime::updateMarkers
    ()
    {
        /*
        auto currentSample = mAudioComponent->getSampleOffset(mSource);
        // has just looped, restore cached events
        if (mLooping && mLastSampleOffset > currentSample)
        {
            LOG_DEBUG("AudioRuntime: Just Looped");
           mMarkerEvents = mMarkerEventsCache;
        }

        for (auto it = mMarkerEvents.begin(); it != mMarkerEvents.end();)
        {
            auto time = (*it).getNumber("time");
            if (currentSample > time)
            {
                mEntityRuntime->addEvent((*it));
                it++;
                mMarkerEvents.pop_front();
            }
            else
            {
                break;
            }
        }
        mLastSampleOffset = currentSample;
        */
    }

    long
    long AudioRuntime::getStartTime
    ()
    const
    {
        return mStartTime;
    }

    void
    AudioRuntime::setStartTime
    (long long startTime)
    {
        mStartTime = startTime;
    }

    int
    AudioRuntime::getChannels
    ()
    const
    {
        return mChannels;
    }

    AudioStatus
    AudioRuntime::getState
    ()
    {
        int state;
        alGetSourcei(mSource, AL_SOURCE_STATE, &state);
        switch (state)
        {
            case AL_STOPPED:
                return AUDIO_STATUS_STOPPED;
            case AL_PLAYING:
                return AUDIO_STATUS_PLAYING;
            case AL_PAUSED:
                return AUDIO_STATUS_PAUSED;
            default:
                LOG_ERROR("AudioRuntime: Unknown Audio State for {} " , getNameAndUuidString());
                return AUDIO_STATUS_UNKNOWN;
        }
    }

    vector<char>
    AudioRuntime::getAudioBuffer
    (size_t offset, size_t length)
    const
    {
        vector<char> retval = vector<char>(length);
        vector<char> audioData = getAudioDataBuffer();
        char* dataBegin = &audioData[offset];
        retval.insert(retval.begin(), dataBegin, dataBegin+length);
        return retval;
    }

    int
    AudioRuntime::getDurationInSamples
    ()
    {
        if (mDurationInSamples < 0 && mBuffer != 0)
        {
            ALint sizeInBytes;
            ALint channels;
            ALint bits;

            alGetBufferi(mBuffer, AL_SIZE, &sizeInBytes);
            alGetBufferi(mBuffer, AL_CHANNELS, &channels);
            alGetBufferi(mBuffer, AL_BITS, &bits);

            mDurationInSamples = sizeInBytes * 8 / (channels * bits);
        }
        return mDurationInSamples;
    }

    AudioMarkersUpdateTask *AudioRuntime::getMarkersUpdateTask()
    {
       return &mMarkersUpdateTask;
    }

    ALint
    AudioRuntime::getSampleOffset
    ()
    const
    {
        ALint sampleOffset;
        alGetSourcei(mSource, AL_SAMPLE_OFFSET, &sampleOffset);
        return sampleOffset;
    }

    void
    AudioRuntime::setSampleOffset
    (ALint offset)
    {
        alSourcei(mSource,AL_SAMPLE_OFFSET, offset);
    }

    ALuint
    AudioRuntime::generateSource
    ()
    {
        alGetError();
        alGenSources(1, &mSource);
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            mSource = static_cast<ALuint>(-1);
        }
        return mSource;
    }

    ALuint
    AudioRuntime::generateBuffer
    ()
    {
        alGetError();
        alGenBuffers(1, &mBuffer);
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            mBuffer = static_cast<ALuint>(-1);
        }
        return mBuffer;
    }

    bool
    AudioRuntime::loadIntoAL
    ()
    {
        LOG_ERROR("AudioRuntime: Loading Into AL {}",getNameAndUuidString());

        if (mSource == 0 && mBuffer == 0)
        {
            generateBuffer();
            generateSource();

            if (mAudioDataBuffer.empty())
            {
                LOG_ERROR("AudioRuntime: Unable to load audio data: Empty Buffer");
                return false;
            }

            alBufferData(mBuffer, mFormat, &mAudioDataBuffer[0], static_cast<ALsizei>(mAudioDataBuffer.size()), mFrequency);
            alSourcei(mSource, AL_BUFFER, static_cast<ALint>(mBuffer));

            alSourcei(mSource, AL_LOOPING, mLooping ? 1 : 0 );
            mAudioDataBuffer.clear();
        }
        else
        {
            LOG_ERROR("AudioRuntime: Unable to load audio, source or buffer is empty");
            return false;
        }

        LOG_DEBUG("AudioRuntime: Pushed audio asset to play queue");
        return true;
    }
}
