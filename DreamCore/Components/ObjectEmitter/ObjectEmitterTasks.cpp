#include "ObjectEmitterTasks.h"
#include "ObjectEmitterRuntime.h"

namespace Dream
{
    ObjectEmitterUpdateTask::ObjectEmitterUpdateTask
    (ObjectEmitterRuntime* rt)
        : Task(),
          mRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("ObjectEmitterUpdateTask");
        #endif
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
