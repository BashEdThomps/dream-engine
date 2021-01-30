#include "TextureRuntime.h"
#include "TextureTasks.h"

namespace octronic::dream
{
    TextureConstructionTask::TextureConstructionTask(TextureRuntime* rt)
        : GraphicsComponentTask("TextureConstructionTask"), mTextureRuntime(rt)
    {
    }

    void TextureConstructionTask::execute()
    {
        LOG_TRACE("TextureConstructionTask: {} Executing on Graphics thread",mTextureRuntime->getNameAndUuidString());
        if (!mTextureRuntime->tryLock())
        {
            if (!mTextureRuntime->loadIntoGL())
            {
                setState(TaskState::TASK_STATE_FAILED);
            }
            else
            {
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            mTextureRuntime->unlock();
        }
        else
        {
            mDeferralCount++;
            setState(TaskState::TASK_STATE_WAITING);
        }
    }

    TextureDestructionTask::TextureDestructionTask
    ()
        : GraphicsComponentDestructionTask("TextureDestructionTask")
    {
    }

    void TextureDestructionTask::execute()
    {
        LOG_TRACE("TextureDestructionTask: for texture {}, Executing on Graphics thread",mTextureId);
        glDeleteTextures(1,&mTextureId);
        setState(TaskState::TASK_STATE_COMPLETED);
    }

    void TextureDestructionTask::setGLID(GLuint id)
    {
        mTextureId = id;
    }
}
