/*
 * WavAudioInstance
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

#pragma once

#include <string>
#include <fstream>

#pragma once

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif


#include "../AudioInstance.h"
#include "WavHeader.h"

using std::string;
using nlohmann::json;

namespace Dream
{
    class WavAudioInstance : public AudioInstance
    {

    private:
        WavHeader mWavHeader;

    public:
        WavAudioInstance(weak_ptr<AudioComponent>, shared_ptr<AudioDefinition>, shared_ptr<SceneObjectRuntime>);
        ~WavAudioInstance() override;

        long getFileSize(FILE* inFile);

        bool load(string) override;
        void loadExtraAttributes(json) override;

    }; // End WavAudioInstance

} // End of Dream
