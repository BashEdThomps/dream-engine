#include "AudioTasks.h"
#include "AudioRuntime.h"

namespace Dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (AudioRuntime* rt)
        : Task(), mAudioRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("AudioMarkersUpdateTask");
        #endif
        rt->lock();
        rt->setMarkersUpdateTask(this);
        rt->unlock();
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif
        if(mAudioRuntime->tryLock())
        {
            mAudioRuntime->updateMarkers();
            mAudioRuntime->clearMarkersUpdateTask();
            mAudioRuntime->unlock();
            mCompleted = true;
        }
        else
        {
            mDeferralCount++;
        }
    }
}
