#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace Dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (AudioRuntime* rt)
        : Task(), mAudioRuntime(rt)
    {
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        if(mAudioRuntime->tryLock())
        {
            mAudioRuntime->updateMarkers();
            mAudioRuntime->unlock();
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }
}
