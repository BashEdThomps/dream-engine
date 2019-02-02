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


#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <al.h>
    #include <alc.h>
#endif

#include "AudioRuntime.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"
#include "AudioTasks.h"

namespace Dream
{
    AudioRuntime::AudioRuntime
    (AudioDefinition* def, ProjectRuntime* project)
        : SharedAssetRuntime(def,project),
          mStartTime(-1),
          mLastSampleOffset(0),
          mChannels(-1),
          mDurationInSamples(-1),
          mMarkersUpdateTask(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("AudioRuntime");
        #endif
        setLooping(false);
        setBuffer(0);
        setSource(0);
        generateEventList();
    }

    AudioRuntime::~AudioRuntime()
    {
          alDeleteBuffers(1,&mBuffer);
          alDeleteSources(1,&mSource);
          if (mMarkersUpdateTask)
          {
              mMarkersUpdateTask->setExpired(true);
          }
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
        #ifdef DREAM_LOG
        getLog()->debug(  "Playing source {}" , mSource);
        #endif
        alSourcePlay(mSource);
    }

    void
    AudioRuntime::stop
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug(  "Stopping source {}" , mSource);
        #endif
        alSourceStop(mSource);
    }

    void
    AudioRuntime::pause
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug(  "Pausing source {}" , mSource);
        #endif
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
        // TODO = Rethink, SharedAssetRuntime has no SceneObjectRuntime to send
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
            getLog()->trace("Generating events for marker {}", markerIndex);
            auto markerStart = ad->getMarkerSampleIndex(markerIndex);
            auto count = ad->getMarkerRepeat(markerIndex);
            auto step = ad->getMarkerRepeatPeriod(markerIndex);
            auto markerName = ad->getMarkerName(markerIndex);

            auto next = markerStart;
            getLog()->trace("Marker {}'s is : ", markerIndex, next);
            Event e(mSceneObjectRuntime,"audio");
            e.setString("name",markerName);
            e.setNumber("time",markerStart);
            mMarkerEvents.push_back(e);

            for (int i=0; i<count; i++)
            {
                auto repeatIndex = i+1;
                auto next = markerStart + (repeatIndex*step);
                getLog()->trace("Marker {}'s {}th step is : {}", markerIndex, repeatIndex, next);
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
        */
    }

    void
    AudioRuntime::updateMarkers
    ()
    {
        /*
        auto log = getLog();
        auto currentSample = mAudioComponent->getSampleOffset(mSource);
        // has just looped, restore cached events
        if (mLooping && mLastSampleOffset > currentSample)
        {
            getLog()->debug("Just Looped");
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
                return STOPPED;
            case AL_PLAYING:
                return PLAYING;
            case AL_PAUSED:
                return PAUSED;
            default:
                #if DREAM_LOG
                getLog()->error("Unknown Audio State for {} " , getNameAndUuidString());
                #endif
                return UNKNOWN;
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

    void AudioRuntime::clearMarkersUpdateTask()
    {
       mMarkersUpdateTask = nullptr;
    }

    void AudioRuntime::setMarkersUpdateTask(AudioMarkersUpdateTask* t)
    {
       mMarkersUpdateTask = t;
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
        #ifdef DREAM_LOG
        getLog()->error("Loading Into AL {}",getNameAndUuidString());
        #endif

        if (mSource == 0 && mBuffer == 0)
        {
            generateBuffer();
            generateSource();

            if (mAudioDataBuffer.empty())
            {
                #ifdef DREAM_LOG
                getLog()->error("Unable to load audio data: Empty Buffer");
                #endif
                return false;
            }

            alBufferData(mBuffer, mFormat, &mAudioDataBuffer[0], static_cast<ALsizei>(mAudioDataBuffer.size()), mFrequency);
            alSourcei(mSource, AL_BUFFER, static_cast<ALint>(mBuffer));

            alSourcei(mSource, AL_LOOPING, mLooping ? 1 : 0 );
        }
        else
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to load audio, source or buffer is empty");
            #endif
            return false;
        }

        #ifdef DREAM_LOG
        getLog()->debug("Pushed audio asset to play queue");
        #endif
        return true;
    }
} // End of Dream
