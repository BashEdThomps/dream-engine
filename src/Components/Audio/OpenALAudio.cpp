/*
* Dream::Components::Audio::OpenAL::OpenALAudio
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

#include "OpenALAudio.h"

#include <iostream>
namespace Dream {
  namespace Components {
    namespace Audio {

      OpenALAudio::OpenALAudio() : AudioComponentInterface() {}

      OpenALAudio::~OpenALAudio() {
        deleteAllSources();
        deleteAllBuffers();
      }

      bool OpenALAudio::init() {
        std::cout << "OpenALAudio: Initialising...";
        mDevice = alcOpenDevice(NULL);
        mContext = alcCreateContext(mDevice,NULL);
        alcMakeContextCurrent(mContext);
        std::cout << "done." << std::endl;
        std::vector<float> position = {0.0f,0.0f,0.0f};
        setListenerPosition(position);
        return true;
      }

      ALuint OpenALAudio::generateBuffers(size_t size) {
        alGetError();
        ALuint buffer;
        alGenBuffers( size, &buffer );
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR ){
          return -1;
        }
        mBuffers.push_back(buffer);
        return buffer;
      }

      void OpenALAudio::deleteAllBuffers() {
        std::vector<ALuint>::iterator bufferIt;
        for (bufferIt = mBuffers.begin(); bufferIt != mBuffers.end(); bufferIt++) {
          deleteBuffers(1, (*bufferIt));
        }
      }

      ALuint OpenALAudio::generateSources(size_t size) {
        alGetError();
        ALuint buffer;
        alGenSources( size, &buffer );
        ALenum error = alGetError();
        if ( error != AL_NO_ERROR ){
          return -1;
        }
        mSources.push_back(buffer);
        return buffer;
      }

      void OpenALAudio::deleteAllSources() {
        std::vector<ALuint>::iterator sourceIt;
        for (sourceIt = mSources.begin(); sourceIt != mSources.end(); sourceIt++) {
          deleteSources(1, (*sourceIt));
        }
      }

      void OpenALAudio::setSourcePosision(ALuint sourceId, std::vector<float> position) {
        alSource3f(sourceId, AL_POSITION, position[0],position[1],position[2]);
      }

      void OpenALAudio::setListenerPosition(std::vector<float> position) {
        alListener3f(AL_POSITION, position[0],position[1],position[2]);
      }

      void OpenALAudio::deleteBuffers(int count, ALuint buffer) {
        alDeleteBuffers(count, &buffer);
      }

      void OpenALAudio::deleteSources(int count, ALuint source) {
        alDeleteSources(count, &source);
      }

      void OpenALAudio::playSource(ALuint source) {
        alSourcePlay(source);
      }

      void OpenALAudio::stopSource(ALuint source) {
        alSourceStop(source);
      }

      void OpenALAudio::pauseSource(ALuint source) {
        alSourcePause(source);
      }


      void OpenALAudio::pushToPlayQueue(AssetInstance* asset) {
        try {
          if (std::find(mPlayQueue.begin(),mPlayQueue.end(), asset) == mPlayQueue.end()){
           AudioAssetInstance* audioAsset;
            audioAsset = dynamic_cast<AudioAssetInstance*>(asset);
            if (audioAsset->getSource() == 0 && audioAsset->getBuffer() == 0) {
              audioAsset->setBuffer(generateBuffers(1));
              audioAsset->setSource(generateSources(1));
              std::vector<float> position = audioAsset->getParentSceneObject()->getTranslation();
              std::vector<char>  bufferData = audioAsset->getAudioDataBuffer();
              alBufferData(audioAsset->getBuffer(), audioAsset->getFormat(), &bufferData[0],
                  static_cast<ALsizei> (bufferData.size()), audioAsset->getFrequency());
              alSourcei(audioAsset->getSource(), AL_BUFFER, audioAsset->getBuffer());
              setSourcePosision(audioAsset->getSource(), position);
              //audioAsset->getAudioDataBuffer().clear();
            }
            mPlayQueue.push_back(audioAsset);
          }
        } catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Unable to push asset to play queue" << asset->getNameAndUUIDString() << std::endl
                    << ex.what() << std::endl;
        }
      }

      void OpenALAudio::pushToPauseQueue(AssetInstance* asset) {
        try {
          if (std::find(mPauseQueue.begin(),mPauseQueue.end(), asset) == mPauseQueue.end()) {
            mPauseQueue.push_back(dynamic_cast<AudioAssetInstance*>(asset));
          }
        }  catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Unable to push asset to pause queue" << asset->getNameAndUUIDString() << std::endl
                    << ex.what() << std::endl;
        }
      }

      void OpenALAudio::pushToStopQueue(AssetInstance* asset) {
        try {
          if (std::find(mStopQueue.begin(),mStopQueue.end(), asset) == mStopQueue.end()) {
            mStopQueue.push_back(dynamic_cast<AudioAssetInstance*>(asset));
          }
        } catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Unable to push asset to stop queue" << asset->getNameAndUUIDString() << std::endl
                    << ex.what() << std::endl;
        }
      }

      void OpenALAudio::update(Scene*) {
        updatePlayQueue();
        updatePauseQueue();
        updateStopQueue();
        cleanUpBuffersAndSources();
      }

      void OpenALAudio::cleanUpBuffersAndSources() {
        std::vector<ALuint>::iterator sourceIterator;
        for (sourceIterator = mSources.begin(); sourceIterator != mSources.end(); sourceIterator++) {
          // Clean Up in the Source & Buffer Isle
        }
      }

      void OpenALAudio::updatePlayQueue() {
        std::vector<AudioAssetInstance*>::iterator iterator;
        for (iterator = mPlayQueue.begin(); iterator != mPlayQueue.end(); iterator++) {
          AudioAssetInstance *audioAsset = (*iterator);
          if (getAudioAssetState(audioAsset) != PLAYING){
            playSource(audioAsset->getSource());
          }
        }
        mPlayQueue.clear();
      }

      void OpenALAudio::updatePauseQueue() {
        std::vector<AudioAssetInstance*>::iterator iterator;
        for (iterator = mPauseQueue.begin(); iterator != mPauseQueue.end(); iterator++) {
          AudioAssetInstance *audioAsset = (*iterator);
          if (getAudioAssetState(audioAsset) != PAUSED) {
            pauseSource(audioAsset->getSource());
          }
        }
        mPauseQueue.clear();
      }

      void OpenALAudio::updateStopQueue() {
        std::vector<AudioAssetInstance*>::iterator iterator;
        for (iterator = mStopQueue.begin(); iterator != mStopQueue.end(); iterator++) {
          AudioAssetInstance *audioAsset = (*iterator);
          if (getAudioAssetState(audioAsset) != STOPPED) {
            stopSource(audioAsset->getSource());
          }
        }
        mStopQueue.clear();
      }

      void OpenALAudio::playAudioAsset(AssetInstance *asset) {
        pushToPlayQueue(asset);
      }

      void OpenALAudio::pauseAudioAsset(AssetInstance *asset) {
        pushToPauseQueue(asset);
      }

      void OpenALAudio::stopAudioAsset(AssetInstance *asset) {
        pushToStopQueue(asset);
      }

      ALfloat OpenALAudio::getSampleOffset(ALuint source) {
        ALfloat sampleOffset;
        alGetSourcef(source, AL_SAMPLE_OFFSET, &sampleOffset );
        return sampleOffset;
      }

      std::vector<char> OpenALAudio::getAudioBuffer(AssetInstance* asset, int offset, int length) {
        std::vector<char> retval = std::vector<char>(length);
        try {
          AudioAssetInstance* audioAsset;
          audioAsset = dynamic_cast<AudioAssetInstance*>(asset);
          std::vector<char> audioData = audioAsset->getAudioDataBuffer();
          char* dataBegin = &audioData[0];
          retval.insert(retval.begin(), dataBegin, dataBegin+length);
        } catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Unable to get buffer data for " << asset->getNameAndUUIDString()
                    << ex.what() << std::endl;
        }
        return retval;
      }

      float OpenALAudio::getSampleOffset(AssetInstance* asset) {
        try {
          AudioAssetInstance* audioAsset;
          audioAsset = dynamic_cast<AudioAssetInstance*>(asset);
          return getSampleOffset(audioAsset->getSource());
        } catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Could not get sample offset for asset" << asset->getNameAndUUIDString() << std::endl;
          std::cerr << ex.what() << std::endl;
        }
        return 0.0f;
      }

      AudioAssetStatus
      OpenALAudio::getAudioAssetState(AssetInstance* asset) {
        try {
          AudioAssetInstance* audioAsset;
          audioAsset = dynamic_cast<AudioAssetInstance*>(asset);
          ALint state;
          alGetSourcei(audioAsset->getSource(), AL_SOURCE_STATE, &state);
          switch (state) {
            case AL_STOPPED:
              return STOPPED;
            case AL_PLAYING:
              return PLAYING;
            case AL_PAUSED:
              return PAUSED;
            default:
              std::cerr << "OpenALAudio: Unknown Audio State for " << asset->getNameAndUUIDString() << std::endl;
              return UNKNOWN;
          }
        } catch (const std::exception &ex) {
          std::cerr << "OpenALAudio: Unable to get state of asset " << asset->getNameAndUUIDString()
                    << std::endl << ex.what() << std::endl;
        }
        return UNKNOWN;
      }

    } // End of Audio
  } // End of Components
} // End of Dream
