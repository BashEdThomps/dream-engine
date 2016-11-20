/*
 * OggAudio
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

#ifndef OGGAUDIO_H
#define OGGAUDIO_H

#include "AudioAssetInstance.h"
#include <vorbis/vorbisfile.h>


#define ASSET_FORMAT_OGG "ogg"

namespace Dream     {
  namespace Components {
    namespace Audio     {

      class OggAudioInstance : public AudioAssetInstance {
      public:
        OggAudioInstance(AssetDefinition*);
        ~OggAudioInstance();
        bool load(std::string);
        void loadExtraAttributes(nlohmann::json);
      }; // End of OggAudio

    } // End of Audio
  } // End of Components
} // End of Dream

#endif // End of OGGAUDIO_H
