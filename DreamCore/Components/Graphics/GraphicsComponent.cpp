/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "Entity/EntityDefinition.h"
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
using std::static_pointer_cast;

namespace octronic::dream
{
  GraphicsComponent::GraphicsComponent
  (const weak_ptr<ProjectRuntime>& pr)
    : Component(pr),
      mMaxFrameBufferSize(0),
      // Shadow Pass Vars
      mShadowPassFB(0),
      mShadowPassDepthBuffer(0),
      mShadowMatrix(mat4(1.0f)),
      // Sprite
      mSpriteQuadVAO(0),
      mSpriteQuadVBO(0),
      // Tasks
      mTaskQueue(make_shared<GraphicsTaskQueue>("GraphicsTaskQueue")),
      mDestructionTaskQueue(make_shared<GraphicsDestructionTaskQueue>("GraphicsDestructionTaskQueue")),
      mSetupBuffersTask(make_shared<SetupBuffersTask>(pr)),
      mResizeTask(make_shared<ResizeTask>(pr)),
      mRenderTask(make_shared<RenderTask>(pr)),
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
    return true;
  }

  bool
  GraphicsComponent::setupBuffers
  ()
  {
    LOG_DEBUG("GraphicsComponent: setupBuffers");

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto wcLock = prLock->getWindowComponent().lock())
      {
        // Define the viewport dimensions
        int windowWidth  = wcLock->getWidth();
        int windowHeight = wcLock->getHeight();

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
    }
    return false;
  }

  bool
  GraphicsComponent::handleResize
  ()
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto wcLock = prLock->getWindowComponent().lock())
      {
        if (wcLock->getWindowSizeChangedFlag())
        {
          LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
          // Define the viewport dimensions
          int windowWidth  = wcLock->getWidth();
          int windowHeight = wcLock->getHeight();

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
          wcLock->setWindowSizeChangedFlag(false);
        }
        return true;
      }
    }
    return false;
  }

  void
  GraphicsComponent::clearBuffers
  (const weak_ptr<SceneRuntime>& sr)
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto wcLock = prLock->getWindowComponent().lock())
      {
        if (auto srLock = sr.lock())
        {
          auto clear = srLock->getClearColor();

          glBindFramebuffer(GL_FRAMEBUFFER,wcLock->getFrameBuffer());
          GLCheckError();

          glClearColor(clear.r,clear.g,clear.b,clear.a);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          glEnable(GL_DEPTH_TEST);
          glDepthFunc(GL_LEQUAL);
        }
      }
    }
  }

  // Environment =============================================================

  void
  GraphicsComponent::renderEnvironment
  (const weak_ptr<SceneRuntime>& sr)
  {

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto srLock = sr.lock())
      {
        if (auto envShader = srLock->getEnvironmentShader().lock())
        {
          if (!envShader->getLoaded()) return;

          if (auto envTexture = srLock->getEnvironmentTexture().lock())
          {
            if (!envTexture->getLoaded()) return;

            if (auto wcLock = prLock->getWindowComponent().lock())
            {
              wcLock->bindFrameBuffer();
              GLCheckError();

              // set depth function to less than AND equal for skybox depth trick.
              // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
              glEnable(GL_DEPTH_TEST);
              glDepthFunc(GL_LEQUAL);
              glDisable(GL_CULL_FACE);

              if (auto camera = srLock->getCamera().lock())
              {
                envShader->use();
                envShader->setViewMatrixUniform(camera->getViewMatrix());
                envShader->setProjectionMatrixUniform(camera->getProjectionMatrix());

                GLuint envTextureId = 0;

                if (envTexture->getCubeDebugMode() == CUBE_DEBUG_NONE)
                {
                  envTextureId = envTexture->getCubeMapTextureID();
                }
                else
                {
                  envTextureId = envTexture->getCubeDebugTexture();
                }
                envShader->setTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, envTextureId);
                envShader->syncUniforms();
                envTexture->renderCube();
              }
            }
          }
        }
      }
    }


  }

  // Geometry Pass ============================================================

  void
  GraphicsComponent::renderModels
  (const weak_ptr<SceneRuntime>& sr)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto srLock = sr.lock())
      {
        if (auto camera = srLock->getCamera().lock())
        {
          if (auto shaderCache = prLock->getShaderCache().lock())
          {
            if (auto windowComp = prLock->getWindowComponent().lock())
            {
              checkFrameBufferDimensions();

              LOG_DEBUG("GraphicsComponent: ==> Running Model Render Pass");

              glBindFramebuffer(GL_FRAMEBUFFER,windowComp->getFrameBuffer());
              GLCheckError();

              glEnable(GL_CULL_FACE);
              glCullFace(GL_BACK);

              if (auto envTexture = srLock->getEnvironmentTexture().lock())
              {
                for (auto runtime : shaderCache->getRuntimeVector())
                {
                  if (auto shader = runtime.lock())
                  {

                    LOG_DEBUG("GraphicsComponent: Rending shader {}",shader->getNameAndUuidString());

                    if (shader->countMaterials() == 0)
                    {
                      LOG_DEBUG("GraphicsComponent: Shader {} has no materials",shader->getNameAndUuidString());
                      continue;
                    }

                    if(shader->use())
                    {
                      LOG_INFO("GraphicsComponent: Shader {} all good, rendering geometry pass",shader->getNameAndUuidString());
                      shader->setViewMatrixUniform(camera->getViewMatrix());
                      shader->setProjectionMatrixUniform(camera->getProjectionMatrix());
                      shader->setCameraPositionUniform(camera->getTransform().getTranslation());

                      shader->setIrradianceTextureUniform(envTexture->getIrradianceTextureID());
                      shader->setPreFilterTextureUniform(envTexture->getPreFilterTextureID());
                      shader->setBrdfLutTextureUniform(envTexture->getBrdfLutTextureID());
                      shader->setLightPositionsUniform(&mLightPositions[0], 4);
                      shader->setLightColorsUniform(&mLightColors[0], 4);

                      for (auto material : shader->getMaterialsVector())
                      {
                        if (auto matLock = material.lock())
                        {
                          if (matLock->getUsedByVector().size() == 0) continue;

                          shader->bindMaterial(material);
                          for (auto modelMesh : matLock->getUsedByVector())
                          {
                            if (auto modelLock = modelMesh.lock())
                            {
                              if (modelLock->getLoaded())
                              {
                                modelLock->drawModelRuntimes(camera,shader);
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
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
  (const weak_ptr<SceneRuntime>& sr)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto srLock = sr.lock())
      {
        if (auto shadowPassShader = srLock->getShadowPassShader().lock())
        {
          if (!shadowPassShader->getLoaded()) return;

          if (auto shaderCache = prLock->getShaderCache().lock())
          {
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

            if (auto shadowLight = mShadowLight.lock())
            {

              mat4 lightMat = shadowLight->getTransform().getMatrix();
              // TODO - could this just be the light transform matrix?
              mat4 lightView = glm::lookAt(
                    shadowLight->getTransform().getTranslation(), // Light Pos
                    vec3(glm::translate(lightMat,vec3(0,0,-far_plane))[3]),//vec3(0.0f),  // target
                  vec3(0.0f,1.0f,0.0f) // Up
                  );

              //DirLight dir = mShadowLight->getLightRuntime()->getDirectionalLightData();
              //vec3 dirVec = dir.direction.toGLM();
              //mat4 lightView = eulerAngleYXZ(dirVec.y,dirVec.x,dirVec.z);
              mShadowMatrix = lightProjection*lightView;

              glDisable(GL_CULL_FACE);
              GLCheckError();

              if(shadowPassShader->use())
              {
                //shadowPassShader->setLightSpaceMatrixUniform(lightMat);

                for (auto runtime : shaderCache->getRuntimeVector())
                {
                  if (auto nextShader = runtime.lock())
                  {
                    nextShader->drawShadowPass(shadowPassShader);
                  }
                }
              }
            }
          }
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
  (const weak_ptr<SceneRuntime>& sceneRuntime)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    LOG_DEBUG("==> Running Font Pass");

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto srLock = sceneRuntime.lock())
      {
        if (auto fontShader = srLock->getFontShader().lock())
        {
          if (fontShader->getLoaded())
          {
            LOG_ERROR("GraphicsComponent: Font shader not found");
            return;
          }

          if (auto wcLock = prLock->getWindowComponent().lock())
          {
            wcLock->bindFrameBuffer();

            glEnable(GL_BLEND);
            GLCheckError();
            glDisable(GL_DEPTH_TEST);
            GLCheckError();

            if (auto fontCache = prLock->getFontCache().lock())
            {

              if (fontCache->runtimeCount() == 0) return;

              // Activate the Font Shader
              fontShader->use();

              fontShader->setProjectionMatrixUniform(mScreenSpaceProjectionMatrix);

              // Iterate through FontRuntimes
              for (auto saRuntime : fontCache->getRuntimeVector())
              {
                if (auto fontRuntime = saRuntime.lock())
                {
                  // Iterate through FontTextInstances
                  for (auto entityWeak : fontRuntime->getInstanceVector())
                  {
                    if (auto entity = entityWeak.lock())
                    {
                      fontShader->setModelMatrixUniform(entity->getTransform().getMatrix());
                      GLCheckError();
                      fontShader->setColorUniform(entity->getFontColor());
                      fontShader->syncUniforms();
                      GLCheckError();
                      fontRuntime->draw(entity);
                    }
                  }
                }
              }
            }
          }
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
  (const weak_ptr<SceneRuntime>& sceneRuntime)
  {
    LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
    LOG_DEBUG("==> Running Sprite Pass");

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto srLock = sceneRuntime.lock())
      {
        if (auto shader = srLock->getSpriteShader().lock())
        {
          if (auto wcLock = prLock->getWindowComponent().lock())
          {
            wcLock->bindFrameBuffer();

            glEnable(GL_BLEND);
            GLCheckError();

            glDisable(GL_DEPTH_TEST);
            GLCheckError();

            if (auto textureCache = prLock->getTextureCache().lock())
            {

              if (textureCache->runtimeCount() == 0) return;

              if (shader == nullptr)
              {
                LOG_ERROR("GraphicsComponent: Sprite shader not found");
                return;
              }

              if (!shader->getLoaded()) return;

              // Activate the Shader
              shader->use();
              shader->setProjectionMatrixUniform(mScreenSpaceProjectionMatrix);
              shader->bindVertexArray(mSpriteQuadVAO);

              // Iterate through Textureuntimes
              for (auto textureRuntime : textureCache->getRuntimeVector())
              {
                if (auto trLock = textureRuntime.lock())
                {

                  // None
                  if (trLock->getInstanceVector().empty()) continue;

                  glBindTexture(GL_TEXTURE_2D, trLock->getTextureID());
                  GLCheckError();

                  // Iterate through SpriteRuntimes
                  for (auto entityRuntime : trLock->getInstanceVector())
                  {
                    if (auto erLock = entityRuntime.lock())
                    {
                      shader->setModelMatrixUniform(erLock->getTransform().getMatrix());
                      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                      GLCheckError();
                    }
                  }
                }
              }
            }
          }
        }
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
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto wcLock = prLock->getWindowComponent().lock())
      {
        int width = wcLock->getWidth();
        int height = wcLock->getHeight();
        assert(mMaxFrameBufferSize > 0);
        assert(width > 0 && width < mMaxFrameBufferSize && height > 0 && height < mMaxFrameBufferSize);
      }
    }
  }

  void
  GraphicsComponent::logShaders
  ()
  {
    LOG_DEBUG("GraphicsComponent: Contents of shader cache");
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scLock = prLock->getShaderCache().lock())
      {
        for (auto runtime : scLock->getRuntimeVector())
        {
          if (auto shader = runtime.lock())
          {
            LOG_DEBUG("GraphicsComponent: {}",shader->getNameAndUuidString());
            shader->logMaterials();
          }
        }
      }
    }
  }

  void
  GraphicsComponent::pushTasks
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      // Materials
      if (auto materialCache = prLock->getMaterialCache().lock())
      {
        for (auto material : materialCache->getRuntimeVector())
        {
          if (auto matLock = material.lock())
          {
            matLock->pushTasks();
          }
        }
      }

      // Models
      if (auto modelCache = prLock->getModelCache().lock())
      {
        for (auto model : modelCache->getRuntimeVector())
        {
          if (auto mLock = model.lock())
          {
            mLock->pushTasks();
          }
        }
      }

      // Shaders
      if (auto shaderCache = prLock->getShaderCache().lock())
      {
        for (auto shader : shaderCache->getRuntimeVector())
        {
          if (auto sLock = shader.lock())
          {
            sLock->pushTasks();
          }
        }
      }

      // Textures
      if (auto textureCache = prLock->getTextureCache().lock())
      {
        for (auto texture : textureCache->getRuntimeVector())
        {
          if (auto txLock = texture.lock())
          {
            txLock->pushTasks();
          }
        }
      }

      // Fonts
      if (auto fontCache = prLock->getFontCache().lock())
      {
        for (auto font : fontCache->getRuntimeVector())
        {
          if (auto fLock = font.lock())
          {
            fLock->pushTasks();
          }
        }
      }

      // This

      mTaskQueue->pushTask(mResizeTask);

      if (mSetupBuffersTask->hasState(TASK_STATE_QUEUED))
      {
        mTaskQueue->pushTask(mSetupBuffersTask);
      }

      if (mSetupBuffersTask->hasState(TASK_STATE_COMPLETED))
      {
        mTaskQueue->pushTask(mRenderTask);
      }
    }
  }

  weak_ptr<GraphicsTaskQueue>
  GraphicsComponent::getTaskQueue
  ()
  const
  {
    return mTaskQueue;
  }

  weak_ptr<GraphicsDestructionTaskQueue>
  GraphicsComponent::getDestructionTaskQueue
  ()
  const
  {
    return mDestructionTaskQueue;
  }
}
