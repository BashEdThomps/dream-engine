#pragma once

#include "Common/GLHeader.h"

#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class TextureRuntime;

    class TextureLoadIntoGLTask : public GraphicsTask
    {
        weak_ptr<TextureRuntime> mTextureRuntime;
    public:
        TextureLoadIntoGLTask(const weak_ptr<ProjectRuntime>& pr,
                              const weak_ptr<TextureRuntime>& rt);
        void execute();
    };

    class TextureSetupEnvironmentTask : public GraphicsTask
    {
        weak_ptr<TextureRuntime> mTextureRuntime;
    public:
        TextureSetupEnvironmentTask(const weak_ptr<ProjectRuntime>& pr,
                                    const weak_ptr<TextureRuntime>& rt);
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
        TextureRemoveFromGLTask(const weak_ptr<ProjectRuntime>& pr);
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
