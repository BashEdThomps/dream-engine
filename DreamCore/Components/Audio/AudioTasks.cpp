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
            mAudioRuntime->unlock();
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
            mDeferralCount++;
        }
    }
}
