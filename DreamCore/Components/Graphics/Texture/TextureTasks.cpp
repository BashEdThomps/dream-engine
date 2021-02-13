#include "TextureRuntime.h"
#include "TextureTasks.h"

namespace octronic::dream
{
    TextureLoadIntoGLTask::TextureLoadIntoGLTask(ProjectRuntime* pr, TextureRuntime* rt)
        : GraphicsTask(pr, "TextureLoadIntoGLTask"),
          mTextureRuntime(rt)
    {
    }

    void TextureLoadIntoGLTask::execute()
    {
        LOG_TRACE("TextureLoadIntoGLTask: {} Executing on Graphics thread",mTextureRuntime->getNameAndUuidString());
        if (!mTextureRuntime->loadIntoGL())
        {
            setState(TaskState::TASK_STATE_FAILED);
        }
        else
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
    }

    TextureRemoveFromGLTask::TextureRemoveFromGLTask
    (ProjectRuntime* pr)
        : GraphicsDestructionTask(pr, "TextureRemoveFromGLTask")
    {
    }

    void TextureRemoveFromGLTask::execute()
    {
        LOG_TRACE("TextureRemoveFromGLTask: for texture {}, Executing on Graphics thread",mTextureID);
        glDeleteTextures(1,&mTextureID);
        setState(TaskState::TASK_STATE_COMPLETED);
    }

    void TextureRemoveFromGLTask::setGLID(GLuint id)
    {
        mTextureID = id;
    }
}
