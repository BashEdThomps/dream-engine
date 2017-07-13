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

#include "WavAudioInstance.h"
#include "../AudioDefinition.h"

namespace Dream
{

    WavAudioInstance::WavAudioInstance
    (AudioDefinition* definition, SceneObjectRuntime* transform)
        : AudioInstance(definition, transform)
    {
        return;
    }

    WavAudioInstance::~WavAudioInstance
    ()
    {
        return;
    }

    bool
    WavAudioInstance::load
    (string projectPath)
    {
        string absPath = projectPath + mDefinitionHandle->getAssetPath();
        cout << "WavAudioInstance: Loading wav file from " << absPath << endl;
        int headerSize = sizeof(mWavHeader), filelength = 0;
        FILE* wavFile = fopen(absPath.c_str(), "r");

        if (wavFile == NULL)
        {
            cerr << "WavAudioInstance:: Unable to open wave file: " << absPath << endl;
            return false;
        }

        //Read the header
        size_t bytesRead = fread(&mWavHeader, 1, headerSize, wavFile);
        cout << "WavAudioInstance: Header Read " << bytesRead << " bytes." << endl;
        if (bytesRead > 0)
        {
            //Read the data
            //uint16_t bytesPerSample = mWavHeader.BitsPerSample / 8;      // Number of bytes per sample
            //uint64_t numSamples = mWavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
            int8_t* buffer = new int8_t[Constants::AUDIO_BUFFER_SIZE];
            while ((bytesRead = fread(buffer, sizeof buffer[0], Constants::AUDIO_BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
            {
                mAudioDataBuffer.insert(mAudioDataBuffer.end(), buffer, buffer + bytesRead);
            }
            mFrequency = mWavHeader.SamplesPerSecond;
            if (mWavHeader.NumOfChannels == 1)
            {
                mFormat = AL_FORMAT_MONO16;
            }
            else
            {
                mFormat = AL_FORMAT_STEREO16;
            }
            cout << "WavAudioInstance: Read " << mAudioDataBuffer.size() << " bytes." << endl;
            delete [] buffer;
            buffer = NULL;
            filelength = getFileSize(wavFile);
            cout << "WavAudioInstance: Status..."    << endl;
            cout << "\tFile is          :" << filelength << " bytes." << endl;
            cout << "\tRIFF header      :"
                      << mWavHeader.RIFF[0] << mWavHeader.RIFF[1]
                      << mWavHeader.RIFF[2] << mWavHeader.RIFF[3]
                      << endl;
            cout << "\tWAVE header      :" << mWavHeader.WAVE[0]
                      << mWavHeader.WAVE[1]     << mWavHeader.WAVE[2]
                      << mWavHeader.WAVE[3]     << endl;
            cout << "\tFMT              :" << mWavHeader.fmt[0]
                      << mWavHeader.fmt[1]      << mWavHeader.fmt[2]
                      << mWavHeader.fmt[3]      << endl;
            cout << "\tData size        :" << mWavHeader.ChunkSize        << endl;
            cout << "\tSampling Rate    :" << mWavHeader.SamplesPerSecond << endl;
            cout << "\tBits used        :" << mWavHeader.BitsPerSample    << endl;
            cout << "\tChannels         :" << mWavHeader.NumOfChannels    << endl;
            cout << "\tBytes per second :" << mWavHeader.BytesPerSecond   << endl;
            cout << "\tData length      :" << mWavHeader.Subchunk2Size    << endl;
            cout << "\tAudio Format     :" << mWavHeader.AudioFormat      << endl;
            cout << "\tBlock align      :" << mWavHeader.BlockAlign       << endl;
            cout << "\tData string      :"
                      << mWavHeader.Subchunk2ID[0] << mWavHeader.Subchunk2ID[1]
                      << mWavHeader.Subchunk2ID[2] << mWavHeader.Subchunk2ID[3]
                      << endl;
        }
        fclose(wavFile);
        return true;
    }

    // find the file size
    long
    WavAudioInstance::getFileSize
    (FILE* inFile)
    {
        size_t fileSize = 0;
        fseek(inFile, 0, SEEK_END);
        fileSize = ftell(inFile);
        fseek(inFile, 0, SEEK_SET);
        return fileSize;
    }

    void
    WavAudioInstance::loadExtraAttributes
    (json json)
    {
        return;
    }

} // End of Dream
