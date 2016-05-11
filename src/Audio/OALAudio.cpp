/*
* OALAudio
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

#include "OALAudio.h"

#include <iostream>
namespace Dream {
	namespace Audio {
		OALAudio::OALAudio(void) : AudioInterface() {}
		OALAudio::~OALAudio(void) {}

		bool OALAudio::init() {
			mDevice = alcOpenDevice(NULL);
			mContext = alcCreateContext(mDevice,NULL);
			alcMakeContextCurrent(mContext);
			return false;
		}

		ALuint OALAudio::generateBuffer() {
			alGetError();
			ALuint buffer;
			alGenBuffers( 1, &buffer );
			ALenum error = alGetError();
			if ( error != AL_NO_ERROR ){
				return -1;
			}
			return buffer;
		}

		void OALAudio::update() {}

	} // End of Audio
} // End of Dream
