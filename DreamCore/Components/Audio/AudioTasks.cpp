#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace octronic::dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (ProjectRuntime* pr, AudioRuntime* rt)
        : Task(pr,"AudioMarkersUpdateTask"),
          mAudioRuntime(rt)
    {
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        LOG_TRACE("AudioMarkersUpdateTask: Executing {}",getID());
		mAudioRuntime->updateMarkers();
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
