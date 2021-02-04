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

#include "WavLoader.h"

#include "Common/Logger.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Components/Storage/File.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"



namespace octronic::dream
{
    WavLoader::WavLoader
    ()
        : AudioLoader()
    {
        LOG_TRACE("WavLoader: {}",__FUNCTION__);
    }

    WavLoader::~WavLoader()
    {
        LOG_TRACE("WavLoader: {}",__FUNCTION__);
    }

    bool
    WavLoader::loadIntoBuffer
    (AudioDefinition* audioDefinition, ProjectRuntime* projectRuntime)
    {
        LOG_TRACE("WavLoader: {}",__FUNCTION__);
        Project* project = projectRuntime->getProject();
        ProjectDirectory* pDir = project->getDirectory();
        string absPath = pDir->getAssetAbsolutePath(audioDefinition);

        LOG_DEBUG("WavLoader: Loading wav file from {}", absPath);

        int headerSize = sizeof(mWavHeader), filelength = 0;
        StorageManager* sm = projectRuntime->getStorageManager();
        File* wavFile = sm->openFile(absPath);

        if (wavFile == nullptr)
        {
            LOG_ERROR("WavLoader: Unable to open wave file: {}", absPath);
            sm->closeFile(wavFile);
            return false;
        }

        if (!wavFile->exists())
        {

            LOG_ERROR("WavLoader: Wav file does not exist: {}", absPath);
            sm->closeFile(wavFile);
            return false;
        }

        //Read the header
        if (!wavFile->readBinary())
        {
            sm->closeFile(wavFile);
            wavFile = nullptr;
            return false;
        }

        // Read in the headerr
        //size_t bytesRead = fread(&mWavHeader, 1, headerSize, wavFile);
        memcpy(&mWavHeader, wavFile->getBinaryData(), headerSize);
        size_t bytesRead = headerSize;
        LOG_DEBUG("WavLoader: Header Read {} bytes" ,bytesRead);
        mAudioBuffer = new uint8_t[mWavHeader.Subchunk2Size];
        mAudioBufferSize = mWavHeader.Subchunk2Size;

        LOG_DEBUG("WavLoader: Reserved Subchunk2Size {} bytes" ,mWavHeader.Subchunk2Size);

        // Read the data

        // OOB check
        assert(bytesRead + mWavHeader.Subchunk2Size <= wavFile->getBinaryDataSize());

        memcpy(mAudioBuffer, &wavFile->getBinaryData()[bytesRead], mWavHeader.Subchunk2Size);
        LOG_DEBUG("WavLoader: Read subchunk2 from index [{}], size {} bytes", bytesRead, mWavHeader.Subchunk2Size);
        bytesRead = mWavHeader.Subchunk2Size;

        mSampleRate = mWavHeader.SamplesPerSecond;

        if (mWavHeader.NumOfChannels == 1)
        {
            LOG_DEBUG("WavLoader: 1 Channel");
            mChannels = 1;
        }
        else if (mWavHeader.NumOfChannels == 2)
        {
            LOG_DEBUG("WavLoader: 2 Channels");
            mChannels = 2;
        }
        else
        {
            LOG_DEBUG("WavLoader: Error in number of channels");
            assert(false);
        }

        LOG_DEBUG("WavLoader: Read {} bytes", mAudioBufferSize);

        filelength = wavFile->getBinaryDataSize();

        LOG_DEBUG(
            "Status...\n"
            "\tFile size is: {} bytes\n"
            "\tRIFF header: {} {} {} {}\n"
            "\tWAVE header: {} {} {} {}\n"
            "\tFMT: {} {} {} {}\n"
            "\tData size: {}\n"
            "\tSampling Rate: {}\n"
            "\tBits used: {}\n"
            "\tChannels: {}\n"
            "\tBytes per second: {}\n"
            "\tData length: {}\n"
            "\tAudio Format: {}\n"
            "\tBlock align: {}\n"
            "\tData string: {} {} {} {}\n",
              filelength,
              mWavHeader.RIFF[0], mWavHeader.RIFF[1], mWavHeader.RIFF[2], mWavHeader.RIFF[3],
              mWavHeader.WAVE[0], mWavHeader.WAVE[1], mWavHeader.WAVE[2], mWavHeader.WAVE[3],
              mWavHeader.fmt[0], mWavHeader.fmt[1], mWavHeader.fmt[2], mWavHeader.fmt[3] ,
              mWavHeader.ChunkSize,
              mWavHeader.SamplesPerSecond,
              mWavHeader.BitsPerSample,
              mWavHeader.NumOfChannels,
              mWavHeader.BytesPerSecond,
              mWavHeader.Subchunk2Size,
              mWavHeader.AudioFormat,
              mWavHeader.BlockAlign,
              mWavHeader.Subchunk2ID[0],
              mWavHeader.Subchunk2ID[1],
              mWavHeader.Subchunk2ID[2],
              mWavHeader.Subchunk2ID[3]
        );

        // Close File
        sm->closeFile(wavFile);
        wavFile = nullptr;
        return checkLoaded();
    }
}
