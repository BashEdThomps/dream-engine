/*
* Dream::Asset::Audio::Ogg::OggAudio
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

#include "../AudioAssetInstance.h"
#include <vorbis/vorbisfile.h>


#define ASSET_FORMAT_OGG "ogg"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
namespace Ogg       {
	
	class OggAudioInstance : public AudioAssetInstance {
	public:
		OggAudioInstance(AssetDefinition*);
		~OggAudioInstance();
		bool load(std::string);
	}; // End of OggAudio
	
} // End of Ogg
} // End of Audio
} // End of Instnace
} // End of Asset
} // End of Dream

#endif // End of OGGAUDIO_H
