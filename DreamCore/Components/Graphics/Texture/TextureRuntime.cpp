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
using std::static_pointer_cast;

namespace octronic::dream
{
  TextureRuntime::TextureRuntime
  (const weak_ptr<ProjectRuntime>& rt,
   const weak_ptr<TextureDefinition>& def)
    : SharedAssetRuntime(rt, def),
      mCubeDebugMode(CUBE_DEBUG_NONE),
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
      // Irradiance Map
      mIrradianceMapTexture(0),
      // Pre Filter
      mPreFilterCubeMapTexture(0),
      // BRDF LUT
      mBrdfLutTexture(0),
      // Cube
      mCubeVAO(0),
      mCubeVBO(0),
      // Quad
      mQuadVAO(0),
      mQuadVBO(0)
  {

  }

  TextureRuntime::~TextureRuntime
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto gcLock = prLock->getGraphicsComponent().lock())
      {
        if (auto gfxDq = gcLock->getDestructionTaskQueue().lock())
        {
          gfxDq->pushTask(mRemoveFromGLTask);
        }
      }
    }

  }

  bool
  TextureRuntime::init
  ()
  {
    // Init Tasks
    mLoadIntoGLTask = make_shared<TextureLoadIntoGLTask>(mProjectRuntime, static_pointer_cast<TextureRuntime>(shared_from_this()));
    mRenderCubeMapTask = make_shared<TextureSetupEnvironmentTask>(mProjectRuntime, static_pointer_cast<TextureRuntime>(shared_from_this()));
    mRemoveFromGLTask = make_shared<TextureRemoveFromGLTask>(mProjectRuntime);
    return true;
  }

  bool
  TextureRuntime::operator==
  (const TextureRuntime& other)
  {
    return
        this->mGLTextureID == other.mGLTextureID &&
        this->mUuid == other.mUuid;
  }

  // Accessors ===============================================================

  int
  TextureRuntime::getWidth
  ()
  const
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
  ()
  const
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
  ()
  const
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
  ()
  const
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
  ()
  const
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
  TextureRuntime::getIsEnvironmentTexture
  () const
  {
    return mIsEnvironmentTexture;
  }

  void
  TextureRuntime::setIsEnvironmentTexture
  (bool b)
  {
    mIsEnvironmentTexture = b;
  }

  GLuint
  TextureRuntime::getCubeMapTextureID
  () const
  {
    return mEquiToCubeTexture;
  }

  GLuint
  TextureRuntime::getIrradianceTextureID
  () const
  {
    return mIrradianceMapTexture;
  }

  GLuint
  TextureRuntime::getPreFilterTextureID
  () const
  {
    return mPreFilterCubeMapTexture;
  }

  GLuint
  TextureRuntime::getBrdfLutTextureID
  () const
  {
    return mBrdfLutTexture;
  }

  bool
  TextureRuntime::isHDR
  () const
  {
    return mIsHDR;
  }

  CubeDebugMode
  TextureRuntime::getCubeDebugMode
  () const
  {
    return mCubeDebugMode;
  }

  void
  TextureRuntime::setCubeDebugMode
  (const CubeDebugMode& debugMode)
  {
    mCubeDebugMode = debugMode;
  }

  GLuint
  TextureRuntime::getCubeDebugTexture
  ()
  {
    switch (mCubeDebugMode)
    {
      case CUBE_DEBUG_NONE:
        return 0;
      case CUBE_DEBUG_ENVIRONMENT:
        return mEquiToCubeTexture;
      case CUBE_DEBUG_IRRADIANCE:
        return mIrradianceMapTexture;
      case CUBE_DEBUG_PREFILTER:
        return mPreFilterCubeMapTexture;
    }
  }

  // Loading =================================================================

  bool
  TextureRuntime::loadFromDefinition
  ()
  {
    LOG_TRACE("TextureRuntime: {}",__FUNCTION__);

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto pDir = prLock->getProjectDirectory().lock())
      {
        if (auto defLock = mDefinition.lock())
        {

          string filename = pDir->getAssetAbsolutePath(defLock->getUuid());

          if (auto storageMan = prLock->getStorageManager().lock())
          {
            if (auto txFile = storageMan->openFile(filename).lock())
            {
              if (!txFile->exists())
              {
                LOG_ERROR("TextureRuntime: Texture file does not exist: {}",filename);
                storageMan->closeFile(txFile);
                mLoadError = true;
                return false;
              }

              LOG_DEBUG("TextureRuntime: Loading texture: {}",filename);

              if (!txFile->readBinary())
              {
                LOG_ERROR("TextureRuntime: Unable to read file data");
                storageMan->closeFile(txFile);
                mLoadError = true;
                return false;
              }

              auto txDef = static_pointer_cast<TextureDefinition>(defLock);

              vector<uint8_t> buffer = txFile->getBinaryData();
              size_t buffer_sz = buffer.size();

              if (txDef->getFlipVertical())
              {
                stbi_set_flip_vertically_on_load(true);
              }
              else
              {
                stbi_set_flip_vertically_on_load(false);
              }

              if (stbi_is_hdr_from_memory(&buffer[0],buffer_sz))
              {
                mIsHDR = true;
                mImage = (float*)stbi_loadf_from_memory(
                      static_cast<const stbi_uc*>(&buffer[0]),
                    buffer_sz, &mWidth, &mHeight, &mChannels, 0);
              }
              else
              {
                mImage = (uint8_t*)stbi_load_from_memory(
                      static_cast<const stbi_uc*>(&buffer[0]),
                    buffer_sz, &mWidth, &mHeight, &mChannels, 0);
              }

              storageMan->closeFile(txFile);

              LOG_DEBUG("TextureRuntime: Loaded texture {} with width {}, height {}, channels {}",
                        filename, mWidth,mHeight,mChannels);


              mIsEnvironmentTexture = txDef->getIsEnvironmentTexture();

              if (mIsEnvironmentTexture)
              {
                if (auto shaderCache = prLock->getShaderCache().lock())
                {
                  if (txDef->getEquiToCubeMapShader() != Uuid::INVALID)
                  {
                    mEquiToCubeShader = shaderCache->getRuntime(txDef->getEquiToCubeMapShader());
                  }
                  else
                  {
                    return false;
                  }

                  if (txDef->getIrradianceMapShader() != Uuid::INVALID)
                  {
                    mIrradianceMapShader = shaderCache->getRuntime(txDef->getIrradianceMapShader());
                  }
                  else
                  {
                    return false;
                  }

                  if (txDef->getPreFilterShader() != Uuid::INVALID)
                  {
                    mPreFilterShader = shaderCache->getRuntime(txDef->getPreFilterShader());
                  }
                  else
                  {
                    return false;
                  }

                  if (txDef->getBrdfLutShader() != Uuid::INVALID)
                  {
                    mBrdfLutShader = shaderCache->getRuntime(txDef->getBrdfLutShader());
                  }
                  else
                  {
                    return false;
                  }
                }
                return true;
              }
            }
          }
        }
      }
    }
    return false;
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
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, getWidth(), getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, getImage());
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, 0);
    GLCheckError();

    stbi_image_free(mImage);
    setImage(nullptr);
    mLoaded = true;

    mRemoveFromGLTask->setTextureID(mGLTextureID);

    if (mIsEnvironmentTexture)
    {
      glGenFramebuffers(1, &mCaptureFBO);
      glGenRenderbuffers(1, &mCaptureRBO);
      glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
      glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCaptureRBO);
      mRemoveFromGLTask->setCaptureBuffers(mCaptureFBO,mCaptureRBO);
    }

    return true;
  }

  bool TextureRuntime::renderEquirectangularToCubeMap()
  {
    if (auto equiShader = mEquiToCubeShader.lock())
    {
      if (!equiShader->getLoaded())
      {
        LOG_WARN("TextureRuntime: CubeMap shader is not loaded");
        return false;
      }

      glGenTextures(1, &mEquiToCubeTexture);
      glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
      GLCheckError();

      for (unsigned int i = 0; i < 6; ++i)
      {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        GLCheckError();
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GLCheckError();


      // convert HDR equirectangular environment map to cubemap equivalent
      equiShader->use();
      equiShader->setEquirectangularMapUniform(0);
      equiShader->setProjectionMatrixUniform(CubeCaptureProjection);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, mGLTextureID);
      GLCheckError();

      glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
      GLCheckError();

      glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
      GLCheckError();

      for (unsigned int i = 0; i < 6; ++i)
      {
        equiShader->setViewMatrixUniform(CubeCaptureViews[i]);
        equiShader->syncUniforms();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               mEquiToCubeTexture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

        renderCube(); // renders a 1x1 cube
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      GLCheckError();

      // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
      glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

      mRemoveFromGLTask->setEquiToCubeTexture(mEquiToCubeTexture);
      return true;
    }
    return false;
  }

  bool
  TextureRuntime::renderIrradianceCubeMap
  ()
  {

    if (auto irradianceMapShader  = mIrradianceMapShader.lock())
    {

      if (!irradianceMapShader->getLoaded())
      {
        LOG_WARN("TextureRuntime: Irradiance Map shader is not loaded");
        return false;
      }


      // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
      // --------------------------------------------------------------------------------
      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1, &mIrradianceMapTexture);
      glBindTexture(GL_TEXTURE_CUBE_MAP, mIrradianceMapTexture);
      GLCheckError();

      for (unsigned int i = 0; i < 6; ++i)
      {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        GLCheckError();
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GLCheckError();

      glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
      glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
      GLCheckError();

      // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
      // -----------------------------------------------------------------------------
      irradianceMapShader->use();
      irradianceMapShader->setEnvironmentMapUniform(0);
      irradianceMapShader->setProjectionMatrixUniform(CubeCaptureProjection);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, mEquiToCubeTexture);
      GLCheckError();

      glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
      glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
      GLCheckError();

      for (unsigned int i = 0; i < 6; ++i)
      {
        irradianceMapShader->setViewMatrixUniform(CubeCaptureViews[i]);
        irradianceMapShader->syncUniforms();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               mIrradianceMapTexture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

        renderCube();
      }

      glBindTexture(GL_TEXTURE_2D, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      GLCheckError();

      mRemoveFromGLTask->setIrradianceTexture(mIrradianceMapTexture);
      return true;
    }
    return false;
  }

  bool TextureRuntime::renderPreFilterCubeMap()
  {
    if (auto preFilterShader = mPreFilterShader.lock())
    {

      if (!preFilterShader->getLoaded())
      {
        LOG_WARN("TextureRuntime: PreFilter shader is not loaded");
        return false;
      }

      // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

      // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
      // --------------------------------------------------------------------------------
      glGenTextures(1, &mPreFilterCubeMapTexture);
      glBindTexture(GL_TEXTURE_CUBE_MAP, mPreFilterCubeMapTexture);
      GLCheckError();

      for (unsigned int i = 0; i < 6; ++i)
      {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        GLCheckError();
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GLCheckError();

      // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
      GLCheckError();

      // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
      // ----------------------------------------------------------------------------------------------------
      preFilterShader->use();
      preFilterShader->setEnvironmentMapUniform(0);
      preFilterShader->setProjectionMatrixUniform(CubeCaptureProjection);

      glActiveTexture(GL_TEXTURE0);
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
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                              mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        GLCheckError();

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        preFilterShader->setRoughnessUniform(roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
          preFilterShader->setViewMatrixUniform(CubeCaptureViews[i]);
          preFilterShader->syncUniforms();

          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                 mPreFilterCubeMapTexture, mip);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          renderCube();

          GLCheckError();
        }
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      GLCheckError();

      mRemoveFromGLTask->setPreFilterTexture(mPreFilterCubeMapTexture);

      return true;
    }
    return false;
  }

  bool TextureRuntime::renderBrdfLut()
  {

    if (auto shaderLock = mBrdfLutShader.lock())
    {
      if (!shaderLock->getLoaded())
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
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
      GLCheckError();

      // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GLCheckError();

      // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
      glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
      glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBrdfLutTexture, 0);
      GLCheckError();

      glViewport(0, 0, 512, 512);
      GLCheckError();

      shaderLock->use();
      shaderLock->syncUniforms();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      GLCheckError();

      renderQuad();

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      GLCheckError();

      mRemoveFromGLTask->setBrdfLutTexture(mBrdfLutTexture);

      return true;

    }
    return false;
  }

  /**
     *  @brief renders a 1x1 3D cube in NDC.
    */
  void TextureRuntime::renderCube()
  {
    // initialize (if necessary)
    if (mCubeVAO == 0)
    {
      glGenVertexArrays(1, &mCubeVAO);
      glGenBuffers(1, &mCubeVBO);
      GLCheckError();

      // fill buffer
      glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
      GLCheckError();

      // link vertex attributes
      glBindVertexArray(mCubeVAO);
      // Pos
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      // Normal
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(2);
      // UV
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      GLCheckError();
      mRemoveFromGLTask->setCubeBuffers(mCubeVAO, mCubeVBO);
    }

    // render Cube
    glBindVertexArray(mCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    GLCheckError();
  }

  /**
     *  @brief Renders a 1x1 XY quad in NDC
     */
  void TextureRuntime::renderQuad()
  {
    if (mQuadVAO == 0)
    {
      // setup plane VAO
      glGenVertexArrays(1, &mQuadVAO);
      glGenBuffers(1, &mQuadVBO);
      glBindVertexArray(mQuadVAO);
      glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      GLCheckError();
      mRemoveFromGLTask->setQuadBuffers(mQuadVAO, mQuadVBO);
    }
    glBindVertexArray(mQuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    GLCheckError();
  }

  // Tasks ===================================================================

  void TextureRuntime::pushTasks()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto prTaskQueue = prLock->getTaskQueue().lock())
      {
        if (auto gcLock = prLock->getGraphicsComponent().lock())
        {
          if (auto gfxQueue = gcLock->getTaskQueue().lock())
          {
            if (auto gfxDestQueue = gcLock->getDestructionTaskQueue().lock())
            {
              if (mReloadFlag)
              {
                if (mRemoveFromGLTask->hasState(TASK_STATE_QUEUED))
                {
                  gfxDestQueue->pushTask(mRemoveFromGLTask);

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
                  mEquiToCubeShader.reset();

                  mIrradianceMapTexture = 0;
                  mIrradianceMapShader.reset();

                  mPreFilterCubeMapTexture = 0;
                  mPreFilterShader.reset();

                  mBrdfLutTexture = 0;
                  mBrdfLutShader.reset();

                  // Cube
                  mCubeVAO = 0;
                  mCubeVBO = 0;

                  // Quad
                  mQuadVAO = 0;
                  mQuadVBO = 0;
                }
                else if (mRemoveFromGLTask->hasState(TASK_STATE_COMPLETED))
                {
                  mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);
                  mLoadIntoGLTask->setState(TASK_STATE_QUEUED);
                  mRenderCubeMapTask->setState(TASK_STATE_QUEUED);
                  mRemoveFromGLTask->clearVariables();
                  mRemoveFromGLTask->setState(TASK_STATE_QUEUED);
                  mReloadFlag = false;
                }
              }
              else if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
              {
                prTaskQueue->pushTask(mLoadFromDefinitionTask);
              }
              else
              {
                if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) &&
                    mLoadIntoGLTask->hasState(TASK_STATE_QUEUED))
                {
                  gfxQueue->pushTask(mLoadIntoGLTask);
                }

                if (mIsEnvironmentTexture &&
                    mLoadIntoGLTask->hasState(TASK_STATE_COMPLETED) &&
                    mRenderCubeMapTask->hasState(TASK_STATE_QUEUED))
                {
                  gfxQueue->pushTask(mRenderCubeMapTask);
                }
              }
            }
          }
        }
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

  const float TextureRuntime::CubeVertices[288] =
  {
    // [ Translation(3) -- Normal(3) -- UV(2) ]
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

  const float TextureRuntime::QuadVertices[20] =
  {
    // positions        // texture Coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  };
}
