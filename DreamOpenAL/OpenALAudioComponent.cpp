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
#include "OpenALAudioComponent.h"
#include "OpenALAudioRuntime.h"

#include <iostream>

using octronic::dream::OggLoader;
using octronic::dream::WavLoader;

namespace octronic::dream::open_al
{

    OpenALAudioComponent::OpenALAudioComponent
    ()
        : AudioComponent(),
          mDevice(nullptr),
          mContext(nullptr)
    {
        LOG_TRACE("AudioComponent: Constructing");
    }

    OpenALAudioComponent::~OpenALAudioComponent
    ()
    {
        LOG_TRACE("AudioComponent: Destructing");
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
    OpenALAudioComponent::init
    ()
    {
        LOG_DEBUG("AudioComponent: Initialising...");
        mDevice = alcOpenDevice(nullptr);
        mContext  = alcCreateContext(mDevice, nullptr);
        alcMakeContextCurrent(mContext);
        vec3 position(0.0f);
        setListenerPosition(position);
        return true;
    }

    void
    OpenALAudioComponent::setListenerPosition
    (const vec3& pos)
    {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    OpenALAudioComponent::setVolume
    (float volume)
    {
        alListenerf(AL_GAIN,volume);
    }

    float
    OpenALAudioComponent::getVolume
    ()
    {
        float vol;
        alGetListenerf(AL_GAIN,&vol);
        return vol;
    }

    AudioRuntime* OpenALAudioComponent::newAudioRuntime(AudioDefinition* def)
    {
        AudioRuntime* aRunt = nullptr;
        if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
        	aRunt = new OpenALAudioRuntime(new OggLoader(),def,mProjectRuntime);
        }
        else if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
        	aRunt = new OpenALAudioRuntime(new WavLoader(),def,mProjectRuntime);
        }
        return aRunt;
    }
}