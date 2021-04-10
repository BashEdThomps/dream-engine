#include "AudioComponent.h"

#include "Common/Logger.h"
#include "Components/Cache.h"
#include "AudioDefinition.h"
#include "AudioRuntime.h"
#include "Project/ProjectRuntime.h"

#include <iostream>

namespace octronic::dream
{
  AudioComponent::AudioComponent
  () : Component()
  {
    LOG_TRACE("AudioComponent: Constructing");
  }

  AudioComponent::~AudioComponent
  ()
  {
    LOG_TRACE("AudioComponent: Destructing");
  }

  void AudioComponent::pushTasks()
  {
    if (mProjectRuntime)
    {
      ProjectRuntime& pr = mProjectRuntime.value().get();
			AudioCache& audioCache = pr.getAudioCache();
			for (auto& audioRuntime : audioCache.getRuntimeVector())
			{
			  audioRuntime.get().pushTasks();
			}
    }
  }
}
