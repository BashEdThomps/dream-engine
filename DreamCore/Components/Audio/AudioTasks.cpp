#include "AudioTasks.h"
#include "AudioRuntime.h"

#include "Common/Logger.h"

namespace octronic::dream
{
    AudioMarkersUpdateTask::AudioMarkersUpdateTask
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<AudioRuntime>& rt)
        : Task(pr,"AudioMarkersUpdateTask"),
          mAudioRuntime(rt)
    {
    }

    void
    AudioMarkersUpdateTask::execute
    ()
    {
        LOG_TRACE("AudioMarkersUpdateTask: Executing {}",getID());
        if (auto arLock = mAudioRuntime.lock())
        {
			arLock->updateMarkers();
        }
		setState(TaskState::TASK_STATE_COMPLETED);
    }
}
