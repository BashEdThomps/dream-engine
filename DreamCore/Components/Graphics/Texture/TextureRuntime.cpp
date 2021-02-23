#include "TextureRuntime.h"
#include "TextureDefinition.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Cache.h"
#include "Storage/File.h"
#include "Storage/StorageManager.h"
#include "Storage/ProjectDirectory.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::make_shared;

namespace octronic::dream
{
    TextureRuntime::TextureRuntime
    (ProjectRuntime* rt, TextureDefinition* def)
        : SharedAssetRuntime(rt, def),
          mIsHDR(false),
          mGLTextureID(0),
          mWidth(0),
          mHeight(0),
          mChannels(0),
          mImage(nullptr),
          // FBO/RBO
          mCaptureFBO(0),
          mCaptureRBO(0),
          // Equirectangular to Cube Map
          mEquiToCubeTexture(0),
          mEquiToCubeShader(nullptr),
          // Irradiance Map
          mIrradianceMapTexture(0),
          mIrradianceMapShader(nullptr),
          // Pre Filter
          mPreFilterCubeMapTexture(0),
          mPreFilterShader(nullptr),
          // BRDF LUT
          mBrdfLutTexture(0),
          mBrdfLutShader(nullptr),
          // Cube
          mCubeVAO(0),
          mCubeVBO(0),
          // Quad
          mQuadVAO(0),
          mQuadVBO(0),
          // Tasks
          mLoadIntoGLTask(make_shared<TextureLoadIntoGLTask>(rt, this)),
          mRenderCubeMapTask(make_shared<TextureSetupEnvironmentTask>(rt, this)),
          mRemoveFromGLTask(make_shared<TextureRemoveFromGLTask>(rt))
    {

    }

    TextureRuntime::~TextureRuntime
    ()
    {
        auto gfxDestructionQueue = mProjectRuntimeHandle->getGraphicsComponent()->getDestructionTaskQueue();
        gfxDestructionQueue->pushTask(mRemoveFromGLTask);
    }

    int
    TextureRuntime::getWidth
    () const
    {
        return mWidth;
    }

    void
    TextureRuntime::setWidth
    (int width)
    {
        mWidth = width;
    }

    int
    TextureRuntime::getHeight
    () const
    {
        return mHeight;
    }

    void
    TextureRuntime::setHeight
    (int height)
    {
        mHeight = height;
    }

    int
    TextureRuntime::getChannels
    () const
    {
        return mChannels;
    }

    void
    TextureRuntime::setChannels
    (int channels)
    {
        mChannels = channels;
    }

    void*
    TextureRuntime::getImage
    () const
    {
        return mImage;
    }

    void
    TextureRuntime::setImage
    (void* image)
    {
        mImage = image;
    }

    GLuint
    TextureRuntime::getTextureID
    () const
    {
        return mGLTextureID;
    }

    void
    TextureRuntime::setTextureID
    (const GLuint& gLID)
    {
        mGLTextureID = gLID;
    }

    bool
    TextureRuntime::operator==
    (const TextureRuntime& other)
    {
        return this->mGLTextureID == other.mGLTextureID && this->mUuid == other.mUuid;
    }

