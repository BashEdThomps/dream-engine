#pragma once

#include "../TaskManager/Task.h"
#include "AudioRuntime.h"

namespace Dream
{
    class AudioMarkersUpdateTask : public Task
    {
    protected:
        AudioRuntime* mAudioRuntime;
    public:
        inline AudioMarkersUpdateTask(AudioRuntime* rt)
            : Task(), mAudioRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("AudioUpdateTask");
            #endif
            rt->lock();
            rt->setMarkersUpdateTask(this);
            rt->unlock();
        }

        inline  ~AudioMarkersUpdateTask() {}

        inline bool execute()
        {
            mAudioRuntime->lock();
            mAudioRuntime->updateMarkers();
            mAudioRuntime->clearMarkersUpdateTask();
            mAudioRuntime->unlock();
            return true;
        }
    };
}
