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
            log->info("Stopping source {}" , source);
            alSourceStop(source);
        }
        deleteAllSources();
        deleteAllBuffers();
        if (mDevice)
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
        log->info("Initialising...");
        mDevice = alcOpenDevice(nullptr);
        mContext  = alcCreateContext(mDevice, nullptr);
        alcMakeContextCurrent(mContext);
        vector<float> position = {0.0f,0.0f,0.0f};
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
            log->info("Deleting buffer {}" , buffer);
            deleteBuffers(1, buffer);
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
            log->info("Deleting buffer {}" , source);
            deleteSources(1, source);
        }
    }

    void
    AudioComponent::setSourcePosision
    (ALuint sourceId, vector<float> position)
    {
        alSource3f(sourceId, AL_POSITION, position[0],position[1],position[2]);
    }

    void
    AudioComponent::setListenerPosition
    (vector<float> position)
    {
        alListener3f(AL_POSITION, position[0],position[1],position[2]);
    }

    void
    AudioComponent::deleteBuffers
    (int count, ALuint buffer)
    {
        alDeleteBuffers(count, &buffer);
    }

    void
    AudioComponent::deleteSources
    (int count, ALuint source)
    {
        alDeleteSources(count, &source);
    }

    void
    AudioComponent::playSource
    (ALuint source)
    {
        auto log = getLog();
        log->info(  "Playing source {}" , source);
        alSourcePlay(source);
    }

    void
    AudioComponent::stopSource
    (ALuint source)
    {
        auto log = getLog();
        log->info(  "Stopping source {}" , source);
        alSourceStop(source);
    }

    void
    AudioComponent::pauseSource
    (ALuint source)
    {
        auto log = getLog();
        log->info(  "Pausing source {}" , source);
        alSourcePause(source);
    }


    void
    AudioComponent::pushToPlayQueue
    (shared_ptr<AudioInstance> audioAsset)
    {
        auto log = getLog();
        try
        {
            if (find(mPlayQueue.begin(),mPlayQueue.end(), audioAsset) == mPlayQueue.end())
            {
                if (audioAsset->getSource() == 0 && audioAsset->getBuffer() == 0)
                {
                    audioAsset->setBuffer(generateBuffers(1));
                    audioAsset->setSource(generateSources(1));

                    auto transform = audioAsset->getSceneObjectRuntime()->getTransform();

                    vector<float> position = {
                        transform->getTranslation().x,
                        transform->getTranslation().y,
                        transform->getTranslation().z,
                    };
                    vector<char>  bufferData = audioAsset->getAudioDataBuffer();
                    alBufferData(audioAsset->getBuffer(), audioAsset->getFormat(), &bufferData[0],
                            static_cast<ALsizei> (bufferData.size()), audioAsset->getFrequency());
                    alSourcei(
                        audioAsset->getSource(),
                        AL_BUFFER,
                        static_cast<ALint>(audioAsset->getBuffer())
                    );
                    setSourcePosision(audioAsset->getSource(), position);
                    //audioAsset->getAudioDataBuffer().clear();
                }
                mPlayQueue.push_back(audioAsset);
                log->info("Pushed {} to play queue" , audioAsset->getNameAndUuidString());
            }
        }
        catch (const exception &ex)
        {
            log->error(
                "Unable to push asset to play queue {}\n{}",
                audioAsset->getNameAndUuidString(),
                ex.what()
            );
        }
    }

    void
    AudioComponent::pushToPauseQueue
    (shared_ptr<AudioInstance> audioAsset)
    {
        auto log = getLog();
        try
        {
            if (find(mPauseQueue.begin(),mPauseQueue.end(), audioAsset) == mPauseQueue.end())
            {
                mPauseQueue.push_back(audioAsset);
                log->info("Pushed {} to play queue", audioAsset->getNameAndUuidString());
            }
        }
        catch (const exception &ex)
        {
            log->error(
                "Unable to push asset to pause queue {}\n{}",
                audioAsset->getNameAndUuidString(),ex.what()
            );
        }
    }

    void
    AudioComponent::pushToStopQueue
    (shared_ptr<AudioInstance> asset)
    {
        auto log = getLog();
        try
        {
            if (find(mStopQueue.begin(),mStopQueue.end(), asset) == mStopQueue.end())
            {
                mStopQueue.push_back(asset);
                log->info("Pushed " , asset->getNameAndUuidString() , " to stop queue.");;
            }
        }
        catch (const exception &ex)
        {
            log->error
            (
                "Unable to push asset to stop queue {}\n{}",
                asset->getNameAndUuidString(),ex.what()
            );
        }
    }

    void
    AudioComponent::updateComponent
    ()
    {
        beginUpdate();
        updatePlayQueue();
        updatePauseQueue();
        updateStopQueue();
        endUpdate();
    }

    void AudioComponent::updatePlayQueue()
    {
        auto log = getLog();
        log->info("Updating Play Queue");
        for (shared_ptr<AudioInstance> audioAsset : mPlayQueue)
        {
            if (getAudioStatus(audioAsset) != PLAYING)
            {
                playSource(audioAsset->getSource());
                audioAsset->setStatus(PLAYING);
            }
            else
            {
                log->info("" , audioAsset->getNameAndUuidString() , " is already playing");
            }
        }
        mPlayQueue.clear();
    }

    void
    AudioComponent::updatePauseQueue
    ()
    {
        auto log = getLog();
        log->info("Updating Pause Queue");
        for (shared_ptr<AudioInstance> audioAsset : mPauseQueue)
        {
            if (getAudioStatus(audioAsset) != PAUSED)
            {
                pauseSource(audioAsset->getSource());
                audioAsset->setStatus(PAUSED);
            }
            else
            {
                log->info("{} is already paused", audioAsset->getNameAndUuidString());
            }
        }
        mPauseQueue.clear();
    }

    void
    AudioComponent::updateStopQueue
    ()
    {
        auto log = getLog();
        log->info("Updating Stop Queue");

        for (shared_ptr<AudioInstance> audioAsset : mStopQueue)
        {
            if (getAudioStatus(audioAsset) != STOPPED)
            {
                stopSource(audioAsset->getSource());
                audioAsset->setStatus(STOPPED);
            }
            else
            {
                log->info("" , audioAsset->getNameAndUuidString() , " is already stopped");
            }
        }
        mStopQueue.clear();
    }

    void
    AudioComponent::playAudioAsset
    (shared_ptr<AudioInstance> asset)
    {
        pushToPlayQueue(asset);
    }

    void
    AudioComponent::pauseAudioAsset
    (shared_ptr<AudioInstance> asset)
    {
        pushToPauseQueue(asset);
    }

    void
    AudioComponent::stopAudioAsset
    (shared_ptr<AudioInstance> asset)
    {
        pushToStopQueue(asset);
    }

    float
    AudioComponent::getSampleOffset
    (ALuint source)
    {
        float sampleOffset;
        alGetSourcef(source, AL_SAMPLE_OFFSET, &sampleOffset );
        return sampleOffset;
    }

    vector<char>
    AudioComponent::getAudioBuffer
    (shared_ptr<AudioInstance> audioAsset, int offset, int length)
    {
        auto log = getLog();
        vector<char> retval = vector<char>(length);
        try
        {
            vector<char> audioData = audioAsset->getAudioDataBuffer();
            char* dataBegin = &audioData[0];
            retval.insert(retval.begin(), dataBegin, dataBegin+length);
        }
        catch (const exception &ex)
        {
            log->error
            (
                "Unable to get buffer data for {}\n{}",
                audioAsset->getNameAndUuidString(),
                ex.what()
            );
        }
        return retval;
    }

    float
    AudioComponent::getSampleOffset
    (shared_ptr<AudioInstance> audioAsset)
    {
        auto log = getLog();
        try
        {
            return getSampleOffset(audioAsset->getSource());
        }
        catch (const exception &ex)
        {
            log->error(
                "Could not get sample offset for asset {}\n{}" ,
                audioAsset->getNameAndUuidString(),
                ex.what()
            );
        }
        return 0.0f;
    }

    AudioStatus
    AudioComponent::getAudioStatus
    (shared_ptr<AudioInstance> audioAsset)
    {
        auto log = getLog();
        try
        {
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
        catch (const exception &ex)
        {
           log->error(
              "Unable to get state of asset {} {} ",
              audioAsset->getNameAndUuidString(),
              ex.what()
            );
        }
        return UNKNOWN;
    }

    shared_ptr<AudioInstance>
    AudioComponent::newAudioInstance
    (shared_ptr<AudioDefinition> definition, shared_ptr<SceneObjectRuntime> rt)
    {
        auto log = getLog();
        if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
            auto ai = make_shared<WavAudioInstance>(definition,rt);
            return ai;
        }
        else if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
            auto ai = make_shared<OggAudioInstance>(definition,rt);
            return ai;
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
