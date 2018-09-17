/*
* Dream::Plugnis::Audio::Open::AudioComponent
*
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

#include <glm/vec3.hpp>
#include "AudioStatus.h"
#include "../IComponent.h"

using std::vector;

namespace Dream
{
    class AudioInstance;
    class AudioDefinition;
    class SceneObjectRuntime;

    class AudioComponent : public IComponent
    {

    private:
        vector<AudioInstance*> mPlayQueue;
        vector<AudioInstance*> mPauseQueue;
        vector<AudioInstance*> mStopQueue;
        vector<ALuint> mSources;
        vector<ALuint> mBuffers;
        ALCdevice*  mDevice;
        ALCcontext* mContext;

    public:
        AudioComponent();
        ~AudioComponent() override;
        bool init() override;
        void updateComponent() override;

        void setSourcePosision(ALuint, glm::vec3);
        void setListenerPosition(glm::vec3);

        void pushToPlayQueue (AudioInstance*);
        void pushToPauseQueue(AudioInstance*);
        void pushToStopQueue (AudioInstance*);

        AudioStatus getAudioStatus(AudioInstance*) const;

        float getSampleOffset(ALuint) const;
        float getSampleOffset(AudioInstance*) const;
        vector<char> getAudioBuffer(AudioInstance*, ALint, ALint) const;
        AudioInstance* newAudioInstance(AudioDefinition*,SceneObjectRuntime*);
        void setVolume(float);

    protected:
        ALuint generateBuffers(size_t);
        ALuint generateSources(size_t);

        void deleteBuffers(int, ALuint);
        void deleteSources(int, ALuint);

        void playSource(ALuint);
        void stopSource(ALuint);
        void pauseSource(ALuint);

        void updatePlayQueue();
        void updatePauseQueue();
        void updateStopQueue();

        void deleteAllSources();
        void deleteAllBuffers();

    }; // End of AudioComponent

} // End of Dream
