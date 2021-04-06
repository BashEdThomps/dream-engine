#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;

namespace octronic::dream
{
  class AudioDefinition;
  class ProjectRuntime;
  class AudioLoader
  {
  public:
    AudioLoader();
    virtual ~AudioLoader();
    virtual bool loadIntoBuffer(ProjectRuntime& pDef, AudioDefinition& aDef) = 0;

    vector<uint8_t> getAudioBuffer() const;
    uint8_t getChannels() const;
    long getSampleRate() const;
  protected:
    bool checkLoaded() const;

  protected:
    vector<uint8_t> mAudioBuffer;
    uint8_t  mChannels;
    long     mSampleRate;
  };
}
