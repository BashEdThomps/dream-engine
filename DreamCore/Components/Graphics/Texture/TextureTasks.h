#pragma once

#include "Common/GLHeader.h"

#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class TextureRuntime;

    class TextureLoadIntoGLTask : public GraphicsTask
    {
        TextureRuntime* mTextureRuntimeHandle;
    public:
        TextureLoadIntoGLTask(ProjectRuntime* pr, TextureRuntime* rt);
        void execute();
    };

    class TextureSetupEnvironmentTask : public GraphicsTask
    {
        TextureRuntime* mTextureRuntimeHandle;
    public:
        TextureSetupEnvironmentTask(ProjectRuntime* pr, TextureRuntime* rt);
        void execute() override;
    };

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
        TextureRemoveFromGLTask(ProjectRuntime* pr);
        void setTextureID(GLuint id);
        void setCaptureBuffers(GLuint fbo, GLuint rbo);
        void setEquiToCubeTexture(GLuint texture);
        void setIrradianceTexture(GLuint texture);
        void setPreFilterTexture(GLuint texture);
        void setBrdfLutTexture(GLuint texture);
        void setCubeBuffers(GLuint VAO, GLuint VBO);
        void setQuadBuffers(GLuint VAO, GLuint VBO);
        void execute() override;
    };
}
