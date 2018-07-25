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

#include <iostream>

#include "AudioComponent.h"
#include "Wav/WavAudioInstance.h"
#include "Ogg/OggAudioInstance.h"

#include "AudioDefinition.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

using std::cout;
using std::endl;

namespace Dream
{
    AudioComponent::AudioComponent
    ()
        : IComponent()
    {

    }

    AudioComponent::~AudioComponent
    ()
    {
        for (ALuint source : mSources)
        {
            cout << "AudioComponent: Stopping source " << source << endl;
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
        cout << "AudioComponent: Initialising..." << endl;
        mDevice = alcOpenDevice(nullptr);
        mContext = alcCreateContext(mDevice, nullptr);
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
        for (ALuint buffer : mBuffers)
        {
            cout << "AudioComponent: Deleting buffer " << buffer << endl;
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
        for (ALuint source : mSources)
        {
            cout << "AudioComponent: Deleting buffer " << source << endl;
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
        cout <<   "AudioComponent: Playing source " << source << endl;
        alSourcePlay(source);
    }

    void
    AudioComponent::stopSource
    (ALuint source)
    {
        cout <<   "AudioComponent: Stopping source " << source << endl;
        alSourceStop(source);
    }

    void
    AudioComponent::pauseSource
    (ALuint source)
    {
        cout <<   "AudioComponent: Pausing source " << source << endl;
        alSourcePause(source);
    }


    void
    AudioComponent::pushToPlayQueue
    (AudioInstance* asset)
    {
        try
        {
            if (find(mPlayQueue.begin(),mPlayQueue.end(), asset) == mPlayQueue.end())
            {
                AudioInstance* audioAsset;
                audioAsset = dynamic_cast<AudioInstance*>(asset);
                if (audioAsset->getSource() == 0 && audioAsset->getBuffer() == 0)
                {
                    audioAsset->setBuffer(generateBuffers(1));
                    audioAsset->setSource(generateSources(1));

                    Transform3D transform = audioAsset->getSceneObjectRuntimeHandle()->getTransform();

                    vector<float> position = {
                        transform.getTranslation().x,
                        transform.getTranslation().y,
                        transform.getTranslation().z,
                    };
                    vector<char>  bufferData = audioAsset->getAudioDataBuffer();
                    alBufferData(audioAsset->getBuffer(), audioAsset->getFormat(), &bufferData[0],
                            static_cast<ALsizei> (bufferData.size()), audioAsset->getFrequency());
                    alSourcei(audioAsset->getSource(), AL_BUFFER, audioAsset->getBuffer());
                    setSourcePosision(audioAsset->getSource(), position);
                    //audioAsset->getAudioDataBuffer().clear();
                }
                mPlayQueue.push_back(audioAsset);
                cout << "AudioComponent: Pushed " << asset->getNameAndUuidString() << " to play queue." << endl;;
            }
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Unable to push asset to play queue" << asset->getNameAndUuidString() << endl
                 << ex.what() << endl;
        }
    }

    void
    AudioComponent::pushToPauseQueue
    (AudioInstance* asset)
    {
        try
        {
            if (find(mPauseQueue.begin(),mPauseQueue.end(), asset) == mPauseQueue.end())
            {
                mPauseQueue.push_back(dynamic_cast<AudioInstance*>(asset));
                cout << "AudioComponent: Pushed " << asset->getNameAndUuidString() << " to pause queue." << endl;;
            }
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Unable to push asset to pause queue" << asset->getNameAndUuidString() << endl
                      << ex.what() << endl;
        }
    }

    void
    AudioComponent::pushToStopQueue
    (AudioInstance* asset)
    {
        try
        {
            if (find(mStopQueue.begin(),mStopQueue.end(), asset) == mStopQueue.end())
            {
                mStopQueue.push_back(asset);
                cout << "AudioComponent: Pushed " << asset->getNameAndUuidString() << " to stop queue." << endl;;
            }
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Unable to push asset to stop queue" << asset->getNameAndUuidString() << endl
                 << ex.what() << endl;
        }
    }

    void
    AudioComponent::updateComponent
    (SceneRuntime*)
    {
        cout << "AudioComponent: Updating Component" << endl;
        updatePlayQueue();
        updatePauseQueue();
        updateStopQueue();
    }

    void AudioComponent::updatePlayQueue()
    {
        cout << "AudioComponent: Updating Play Queue" << endl;
        for (AudioInstance* audioAsset : mPlayQueue)
        {
            if (getAudioStatus(audioAsset) != PLAYING)
            {
                playSource(audioAsset->getSource());
                audioAsset->setStatus(PLAYING);
            }
            else
            {
                cout << "AudioComponent: " << audioAsset->getNameAndUuidString() << " is already playing" << endl;
            }
        }
        mPlayQueue.clear();
    }

    void
    AudioComponent::updatePauseQueue
    ()
    {
        cout << "AudioComponent: Updating Pause Queue" << endl;
        for (AudioInstance* audioAsset : mPauseQueue)
        {
            if (getAudioStatus(audioAsset) != PAUSED)
            {
                pauseSource(audioAsset->getSource());
                audioAsset->setStatus(PAUSED);
            }
            else
            {
                cout << "AudioComponent: " << audioAsset->getNameAndUuidString() << " is already paused" << endl;
            }
        }
        mPauseQueue.clear();
    }

    void
    AudioComponent::updateStopQueue
    ()
    {
        cout << "AudioComponent: Updating Stop Queue" << endl;

        for (AudioInstance* audioAsset : mStopQueue)
        {
            if (getAudioStatus(audioAsset) != STOPPED)
            {
                stopSource(audioAsset->getSource());
                audioAsset->setStatus(STOPPED);
            }
            else
            {
                cout << "AudioComponent: " << audioAsset->getNameAndUuidString() << " is already stopped" << endl;
            }
        }
        mStopQueue.clear();
    }

    void
    AudioComponent::playAudioAsset
    (AudioInstance *asset)
    {
        pushToPlayQueue(asset);
    }

    void
    AudioComponent::pauseAudioAsset
    (AudioInstance *asset)
    {
        pushToPauseQueue(asset);
    }

    void
    AudioComponent::stopAudioAsset
    (AudioInstance *asset)
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
    (AudioInstance* asset, int offset, int length)
    {
        vector<char> retval = vector<char>(length);
        try
        {
            AudioInstance* audioAsset;
            audioAsset = dynamic_cast<AudioInstance*>(asset);
            vector<char> audioData = audioAsset->getAudioDataBuffer();
            char* dataBegin = &audioData[0];
            retval.insert(retval.begin(), dataBegin, dataBegin+length);
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Unable to get buffer data for " << asset->getNameAndUuidString()
                      << ex.what() << endl;
        }
        return retval;
    }

    float
    AudioComponent::getSampleOffset
    (AudioInstance* asset)
    {
        try
        {
            AudioInstance* audioAsset;
            audioAsset = dynamic_cast<AudioInstance*>(asset);
            return getSampleOffset(audioAsset->getSource());
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Could not get sample offset for asset" << asset->getNameAndUuidString() << endl;
            cerr << ex.what() << endl;
        }
        return 0.0f;
    }

    AudioStatus
    AudioComponent::getAudioStatus
    (AudioInstance* audioAsset)
    {
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
                    cerr << "AudioComponent: Unknown Audio State for " << audioAsset->getNameAndUuidString() << endl;
                    return UNKNOWN;
            }
        }
        catch (const exception &ex)
        {
            cerr << "AudioComponent: Unable to get state of asset " << audioAsset->getNameAndUuidString()
                      << endl << ex.what() << endl;
        }
        return UNKNOWN;
    }

    AudioInstance*
    AudioComponent::newAudioInstance
    (AudioDefinition* definition,SceneObjectRuntime* rt)
    {
        if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
            return new WavAudioInstance(definition,rt);
        }
        else if (definition->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
            return new OggAudioInstance(definition,rt);
        }
        cerr << "AudioComponent: Error, unrecognised audio format "
             << definition->getFormat() << endl;
        return nullptr;

    }

} // End of Dream
