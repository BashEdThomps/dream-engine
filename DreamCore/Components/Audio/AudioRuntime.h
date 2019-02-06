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

#include <vector>
#include <deque>
#include "AudioStatus.h"
#include "AudioTasks.h"
#include "../SharedAssetRuntime.h"
#include "../Event.h"

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <al.h>
    #include <alc.h>
#endif

#include <glm/glm.hpp>

using namespace std;

namespace Dream
{
    class AudioDefinition;
    class AudioComponent;
    /**
     * @brief AudioRuntime data for an OpenAL based Audio Clip.
     */
    class AudioRuntime : public SharedAssetRuntime
    {
    protected:
        bool mLooping;
        ALenum mFormat;
        ALsizei mFrequency;
        vector<char> mAudioDataBuffer;
        ALuint mSource;
        ALuint mBuffer;
        long long mStartTime;
        int mLastSampleOffset;
        int mChannels;
        deque<Event> mMarkerEvents;
        deque<Event> mMarkerEventsCache;
        ALint mDurationInSamples;

        void generateEventList();
        ALuint generateSource();
        ALuint generateBuffer();
        bool loadIntoAL();
        AudioMarkersUpdateTask mMarkersUpdateTask;

    public:

        AudioRuntime(AudioDefinition* def, ProjectRuntime* project);
        ~AudioRuntime() override;

        void setLooping(bool);
        bool isLooping() const ;

        vector<ALchar> getAudioDataBuffer()const ;

        ALsizei getFrequency()const ;
        ALenum  getFormat()const ;

        void setBuffer(ALuint buffer);
        ALuint getBuffer()const ;

        void setSource(ALuint source);
        ALuint getSource() const;

        void play();
        void pause();
        void stop();

        void setSourcePosision(glm::vec3 pos);
        long long getStartTime() const;
        void setStartTime(long long startTime);

        void setVolume(float volume);

        int getChannels() const;

        void updateMarkers();
        AudioStatus getState();

        ALint getSampleOffset() const;
        void setSampleOffset(ALint offset);

        vector<char> getAudioBuffer(size_t offset, size_t length) const;
        int getDurationInSamples();
        void clearMarkersUpdateTask();
        void setMarkersUpdateTask(AudioMarkersUpdateTask* t);
        bool hasMarkersUpdateTask();
    };
}
