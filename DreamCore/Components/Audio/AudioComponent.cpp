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

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <iostream>

#include "AudioComponent.h"
#include "Wav/WavAudioRuntime.h"
#include "Ogg/OggAudioRuntime.h"

#include "AudioDefinition.h"
#include "../Time.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    AudioComponent::AudioComponent
    ()
        : Component()
    {
        #ifdef DREAM_LOG
        setLogClassName("AudioComponent");
        getLog()->trace("Constructing");
        #endif
    }

    AudioComponent::~AudioComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
        #endif

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
        #ifdef DREAM_LOG
        getLog()->debug("Initialising...");
        #endif
        mDevice = alcOpenDevice(nullptr);
        mContext  = alcCreateContext(mDevice, nullptr);
        alcMakeContextCurrent(mContext);
        vec3 position(0.0f);
        setListenerPosition(position);
        return true;
    }

    void
    AudioComponent::setListenerPosition
    (vec3 pos)
    {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void
    AudioComponent::pushToUpdateQueue
    (AudioRuntime* ai)
    {
        if (find(mUpdateQueue.begin(), mUpdateQueue.end(), ai) == mUpdateQueue.end())
        {
            mUpdateQueue.push_back(ai);
        }
    }

    void
    AudioComponent::updateComponent
    (SceneRuntime*)
    {

        if (!mEnabled)
        {
            #ifdef DREAM_LOG
            getLog()->warn("Update Disabled");
            #endif
            return;
        }

        beginUpdate();
        updateRuntimes();
        endUpdate();
    }

    void
    AudioComponent::updateRuntimes
    ()
    {
       for (AudioRuntime* ai : mUpdateQueue)
       {
          ai->updateMarkers();
       }
    }

    void
    AudioComponent::setVolume
    (float volume)
    {
        alListenerf(AL_GAIN,volume);
    }

    float AudioComponent::getVolume()
    {
        float vol;
        alGetListenerf(AL_GAIN,&vol);
        return vol;
    }
}
