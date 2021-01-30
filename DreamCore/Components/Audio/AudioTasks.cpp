#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace octronic::dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (AudioRuntime* rt)
        : Task("AudioMarkersUpdateTask"), mAudioRuntime(rt)
    {
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        LOG_TRACE("AudioMarkersUpdateTask: Executing on thread {}",mThreadId);
        if(mAudioRuntime->tryLock())
        {
            mAudioRuntime->updateMarkers();
            mAudioRuntime->unlock();
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
          	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
            setState(TaskState::TASK_STATE_WAITING);
            mDeferralCount++;
        }
    }
}
