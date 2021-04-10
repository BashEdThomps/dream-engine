#include "GraphicsComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include "CameraRuntime.h"
#include "Font/FontRuntime.h"
#include "Model/ModelRuntime.h"
#include "Model/ModelMesh.h"
#include "Shader/ShaderRuntime.h"

#include "Common/GLHeader.h"
#include "Common/Logger.h"

#include "Components/Cache.h"
#include "Math/Transform.h"
#include "Components/Time.h"
#include "Components/Window/WindowComponent.h"

#include "Scene/SceneRuntime.h"
#include "Entity/TemplateEntityDefinition.h"
#include "Entity/EntityRuntime.h"

#include "Project/ProjectRuntime.h"

#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

using glm::vec2;
using glm::vec3;
using glm::ortho;
using std::make_shared;
using std::make_unique;

namespace octronic::dream
{
  GraphicsComponent::GraphicsComponent
  (ProjectRuntime& pr)
    : Component(),
      // Shadow Pass Vars
      mShadowPassFB(0),
      mShadowPassDepthBuffer(0),
      mShadowMatrix(mat4(1.0f)),
      // Sprite
      mSpriteQuadVAO(0),
      mSpriteQuadVBO(0),
      // Tasks
      mTaskQueue("GraphicsTaskQueue"),
      mDestructionTaskQueue("GraphicsDestructionTaskQueue"),
      mMaxFrameBufferSize(0),
      mLightPositions{
  {-20.0f,  20.0f, 20.0f},
  { 20.0f,  20.0f, 20.0f},
  {-20.0f, -20.0f, 20.0f},
  { 20.0f, -20.0f, 20.0f}
},
      mLightColors{
  {300.0f, 300.0f, 300.0f},
  {300.0f, 300.0f, 300.0f},
  {300.0f, 300.0f, 300.0f},
  {300.0f, 300.0f, 300.0f}
}
  {
    setProjectRuntime(pr);
    LOG_TRACE("GraphicsComponent: Constructing");
  }

  GraphicsComponent::~GraphicsComponent()
  {
    LOG_TRACE("GraphicsComponent: Destroying Object");
    freeShadowBuffers();
    freeSpriteQuad();
  }

  // Init/Setup ===============================================================

  bool
  GraphicsComponent::init
  ()
  {
    mSetupBuffersTask = make_shared<SetupBuffersTask>(mProjectRuntime.value());
    mResizeTask = make_shared<ResizeTask>(mProjectRuntime.value());
    mRenderTask = make_shared<RenderTask>(mProjectRuntime.value());
    return true;
  }

  bool
  GraphicsComponent::setupBuffers
  ()
  {
    LOG_DEBUG("GraphicsComponent: setupBuffers");

    auto& wc = mProjectRuntime.value().get().getWindowComponent();
    // Define the viewport dimensions
    int windowWidth  = wc.getWidth();
    int windowHeight = wc.getHeight();

    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&mMaxFrameBufferSize);

    LOG_DEBUG("GraphicsComponent: Max FB size = {}x{}",mMaxFrameBufferSize,mMaxFrameBufferSize);

    if (windowWidth > mMaxFrameBufferSize || windowHeight > mMaxFrameBufferSize)
    {
      LOG_ERROR("GraphicsComponent: Requested framebuffer larger than max "
                      "supported size {}x{} > {}x{}",
                windowWidth, windowHeight, mMaxFrameBufferSize, mMaxFrameBufferSize);
      return false;
    }

    if (windowWidth == 0 || windowHeight == 0)
    {
      LOG_DEBUG("GraphicsComponent: Window size not available");
      return false;
    }

    freeShadowBuffers();
    if (!setupShadowBuffers())
    {
      LOG_ERROR("GraphicsComponent: Unable to create shadow pass buffers");
      return false;
    }

    freeSpriteQuad();
    if (!setupSpriteQuad())
    {
      LOG_ERROR("GraphicsComponent: Unable to create screen quad");
      return false;
    }

    LOG_DEBUG("GraphicsComponent: Initialisation Done.");