    bool
    TextureRuntime::loadFromDefinition
    ()
    {
        LOG_TRACE("TextureRuntime: {}",__FUNCTION__);

        // All aboard
        string filename = mProjectRuntimeHandle
                ->getProject()
                ->getDirectory()
                ->getAssetAbsolutePath(mDefinitionHandle->getUuid());

        StorageManager* fm = mProjectRuntimeHandle->getStorageManager();
        File* txFile = fm->openFile(filename);

        if (!txFile->exists())
        {
            LOG_ERROR("TextureRuntime: Texture file does not exist: {}",filename);
            fm->closeFile(txFile);
            txFile = nullptr;
            mLoadError = true;
            return false;
        }

        LOG_DEBUG("TextureRuntime: Loading texture: {}",filename);

        if (!txFile->readBinary())
        {
            LOG_ERROR("TextureRuntime: Unable to read file data");
            fm->closeFile(txFile);
            txFile = nullptr;
            mLoadError = true;
            return false;
        }

        TextureDefinition* txDef = static_cast<TextureDefinition*>(mDefinitionHandle);

        uint8_t* buffer = txFile->getBinaryData();
        size_t buffer_sz = txFile->getBinaryDataSize();

        if (txDef->getFlipVertical())
        {
            stbi_set_flip_vertically_on_load(true);
        }
        else
        {
            stbi_set_flip_vertically_on_load(false);
        }

        if (stbi_is_hdr_from_memory(buffer,buffer_sz))
        {
            mIsHDR = true;
            mImage = (float*)stbi_loadf_from_memory(static_cast<const stbi_uc*>(buffer),
                                                    buffer_sz, &mWidth, &mHeight, &mChannels, 0);
        }
        else
        {
            mImage = (uint8_t*)stbi_load_from_memory(static_cast<const stbi_uc*>(buffer),
                                                     buffer_sz, &mWidth, &mHeight, &mChannels, 0);
        }
        fm->closeFile(txFile);
        txFile = nullptr;

        LOG_DEBUG(
                    "TextureRuntime: Loaded texture {} with width {}, height {}, channels {}",
                    filename, mWidth,mHeight,mChannels);


        mIsEnvironmentTexture = txDef->getIsEnvironmentTexture();

        if (mIsEnvironmentTexture)
        {
            auto shaderCache = mProjectRuntimeHandle->getShaderCache();
            if (txDef->getEquiToCubeMapShader() != Uuid::INVALID)
            {
                mEquiToCubeShader = shaderCache->getRuntimeHandle(txDef->getEquiToCubeMapShader());
            }

            if (txDef->getIrradianceMapShader() != Uuid::INVALID)
            {
                mIrradianceMapShader = shaderCache->getRuntimeHandle(txDef->getIrradianceMapShader());
            }

            if (txDef->getPreFilterShader() != Uuid::INVALID)
            {
                mPreFilterShader = shaderCache->getRuntimeHandle(txDef->getPreFilterShader());
            }

            if (txDef->getBrdfLutShader() != Uuid::INVALID)
            {
                mBrdfLutShader = shaderCache->getRuntimeHandle(txDef->getBrdfLutShader());
            }

            glGenFramebuffers(1, &mCaptureFBO);
            glGenRenderbuffers(1, &mCaptureRBO);
            mRemoveFromGLTask->setCaptureBuffers(mCaptureFBO,mCaptureRBO);
        }

        return true;
    }

