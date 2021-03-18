#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class AudioRuntime;
    class AudioMarkersUpdateTask : public Task
    {
    public:
        AudioMarkersUpdateTask(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<AudioRuntime>& rt);
        void execute();
    protected:
        weak_ptr<AudioRuntime> mAudioRuntime;
    };
}
