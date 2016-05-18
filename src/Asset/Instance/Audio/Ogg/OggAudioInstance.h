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

#include "../../AssetInstance.h"

#define ASSET_FORMAT_OGG "ogg"

namespace Dream {
namespace Asset {
namespace Instance {
namespace Audio {
namespace Ogg {
	
	class OggAudioInstance : public Dream::Asset::Instance::AssetInstance {
	private:
		std::string mOggPath;
	public:
		OggAudioInstance(Dream::Asset::AssetDefinition*);
		~OggAudioInstance(void);
	}; // End of OggAudio
	
} // End of Ogg
} // End of Audio
} // End of Instnace
} // End of Asset
} // End of Dream

#endif // End of OGGAUDIO_H
