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
#include "AndroidAudioComponent.h"
#include "AndroidAudioRuntime.h"


using octronic::dream::WavLoader;
using octronic::dream::OggLoader;

namespace octronic::dream::android
{

    AndroidAudioComponent::AndroidAudioComponent
    ()
    : AudioComponent()
    {
        LOG_TRACE("AudioComponent: Constructing");
    }

    AndroidAudioComponent::~AndroidAudioComponent
    ()
    {
        LOG_TRACE("AudioComponent: Destructing");
    }

    bool
    AndroidAudioComponent::init
    ()
    {
        LOG_DEBUG("AudioComponent: Initialising...");
        vec3 position(0.0f);
        setListenerPosition(position);
        return true;
    }

    void
    AndroidAudioComponent::setListenerPosition
    (const vec3& pos)
    {
        LOG_TRACE("AudioComponent: Constructing");
    }

    void
    AndroidAudioComponent::setVolume
    (float volume)
    {
        LOG_TRACE("AudioComponent: Constructing");
    }

    float
    AndroidAudioComponent::getVolume
    ()
    {
        LOG_TRACE("AudioComponent: Constructing");
        return 0.f;
    }

    AudioRuntime* AndroidAudioComponent::newAudioRuntime(AudioDefinition* def)
    {
        LOG_TRACE("AudioComponent: Constructing");
       AudioRuntime* aRunt = nullptr;
       if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
       {
           aRunt = new AndroidAudioRuntime(new OggLoader(), def,mProjectRuntime);
       }
       else if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
       {
           aRunt = new AndroidAudioRuntime(new WavLoader(),def,mProjectRuntime);
       }
       return aRunt;
    }
}
