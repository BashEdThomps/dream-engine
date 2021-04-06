#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
  class FontRuntime;

  class FontLoadIntoGLTask : public GraphicsTask
  {
  public:
    FontLoadIntoGLTask(ProjectRuntime& pr, FontRuntime& rt);
    void execute();
  private:
    FontRuntime& getFontRuntime() const;
    reference_wrapper<FontRuntime> mFontRuntime;
  };

  class FontRemoveFromGLTask : public GraphicsDestructionTask
  {

  public:
    FontRemoveFromGLTask(ProjectRuntime& pr);
    void setFontAtlasTexture(GLuint id);
    void setFontVao(GLuint id);
    void setFontVbo(GLuint id);
    void execute() override;
  private:
    GLuint mFontAtlasTexture;
    GLuint mFontVao;
    GLuint mFontVbo;
  };
}
