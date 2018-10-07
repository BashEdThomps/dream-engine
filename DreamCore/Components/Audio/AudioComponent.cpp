/*
* AudioComponent
*
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
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <iostream>

#include "AudioComponent.h"
#include "Wav/WavAudioInstance.h"
#include "Ogg/OggAudioInstance.h"

#include "AudioDefinition.h"
#include "../Time.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    AudioComponent::AudioComponent
    ()
        : IComponent()
    {
        setLogClassName("AudioComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    AudioComponent::~AudioComponent
    ()
    {
        auto log = getLog();
        log->trace("Destructing");
        for (ALuint source : mSources)
        {
            log->debug("Stopping source {}" , source);
            alSourceStop(source);
        }

        deleteAllSources();
        deleteAllBuffers();

        alcMakeContextCurrent(nullptr);

        if (mContext != nullptr)
        {
            alcDestroyContext(mContext);
            mContext = nullptr;
        }

        if (mDevice != nullptr)
        {
            alcCloseDevice(mDevice);
            mDevice = nullptr;
        }
    }

    bool
    AudioComponent::init
    ()
    {
        auto log = getLog();
        log->debug("Initialising...");
        mDevice = alcOpenDevice(nullptr);
        mContext  = alcCreateContext(mDevice, nullptr);
        alcMakeContextCurrent(mContext);
        vec3 position = {0.0f,0.0f,0.0f};
        setListenerPosition(position);
        return true;
    }

    ALuint
    AudioComponent::generateBuffers
    (size_t size)
    {
        alGetError();
        ALuint buffer;
        alGenBuffers( static_cast<int>(size), &buffer );
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            return static_cast<ALuint>(-1);
        }
        mBuffers.push_back(buffer);
        return buffer;
    }

    void
    AudioComponent::deleteAllBuffers
    ()
    {
        auto log = getLog();
        for (ALuint buffer : mBuffers)
        {
            log->debug("Deleting buffer {}" , buffer);
            deleteBuffers(1, buffer);
        }
    }

    void
    AudioComponent::removeBufferFromComponent
    (ALuint buffer)
    {
        for (auto it = mBuffers.begin(); it != mBuffers.end(); it++)
        {
            if ((*it) == buffer)
            {
                mBuffers.erase(it);
                break;
            }
        }
    }

    void
    AudioComponent::removeSourceFromComponent
    (ALuint source)
    {
       for (auto it = mSources.begin(); it != mSources.end(); it++)
       {
           if ((*it) == source)
           {
               mSources.erase(it);
               break;
           }
       }

       for (auto it = mPlayQueue.begin(); it != mPlayQueue.end(); it++)
       {
           if ((*it)->getSource() == source)
           {
               mPlayQueue.erase(it);
               break;
           }
       }

       for (auto it = mPauseQueue.begin(); it != mPauseQueue.end(); it++)
       {
           if ((*it)->getSource() == source)
           {
               mPauseQueue.erase(it);
               break;
           }
       }

       for (auto it = mStopQueue.begin(); it != mStopQueue.end(); it++)
       {
           if ((*it)->getSource() == source)
           {
               mStopQueue.erase(it);
               break;
           }
       }
    }

    ALuint
    AudioComponent::generateSources
    (size_t size)
    {
        alGetError();
        ALuint buffer;
        alGenSources( static_cast<int>(size), &buffer );
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR )
        {
            return static_cast<ALuint>(-1);
        }
        mSources.push_back(buffer);
        return buffer;
    }

    void
    AudioComponent::deleteAllSources
    ()
    {
        auto log = getLog();
        for (ALuint source : mSources)
        {
            log->debug("Deleting buffer {}" , source);
            deleteSources(1, source);
        }
    }

    void
    AudioComponent::setSourcePosision
    (ALuint sourceId, vec3 pos)
    {
        alSource3f(sourceId, AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioComponent::setListenerPosition
    (vec3 pos)
    {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioComponent::deleteBuffers
    (int count, ALuint buffer)
    {
        removeBufferFromComponent(buffer);
        alDeleteBuffers(count, &buffer);
    }

    void
    AudioComponent::deleteSources
    (int count, ALuint source)
    {
        removeSourceFromComponent(source);
        alDeleteSources(count, &source);
    }

    void
    AudioComponent::playSource
    (ALuint source)
    {
        auto log = getLog();
        log->debug(  "Playing source {}" , source);
        alSourcePlay(source);
    }

    void
    AudioComponent::stopSource
    (ALuint source)
    {
        auto log = getLog();
        log->debug(  "Stopping source {}" , source);
        alSourceStop(source);
    }

    void
    AudioComponent::pauseSource
    (ALuint source)
    {
        auto log = getLog();
        log->debug(  "Pausing source {}" , source);
        alSourcePause(source);
    }


    void
    AudioComponent::pushToPlayQueue
    (AudioInstance* audioAsset)
    {
        auto log = getLog();
        if (find(mPlayQueue.begin(),mPlayQueue.end(), audioAsset) == mPlayQueue.end())
        {
            if (audioAsset == nullptr)
            {
                log->error("pushToPlayQueue: Invalid audio asset");
                return;
            }

            if (audioAsset->getSource() == 0 && audioAsset->getBuffer() == 0)
            {
                audioAsset->setBuffer(generateBuffers(1));
                audioAsset->setSource(generateSources(1));

                auto sort = audioAsset->getSceneObjectRuntime();

                if (sort == nullptr) return;

                auto transform = sort->getTransform();

                vec3 tx = transform->getTranslation();
                vector<char>  bufferData = audioAsset->getAudioDataBuffer();
                alBufferData(audioAsset->getBuffer(), audioAsset->getFormat(), &bufferData[0],
                        static_cast<ALsizei> (bufferData.size()), audioAsset->getFrequency());
                alSourcei(
                    audioAsset->getSource(),
                    AL_BUFFER,
                    static_cast<ALint>(audioAsset->getBuffer())
                );
                setSourcePosision(audioAsset->getSource(), tx);

                alSourcei(audioAsset->getSource(), AL_LOOPING, audioAsset->isLooping() ? 1 : 0 );
            }
            mPlayQueue.push_back(audioAsset);
            log->debug("Pushed audio asset to play queue");
        }
    }

    void
    AudioComponent::pushToPauseQueue
    (AudioInstance* audioAsset)
    {
        auto log = getLog();
        if (find(mPauseQueue.begin(),mPauseQueue.end(), audioAsset) == mPauseQueue.end())
        {
            mPauseQueue.push_back(audioAsset);
            log->debug("Pushed audio asset to play queue");
        }
    }

    void
    AudioComponent::pushToStopQueue
    (AudioInstance* asset)
    {
        auto log = getLog();
        if (find(mStopQueue.begin(),mStopQueue.end(), asset) == mStopQueue.end())
        {
            mStopQueue.push_back(asset);
            log->debug("Pushed audio asset to stop queue.");
        }
    }

    void
    AudioComponent::pushToUpdateQueue
    (AudioInstance* ai)
    {
        if (find(mUpdateQueue.begin(), mUpdateQueue.end(), ai) == mUpdateQueue.end())
        {
            mUpdateQueue.push_back(ai);
        }
    }

    void
    AudioComponent::updateComponent
    ()
    {
        beginUpdate();
        updateInstances();
        updatePlayQueue();
        updatePauseQueue();
        updateStopQueue();
        endUpdate();
    }

    void AudioComponent::updatePlayQueue()
    {
        auto log = getLog();
        log->debug("Updating Play Queue");
        for (AudioInstance* audioAsset : mPlayQueue)
        {
            if (audioAsset == nullptr)
            {
                continue;
            }
            //if (getAudioStatus(audioAsset) != PLAYING)
            //{
                playSource(audioAsset->getSource());
                audioAsset->setStartTime(mTime->nowLL());
                audioAsset->setStatus(PLAYING);
            //}
            //else
            //{
            //    log->debug("" , audioAsset->getNameAndUuidString() , " is already playing");
           // }
        }
        mPlayQueue.clear();
    }

    void
    AudioComponent::updatePauseQueue
    ()
    {
        auto log = getLog();
        log->debug("Updating Pause Queue");
        for (AudioInstance* audioAsset : mPauseQueue)
        {
            if (audioAsset == nullptr)
            {
                continue;
            }
            if (getAudioStatus(audioAsset) != PAUSED)
            {
                pauseSource(audioAsset->getSource());
                audioAsset->setStatus(PAUSED);
            }
            else
            {
                log->debug("{} is already paused", audioAsset->getNameAndUuidString());
            }
        }
        mPauseQueue.clear();
    }

    void
    AudioComponent::updateStopQueue
    ()
    {
        auto log = getLog();
        log->debug("Updating Stop Queue");

        for (AudioInstance* audioAsset : mStopQueue)
        {
            if (audioAsset == nullptr)
            {
                continue;
            }
            if (getAudioStatus(audioAsset) != STOPPED)
            {
                stopSource(audioAsset->getSource());
                audioAsset->setStatus(STOPPED);
            }
            else
            {
                log->debug("" , audioAsset->getNameAndUuidString() , " is already stopped");
            }
        }
        mStopQueue.clear();
    }

    void
    AudioComponent::updateInstances
    ()
    {
       for (AudioInstance* ai : mUpdateQueue)
       {
          ai->updateFFT();
          ai->updateMarkers();
       }
    }

    ALint
    AudioComponent::getSampleOffset
    (ALuint source)
    const
    {
        ALint sampleOffset;
        alGetSourcei(source, AL_SAMPLE_OFFSET, &sampleOffset );
        //cout << "Current Offset " << sampleOffset << endl;
        return sampleOffset;
    }

    ALint
    AudioComponent::getSampleOffset
    (AudioInstance* audioAsset )
    const
    {
        if (audioAsset == nullptr)
        {
            return 0;
        }
        return getSampleOffset(audioAsset->getSource());
    }

    vector<char>
    AudioComponent::getAudioBuffer
    (AudioInstance* audioAsset, size_t offset, size_t length)
    const
    {
        vector<char> retval = vector<char>(length);
        if (audioAsset == nullptr)
        {
            return retval;
        }

        vector<char> audioData = audioAsset->getAudioDataBuffer();
        char* dataBegin = &audioData[0+offset];
        retval.insert(retval.begin(), dataBegin, dataBegin+length);

        return retval;
    }

    AudioStatus
    AudioComponent::getAudioStatus
    (AudioInstance* audioAsset)
    const
    {
        auto log = getLog();
        if (audioAsset == nullptr)
        {
            return UNKNOWN;
        }

        int state;
        alGetSourcei(audioAsset->getSource(), AL_SOURCE_STATE, &state);
        switch (state)
        {
            case AL_STOPPED:
                return STOPPED;
            case AL_PLAYING:
                return PLAYING;
            case AL_PAUSED:
                return PAUSED;
            default:
                log->error("Unknown Audio State for {} " , audioAsset->getNameAndUuidString());
                return UNKNOWN;
        }
    }

    AudioInstance*
    AudioComponent::newAudioInstance
    (AudioDefinition* definition, SceneObjectRuntime* rt)
    {
        auto log = getLog();

        if (definition == nullptr || rt == nullptr)
        {
           return nullptr;
        }

        if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
            return new WavAudioInstance(this, definition,rt);
        }
        else if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
            return new OggAudioInstance(this, definition,rt);
        }

        log->error("Error, unrecognised audio format {}", definition->getFormat());

        return nullptr;

    }

    void
    AudioComponent::setVolume
    (float volume)
    {
       alListenerf(AL_GAIN,volume);
    }

} // End of Dream
