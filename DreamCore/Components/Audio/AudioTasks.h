#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class AudioRuntime;
    class AudioMarkersUpdateTask : public Task
    {
    public:
        AudioMarkersUpdateTask(ProjectRuntime* pr, AudioRuntime* rt);
        void execute();
    protected:
        AudioRuntime* mAudioRuntime;
    };
}
