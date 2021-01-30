#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTask.h"

namespace octronic::dream
{
    class FontRuntime;

    class FontConstructionTask : public GraphicsComponentTask
    {
        FontRuntime* mFontRuntime;
    public:
        FontConstructionTask(FontRuntime* rt);
        void execute();
    };

    class FontDestructionTask : public GraphicsComponentDestructionTask
    {
        GLuint mFontAtlasTexture;
        GLuint mFontVao;
        GLuint mFontVbo;
    public:
        FontDestructionTask();
        void setFontAtlasTexture(GLuint id);
        void setFontVao(GLuint id);
        void setFontVbo(GLuint id);
        void execute() override;
    };
}
