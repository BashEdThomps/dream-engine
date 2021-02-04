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
#include "AudioDefinition.h"
#include "AudioComponent.h"

namespace octronic::dream
{
    AudioRuntime::AudioRuntime
    (AudioLoader* loader, AudioDefinition* def, ProjectRuntime* project)
        : SharedAssetRuntime("AudioRuntime",def,project),
          mLoader(loader),
          mLooping(false),
          mStartTime(0),
          mLastSampleOffset(0),
          mMarkersUpdateTask(this)
    {
        LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
        generateEventList();
    }

    AudioRuntime::~AudioRuntime()
    {
        LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
    }

    void
    AudioRuntime::setLooping
    (bool looping)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
            mLooping = looping;
        } dreamElseLockFailed
    }

    bool
    AudioRuntime::isLooping
    ()
    const
    {
        return mLooping;
    }

    void
    AudioRuntime::generateEventList
    ()
    {
        // TODO = Rethink, SharedAssetRuntime has no EntityRuntime to send
        // Events to :thinking:

        /*
        auto ad = static_cast<AudioDefinition*>(mDefinition);
        auto nMarkers = ad->countMarkers();

        if (nMarkers == 0)
        {
           return;
        }

        mMarkerEvents.clear();

        for (int markerIndex = 0; markerIndex< nMarkers; markerIndex++)
        {
            auto log = getLog();
            LOG_trace("AudioRuntime: Generating events for marker {}", markerIndex);
            auto markerStart = ad->getMarkerSampleIndex(markerIndex);
            auto count = ad->getMarkerRepeat(markerIndex);
            auto step = ad->getMarkerRepeatPeriod(markerIndex);
            auto markerName = ad->getMarkerName(markerIndex);

            auto next = markerStart;
            LOG_trace("AudioRuntime: Marker {}'s is : ", markerIndex, next);
            Event e(mEntityRuntime,"audio");
            e.setString("name",markerName);
            e.setNumber("time",markerStart);
            mMarkerEvents.push_back(e);

            for (int i=0; i<count; i++)
            {
                auto repeatIndex = i+1;
                auto next = markerStart + (repeatIndex*step);
                LOG_trace("AudioRuntime: Marker {}'s {}th step is : {}", markerIndex, repeatIndex, next);
                Event e(mEntityRuntime,"audio");
                e.setString("name",markerName);
                e.setNumber("time",next);
                mMarkerEvents.push_back(e);
            }
        }

        std::sort(
            mMarkerEvents.begin(),
            mMarkerEvents.end(),
            [](Event& first, Event& second)
            {
                return first.getNumber("time") < second.getNumber("time");
            }
        );

        mMarkerEventsCache = mMarkerEvents;

        if (mAudioComponent != nullptr)
        {
            mAudioComponent->pushToUpdateQueue(this);
        }
        */
    }

    void
    AudioRuntime::updateMarkers
    ()
    {
        /*
        auto currentSample = mAudioComponent->getSampleOffset(mSource);
        // has just looped, restore cached events
        if (mLooping && mLastSampleOffset > currentSample)
        {
            LOG_DEBUG("AudioRuntime: Just Looped");
           mMarkerEvents = mMarkerEventsCache;
        }

        for (auto it = mMarkerEvents.begin(); it != mMarkerEvents.end();)
        {
            auto time = (*it).getNumber("time");
            if (currentSample > time)
            {
                mEntityRuntime->addEvent((*it));
                it++;
                mMarkerEvents.pop_front();
            }
            else
            {
                break;
            }
        }
        mLastSampleOffset = currentSample;
        */
    }

    long
    long AudioRuntime::getStartTime
    ()
    const
    {
        return mStartTime;
    }

    void
    AudioRuntime::setStartTime
    (long long startTime)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mStartTime = startTime;
        } dreamElseLockFailed
    }

    AudioMarkersUpdateTask* AudioRuntime::getMarkersUpdateTask()
    {
        return &mMarkersUpdateTask;
    }


    AudioLoader* AudioRuntime::getAudioLoader() const
    {
        return mLoader;
    }
}
