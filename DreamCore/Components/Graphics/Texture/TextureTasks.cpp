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
        // Assign texture to ID
        GLuint textureID;

        GLCheckError();

        glGenTextures(1, &textureID);
        GLCheckError();

        glBindTexture(GL_TEXTURE_2D, textureID);
        GLCheckError();
        LOG_DEBUG("TextureConstructionTask: Bound to texture id {}",textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureRuntime->getWidth(),
            mTextureRuntime->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
            mTextureRuntime->getImage());
        GLCheckError();

        glGenerateMipmap(GL_TEXTURE_2D);
        GLCheckError();

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        mTextureRuntime->setGLID(textureID);

        glBindTexture(GL_TEXTURE_2D, 0);

        GLCheckError();
        delete mTextureRuntime->getImage();
        mTextureRuntime->setImage(nullptr);
        setState(TaskState::TASK_STATE_COMPLETED);
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
