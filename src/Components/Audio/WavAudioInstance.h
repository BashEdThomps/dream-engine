/*
 * WavAudioInstance
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

#ifndef WAVAUDIO_H
#define WAVAUDIO_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include "AudioAssetInstance.h"
#include "WavHeader.h"

#define ASSET_FORMAT_WAV  "wav"

namespace Dream {
    namespace Components {
        namespace Audio {
            class WavAudioInstance : public AudioAssetInstance {
            private:
                WavHeader mWavHeader;
            public:
                WavAudioInstance(AssetDefinition*,Transform3D*);
                ~WavAudioInstance();
                bool load(std::string);
                size_t  getFileSize(FILE* inFile);
                void loadExtraAttributes(nlohmann::json);
            }; // End WavAudioInstance
        } // End of Audio
    } // End of Components
} // End of Dream

#endif // End of WAVAUDIO_H
