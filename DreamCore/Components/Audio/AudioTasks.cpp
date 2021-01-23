#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace octronic::dream
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
        LOG_CRITICAL("AudioMarkersUpdateTask: Executing on thread {}",mThreadId);
        if(mAudioRuntime->tryLock())
        {
            mAudioRuntime->updateMarkers();
            mAudioRuntime->unlock();
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }
}
