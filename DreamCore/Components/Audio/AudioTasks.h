#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
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
