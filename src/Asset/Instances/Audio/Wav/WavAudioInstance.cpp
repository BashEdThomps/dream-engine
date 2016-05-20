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

#include "WavAudioInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
namespace Wav       {
	
	WavAudioInstance::WavAudioInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {}
					
	WavAudioInstance::~WavAudioInstance(void) {}
	
	bool WavAudioInstance::load(Dream::Util::FileReader* fileReader) {
		return false;
	}

} // End of Wav
} // End of Audio
} // End of Instance
} // End of Asset
} // End of Dream
