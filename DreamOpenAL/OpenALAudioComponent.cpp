#include "OpenALAudioComponent.h"
#include "OpenALImplementation.h"

#include <iostream>

using octronic::dream::OggLoader;
using octronic::dream::WavLoader;
using std::make_shared;

namespace octronic::dream::open_al
{
  OpenALAudioComponent::OpenALAudioComponent
  ()
    : AudioComponent(),
      mDevice(nullptr),
      mContext(nullptr)
  {
    LOG_TRACE("AudioComponent: Constructing");
  }

  OpenALAudioComponent::~OpenALAudioComponent
  ()
  {
    LOG_TRACE("AudioComponent: Destructing");
    alcMakeContextCurrent(nullptr);

    if (mContext != nullptr)
    {
      alcDestroyContext(mContext);
      mContext = nullptr;
    }

    if (mDevice != nullptr)
    {
      alcCloseDevice(mDevice);
      mDevice = nullptr;
    }
  }

  bool
  OpenALAudioComponent::init
  ()
  {
    LOG_DEBUG("AudioComponent: Initialising...");
    mDevice = alcOpenDevice(nullptr);
    mContext = alcCreateContext(mDevice, nullptr);
    alcMakeContextCurrent(mContext);
    vec3 position(0.0f);
    setListenerPosition(position);
    return true;
  }

  void
  OpenALAudioComponent::setListenerPosition
  (const vec3& pos)
  {
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
  }

  void
  OpenALAudioComponent::setVolume
  (float volume)
  {
    alListenerf(AL_GAIN,volume);
  }

  float
  OpenALAudioComponent::getVolume
  ()
  {
    float vol;
    alGetListenerf(AL_GAIN,&vol);
    return vol;
  }

  AudioRuntime&
  OpenALAudioComponent::getAudioRuntime
  (AudioDefinition& def)
  {
    auto& audioCache = getProjectRuntime().getAudioCache();
    auto& aRunt = audioCache.getRuntime(def);

    if (!aRunt.getLoaded())
    {
      shared_ptr<AudioLoader> loader;

      if (def.getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
      {
        loader = make_shared<OggLoader>();
      }
      else if (def.getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
      {
        loader = make_shared<WavLoader>();
      }
      else
      {
        throw std::exception();
      }

      auto aImpl = make_shared<OpenALImplementation>(aRunt);
      aRunt.setImpl(aImpl);
      aRunt.setAudioLoader(loader);
    }
    return aRunt;
  }
}
