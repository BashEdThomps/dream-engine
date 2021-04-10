#include "WavLoader.h"

#include "Common/Logger.h"
#include "Components/Audio/AudioDefinition.h"
#include "Storage/StorageManager.h"
#include "Project/ProjectDirectory.h"
#include "Storage/File.h"

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
  (ProjectRuntime& projectRuntime, AudioDefinition& audioDefinition)
  {
    LOG_TRACE("WavLoader: {}",__FUNCTION__);
    auto& pDir = projectRuntime.getProjectDirectory();
    string absPath = pDir.getAssetAbsolutePath(audioDefinition);

    LOG_DEBUG("WavLoader: Loading wav file from {}", absPath);

    int headerSize = sizeof(mWavHeader), filelength = 0;
    auto& storageManager = projectRuntime.getStorageManager();
    auto& wavFile = storageManager.openFile(absPath);

    if (!wavFile.exists())
    {

      LOG_ERROR("WavLoader: Wav file does not exist: {}", absPath);
      storageManager.closeFile(wavFile);
      return false;
    }

    //Read the header
    if (!wavFile.readBinary())
    {
      storageManager.closeFile(wavFile);
      return false;
    }

    // Read in the headerr
    //size_t bytesRead = fread(&mWavHeader, 1, headerSize, wavFile);
    memcpy(&mWavHeader, &wavFile.getBinaryData()[0], headerSize);
    size_t bytesRead = headerSize;
    LOG_DEBUG("WavLoader: Header Read {} bytes" ,bytesRead);
    LOG_DEBUG("WavLoader: Reserved Subchunk2Size {} bytes" ,mWavHeader.Subchunk2Size);

    // Read the data

    // OOB check
    if(bytesRead + mWavHeader.Subchunk2Size != wavFile.getBinaryData().size())
    {
      LOG_ERROR("WavLoader: Failed bounds check");
     return false;
    }

    auto& wvData = wavFile.getBinaryData();
    auto wvBegin = wvData.begin() + bytesRead;
    mAudioBuffer.insert(mAudioBuffer.begin(), wvBegin, wvBegin + mWavHeader.Subchunk2Size);
    LOG_DEBUG("WavLoader: Read subchunk2 from index [{}], size {} bytes", bytesRead, mWavHeader.Subchunk2Size);

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
      return false;
    }

    LOG_DEBUG("WavLoader: Read {} bytes", mAudioBuffer.size());

    filelength = wavFile.getBinaryData().size();

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
    storageManager.closeFile(wavFile);
    return checkLoaded();
  }
}
