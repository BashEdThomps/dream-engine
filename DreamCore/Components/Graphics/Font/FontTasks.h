#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class FontRuntime;

    class FontLoadIntoGLTask : public GraphicsTask
    {
        weak_ptr<FontRuntime> mFontRuntime;
    public:
        FontLoadIntoGLTask(const weak_ptr<ProjectRuntime>& pr,
                           const weak_ptr<FontRuntime>& rt);
        void execute();
    };

    class FontRemoveFromGLTask : public GraphicsDestructionTask
    {
        GLuint mFontAtlasTexture;
        GLuint mFontVao;
        GLuint mFontVbo;
    public:
        FontRemoveFromGLTask(const weak_ptr<ProjectRuntime>& pr);
        void setFontAtlasTexture(GLuint id);
        void setFontVao(GLuint id);
        void setFontVbo(GLuint id);
        void execute() override;
    };
}
