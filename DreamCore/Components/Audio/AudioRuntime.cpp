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

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
    AudioRuntime::AudioRuntime
    (const weak_ptr<ProjectRuntime>& project,
     const weak_ptr<AudioDefinition>& def)
        : SharedAssetRuntime(project, def),
          mLooping(false),
          mStartTime(0),
          mLastSampleOffset(0),
          mImpl(nullptr)
    {
        LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
        generateEventList();
    }

    AudioRuntime::~AudioRuntime
    ()
    {
        LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
    }

    bool
    AudioRuntime::init
    ()
    {
        if(!SharedAssetRuntime::init()) return false;
        mMarkersUpdateTask =
                make_shared<AudioMarkersUpdateTask>(mProjectRuntime,
                static_pointer_cast<AudioRuntime>(shared_from_this()));
        return true;
    }

    void
    AudioRuntime::setLooping
    (bool looping)
    {
        LOG_DEBUG("AudioRuntime: {}", __FUNCTION__);
        mLooping = looping;
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
        // --- Iterate through the instances vector...

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
        mStartTime = startTime;
    }

    weak_ptr<AudioMarkersUpdateTask>
    AudioRuntime::getMarkersUpdateTask
    ()
    const
    {
        return mMarkersUpdateTask;
    }

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

    void AudioRuntime::setSourcePosision(vec3 pos)
    {
        mImpl->setSourcePosision(pos);
    }

    void AudioRuntime::setVolume(float volume)
    {
        mImpl->setVolume(volume);
    }

    AudioStatus AudioRuntime::getState()
    {
        return mImpl->getState();
    }

    unsigned int AudioRuntime::getSampleOffset() const
    {
        return mImpl->getSampleOffset();
    }

    void AudioRuntime::setSampleOffset(unsigned int offset)
    {
        mImpl->setSampleOffset(offset);
    }

    int AudioRuntime::getDurationInSamples()
    {
        return mImpl->getDurationInSamples();
    }


    void AudioRuntime::setImplementation
    (const shared_ptr<AudioRuntimeImplementation>& impl)
    {
        mImpl = impl;
        mImpl->setParent(static_pointer_cast<AudioRuntime>(shared_from_this()));
    }

    bool
    AudioRuntime::loadFromDefinition
    ()
    {
        return mImpl->loadFromDefinition(mProjectRuntime,static_pointer_cast<AudioDefinition>(mDefinition.lock()));
    }

    void AudioRuntime::pushTasks()
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            if(auto taskQueue = prLock->getTaskQueue().lock())
            {
                if (!mLoaded && !mLoadError &&
                    mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
                {
                    taskQueue->pushTask(static_pointer_cast<Task>(mLoadFromDefinitionTask));
                }
            }
            else
            {
                if (!mLoadError)
                {
                    for(auto& entity : mInstances)
                    {
                        if (auto entityLock = entity.lock())
                        {
                            if (auto srLock = entityLock->getSceneRuntime().lock())
                            {
                                if (srLock->hasState(SCENE_STATE_ACTIVE))
                                {
                            		// Do entity specific tasks
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
