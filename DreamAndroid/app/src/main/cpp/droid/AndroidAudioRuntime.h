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
#include <DreamCore.h>

using octronic::dream::AudioRuntime;
using octronic::dream::AudioDefinition;
using octronic::dream::ProjectRuntime;
using octronic::dream::AudioLoader;

namespace octronic::dream::android
{
    /**
     * @brief AudioRuntime data for an Android/Oboe based Audio Clip.
     */
    class AndroidAudioRuntime : public AudioRuntime
    {
    public:
        AndroidAudioRuntime(AudioLoader* loader, AudioDefinition* def, ProjectRuntime* project);
        ~AndroidAudioRuntime() override;

        bool useDefinition() override;
        void play() override;
        void pause() override;
        void stop() override;
        void setSourcePosision(vec3 pos) override;
        void setVolume(float volume) override;
        AudioStatus getState() override;
        unsigned int getSampleOffset() const override;
        void setSampleOffset(unsigned int offset) override;
        int getDurationInSamples() override;

    protected:
        bool loadIntoOboe();
    };
}
