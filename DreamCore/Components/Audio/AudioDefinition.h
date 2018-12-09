/*
 * AudioAssetDefinition.h
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
#pragma once

#include "../AssetDefinition.h"

namespace  Dream
{
    /*
    class AudioEventMarker
    {
    public:
        // No-Args Constructor
        AudioEventMarker()
            : index(-1),
              name(""),
              sampleIndex(-1)
        {}

        // Args Constructor
        AudioEventMarker(int idx, string n, int sIndex)
            : index(idx),
              name(n),
              sampleIndex(sIndex)
        {}

        // Copy Constructor
        AudioEventMarker(const AudioEventMarker& other)
        {
            index = other.index;
            name = other.name;
            sampleIndex = other.sampleIndex;
        }

        bool operator==(const AudioEventMarker& other)
        {
            return index == other.index &&
                   name.compare(other.name) == 0 &&
                   sampleIndex == other.sampleIndex;
        }

        int index;
        string name;
        int sampleIndex;
    };
    */

    class AudioDefinition : public AssetDefinition
    {
    public:
        AudioDefinition(ProjectDefinition* pd, json js);
        ~AudioDefinition() override;

        bool isFormatWav();
        bool isFormatOgg();

        void setLoop(bool);
        bool getLoop();

        bool getSpectrumAnalyser();
        void setSpectrumAnalyser(bool);

        int createMarker();
        void removeMarker(const int index);
        int countMarkers();

        string getMarkerName(int index);
        void setMarkerName(int index, string name);

        int getMarkerSampleIndex(int index);
        void setMarkerSampleIndex(int index, int smpl);

        int getMarkerRepeat(int index);
        void setMarkerRepeat(int index, int repeat);

        int getMarkerRepeatPeriod(int index);
        void setMarkerRepeatPeriod(int index, int rp);

    };

}
