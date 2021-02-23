#include "TextureRuntime.h"
#include "TextureTasks.h"

namespace octronic::dream
{

    // TextureLoadIntoGLTask ===================================================

    TextureLoadIntoGLTask::TextureLoadIntoGLTask(ProjectRuntime* pr, TextureRuntime* rt)
        : GraphicsTask(pr, "TextureLoadIntoGLTask"),
          mTextureRuntimeHandle(rt)
    {
    }

    void TextureLoadIntoGLTask::execute()
    {
        LOG_TRACE("TextureLoadIntoGLTask: {} Executing on Graphics thread",mTextureRuntimeHandle->getNameAndUuidString());
        if (!mTextureRuntimeHandle->loadTextureIntoGL())
        {
            setState(TaskState::TASK_STATE_FAILED);
        }
        else
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
    }

    // TextureRenderCubeMapTsk =================================================

    TextureSetupEnvironmentTask::TextureSetupEnvironmentTask
    (ProjectRuntime* pr, TextureRuntime* rt)
        : GraphicsTask(pr, "TextureRenderCubeMapTask"),
          mTextureRuntimeHandle(rt)
    {

    }

    void TextureSetupEnvironmentTask::execute()
    {
        if (!mTextureRuntimeHandle->renderEquirectangularToCubeMap() ||
            !mTextureRuntimeHandle->renderIrradianceCubeMap() ||
            !mTextureRuntimeHandle->renderPreFilterCubeMap() ||
            !mTextureRuntimeHandle->renderBrdfLut())
        {
            setState(TaskState::TASK_STATE_DEFERRED);
        }
        else
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
    }

    // TextureRemoveFromGLTask =================================================

    TextureRemoveFromGLTask::TextureRemoveFromGLTask
    (ProjectRuntime* pr)
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
        LOG_TRACE("TextureRemoveFromGLTask: for texture {}, Executing on Graphics thread",mTextureID);
        // Base Texture
        if (mTextureID > 0) glDeleteTextures(1,&mTextureID);
        // Environment
        if (mCaptureFBO > 0) glDeleteBuffers(1,&mCaptureFBO);
        if (mCaptureRBO > 0) glDeleteBuffers(1,&mCaptureRBO);
        if (mEquiToCubeTexture > 0) glDeleteTextures(1,&mEquiToCubeTexture);
        if (mIrradianceMapTexture > 0) glDeleteTextures(1,&mIrradianceMapTexture);
        if (mPreFilterTexture > 0) glDeleteTextures(1,&mPreFilterTexture);
        if (mBrdfLutTexture > 0) glDeleteTextures(1,&mBrdfLutTexture);
        // Cube
        if(mCubeVAO > 0) glDeleteBuffers(1, &mCubeVAO);
        if(mCubeVBO > 0) glDeleteBuffers(1, &mCubeVBO);
        // Quad
        if(mQuadVAO > 0) glDeleteBuffers(1, &mQuadVAO);
        if(mQuadVBO > 0) glDeleteBuffers(1, &mQuadVBO);

        // Base Texture
        mTextureID = 0;
        // Environment
        mCaptureFBO = 0;
        mCaptureRBO = 0;
        mEquiToCubeTexture = 0;
        mIrradianceMapTexture = 0;
        mPreFilterTexture = 0;
        mBrdfLutTexture = 0;
        // Cube
        mCubeVAO = 0;
        mCubeVBO = 0;
        // Quad
        mQuadVAO = 0;
    	mQuadVBO = 0;

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
}
