#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace octronic::dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (AudioRuntime* rt)
        : Task("AudioMarkersUpdateTask"),
          mAudioRuntime(rt)
    {
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        assert(getThreadID() > INVALID_THREAD_ID);
        LOG_TRACE("AudioMarkersUpdateTask: Executing on thread {}",getThreadID());
		mAudioRuntime->updateMarkers();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
