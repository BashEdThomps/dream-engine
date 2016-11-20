/*
* Dream::Plugnis::Audio::OpenAL::OpenALAudio
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

#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include "AudioComponentInterface.h"
#include "AudioAssetInstance.h"

namespace Dream   {
  namespace Components {
    namespace Audio   {

      class OpenALAudio : public AudioComponentInterface {
      private:
        std::vector<AudioAssetInstance*> mPlayQueue;
        std::vector<AudioAssetInstance*> mPauseQueue;
        std::vector<AudioAssetInstance*> mStopQueue;
        std::vector<ALuint> mSources;
        std::vector<ALuint> mBuffers;
        ALCdevice*  mDevice;
        ALCcontext* mContext;
      public:
        OpenALAudio();
        ~OpenALAudio();
        bool init();
        void update(Scene*);

        void   setSourcePosision(ALuint, std::vector<float>);
        void   setListenerPosition(std::vector<float>);

        void pushToPlayQueue(AssetInstance*);
        void pushToPauseQueue(AssetInstance*);
        void pushToStopQueue(AssetInstance*);

        void playAudioAsset(AssetInstance*);
        void pauseAudioAsset(AssetInstance*);
        void stopAudioAsset(AssetInstance*);
        AudioAssetStatus getAudioAssetState(AssetInstance*);

        ALfloat getSampleOffset(ALuint);
        ALfloat getSampleOffset(AssetInstance*);
        std::vector<char> getAudioBuffer(AssetInstance*, int, int);

      protected:
        ALuint generateBuffers(size_t);
        ALuint generateSources(size_t);

        void   deleteBuffers(int, ALuint);
        void   deleteSources(int, ALuint);

        void   playSource(ALuint);
        void   stopSource(ALuint);
        void   pauseSource(ALuint);

        void   updatePlayQueue();
        void   updatePauseQueue();
        void   updateStopQueue();
        void   cleanUpBuffersAndSources();

        void deleteAllSources();
        void deleteAllBuffers();
      }; // End of OpenALAudio

    } // End of Audio
  } // End of Components
} // End of Dream

#endif // End of AUDIOPLAYBACK_H
