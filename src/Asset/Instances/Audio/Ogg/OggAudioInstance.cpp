/*
* Dream::Asset::Instance::Audio::Ogg::OggAudio
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

#include "OggAudioInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Audio     {
namespace Ogg       {
	
	OggAudioInstance::OggAudioInstance(AssetDefinition* definition)
		: AudioAssetInstance(definition) {
			
	}
	
  OggAudioInstance::~OggAudioInstance() {
		
	}
	
	bool OggAudioInstance::load(std::string projectPath) {
		std::string absPath = projectPath+mDefinition->getAssetPath();
		std::cout << "OggAudioInstance: Loading Instance: " << absPath << std::endl;
		return loadOGG(absPath);
	}
	
	bool OggAudioInstance::loadOGG(std::string fileName) {
		// 0 for Little-Endian, 1 for Big-Endian
		int endian = 0;
		int bitStream;
		long bytes;
		// Local fixed size array
		char array[BUFFER_SIZE];
		FILE *f;
		// Open for binary reading
		f = fopen(fileName.c_str(), "rb");
		if (f == NULL) {
			std::cerr << "OggAudioInstance:: Cannot open " << fileName
			          << " for reading..." << std::endl;
			return false;
		}
		vorbis_info *pInfo;
		OggVorbis_File oggFile;
		// Try opening the given file
		if (ov_open(f, &oggFile, NULL, 0) != 0) {
			std::cerr << "OggAudioInstance: Error opening " << fileName
			          << " for decoding..." << std::endl;
			return false;
		}
		// Get some information about the OGG file
		pInfo = ov_info(&oggFile, -1);
		
		// Check the number of channels... always use 16-bit samples
		if (pInfo->channels == 1) {
			mFormat = AL_FORMAT_MONO16;
		} else {
			mFormat = AL_FORMAT_STEREO16;
		}
		// The frequency of the sampling rate
		mFrequency = pInfo->rate;
		// Keep reading until all is read
		do {
			// Read up to a buffer's worth of decoded sound data
			bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
			
			if (bytes < 0) {
				ov_clear(&oggFile);
				std::cerr << "OggAudioInstance: Error decoding " << fileName
				          << "..." << std::endl;
				return false;
			}
			// Append to end of buffer
			mOggDataBuffer.insert(mOggDataBuffer.end(), array, array + bytes);
		} while (bytes > 0);
		
		// Clean up!
		ov_clear(&oggFile);
		return true;
	}
	
} // End of Ogg
} // End of Audio
} // End of Instance
} // End of Asset
} // End of Dream
