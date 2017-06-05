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

#include "AudioInstance.h"
#include <vorbis/vorbisfile.h>


#define ASSET_FORMAT_OGG "ogg"

namespace Dream
{

    class OggAudioInstance : public AudioInstance
    {
    public:
        OggAudioInstance(AssetDefinition*, Transform3D*);
        ~OggAudioInstance();
        bool load(std::string) override;
        void loadExtraAttributes(nlohmann::json) override;
        bool play() override;
        bool pause() override;
        bool stop() override;
    }; // End of OggAudio

} // End of Dream

#endif // End of OGGAUDIO_H