    bool TextureRuntime::loadTextureIntoGL()
    {
        LOG_TRACE("TextureRuntime: {}",__FUNCTION__);
        // Assign texture to ID

        glGenTextures(1, &mGLTextureID);
        GLCheckError();


        glBindTexture(GL_TEXTURE_2D, mGLTextureID);
        GLCheckError();
        LOG_DEBUG("TextureRuntime: Bound to texture id {}",mGLTextureID);

        if (mIsHDR)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, getWidth(), getHeight(), 0, GL_RGB, GL_FLOAT, getImage());
        }
        else
        {
            switch (mChannels)
            {
                case 1:
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    GLCheckError();
#if defined (GL_ES_VERSION_3_0)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, getWidth(), getHeight(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, getImage());
#else
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, getWidth(), getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, getImage());
#endif
                    break;
                case 3:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,getImage());
                    break;
                case 4:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,getImage());
                    break;
                default:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glDeleteTextures(1, &mGLTextureID);
                    LOG_ERROR("TextureRuntime: Texture has unsupported number of channels: {}", mChannels);
                    return false;
            }
            GLCheckError();
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        GLCheckError();

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        GLCheckError();

        if (mIsHDR)
        {
            delete (float*)mImage;
        }
        else
        {
            delete (uint8_t*)mImage;
        }
        setImage(nullptr);
        mLoaded = true;

        mRemoveFromGLTask->setTextureID(mGLTextureID);

        return true;
    }

    bool TextureRuntime::getIsEnvironmentTexture() const
    {
        return mIsEnvironmentTexture;
    }

    void TextureRuntime::setIsEnvironmentTexture(bool b)
    {
        mIsEnvironmentTexture = b;
    }

    bool TextureRuntime::renderEquirectangularToCubeMap()
    {
        if (mEquiToCubeShader == nullptr)
        {
            LOG_WARN("TextureRuntime: CubeMap Shader is nullptr");
            return false;
        }

        if (!mEquiToCubeShader->getLoaded())
        {
            LOG_WARN("TextureRuntime: CubeMap shader is not loaded");
            return false;
        }

        glGenFramebuffers(1, &mCaptureFBO);
        GLCheckError();
        glGenRenderbuffers(1, &mCaptureRBO);
        GLCheckError();

        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
        GLCheckError();

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        GLCheckError();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCaptureRBO);
        GLCheckError();

        glGenTextures(1, &mEquiToCubeTexture);
        GLCheckError();
        glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
        GLCheckError();

        for (unsigned int i = 0; i < 6; ++i)
        {
            // note that we store each face with 16 bit floating point values
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
            GLCheckError();
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLCheckError();


        // convert HDR equirectangular environment map to cubemap equivalent
        mEquiToCubeShader->use();
        mEquiToCubeShader->setEquirectangularMapUniform(0);
        mEquiToCubeShader->setProjectionMatrixUniform(CubeCaptureProjection);

        glActiveTexture(GL_TEXTURE0);
        GLCheckError();
        glBindTexture(GL_TEXTURE_2D, mGLTextureID);
        GLCheckError();

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        GLCheckError();

        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();

        for (unsigned int i = 0; i < 6; ++i)
        {
            mEquiToCubeShader->setViewMatrixUniform(CubeCaptureViews[i]);
            mEquiToCubeShader->syncUniforms();

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mEquiToCubeTexture, 0);
            GLCheckError();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GLCheckError();

            renderCube(); // renders a 1x1 cube
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCheckError();

        mRemoveFromGLTask->setEquiToCubeTexture(mEquiToCubeTexture);

        return true;
    }

    bool
    TextureRuntime::renderIrradianceCubeMap
    ()
    {
        if (mIrradianceMapShader == nullptr)
        {
            LOG_WARN("TextureRuntime: Irradiance Map Shader is nullptr");
            return false;
        }

        if (!mIrradianceMapShader->getLoaded())
        {
            LOG_WARN("TextureRuntime: Irradiance Map shader is not loaded");
            return false;
        }

        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------
        glGenTextures(1, &mIrradianceMapTexture);
        GLCheckError();

        glBindTexture(GL_TEXTURE_CUBE_MAP, mIrradianceMapTexture);
        GLCheckError();

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
            GLCheckError();
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLCheckError();

        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
        GLCheckError();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
        GLCheckError();

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        // -----------------------------------------------------------------------------
        mIrradianceMapShader->use();
        mIrradianceMapShader->setEnvironmentMapUniform(0);
        mIrradianceMapShader->setProjectionMatrixUniform(CubeCaptureProjection);

        glActiveTexture(GL_TEXTURE0);
        GLCheckError();
        glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
        GLCheckError();

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        GLCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();

        for (unsigned int i = 0; i < 6; ++i)
        {
            mIrradianceMapShader->setViewMatrixUniform(CubeCaptureViews[i]);
            mIrradianceMapShader->syncUniforms();

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mIrradianceMapTexture, 0);
            GLCheckError();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GLCheckError();

            renderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCheckError();

        mRemoveFromGLTask->setIrradianceTexture(mIrradianceMapTexture);

        return true;
    }

    // renderCube() renders a 1x1 3D cube in NDC.
    // -------------------------------------------------
    void TextureRuntime::renderCube()
    {
        // initialize (if necessary)
        if (mCubeVAO == 0)
        {
            static float vertices[] =
            {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
            };
            glGenVertexArrays(1, &mCubeVAO);
            GLCheckError();
            glGenBuffers(1, &mCubeVBO);
            GLCheckError();
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
            GLCheckError();
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            GLCheckError();
            // link vertex attributes
            glBindVertexArray(mCubeVAO);
            GLCheckError();
            glEnableVertexAttribArray(0);
            GLCheckError();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            GLCheckError();
            glEnableVertexAttribArray(1);
            GLCheckError();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            GLCheckError();
            glEnableVertexAttribArray(2);
            GLCheckError();
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            GLCheckError();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            GLCheckError();
            glBindVertexArray(0);
            GLCheckError();
            mRemoveFromGLTask->setCubeBuffers(mCubeVAO, mCubeVBO);
        }
        // render Cube
        glBindVertexArray(mCubeVAO);
        GLCheckError();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        GLCheckError();
        glBindVertexArray(0);
        GLCheckError();
    }

    // renderQuad() renders a 1x1 XY quad in NDC
    // -----------------------------------------
    void TextureRuntime::renderQuad()
    {
        if (mQuadVAO == 0)
        {
            static float quadVertices[] =
            {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &mQuadVAO);
            GLCheckError();
            glGenBuffers(1, &mQuadVBO);
            GLCheckError();
            glBindVertexArray(mQuadVAO);
            GLCheckError();
            glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
            GLCheckError();
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            GLCheckError();
            glEnableVertexAttribArray(0);
            GLCheckError();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            GLCheckError();
            glEnableVertexAttribArray(1);
            GLCheckError();
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            GLCheckError();
            mRemoveFromGLTask->setQuadBuffers(mQuadVAO, mQuadVBO);
        }
        glBindVertexArray(mQuadVAO);
        GLCheckError();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        GLCheckError();
        glBindVertexArray(0);
        GLCheckError();
    }

    bool TextureRuntime::renderPreFilterCubeMap()
    {

        if (mPreFilterShader == nullptr)
        {
            LOG_WARN("TextureRuntime: PreFilter Shader is nullptr");
            return false;
        }

        if (!mPreFilterShader->getLoaded())
        {
            LOG_WARN("TextureRuntime: PreFilter shader is not loaded");
            return false;
        }

        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
        glGenTextures(1, &mPreFilterCubeMapTexture);
        GLCheckError();
        glBindTexture(GL_TEXTURE_CUBE_MAP, mPreFilterCubeMapTexture);
        GLCheckError();
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
            GLCheckError();
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
        GLCheckError();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLCheckError();
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        GLCheckError();

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        // ----------------------------------------------------------------------------------------------------
        mPreFilterShader->use();
        mPreFilterShader->setEnvironmentMapUniform(0);
        mPreFilterShader->setProjectionMatrixUniform(CubeCaptureProjection);
        glActiveTexture(GL_TEXTURE0);
        GLCheckError();
        glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
        GLCheckError();

        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
            GLCheckError();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            GLCheckError();
            glViewport(0, 0, mipWidth, mipHeight);
            GLCheckError();

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            mPreFilterShader->setRoughnessUniform(roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                mPreFilterShader->setViewMatrixUniform(CubeCaptureViews[i]);
                mPreFilterShader->syncUniforms();
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mPreFilterCubeMapTexture, mip);
                GLCheckError();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GLCheckError();
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCheckError();
        mRemoveFromGLTask->setPreFilterTexture(mPreFilterCubeMapTexture);
        return true;
    }

    bool TextureRuntime::renderBrdfLut()
    {
        if (mBrdfLutShader == nullptr)
        {
            LOG_WARN("TextureRuntime: BRDF_LUT Shader is nullptr");
            return false;
        }

        if (!mBrdfLutShader->getLoaded())
        {
            LOG_WARN("TextureRuntime: BRDF_LUT shader is not loaded");
            return false;
        }

        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------
        glGenTextures(1, &mBrdfLutTexture);
        GLCheckError();

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, mBrdfLutTexture);
        GLCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        GLCheckError();
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLCheckError();

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
        GLCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
        GLCheckError();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        GLCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBrdfLutTexture, 0);
        GLCheckError();

        glViewport(0, 0, 512, 512);
        GLCheckError();
        mBrdfLutShader->use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();
        renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCheckError();

        mRemoveFromGLTask->setBrdfLutTexture(mBrdfLutTexture);

        return true;
    }

    GLuint TextureRuntime::getCubeMapTextureID() const
    {
        return mEquiToCubeTexture;
    }

    GLuint TextureRuntime::getIrradianceTextureID() const
    {
        return mIrradianceMapTexture;
    }
    GLuint TextureRuntime::getPreFilterTextureID() const
    {
        return mPreFilterCubeMapTexture;
    }

    GLuint TextureRuntime::getBrdfLutTextureID() const
    {
        return mBrdfLutTexture;
    }

    bool TextureRuntime::isHDR() const
    {
        return mIsHDR;
    }

    void TextureRuntime::pushTasks()
    {
        auto taskQueue = mProjectRuntimeHandle->getTaskQueue();
        auto gfxTaskQueue = mProjectRuntimeHandle->getGraphicsComponent()->getTaskQueue();
        auto gfxDestructionQueue = mProjectRuntimeHandle->getGraphicsComponent()->getDestructionTaskQueue();

        if (mReloadFlag)
        {
            mReloadFlag = false;

            gfxDestructionQueue->pushTask(mRemoveFromGLTask);
            mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);
            mLoadIntoGLTask->setState(TASK_STATE_QUEUED);
            mRenderCubeMapTask->setState(TASK_STATE_QUEUED);

            mLoaded = false;
            mLoadError = false;
            mIsHDR = false;
            mGLTextureID = 0;
            mWidth = 0;
            mHeight = 0;
            mChannels = 0;
            mImage = nullptr;

            // Environment
            mCaptureFBO = 0;
            mCaptureRBO = 0;

            mEquiToCubeTexture = 0;
            mEquiToCubeShader = nullptr;

            mIrradianceMapTexture = 0;
            mIrradianceMapShader = nullptr;

            mPreFilterCubeMapTexture = 0;
            mPreFilterShader = nullptr;

            mBrdfLutTexture = 0;
            mBrdfLutShader = nullptr;

            // Cube
            mCubeVAO = 0;
            mCubeVBO = 0;

            // Quad
            mQuadVAO = 0;
            mQuadVBO = 0;
        }
        else if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
            taskQueue->pushTask(mLoadFromDefinitionTask);
        }
        else
        {
            if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) &&
                mLoadIntoGLTask->hasState(TASK_STATE_QUEUED))
            {
                gfxTaskQueue->pushTask(mLoadIntoGLTask);
            }

            if (mIsEnvironmentTexture &&
                mLoadIntoGLTask->hasState(TASK_STATE_COMPLETED) &&
                mRenderCubeMapTask->hasState(TASK_STATE_QUEUED))
            {
                gfxTaskQueue->pushTask(mRenderCubeMapTask);
            }
        }
    }

    // Static ==================================================================
    const glm::mat4 TextureRuntime::CubeCaptureProjection =
            glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    const glm::mat4 TextureRuntime::CubeCaptureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
}