    return true;
  }

  bool
  GraphicsComponent::handleResize
  ()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    auto& wc = mProjectRuntime.value().get().getWindowComponent();
    if (wc.getWindowSizeChangedFlag())
    {
      LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
      // Define the viewport dimensions
      int windowWidth  = wc.getWidth();
      int windowHeight = wc.getHeight();

      if (windowWidth == 0 || windowHeight == 0)
      {
        LOG_ERROR("GraphicsComponent: Cannot setup buffers with 0,0 viewport");
        return false;
      }

      // Setup Screen Space
      glViewport(0, 0, windowWidth, windowHeight);
      mScreenSpaceProjectionMatrix = ortho(
            0.f, (float)windowWidth,
            0.f, (float)windowHeight,
            -1.f, 1.f);

      GLCheckError();

      LOG_DEBUG("GraphicsComponent: Window dimensions changed: width: {}, height: {}",windowWidth, windowHeight);
      wc.setWindowSizeChangedFlag(false);
    }
    return true;
  }

  void
  GraphicsComponent::clearBuffers
  (SceneRuntime& sr)
  {
    auto& wc = mProjectRuntime.value().get().getWindowComponent();
    auto clear = sr.getClearColor();

    glBindFramebuffer(GL_FRAMEBUFFER,wc.getFrameBuffer());
    GLCheckError();

    glClearColor(clear.r,clear.g,clear.b,clear.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  }

  // Environment =============================================================

  void
  GraphicsComponent::renderEnvironment
  (SceneRuntime& sr)
  {
    auto envShaderOpt = sr.getEnvironmentShader();
    if (!envShaderOpt || !envShaderOpt.value().get().getLoaded()) return;

    auto envTextureOpt = sr.getEnvironmentTexture();
    if (!envTextureOpt || !envTextureOpt.value().get().getLoaded()) return;

    auto& wc = mProjectRuntime.value().get().getWindowComponent();
    wc.bindFrameBuffer();
    GLCheckError();

    // set depth function to less than AND equal for skybox depth trick.
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    auto& camera = sr.getCameraRuntime();
    auto& envShader = envShaderOpt.value().get();
    envShader.use();
    envShader.setViewMatrixUniform(camera.getViewMatrix());
    envShader.setProjectionMatrixUniform(camera.getProjectionMatrix());

    GLuint envTextureId = 0;

    auto& envTexture = envTextureOpt.value().get();
    if (envTexture.getCubeDebugMode() == CUBE_DEBUG_NONE)
    {
      envTextureId = envTexture.getCubeMapTextureID();
    }
    else
    {
      envTextureId = envTexture.getCubeDebugTexture();
    }
    envShader.setTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, envTextureId);
    envShader.syncUniforms();
    envTexture.renderCube();
  }

  // Geometry Pass ============================================================

  void
  GraphicsComponent::renderModels
  (SceneRuntime& sr)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

    auto& camera = sr.getCameraRuntime();
    auto& shaderCache = mProjectRuntime.value().get().getShaderCache();
    auto& windowComp = mProjectRuntime.value().get().getWindowComponent();
    checkFrameBufferDimensions();

    LOG_DEBUG("GraphicsComponent: ==> Running Model Render Pass");

    glBindFramebuffer(GL_FRAMEBUFFER,windowComp.getFrameBuffer());
    GLCheckError();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto envTextureOpt = sr.getEnvironmentTexture();
    if (envTextureOpt)
    {
      auto& envTexture = envTextureOpt.value().get();

      for (auto shaderWrapper : shaderCache.getRuntimeVector())
      {
        auto& shader = shaderWrapper.get();
        LOG_DEBUG("GraphicsComponent: Rending shader {}",shader.getNameAndUuidString());

        if (shader.countMaterials() == 0)
        {
          LOG_DEBUG("GraphicsComponent: Shader {} has no materials",shader.getNameAndUuidString());
          continue;
        }

        if(shader.use())
        {
          LOG_INFO("GraphicsComponent: Shader {} all good, rendering geometry pass",shader.getNameAndUuidString());
          shader.setViewMatrixUniform(camera.getViewMatrix());
          shader.setProjectionMatrixUniform(camera.getProjectionMatrix());
          shader.setCameraPositionUniform(camera.getTransform().getTranslation());

          shader.setIrradianceTextureUniform(envTexture.getIrradianceTextureID());
          shader.setPreFilterTextureUniform(envTexture.getPreFilterTextureID());
          shader.setBrdfLutTextureUniform(envTexture.getBrdfLutTextureID());
          shader.setLightPositionsUniform(&mLightPositions[0], 4);
          shader.setLightColorsUniform(&mLightColors[0], 4);

          for (auto& materialWrap : shader.getMaterialsVector())
          {
            auto& material = materialWrap.get();
            if (material.getUsedByVector().size() == 0) continue;

            shader.bindMaterial(material);
            for (auto& modelMeshWrap : material.getUsedByVector())
            {
              auto& modelMesh = modelMeshWrap.get();

              if (modelMesh.getLoaded())
              {
                modelMesh.drawModelRuntimes(camera,shader);
              }
            }
          }
        }
      }
    }
  }

  // Shadow Pass ==============================================================

  bool
  GraphicsComponent::setupShadowBuffers
  ()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    glGenFramebuffers(1,&mShadowPassFB);
    GLCheckError();

    assert(SHADOW_SIZE > 0);

    if (mShadowPassFB == 0)
    {
      LOG_ERROR("GraphicsComponent: Unable to create shadow pass FB");
      return false;
    }

    glGenTextures(1, &mShadowPassDepthBuffer);
    GLCheckError();

    if (mShadowPassDepthBuffer == 0)
    {

      LOG_ERROR("GraphicsComponent: Unable to create shadow pass depth buffer");
      return false;
    }

    glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);
    GLCheckError();
