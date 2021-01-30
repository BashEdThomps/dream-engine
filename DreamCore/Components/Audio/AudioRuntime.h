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

#include <deque>

using std::deque;
using glm::vec3;


namespace octronic::dream
{
    class AudioDefinition;
    class AudioComponent;

    /**
     * @brief AudioRuntime data for an OpenAL based Audio Clip.
     */
    class AudioRuntime : public SharedAssetRuntime
    {
    public:

        AudioRuntime(AudioLoader* loader, AudioDefinition* def, ProjectRuntime* project);
        virtual ~AudioRuntime() override;

        bool isLooping() const ;
        long long getStartTime() const;
        void setStartTime(long long startTime);
        void updateMarkers();
        AudioMarkersUpdateTask* getMarkersUpdateTask();
        AudioLoader* getAudioLoader() const;

        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;
        virtual void setLooping(bool);
        virtual void setSourcePosision(vec3 pos) =0;
        virtual void setVolume(float volume) = 0;
        virtual AudioStatus getState() = 0;
        virtual unsigned int getSampleOffset() const = 0;
        virtual void setSampleOffset(unsigned int offset) = 0;
        virtual int getDurationInSamples() = 0;

    protected:
        void generateEventList();

    protected:
        bool mLooping;
        long long mStartTime;
        int mLastSampleOffset;
        deque<Event> mMarkerEvents;
        deque<Event> mMarkerEventsCache;
        AudioMarkersUpdateTask mMarkersUpdateTask;
        AudioLoader* mLoader;
    };
}
