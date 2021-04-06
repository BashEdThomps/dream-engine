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

#include "AudioRuntime.h"

#include "Common/Logger.h"

#include "Components/Cache.h"
#include "AudioTasks.h"
#include "AudioDefinition.h"
#include "AudioComponent.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
  AudioRuntime::AudioRuntime
  (ProjectRuntime& project, AudioDefinition& def)
    : SharedAssetRuntime(project, def),
      mImpl(nullptr),
      mLoader(nullptr),
      mLooping(false)
  {
    LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
  }

  void
  AudioRuntime::setLooping
  (bool looping)
  {
    LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
    mLooping = looping;
  }

  bool
  AudioRuntime::getLooping
  ()
  const
  {
    return mLooping;
  }


  bool AudioRuntime::loadFromDefinition()
  {
    return mImpl->loadFromDefinition();
  }

  shared_ptr<AudioLoader>
  AudioRuntime::getAudioLoader
  ()
  const
  {
    return mLoader;
  }

  void
  AudioRuntime::setAudioLoader
  (const shared_ptr<AudioLoader>& loader)
  {
    mLoader = loader;
  }

	shared_ptr<AudioRuntimeImplementation>
  AudioRuntime::getImpl
  ()
  const
  {
    return mImpl;
  }

  void
  AudioRuntime::setImpl
  (const shared_ptr<AudioRuntimeImplementation>& impl)
  {
    mImpl = impl;
  }

  void AudioRuntime::pushTasks()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();

    if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
    {
      taskQueue.pushTask(mLoadFromDefinitionTask);
    }
    else
    {
      if (!mLoadError)
      {
        for(auto& entity : mInstances)
        {
          auto& sr = entity.get().getSceneRuntime();
          if (sr.hasState(SCENE_STATE_ACTIVE))
          {
            // Do entity specific tasks
          }
        }
      }
    }
  }

  // Implementation ============================================================

  void AudioRuntime::play()
  {
    mImpl->play();
  }

  void AudioRuntime::pause()
  {
    mImpl->pause();
  }

  void AudioRuntime::stop()
  {
    mImpl->stop();
  }

  vec3 AudioRuntime::getSourcePosition() const
  {
    return mImpl->getSourcePosition();
  }

  void AudioRuntime::setSourcePosision(vec3 pos)
  {
    mImpl->setSourcePosition(pos);
  }

  float AudioRuntime::getVolume() const
  {
    return mImpl->getVolume();
  }

  void AudioRuntime::setVolume(float volume)
  {
    mImpl->setVolume(volume);
  }

  AudioStatus AudioRuntime::getState()
  {
    return mImpl->getState();
  }

  unsigned long AudioRuntime::getSampleOffset() const
  {
    return mImpl->getSampleOffset();
  }

  void AudioRuntime::setSampleOffset(unsigned long offset)
  {
    mImpl->setSampleOffset(offset);
  }

  unsigned long AudioRuntime::getDurationInSamples()
  {
    return mImpl->getDurationInSamples();
  }
}
