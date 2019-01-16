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

#include <iostream>
#include <vector>
#include <memory>

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include "AudioStatus.h"
#include "../Component.h"
#include <glm/vec3.hpp>

using std::vector;

namespace Dream
{
    class AudioRuntime;
    class AudioDefinition;

    class AudioComponent : public Component
    {

    private:
        vector<AudioRuntime*> mUpdateQueue;
        ALCdevice*  mDevice;
        ALCcontext* mContext;

    public:
        AudioComponent();
        ~AudioComponent() override;
        bool init() override;
        void updateComponent(SceneRuntime*) override;

        void setListenerPosition(glm::vec3);

        void pushToUpdateQueue(AudioRuntime*);

        void setVolume(float);
        float getVolume();

    protected:
        void updateRuntimes();

    }; // End of AudioComponent

} // End of Dream
