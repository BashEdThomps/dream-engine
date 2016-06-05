/*
* Dream::Plugins::Audio::OpenAL::OALAudio
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

#include "OALAudio.h"

#include <iostream>
namespace Dream   {
namespace Plugins {
namespace Audio   {
namespace OpenAL  {
	
	OALAudio::OALAudio() : AudioPluginInterface() {}
	
	OALAudio::~OALAudio() {
		deleteAllSources();
		deleteAllBuffers();
	}

	bool OALAudio::init() {
		std::cout << "OALAudio: Initialising...";
		mDevice = alcOpenDevice(NULL);
		mContext = alcCreateContext(mDevice,NULL);
		alcMakeContextCurrent(mContext);
		std::cout << "done." << std::endl;
		return true;
	}

	ALuint OALAudio::generateBuffers(size_t size) {
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
	
	void OALAudio::deleteAllBuffers() {
		std::vector<ALuint>::iterator bufferIt;
		for (bufferIt = mBuffers.begin(); bufferIt != mBuffers.end(); bufferIt++) {
			deleteBuffers(1, (*bufferIt));
		}
	}
	
	ALuint OALAudio::generateSources(size_t size) {
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
	
	void OALAudio::deleteAllSources() {
		std::vector<ALuint>::iterator sourceIt;
		for (sourceIt = mSources.begin(); sourceIt != mSources.end(); sourceIt++) {
			deleteSources(1, (*sourceIt));
		}
	}
	
	
	void OALAudio::setSourcePosision(ALuint sourceId, float x, float y, float z) {
		alSource3f(sourceId, AL_POSITION, x,y,z);
	}
	
	void OALAudio::setListenerPosition(float x, float y, float z) {
		alListener3f(AL_POSITION, x,y,z);
	}
	
	void OALAudio::deleteBuffers(int count, ALuint buffer) {
		alDeleteBuffers(count, &buffer);
	}
	
	void OALAudio::deleteSources(int count, ALuint source) {
		alDeleteSources(count, &source);
	}
	
	void OALAudio::playSource(ALuint source) {
		alSourcePlay(source);
	}
	
	void OALAudio::stopSource(ALuint source) {
		alSourceStop(source);
	}
	
	void OALAudio::pauseSource(ALuint source) {
		alSourcePause(source);
	}
	
	ALenum OALAudio::getSourceState(ALuint source) {
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return state;
	}
	
	void OALAudio::pushToPlayQueue(ALuint source) {
		if (std::find(mPlayQueue.begin(),mPlayQueue.end(),source) == mPlayQueue.end()){
			mPlayQueue.push_back(source);
		}
	}
	
	void OALAudio::pushToPauseQueue(ALuint source) {
		if (std::find(mPauseQueue.begin(),mPauseQueue.end(),source) == mPauseQueue.end()) {
			mPauseQueue.push_back(source);
		}
	}
	
	void OALAudio::pushToStopQueue(ALuint source) {
		if (std::find(mStopQueue.begin(),mStopQueue.end(),source) == mStopQueue.end()) {
			mStopQueue.push_back(source);
		}
	}

	void OALAudio::update(Scene::Scene*) {
		updatePlayQueue();
		updatePauseQueue();
		updateStopQueue();
	}
	
	void OALAudio::updatePlayQueue() {
		std::vector<ALuint>::iterator iterator;
		for (iterator = mPlayQueue.begin(); iterator != mPlayQueue.end(); iterator++) {
			playSource((*iterator));
		}
		mPlayQueue.clear();
	}
	
	void OALAudio::updatePauseQueue() {
		std::vector<ALuint>::iterator iterator;
		for (iterator = mPauseQueue.begin(); iterator != mPauseQueue.end(); iterator++) {
			pauseSource((*iterator));
		}
		mPauseQueue.clear();
	}
	
	void OALAudio::updateStopQueue() {
		std::vector<ALuint>::iterator iterator;
		for (iterator = mStopQueue.begin(); iterator != mStopQueue.end(); iterator++) {
			stopSource((*iterator));
		}
		mStopQueue.clear();
	}
	
} // End of OpenAL
} // End of Audio
} // End of Plugins
} // End of Dream
