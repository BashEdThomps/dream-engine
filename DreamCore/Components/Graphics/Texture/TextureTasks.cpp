#pragma once

#include "TextureRuntime.h"
#include "TextureTasks.h"

namespace Dream
{
    TextureConstructionTask::TextureConstructionTask(TextureRuntime* rt)
        : GraphicsComponentTask(), mTextureRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("TextureCreationTask");
        #endif
    }

    void TextureConstructionTask::execute()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif
        // Assign texture to ID
        GLuint textureID;

        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glGenTextures(1, &textureID);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glBindTexture(GL_TEXTURE_2D, textureID);
        #ifdef DREAM_LOG
        checkGLError();
        getLog()->debug("Bound to texture id {}",textureID);
        #endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureRuntime->getWidth(),
            mTextureRuntime->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
            mTextureRuntime->getImage());
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glGenerateMipmap(GL_TEXTURE_2D);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        mTextureRuntime->setGLID(textureID);

        glBindTexture(GL_TEXTURE_2D, 0);

        #ifdef DREAM_LOG
        checkGLError();
        #endif
        setState(TaskState::COMPLETED);
    }

    TextureDestructionTask::TextureDestructionTask
    ()
        : GraphicsComponentDestructionTask()
    {
        #ifdef DREAM_LOG
        setLogClassName("TextureDeletionTask");
        #endif
    }

    void TextureDestructionTask::execute()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif
        glDeleteTextures(1,&mTextureId);
        setState(TaskState::COMPLETED);
    }

    void TextureDestructionTask::setGLID(GLuint id)
    {
        mTextureId = id;
    }
}
