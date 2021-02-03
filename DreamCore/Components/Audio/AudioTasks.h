#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
{
    class AudioRuntime;
    class AudioMarkersUpdateTask : public Task
    {
    public:
        AudioMarkersUpdateTask(AudioRuntime* rt);
        void execute();
    protected:
        AudioRuntime* mAudioRuntime;
    };
}
