#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTask.h"

namespace octronic::dream
{
    class TextureRuntime;
    class TextureConstructionTask : public GraphicsComponentTask
    {
        TextureRuntime* mTextureRuntime;
    public:
        TextureConstructionTask(TextureRuntime* rt);
        void execute();
    };

    class TextureDestructionTask : public GraphicsComponentDestructionTask
    {
        GLuint mTextureId;
    public:
        TextureDestructionTask();
        void setGLID(GLuint id);
        void execute() override;
    };
}
