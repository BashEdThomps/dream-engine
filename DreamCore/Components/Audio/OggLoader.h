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

    bool loadIntoBuffer(ProjectRuntime& project, AudioDefinition& def) override;
  };
}
