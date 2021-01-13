#include "TextureRuntime.h"
#include "TextureTasks.h"
#include <SOIL.h>

namespace Dream
{
    TextureConstructionTask::TextureConstructionTask(TextureRuntime* rt)
        : GraphicsComponentTask(), mTextureRuntime(rt)
    {
    }

    void TextureConstructionTask::execute()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        // Assign texture to ID
        GLuint textureID;

        GLCheckError();

        glGenTextures(1, &textureID);
        GLCheckError();

        glBindTexture(GL_TEXTURE_2D, textureID);
        GLCheckError();
        LOG_DEBUG("Bound to texture id {}",textureID);

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
        SOIL_free_image_data(mTextureRuntime->getImage());
        mTextureRuntime->setImage(nullptr);
        setState(TaskState::COMPLETED);
    }

    TextureDestructionTask::TextureDestructionTask
    ()
        : GraphicsComponentDestructionTask()
    {
    }

    void TextureDestructionTask::execute()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        glDeleteTextures(1,&mTextureId);
        setState(TaskState::COMPLETED);
    }

    void TextureDestructionTask::setGLID(GLuint id)
    {
        mTextureId = id;
    }
}
