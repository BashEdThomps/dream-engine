/*
* WavAudio
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

#include "../../AssetInstance.h"

#define ASSET_FORMAT_WAV  "wav"

namespace Dream    {
namespace Asset    {
namespace Instance {
namespace Audio    {
namespace Wav      {
	
	class WavAudioInstance : public Dream::Asset::Instance::AssetInstance {
	public:
		WavAudioInstance(Dream::Asset::AssetDefinition*);
		~WavAudioInstance(void);
	};
	
}
}
}
}
}

#endif // End of WAVAUDIO_H
