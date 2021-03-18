/*
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

#pragma once

#include "AudioLoader.h"
#include <string>

using std::string;

namespace octronic::dream
{
    class OggLoader : public AudioLoader
    {
    public:
		static const uint16_t OGG_LOAD_BUFFER_SIZE;
        OggLoader();
	    ~OggLoader();
		string getOggErrorString(int error);

		bool loadIntoBuffer
        (const weak_ptr<AudioDefinition>& def,
         const weak_ptr<ProjectRuntime>& project) override;
	};
}
