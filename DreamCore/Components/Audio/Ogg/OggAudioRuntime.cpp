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

#include "OggAudioRuntime.h"

#include <Common/Logger.h>
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/File.h"
#include "Project/ProjectRuntime.h"
#include "Components/Audio/AudioDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

#include <vorbis/vorbisfile.h>

#include <fstream>

#define AUDIO_BUFFER_SIZE 1024

namespace octronic::dream
{
    size_t
    octronic_dream_ogg_read
    (void* buffer, size_t elementSize, size_t elementCount, void* dataSource)
    {
        assert(elementSize == 1);

        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        stream.read(static_cast<char*>(buffer), elementCount);
        const std::streamsize bytesRead = stream.gcount();
        stream.clear(); // In case we read past EOF
        return static_cast<size_t>(bytesRead);
    }

    int
    octronic_dream_ogg_seek
    (void* dataSource, ogg_int64_t offset, int origin)
    {
        static const std::vector<std::ios_base::seekdir> seekDirections
        {
            std::ios_base::beg, std::ios_base::cur, std::ios_base::end
        };

        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        stream.seekg(offset, seekDirections.at(origin));
        stream.clear(); // In case we seeked to EOF
        return 0;
    }

    long
    octronic_dream_ogg_tell
    (void* dataSource)
    {
        std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
        const auto position = stream.tellg();
        assert(position >= 0);
        return static_cast<long>(position);
    }


    OggAudioRuntime::OggAudioRuntime
    (AudioDefinition* def, ProjectRuntime* project)
        : AudioRuntime(def,project)
    {
    }

    bool
    OggAudioRuntime::useDefinition
    ()
    {
        auto absPath = getAssetFilePath();
        LOG_DEBUG("OggAudioRuntime: Loading Runtime: {}", absPath);

        // 0 for Little-Endian, 1 for Big-Endian
        int endian = 0;
        int bitStream;
        long bytes;

        // Local fixed size array
        char buffer[AUDIO_BUFFER_SIZE];
        StorageManager* fm = mProjectRuntime->getStorageManager();
        File *file = fm->openFile(absPath);
        if (file == nullptr)
        {
            LOG_ERROR("OggAudioRuntime: Cannot open {} for reading", absPath);
            return false;
        }

        if (!file->readBinary())
        {
            LOG_ERROR("OggAudioRuntime: Error reading binary data");
            fm->closeFile(file);
            file = nullptr;
            return false;
        }

        std::fstream sbuf;
        sbuf.rdbuf()->pubsetbuf((char*)file->getBinaryData(), file->getBinaryDataSize());

        ov_callbacks callbacks;
        memset(&callbacks,0,sizeof(ov_callbacks));
        callbacks.read_func  = octronic_dream_ogg_read;
        callbacks.seek_func  = octronic_dream_ogg_seek;
        callbacks.close_func = nullptr;
        callbacks.tell_func  = octronic_dream_ogg_tell;

        // Try opening the given file
        OggVorbis_File oggFile;

        //if (ov_open(file, &oggFile, nullptr, 0) != 0)
        if (ov_open_callbacks(&sbuf, &oggFile, nullptr, 0, callbacks) != 0)
        {
            LOG_ERROR("OggAudioRuntime: Error opening {} for decoding");
            return false;
        }

        // Get some information about the OGG file
        vorbis_info *oggInfo;
        oggInfo = ov_info(&oggFile, -1);

        // Check the number of channels... always use 16-bit samples
        if (oggInfo->channels == 1)
        {
            mChannels = 1;
            mFormat = AL_FORMAT_MONO16;
        }
        else
        {
            mChannels = 2;
            mFormat = AL_FORMAT_STEREO16;
        }

        setLooping(static_cast<AudioDefinition*>(mDefinition)->getLoop());

        // The frequency of the sampling rate
        mFrequency = oggInfo->rate;

        // Keep reading until all is read
        do
        {
            // Read up to a buffer's worth of decoded sound data
            bytes = ov_read(&oggFile, &buffer[0], AUDIO_BUFFER_SIZE, endian, 2, 1, &bitStream);

            if (bytes < 0)
            {
                ov_clear(&oggFile);
                LOG_ERROR("OggAudioRuntime: Error decoding {}", absPath);
                return false;
            }
            // Append to end of buffer
            mAudioDataBuffer.insert(mAudioDataBuffer.end(), &buffer[0], &buffer[0] + bytes);
        }
        while (bytes > 0);

        // Clean up!
        ov_clear(&oggFile);

        return loadIntoAL();
    }

} // End of Dream


