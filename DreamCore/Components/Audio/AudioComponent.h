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
    	  virtual AudioRuntime& getAudioRuntime(AudioDefinition& def) = 0;
        void pushTasks() override;
    };
}
