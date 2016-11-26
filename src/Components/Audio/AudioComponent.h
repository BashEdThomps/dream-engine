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

#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <iostream>
#include <vector>
#include <SDL2/SDL_mixer.h>
#include "AudioStatus.h"
#include "AudioInstance.h"
#include "../ComponentInterface.h"

namespace Dream {

      class AudioComponent : public ComponentInterface {
      private:
        vector<AudioInstance*> mPlayQueue;
        vector<AudioInstance*> mPauseQueue;
        vector<AudioInstance*> mStopQueue;
      public:
        AudioComponent();
        ~AudioComponent();
        bool init();
        void update(Scene*);

        void pushToPlayQueue(AssetInstance*);
        void pushToPauseQueue(AssetInstance*);
        void pushToStopQueue(AssetInstance*);

        void playAsset(AssetInstance*);
        void pauseAsset(AssetInstance*);
        void stopAsset(AssetInstance*);

      protected:
        void   play(AudioInstance*);
        void   stop(AudioInstance*);
        void   pause(AudioInstance*);

        void   updatePlayQueue();
        void   updatePauseQueue();
        void   updateStopQueue();

      }; // End of AudioComponent

} // End of Dream

#endif // End of AUDIOCOMPONENT_H
