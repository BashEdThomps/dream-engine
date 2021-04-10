#pragma once

#include <fstream>

#include "Components/Audio/AudioLoader.h"
#include "WavHeader.h"

namespace octronic::dream
{
  class WavLoader : public AudioLoader
  {
  public:
    WavLoader();
    ~WavLoader();
    bool loadIntoBuffer(ProjectRuntime& project, AudioDefinition& definition) override;
  private:
    WavHeader mWavHeader;
  };
}
