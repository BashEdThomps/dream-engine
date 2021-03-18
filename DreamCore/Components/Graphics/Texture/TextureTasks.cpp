#include "TextureRuntime.h"
#include "TextureTasks.h"

namespace octronic::dream
{

  // TextureLoadIntoGLTask ===================================================

  TextureLoadIntoGLTask::TextureLoadIntoGLTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<TextureRuntime>& rt)
    : GraphicsTask(pr, "TextureLoadIntoGLTask"),
      mTextureRuntime(rt)
  {
  }

  void TextureLoadIntoGLTask::execute()
  {
    if (auto rtLock = mTextureRuntime.lock())
    {
      LOG_TRACE("TextureLoadIntoGLTask: {} Executing on Graphics thread",
                rtLock->getNameAndUuidString());
      if (!rtLock->loadTextureIntoGL())
      {
        setState(TaskState::TASK_STATE_FAILED);
      }
      else
      {
        setState(TaskState::TASK_STATE_COMPLETED);
      }

      GLCheckError();
    }
  }

  // TextureRenderCubeMapTsk =================================================

  TextureSetupEnvironmentTask::TextureSetupEnvironmentTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<TextureRuntime>& rt)
    : GraphicsTask(pr, "TextureRenderCubeMapTask"),
      mTextureRuntime(rt)
  {

  }

  void TextureSetupEnvironmentTask::execute()
  {
    if (auto rtLock = mTextureRuntime.lock())
    {
      if (!rtLock->renderEquirectangularToCubeMap() ||
          !rtLock->renderIrradianceCubeMap() ||
          !rtLock->renderPreFilterCubeMap() ||
          !rtLock->renderBrdfLut())
      {

        setState(TaskState::TASK_STATE_DEFERRED);
      }
      else
      {
        setState(TaskState::TASK_STATE_COMPLETED);
      }

      GLCheckError();
    }
  }

  // TextureRemoveFromGLTask =================================================

  TextureRemoveFromGLTask::TextureRemoveFromGLTask
  (const weak_ptr<ProjectRuntime>& pr)
    : GraphicsDestructionTask(pr, "TextureRemoveFromGLTask"),
      // Base Texture
      mTextureID(0),
      // Environment
      mCaptureFBO(0),
      mCaptureRBO(0),
      mEquiToCubeTexture(0),
      mIrradianceMapTexture(0),
      mPreFilterTexture(0),
      mBrdfLutTexture(0),
      // Cube
      mCubeVAO(0),
      mCubeVBO(0),
      // Quad
      mQuadVAO(0),
      mQuadVBO(0)
  {
  }

  void
  TextureRemoveFromGLTask::execute
  ()
  {
    LOG_TRACE("TextureRemoveFromGLTask: for texture {}, Executing on Graphics thread",
              mTextureID);

    // Textures
    if (mTextureID > 0) glDeleteTextures(1,&mTextureID);
    if (mEquiToCubeTexture > 0) glDeleteTextures(1,&mEquiToCubeTexture);
    if (mIrradianceMapTexture > 0) glDeleteTextures(1,&mIrradianceMapTexture);
    if (mPreFilterTexture > 0) glDeleteTextures(1,&mPreFilterTexture);
    if (mBrdfLutTexture > 0) glDeleteTextures(1,&mBrdfLutTexture);

    // Environment
    if (mCaptureRBO > 0) glDeleteRenderbuffers(1,&mCaptureRBO);
    if (mCaptureFBO > 0) glDeleteFramebuffers(1,&mCaptureFBO);

    // Cube
    if(mCubeVBO > 0) glDeleteBuffers(1, &mCubeVBO);
    if(mCubeVAO > 0) glDeleteVertexArrays(1, &mCubeVAO);
    // Quad
    if(mQuadVBO > 0) glDeleteBuffers(1, &mQuadVBO);
    if(mQuadVAO > 0) glDeleteVertexArrays(1, &mQuadVAO);

    GLCheckError();

    clearVariables();

    setState(TaskState::TASK_STATE_COMPLETED);
  }

  void
  TextureRemoveFromGLTask::setTextureID
  (GLuint id)
  {
    mTextureID = id;
  }

  void
  TextureRemoveFromGLTask::setCaptureBuffers
  (GLuint fbo, GLuint rbo)
  {
    mCaptureFBO = fbo;
    mCaptureRBO = rbo;
  }

  void
  TextureRemoveFromGLTask::setEquiToCubeTexture
  (GLuint texture)
  {
    mEquiToCubeTexture = texture;
  }

  void
  TextureRemoveFromGLTask::setIrradianceTexture
  (GLuint texture)
  {
    mIrradianceMapTexture = texture;
  }

  void
  TextureRemoveFromGLTask::setPreFilterTexture
  (GLuint texture)
  {
    mPreFilterTexture = texture;
  }

  void
  TextureRemoveFromGLTask::setBrdfLutTexture
  (GLuint texture)
  {
    mBrdfLutTexture = texture;
  }

  void
  TextureRemoveFromGLTask::setCubeBuffers
  (GLuint VAO, GLuint VBO)
  {
    mCubeVAO = VAO;
    mCubeVBO = VBO;
  }

  void
  TextureRemoveFromGLTask::setQuadBuffers
  (GLuint VAO, GLuint VBO)
  {
    mQuadVAO = VAO;
    mQuadVBO = VBO;
  }

  void
  TextureRemoveFromGLTask::clearVariables
  ()
  {
    // Base Texture
    GLuint mTextureID = 0;
    // Environment
    GLuint mCaptureFBO = 0;
    GLuint mCaptureRBO = 0;
    GLuint mEquiToCubeTexture = 0;
    GLuint mIrradianceMapTexture = 0;
    GLuint mPreFilterTexture = 0;
    GLuint mBrdfLutTexture = 0;
    // Cube
    GLuint mCubeVAO = 0;
    GLuint mCubeVBO = 0;
    // Quad
    GLuint mQuadVAO = 0;
    GLuint mQuadVBO = 0;
  }
}
