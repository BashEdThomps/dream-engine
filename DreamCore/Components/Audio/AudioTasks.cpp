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
        setActive(true);
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
            setActive(false);
            mAudioRuntime->unlock();
            mCompleted = true;
        }
        else
        {
            mDeferralCount++;
        }
    }
}
