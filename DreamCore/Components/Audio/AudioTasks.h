#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    class AudioRuntime;
    class AudioMarkersUpdateTask : public Task
    {
    protected:
        AudioRuntime* mAudioRuntime;
    public:
        AudioMarkersUpdateTask(AudioRuntime* rt);
        void execute();
    };
}
