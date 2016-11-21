/*
* Dream::Components::Audio::AudioComponentInterface
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

#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include "../ComponentInterface.h"
#include "../../AssetInstance.h"

namespace Dream {
  namespace Components {
    namespace Audio {

      class AudioComponentInterface : public Components::ComponentInterface {
      public:
        AudioComponentInterface(void);
        virtual ~AudioComponentInterface(void) {};
        virtual float getSampleOffset(AssetInstance*) = 0;
        virtual std::vector<char> getAudioBuffer(AssetInstance*, int, int) = 0;
        virtual void  playAudioAsset(AssetInstance*)  = 0;
        virtual void  pauseAudioAsset(AssetInstance*) = 0;
        virtual void  stopAudioAsset(AssetInstance*)  = 0;
      }; // End of AudioComponentInterface

    } // End of Components
  } // End of Audio
} // End of Dream

#endif // End of AUDIOINTERFACE_H
