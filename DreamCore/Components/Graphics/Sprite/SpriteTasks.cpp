#include "SpriteRuntime.h"

#include "SpriteTasks.h"
#include "Components/Graphics/Texture/TextureRuntime.h"

namespace octronic::dream
{
    // SpriteConstructionTask ==================================================

    SpriteConstructionTask::SpriteConstructionTask(SpriteRuntime* rt)
        : GraphicsComponentTask("SpriteConstructionTask"),
        mSpriteRuntime(rt)
    {
    }

    void SpriteConstructionTask::execute()
    {
        LOG_TRACE("SpriteConstructionTask: {} Executing on Graphics thread",mSpriteRuntime->getNameAndUuidString());
        mSpriteRuntime->pushInstance();
		setState(TaskState::TASK_STATE_COMPLETED);
    }

    // SpriteDestructionTask ===================================================

    SpriteDestructionTask::SpriteDestructionTask
    (TextureRuntime* texture, UuidType entityUuid)
        : GraphicsComponentDestructionTask("SpriteDestructionTask"),
          mTextureRuntime(texture),
          mEntityUuid(entityUuid)
    {
    }

    void SpriteDestructionTask::execute()
    {
        LOG_TRACE("SpriteDestructionTask: ");
        mTextureRuntime->popSpriteInstanceByUuid(mEntityUuid);
        setState(TaskState::TASK_STATE_COMPLETED);
    }
}
