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
#include "Components/Component.h"
#include "Math/Vector.h"

namespace octronic::dream
{
    class AudioRuntime;
    class AudioDefinition;
    class AudioComponent : public Component
    {
    public:
        AudioComponent();
        virtual ~AudioComponent();
        virtual void setListenerPosition(const vec3&) = 0;
        virtual void setVolume(float) = 0;
        virtual float getVolume() = 0;
    	  virtual weak_ptr<AudioRuntime> getAudioRuntime(const weak_ptr<AudioDefinition>& def) = 0;
        void pushTasks() override;
    };
}
