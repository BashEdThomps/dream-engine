#pragma once

#include "ALHeader.h"
#include <DreamCore.h>

#include <iostream>
#include <vector>
#include <memory>


using octronic::dream::Vector3;
using octronic::dream::AudioComponent;
using octronic::dream::AudioDefinition;
using octronic::dream::AudioRuntime;

namespace octronic::dream::open_al
{
  class OpenALAudioComponent : public AudioComponent
  {

  public:
    OpenALAudioComponent();
    ~OpenALAudioComponent() override;
    bool init() override;
    void setListenerPosition(const vec3&) override;
    void setVolume(float) override;
    float getVolume() override;
    AudioRuntime& getAudioRuntime(AudioDefinition& def) override;

  private:
    ALCdevice*  mDevice;
    ALCcontext* mContext;
  };
}
