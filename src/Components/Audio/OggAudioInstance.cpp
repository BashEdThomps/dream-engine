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

namespace Dream {
  namespace Components {
    namespace Audio     {

      OggAudioInstance::OggAudioInstance(
          AssetDefinition* definition,Transform3D* transform
      ) : AudioAssetInstance(definition,transform) {}

      OggAudioInstance::~OggAudioInstance() {}

      bool OggAudioInstance::load(std::string projectPath) {
        std::string absPath = projectPath+mDefinition->getAssetPath();
        std::cout << "OggAudioInstance: Loading Instance: " << absPath << std::endl;

        // 0 for Little-Endian, 1 for Big-Endian
        int endian = 0;
        int bitStream;
        long bytes;

        // Local fixed size array
        char buffer[BUFFER_SIZE];
        FILE *file = fopen(absPath.c_str(), "rb");
        if (file == NULL) {
          std::cerr << "OggAudioInstance:: Cannot open " << absPath
                    << " for reading..." << std::endl;
          return false;
        }

        // Try opening the given file
        OggVorbis_File oggFile;
        if (ov_open(file, &oggFile, NULL, 0) != 0) {
          std::cerr << "OggAudioInstance: Error opening " << absPath
                    << " for decoding..." << std::endl;
          return false;
        }

        // Get some information about the OGG file
        vorbis_info *oggInfo;
        oggInfo = ov_info(&oggFile, -1);

        // Check the number of channels... always use 16-bit samples
        if (oggInfo->channels == 1) {
          mFormat = AL_FORMAT_MONO16;
        } else {
          mFormat = AL_FORMAT_STEREO16;
        }

        // The frequency of the sampling rate
        mFrequency = oggInfo->rate;

        // Keep reading until all is read
        do {
          // Read up to a buffer's worth of decoded sound data
          bytes = ov_read(&oggFile, buffer, BUFFER_SIZE, endian, 2, 1, &bitStream);

          if (bytes < 0) {
            ov_clear(&oggFile);
            std::cerr << "OggAudioInstance: Error decoding " << absPath << std::endl;
            return false;
          }
          // Append to end of buffer
          mAudioDataBuffer.insert(mAudioDataBuffer.end(), buffer, buffer + bytes);
        } while (bytes > 0);

        // Clean up!
        ov_clear(&oggFile);
        return true;
      }

      void OggAudioInstance::loadExtraAttributes(nlohmann::json json) {}

    } // End of Audio
  } // End of Components
} // End of Dream
