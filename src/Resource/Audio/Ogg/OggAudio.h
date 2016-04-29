/*
* Dream::Resource::Audio::Ogg::OggAudio
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

#include "../Audio.h"

#define RESOURCE_FORMAT_OGG "ogg"

namespace Dream {
	namespace Resource {
		namespace Audio {
			namespace Ogg {
				class OggAudio : public Dream::Resource::Audio::Audio {
				private:
					std::string mOggPath;
				public:
					OggAudio(void);
					OggAudio(nlohmann::json);
					~OggAudio(void);
					void generateAbsolutePaths(std::string,std::string);
					std::string getOggPath();
				}; // End of OggAudio
			} // End of Ogg
		} // End of Audio
	} // End of Resource
} // End of Dream

#endif // End of OGGAUDIO_H
