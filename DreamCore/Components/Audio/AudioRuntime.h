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

#pragma once

#include "AudioStatus.h"
#include "AudioTasks.h"
#include "AudioLoader.h"
#include "AudioRuntimeImplementation.h"
#include "Components/SharedAssetRuntime.h"
#include "Components/Event.h"
#include "Math/Vector.h"

#include <deque>

using std::deque;

namespace octronic::dream
{
  class AudioDefinition;
  class AudioComponent;

  /**
     * @brief AudioRuntime holds data for an Audio Clip.
     */
  class AudioRuntime : public SharedAssetRuntime
  {
  public:
    AudioRuntime(ProjectRuntime& project, AudioDefinition& def);

    shared_ptr<AudioRuntimeImplementation> getImpl() const;
    void setImpl(const shared_ptr<AudioRuntimeImplementation>& impl);

    shared_ptr<AudioLoader> getAudioLoader() const;
    void setAudioLoader(const shared_ptr<AudioLoader>& loader);

    void pushTasks() override;

    void setLooping(bool);
    bool getLooping() const;

    // Implementation
    void play();
    void pause();
    void stop();
    vec3 getSourcePosition() const;
    void setSourcePosision(vec3 pos);
    float getVolume() const;
    void setVolume(float volume);
    AudioStatus getState();
    unsigned long getSampleOffset() const;
    void setSampleOffset(unsigned long offset);
    unsigned long getDurationInSamples();
    bool loadFromDefinition() override;

  protected:
    shared_ptr<AudioRuntimeImplementation> mImpl;
    shared_ptr<AudioLoader> mLoader;
    bool mLooping;
  };
}
