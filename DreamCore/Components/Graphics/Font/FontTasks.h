#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class FontRuntime;

    class FontLoadIntoGLTask : public GraphicsTask
    {
        FontRuntime* mFontRuntime;
    public:
        FontLoadIntoGLTask(ProjectRuntime* pr, FontRuntime* rt);
        void execute();
    };

    class FontRemoveFromGLTask : public GraphicsDestructionTask
    {
        GLuint mFontAtlasTexture;
        GLuint mFontVao;
        GLuint mFontVbo;
    public:
        FontRemoveFromGLTask(ProjectRuntime* pr);
        void setFontAtlasTexture(GLuint id);
        void setFontVao(GLuint id);
        void setFontVbo(GLuint id);
        void execute() override;
    };
}
