#include "ObjectEmitterTasks.h"

#include "ObjectEmitterRuntime.h"

namespace Dream
{
    ObjectEmitterUpdateTask::ObjectEmitterUpdateTask
    (ObjectEmitterRuntime* rt)
        : Task(),
          mRuntime(rt)
    {
    }

    void
    ObjectEmitterUpdateTask::execute
    ()
    {
        if (mRuntime->update())
        {
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
        }
    }
}
