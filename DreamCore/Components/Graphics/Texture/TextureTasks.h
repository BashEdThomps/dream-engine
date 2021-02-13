#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class TextureRuntime;
    class TextureLoadIntoGLTask : public GraphicsTask
    {
        TextureRuntime* mTextureRuntime;
    public:
        TextureLoadIntoGLTask(ProjectRuntime* pr, TextureRuntime* rt);
        void execute();
    };

    class TextureRemoveFromGLTask : public GraphicsDestructionTask
    {
        GLuint mTextureID;
    public:
        TextureRemoveFromGLTask(ProjectRuntime* pr);
        void setGLID(GLuint id);
        void execute() override;
    };
}
