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

#include "ALHeader.h"
#include <DreamCore.h>

#include <iostream>
#include <vector>
#include <memory>

#include <glm/vec3.hpp>

using octronic::dream::AudioComponent;
using octronic::dream::AudioDefinition;
using octronic::dream::AudioRuntime;
using glm::vec3;

namespace octronic::dream::open_al
{
    class OpenALAudioComponent : public AudioComponent
    {
    private:
        ALCdevice*  mDevice;
        ALCcontext* mContext;
    public:
        OpenALAudioComponent();
        ~OpenALAudioComponent() override;
        bool init() override;
        void setListenerPosition(const vec3&) override;
        void setVolume(float) override;
        float getVolume() override;
        AudioRuntime* newAudioRuntime(AudioDefinition* def) override;
    };
}
