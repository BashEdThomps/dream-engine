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
#include "Components/SharedAssetRuntime.h"
#include "Components/Event.h"
#include "Math/Vector.h"
#include "AudioRuntimeImplementation.h"

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

        AudioRuntime(const weak_ptr<ProjectRuntime>& project,
                     const weak_ptr<AudioDefinition>& def);

        ~AudioRuntime() override;

        bool isLooping() const ;
        long long getStartTime() const;
        void setStartTime(long long startTime);
        void updateMarkers();
        weak_ptr<AudioMarkersUpdateTask> getMarkersUpdateTask() const;
        weak_ptr<AudioLoader> getAudioLoader() const;

        void play();
        void pause();
        void stop();
        bool init() override;
        void setLooping(bool);
        void setSourcePosision(vec3 pos);
        void setVolume(float volume);
        AudioStatus getState();
        unsigned int getSampleOffset() const;
        void setSampleOffset(unsigned int offset);
        int getDurationInSamples();
        void setImplementation(const shared_ptr<AudioRuntimeImplementation>& impl);
        bool loadFromDefinition() override;
        void pushTasks() override;

    protected:
        void generateEventList();

    protected:
        /**
         * @brief mImpl - We take ownership of the Impl
         */
        shared_ptr<AudioRuntimeImplementation> mImpl;
        bool mLooping;
        long long mStartTime;
        int mLastSampleOffset;
        deque<Event> mMarkerEvents;
        deque<Event> mMarkerEventsCache;
        shared_ptr<AudioMarkersUpdateTask> mMarkersUpdateTask;
    };
}
