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

namespace octronic::dream
{
    AudioDefinition::AudioDefinition
    (ProjectDefinition* pd, const json &js)
        : AssetDefinition(pd,js)
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
        return getFormat() == Constants::ASSET_FORMAT_AUDIO_WAV;
    }

    bool
    AudioDefinition::isFormatOgg
    ()
    {
        return getFormat() == Constants::ASSET_FORMAT_AUDIO_OGG;
    }

    void
    AudioDefinition::setLoop
    (bool loop)
    {
        mJson[Constants::ASSET_ATTR_LOOP] = loop;
    }

    bool
    AudioDefinition::getLoop
    ()
    {
        if (mJson[Constants::ASSET_ATTR_LOOP].is_null())
        {
            mJson[Constants::ASSET_ATTR_LOOP] = false;
        }
        return mJson[Constants::ASSET_ATTR_LOOP];
    }

    bool
    AudioDefinition::getSpectrumAnalyser
    ()
    {
        if (mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER].is_null())
        {
            mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER] = false;
        }
        return mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER];
    }

    void
    AudioDefinition::setSpectrumAnalyser
    (bool sa)
    {
        mJson[Constants::ASSET_ATTR_SPECTRUM_ANALYSER] = sa;
    }

    string
    AudioDefinition::getMarkerName
    (unsigned int index)
    {
        return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_NAME];
    }

    void
    AudioDefinition::setMarkerName
    (unsigned int index, string name)
    {
        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_NAME] = name;
    }

    int
    AudioDefinition::getMarkerSampleIndex
    (unsigned int index)
    {
        return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_S_INDEX];
    }

    void
    AudioDefinition::setMarkerSampleIndex
    (unsigned int index, int smpl)
    {
        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_S_INDEX] = smpl;
    }

    int AudioDefinition::getMarkerRepeat(unsigned int index)
    {

        if (mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT].is_null())
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT] = 0;
        return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT];
    }

    void AudioDefinition::setMarkerRepeat(unsigned int index, int repeat)
    {

        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT] = repeat;
    }

    int AudioDefinition::getMarkerRepeatPeriod(unsigned int index)
    {

        if (mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD].is_null())
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD] = 0;
        return mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD];
    }

    void AudioDefinition::setMarkerRepeatPeriod(unsigned int index, int rp)
    {

        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS][index][Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD] = rp;
    }

    int
    AudioDefinition::createMarker
    ()
    {
        json marker = json::object();
        marker[Constants::ASSET_ATTR_AUDIO_EM_INDEX] = countMarkers();
        marker[Constants::ASSET_ATTR_AUDIO_EM_S_INDEX] = "-1";
        marker[Constants::ASSET_ATTR_AUDIO_EM_NAME] = "New Marker";
        mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].push_back(marker);
        return marker[Constants::ASSET_ATTR_AUDIO_EM_INDEX];
    }

    void
    AudioDefinition::removeMarker
    (const int index)
    {
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
    }

    int
    AudioDefinition::countMarkers
    ()
    {
        return static_cast<int>(
            mJson[Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS].size()
        );
    }

}
