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

#include "OggAudio.h"

namespace Dream {
	namespace Resource {
		namespace Audio {
			namespace Ogg {

				OggAudio::OggAudio(void) : Dream::Resource::Audio::Audio () {}

				OggAudio::OggAudio(nlohmann::json jsonOgg) : Dream::Resource::Audio::Audio (jsonOgg) {}

				OggAudio::~OggAudio(void) {}

				void OggAudio::generateAbsolutePaths(std::string projectDir, std::string projectUUID) {
					mOggPath = projectDir         + PATH_SEP +
					           RESOURCE_DIR       + PATH_SEP +
										 RESOURCE_DIR_AUDIO + PATH_SEP +
										 getUUID()          + PATH_SEP +
										 RESOURCE_FORMAT_OGG;
					std::cout << "Generated path for ogg resource: " << mOggPath << std::endl;
				}

				std::string OggAudio::getOggPath() {
					return mOggPath;
				}

			} // End of Ogg
		} // End of Audio
	} // End of Resource
} // End of Dream
