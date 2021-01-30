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

#include "AndroidAudioRuntime.h"

namespace octronic::dream::android
{
    AndroidAudioRuntime::AndroidAudioRuntime
    (AudioLoader* loader, AudioDefinition* def, ProjectRuntime* project)
        : AudioRuntime(loader, def,project)
    {
        LOG_DEBUG("AndroidAudioRuntime: {}", __FUNCTION__);
    }

    AndroidAudioRuntime::~AndroidAudioRuntime()
    {
        LOG_DEBUG("AndroidAudioRuntime: {}", __FUNCTION__);
    }

    void AndroidAudioRuntime::play()
    {
        LOG_DEBUG(  "AndroidAudioRuntime: Playing source ");
    }

    void AndroidAudioRuntime::stop()
    {
        LOG_DEBUG(  "AndroidAudioRuntime: Stopping source");
    }

    void AndroidAudioRuntime::pause()
    {
        LOG_DEBUG(  "AndroidAudioRuntime: Pausing source");
    }

    void AndroidAudioRuntime::setSourcePosision(vec3 pos)
    {
        LOG_DEBUG(  "AndroidAudioRuntime: SetSource Position {},{},{}" , pos.x,pos.y,pos.z);
    }

    void AndroidAudioRuntime::setVolume(float volume)
    {
        LOG_DEBUG(  "AndroidAudioRuntime: SetVolume vol:{}" , volume);
    }

    AudioStatus AndroidAudioRuntime::getState()
    {
        LOG_TRACE("AndroidAudioRuntime: {}", __FUNCTION__);
        return AudioStatus::AUDIO_STATUS_UNKNOWN;
    }

    int AndroidAudioRuntime::getDurationInSamples()
    {
        LOG_TRACE("AndroidAudioRuntime: {}", __FUNCTION__);
        return 0;
    }

    unsigned int AndroidAudioRuntime::getSampleOffset()const
    {
        LOG_TRACE("AndroidAudioRuntime: {}", __FUNCTION__);
        return 0;
    }

    void AndroidAudioRuntime::setSampleOffset(unsigned int offset)
    {
        LOG_DEBUG("AndroidAudioRuntime: {}", __FUNCTION__);
    }

    bool AndroidAudioRuntime::loadIntoOboe()
    {
        LOG_DEBUG("AndroidAudioRuntime: {}", __FUNCTION__);
		return false;
    }


	bool AndroidAudioRuntime::useDefinition()
	{
        LOG_DEBUG("AndroidAudioRuntime: {}", __FUNCTION__);
        return true;
	}
}
