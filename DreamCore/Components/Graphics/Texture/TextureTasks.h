#pragma once

// Defer GL Operations using Task objects
#include "TextureRuntime.h"
#include "../GraphicsComponentTask.h"

namespace Dream
{
    class TextureCreationTask : public GraphicsComponentTask
    {
        TextureRuntime* mTextureRuntime;
    public:
        TextureCreationTask(TextureRuntime* rt)
            : GraphicsComponentTask(), mTextureRuntime(rt)
        {}

        inline bool
        execute
        () override
        {
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            #ifdef DREAM_LOG
            checkGLError();
            #endif

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
            mTextureRuntime->clearCreateTextureTask();
            return textureID != 0;
        }
    };

    class TextureDeletionTask : public GraphicsComponentTask
    {
        GLuint mTextureId;
    public:
        inline TextureDeletionTask
        (GLuint id)
            : GraphicsComponentTask(), mTextureId(id)
        {}

        inline bool
        execute
        () override
        {
            glDeleteTextures(1,&mTextureId);
            return true;
        }
    };
}
