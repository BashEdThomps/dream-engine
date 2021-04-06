#pragma once

#include "Common/GLHeader.h"

#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
  class TextureRuntime;

  // TextureRuntimeTask =======================================================

  class TextureRuntimeTask : public GraphicsTask
  {
  public:
    TextureRuntimeTask(ProjectRuntime&, TextureRuntime&, const string& name);
  protected:
    TextureRuntime& getTextureRuntime() const;
  private:
    reference_wrapper<TextureRuntime> mTextureRuntime;
  };

  // TextureLoadIntoGLTask =====================================================

  class TextureLoadIntoGLTask : public TextureRuntimeTask
  {
  public:
    TextureLoadIntoGLTask(ProjectRuntime& pr, TextureRuntime& rt);
    void execute();
  };

  // TextureSetupEnvironmentTask ===============================================

  class TextureSetupEnvironmentTask : public TextureRuntimeTask
  {
  public:
    TextureSetupEnvironmentTask(ProjectRuntime& pr, TextureRuntime& rt);
    void execute() override;
  };

  // TextureRemoveFromGLTask ===================================================

  class TextureRemoveFromGLTask : public GraphicsDestructionTask
  {
    // Base Texture
    GLuint mTextureID;
    // Environment
    GLuint mCaptureFBO;
    GLuint mCaptureRBO;
    GLuint mEquiToCubeTexture;
    GLuint mIrradianceMapTexture;
    GLuint mPreFilterTexture;
    GLuint mBrdfLutTexture;
    // Cube
    GLuint mCubeVAO;
    GLuint mCubeVBO;
    // Quad
    GLuint mQuadVAO;
    GLuint mQuadVBO;
  public:
    TextureRemoveFromGLTask(ProjectRuntime& pr);
    void setTextureID(GLuint id);
    void setCaptureBuffers(GLuint fbo, GLuint rbo);
    void setEquiToCubeTexture(GLuint texture);
    void setIrradianceTexture(GLuint texture);
    void setPreFilterTexture(GLuint texture);
    void setBrdfLutTexture(GLuint texture);
    void setCubeBuffers(GLuint VAO, GLuint VBO);
    void setQuadBuffers(GLuint VAO, GLuint VBO);
    void clearVariables();
    void execute() override;
  };
}
