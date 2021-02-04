/*
 * AudioDefinition.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "AudioDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"


using std::lock_guard;

namespace octronic::dream
{
    AudioDefinition::AudioDefinition
    (ProjectDefinition* pd, const json &js)
        : AssetDefinition("AudioDefinition",pd,js)
    {
        LOG_TRACE("AudioDefinition: Constructing");
    }

    AudioDefinition::~AudioDefinition()
    {
        LOG_TRACE("AudioDefinition: Destructing");
    }

    bool
    AudioDefinition::isFormatWav
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return getFormat() == Constants::ASSET_FORMAT_AUDIO_WAV;
        } dreamElseLockFailed
    }

    bool
    AudioDefinition::isFormatOgg
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return getFormat() == Constants::ASSET_FORMAT_AUDIO_OGG;
        } dreamElseLockFailed
    }

    void
    AudioDefinition::setLoop
    (bool loop)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_LOOP] = loop;
        } dreamElseLockFailed
    }

    bool
    AudioDefinition::getLoop
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_LOOP) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_LOOP] = false;
            }
            return mJson[Constants::ASSET_ATTR_LOOP];
        } dreamElseLockFailed
    }

    bool
    AudioDefinition::getSpectrumAnalyser
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_SPECTRUM_ANALYSER) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER] = false;
            }
            return mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER];
        } dreamElseLockFailed
    }

    void
    AudioDefinition::setSpectrumAnalyser
    (bool sa)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER] = sa;
        } dreamElseLockFailed
    }

    string
    AudioDefinition::getMarkerName
    (unsigned int index)
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_NAME];
        } dreamElseLockFailed
    }

    void
    AudioDefinition::setMarkerName
    (unsigned int index, string name)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_NAME] = name;
        } dreamElseLockFailed
    }

    int
    AudioDefinition::getMarkerSampleIndex
    (unsigned int index)
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_S_INDEX];
        } dreamElseLockFailed
    }

    void
    AudioDefinition::setMarkerSampleIndex
    (unsigned int index, int smpl)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_S_INDEX] = smpl;
        } dreamElseLockFailed
    }

    int AudioDefinition::getMarkerRepeat(unsigned int index)
    {
        if(dreamTryLock())
        {
            dreamLock();
            json& obj = mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index];
            if (obj.find(Constants::ASSET_ATTR_AUDIO_EM_REPEAT) == obj.end())
                obj[Constants::ASSET_ATTR_AUDIO_EM_REPEAT] = 0;
            return obj[Constants::ASSET_ATTR_AUDIO_EM_REPEAT];
        } dreamElseLockFailed
    }

    void AudioDefinition::setMarkerRepeat(unsigned int index, int repeat)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT] = repeat;
        } dreamElseLockFailed
    }

    int AudioDefinition::getMarkerRepeatPeriod(unsigned int index)
    {
        if(dreamTryLock())
        {
            dreamLock();
            json& obj = mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index];
            if (obj.find(Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD) == obj.end())
                obj[Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD] = 0;
            return obj[Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD];
        } dreamElseLockFailed
    }

    void AudioDefinition::setMarkerRepeatPeriod(unsigned int index, int rp)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD] = rp;
        } dreamElseLockFailed
    }

    int
    AudioDefinition::createMarker
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            json marker = json::object();
            marker[Constants::ASSET_ATTR_AUDIO_EM_INDEX] = countMarkers();
            marker[Constants::ASSET_ATTR_AUDIO_EM_S_INDEX] = "-1";
            marker[Constants::ASSET_ATTR_AUDIO_EM_NAME] = "New Marker";
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].push_back(marker);
            return marker[Constants::ASSET_ATTR_AUDIO_EM_INDEX];
        } dreamElseLockFailed
    }

    void
    AudioDefinition::removeMarker
    (const int index)
    {
        if(dreamTryLock())
        {
            dreamLock();
            auto itr = std::find_if(
                        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].begin(),
                    mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].end(),
                    [&index](json next)
            {
                int idx = next[Constants::ASSET_ATTR_AUDIO_EM_INDEX];
                return idx == index;
            }
            );

            if (itr !=  mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].end())
            {
                mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].erase(itr);
            }
        } dreamElseLockFailed
    }

    int
    AudioDefinition::countMarkers
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return static_cast<int>(mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].size());
        } dreamElseLockFailed
    }
}
