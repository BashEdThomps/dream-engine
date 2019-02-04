#pragma once

#include "../../TaskManager/Task.h"
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
            setLogClassName("AudioMarkersUpdateTask");
            #endif
            rt->lock();
            rt->setMarkersUpdateTask(this);
            rt->unlock();
        }

        inline  ~AudioMarkersUpdateTask() {}

        inline void execute()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mAudioRuntime->tryLock())
            {
                mAudioRuntime->updateMarkers();
                mAudioRuntime->clearMarkersUpdateTask();
                mAudioRuntime->unlock();
                clearDeferred();
            }
            else
            {
                setDeferred();
            }
        }
    };
}