#if defined (GL_ES_VERSION_3_0)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_SIZE, SHADOW_SIZE, 0,GL_DEPTH_COMPONENT,GL_UNSIGNED_SHORT,nullptr);
    GLCheckError();
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
    GLCheckError();
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    GLCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLCheckError();
#if defined (GL_ES_VERSION_3_0)
#else
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    GLCheckError();
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
    GLCheckError();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowPassDepthBuffer, 0);
    GLCheckError();

#if defined (GL_ES_VERSION_3_0)
    //glDrawBuffers(1, GL_NONE);
    //GLCheckError();
#else
    glDrawBuffer(GL_NONE);
    GLCheckError();
#endif
    glReadBuffer(GL_NONE);
    GLCheckError();

    return true;
  }

  void
  GraphicsComponent::renderShadowPass
  (SceneRuntime& sr)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

    auto shadowPassShaderOpt = sr.getShadowPassShader();
    if (!shadowPassShaderOpt || !shadowPassShaderOpt.value().get().getLoaded()) return;

    auto& shaderCache = getProjectRuntime().getShaderCache();
    LOG_DEBUG("==> Running Shadow Render Pass");

    glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
    GLCheckError();

    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    GLCheckError();

    glClear(GL_DEPTH_BUFFER_BIT);
    GLCheckError();

    // Setup Uniforms
    static float near_plane = 1.0f;
    static float far_plane = 300.0f;
    static float sz = 100.f;

    static glm::mat4 lightProjection = glm::ortho(-sz, sz, -sz, sz, near_plane, far_plane);

    if (mShadowLight)
    {

      mat4 lightMat = mShadowLight.value().get().getTransform().getMatrix();
      // TODO - could this just be the light transform matrix?
      mat4 lightView = glm::lookAt(
            mShadowLight.value().get().getTransform().getTranslation(),
            vec3(glm::translate(lightMat,vec3(0,0,-far_plane))[3]),
          vec3(0.0f,1.0f,0.0f)); // Up

      //DirLight dir = mShadowLight.value().getLightRuntime().value().getDirectionalLightData();
      //vec3 dirVec = dir.direction.toGLM();
      //mat4 lightView = eulerAngleYXZ(dirVec.y,dirVec.x,dirVec.z);
      mShadowMatrix = lightProjection*lightView;

      glDisable(GL_CULL_FACE);
      GLCheckError();

      auto &shadowPassShader = shadowPassShaderOpt.value().get();
      if(shadowPassShader.use())
      {
        //shadowPassShader.value().get().setLightSpaceMatrixUniform(lightMat);

        for (auto shaderWrapper : shaderCache.getRuntimeVector())
        {
          auto& shader = shaderWrapper.get();
          shader.drawShadowPass(shadowPassShader);
        }
      }
    }
  }

  void GraphicsComponent::freeShadowBuffers()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    if (mShadowPassFB != 0)
    {
      glDeleteFramebuffers(1, &mShadowPassFB);
      GLCheckError();
    }

    if (mShadowPassDepthBuffer != 0)
    {
      glDeleteTextures(1, &mShadowPassDepthBuffer);
      GLCheckError();
    }
  }

  // Font ====================================================================

  void
  GraphicsComponent::renderFonts
  (SceneRuntime& sceneRuntime)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    LOG_DEBUG("==> Running Font Pass");

    auto fontShaderOpt = sceneRuntime.getFontShader();
    if (!fontShaderOpt || fontShaderOpt.value().get().getLoaded())
    {
      LOG_ERROR("GraphicsComponent: Font shader not found");
      return;
    }

    auto& wc = mProjectRuntime.value().get().getWindowComponent();
    {
      wc.bindFrameBuffer();

      glEnable(GL_BLEND);
      GLCheckError();
      glDisable(GL_DEPTH_TEST);
      GLCheckError();

      auto& fontCache = mProjectRuntime.value().get().getFontCache();

      if (fontCache.runtimeCount() == 0) return;

      auto& fontShader = fontShaderOpt.value().get();

      // Activate the Font Shader
      fontShader.use();

      fontShader.setProjectionMatrixUniform(mScreenSpaceProjectionMatrix);

      // Iterate through FontRuntimes
      for (auto& fontRuntimeWrapper : fontCache.getRuntimeVector())
      {
        auto& fontRuntime = fontRuntimeWrapper.get();
        // Iterate through FontTextInstances
        for (auto& entityWrapper : fontRuntime.getInstanceVector())
        {
          auto& entity = entityWrapper.get();
          fontShader.setModelMatrixUniform(entity.getTransform().getMatrix());
          GLCheckError();
          fontShader.setColorUniform(entity.getFontColor());
          fontShader.syncUniforms();
          GLCheckError();
          fontRuntime.draw(entity);
        }
      }
    }
  }

  // Sprite ==================================================================

  void GraphicsComponent::freeSpriteQuad()
  {
    if (mSpriteQuadVBO > 0) glDeleteBuffers(1,&mSpriteQuadVBO);
    if (mSpriteQuadVAO > 0) glDeleteBuffers(1,&mSpriteQuadVAO);
  }

  bool
  GraphicsComponent::setupSpriteQuad()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

    static float quadVertices[] = {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // setup plane VAO
    glGenVertexArrays(1, &mSpriteQuadVAO);
    GLCheckError();
    glGenBuffers(1, &mSpriteQuadVBO);
    GLCheckError();
    glBindVertexArray(mSpriteQuadVAO);
    GLCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, mSpriteQuadVBO);
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
    glBindVertexArray(0);
    GLCheckError();
    return true;
  }

  void
  GraphicsComponent::renderSprites
  (SceneRuntime& sceneRuntime)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    LOG_DEBUG("==> Running Sprite Pass");

    auto shaderOpt = sceneRuntime.getSpriteShader();
    if (!shaderOpt || !shaderOpt.value().get().getLoaded()) return;

    auto& pr = mProjectRuntime.value().get();
    auto& windowComp = pr.getWindowComponent();
    windowComp.bindFrameBuffer();

    glEnable(GL_BLEND);
    GLCheckError();

    glDisable(GL_DEPTH_TEST);
    GLCheckError();

    auto& textureCache = pr.getTextureCache();
    if (textureCache.runtimeCount() == 0) return;

    auto& shader = shaderOpt.value().get();
    // Activate the Shader
    shader.use();
    shader.setProjectionMatrixUniform(mScreenSpaceProjectionMatrix);
    shader.bindVertexArray(mSpriteQuadVAO);

    // Iterate through Textureuntimes
    for (auto& textureRuntimeWrapper : textureCache.getRuntimeVector())
    {
      auto& textureRuntime = textureRuntimeWrapper.get();
      // None
      if (textureRuntime.getInstanceVector().empty()) continue;

      glBindTexture(GL_TEXTURE_2D, textureRuntime.getTextureID());
      GLCheckError();

      // Iterate through SpriteRuntimes
      for (auto& erWrapper : textureRuntime.getInstanceVector())
      {
        auto& er = erWrapper.get();
        shader.setModelMatrixUniform(er.getTransform().getMatrix());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        GLCheckError();
      }
    }
  }

  // Lights ==============================================================

  vec3
  GraphicsComponent::getLightPosition
  (size_t index)
  const
  {
    return mLightPositions[index];
  }

  void
  GraphicsComponent::setLightPosition
  (size_t index, const vec3& p)
  {
    mLightPositions[index] = p;
  }

  vec3
  GraphicsComponent::getLightColor
  (size_t index)
  const
  {
    return mLightColors[index];
  }

  void
  GraphicsComponent::setLightColor
  (size_t index, const vec3& p)
  {
    mLightColors[index] = p;
  }

  size_t
  GraphicsComponent::getLightCount
  ()
  const
  {
    return GC_LIGHT_COUNT;
  }

  // Accessors ================================================================

  GLuint
  GraphicsComponent::getShadowPassDepthBuffer
  ()
  const
  {
    return mShadowPassDepthBuffer;
  }

  void
  GraphicsComponent::checkFrameBufferDimensions
  ()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&mMaxFrameBufferSize);
    auto& pr = mProjectRuntime.value().get();
    auto& windowComponent = pr.getWindowComponent();
    int width = windowComponent.getWidth();
    int height = windowComponent.getHeight();
    assert(mMaxFrameBufferSize > 0);
    assert(width > 0 && width < mMaxFrameBufferSize && height > 0 && height < mMaxFrameBufferSize);
  }

  void
  GraphicsComponent::pushTasks
  ()
  {
    auto& pr = mProjectRuntime.value().get();
    // Materials
    MaterialCache& materialCache = pr.getMaterialCache();
    for (auto& material : materialCache.getRuntimeVector())
    {
      material.get().pushTasks();
    }

    // Models
    ModelCache& modelCache = pr.getModelCache();
    for (auto& model : modelCache.getRuntimeVector())
    {
      model.get().pushTasks();
    }

    // Shaders
    ShaderCache& shaderCache = pr.getShaderCache();
    for (auto& shader : shaderCache.getRuntimeVector())
    {
      shader.get().pushTasks();
    }

    // Textures
    TextureCache& textureCache = pr.getTextureCache();
    for (auto& texture : textureCache.getRuntimeVector())
    {
      texture.get().pushTasks();
    }

    // Fonts
    FontCache& fontCache = pr.getFontCache();
    for (auto& font : fontCache.getRuntimeVector())
    {
      font.get().pushTasks();
    }

    // This

    mTaskQueue.pushTask(mResizeTask);

    if (mSetupBuffersTask->hasState(TASK_STATE_QUEUED))
    {
      mTaskQueue.pushTask(mSetupBuffersTask);
    }

    if (mSetupBuffersTask->hasState(TASK_STATE_COMPLETED))
    {
      mTaskQueue.pushTask(mRenderTask);
    }
  }

  GraphicsTaskQueue&
  GraphicsComponent::getTaskQueue
  ()
  {
    return mTaskQueue;
  }

  GraphicsDestructionTaskQueue&
  GraphicsComponent::getDestructionTaskQueue
  ()
  {
    return mDestructionTaskQueue;
  }
}
