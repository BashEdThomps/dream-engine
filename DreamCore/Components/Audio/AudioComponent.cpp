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
#include "AudioComponent.h"

#include "Common/Logger.h"
#include "Wav/WavAudioRuntime.h"
#include "Ogg/OggAudioRuntime.h"
#include "AudioDefinition.h"
#include "Components/Time.h"
#include "Scene/Entity/EntityRuntime.h"

#include <iostream>

namespace Dream
{
    AudioComponent::AudioComponent
    (ProjectRuntime* rt)
        : Component(rt),
          mDevice(nullptr),
          mContext(nullptr)
    {
        LOG_TRACE("Constructing");
    }

    AudioComponent::~AudioComponent
    ()
    {
        LOG_TRACE("Destructing");

        alcMakeContextCurrent(nullptr);

        if (mContext != nullptr)
        {
            alcDestroyContext(mContext);
            mContext = nullptr;
        }

        if (mDevice != nullptr)
        {
            alcCloseDevice(mDevice);
            mDevice = nullptr;
        }
    }

    bool
    AudioComponent::init
    ()
    {
        LOG_DEBUG("Initialising...");
        mDevice = alcOpenDevice(nullptr);
        mContext  = alcCreateContext(mDevice, nullptr);
        alcMakeContextCurrent(mContext);
        vec3 position(0.0f);
        setListenerPosition(position);
        return true;
    }

    void
    AudioComponent::setListenerPosition
    (const vec3& pos)
    {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioComponent::setVolume
    (float volume)
    {
        alListenerf(AL_GAIN,volume);
    }

    float
    AudioComponent::getVolume
    ()
    {
        float vol;
        alGetListenerf(AL_GAIN,&vol);
        return vol;
    }
}
