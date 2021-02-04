#include "SpriteRuntime.h"

#include "SpriteTasks.h"

namespace octronic::dream
{
    SpriteConstructionTask::SpriteConstructionTask(SpriteRuntime* rt)
        : GraphicsComponentTask("SpriteConstructionTask"),
        mSpriteRuntime(rt)
    {
    }

    void SpriteConstructionTask::execute()
    {
        LOG_TRACE("SpriteConstructionTask: {} Executing on Graphics thread",mSpriteRuntime->getNameAndUuidString());
		setState(TaskState::TASK_STATE_COMPLETED);
    }

    SpriteDestructionTask::SpriteDestructionTask
    ()
        : GraphicsComponentDestructionTask("SpriteDestructionTask")
    {
    }

    void SpriteDestructionTask::execute()
    {
        LOG_TRACE("SpriteDestructionTask: ");
        setState(TaskState::TASK_STATE_COMPLETED);
    }
}
