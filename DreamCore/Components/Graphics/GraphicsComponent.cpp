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

#include "Camera.h"
#include "Font/FontRuntime.h"
#include "Light/LightRuntime.h"
#include "Model/ModelRuntime.h"
#include "Model/ModelMesh.h"
#include "Shader/ShaderRuntime.h"

#include "Common/GLHeader.h"
#include "Common/Logger.h"
#include "Common/Math.h"

#include "Components/Cache.h"
#include "Components/Transform.h"
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

namespace octronic::dream
{
    GraphicsComponent::GraphicsComponent(ProjectRuntime* pr)
        : Component(pr),
          mMaxFrameBufferSize(0),
          // Geometry Pass Vars
          mGeometryPassFB(0),
          mGeometryPassPositionBuffer(0),
          mGeometryPassAlbedoBuffer(0),
          mGeometryPassNormalBuffer(0),
          mGeometryPassDepthBuffer(0),
          mGeometryPassIgnoreBuffer(0),
          // Shadow Pass Vars
          mShadowPassFB(0),
          mShadowPassDepthBuffer(0),
          mShadowMatrix(mat4(1.0f)),
          mShadowLight(nullptr),
          // Light Pass Vars
          mLightPassQuadVAO(0),
          mLightPassQuadVBO(0),
          // Tasks
          mTaskQueue("GraphicsTaskQueue"),
          mDestructionTaskQueue("GraphicsDestructionTaskQueue"),
          mSetupBuffersTask(make_shared<SetupBuffersTask>(pr)),
          mHandleResizeTask(make_shared<HandleResizeTask>(pr)),
          mRenderTask(make_shared<RenderTask>(pr))
    {
        LOG_TRACE("GraphicsComponent: Constructing");
        GLCheckError();
    }

    GraphicsComponent::~GraphicsComponent()
    {
        LOG_TRACE("GraphicsComponent: Destroying Object");
        freeGeometryBuffers();
        freeShadowBuffers();
    }

    // Init/Setup ===============================================================

    bool GraphicsComponent::init()
    {
        return true;
    }

    bool GraphicsComponent::setupBuffers()
    {
        LOG_DEBUG("GraphicsComponent: setupBuffers");

        // Define the viewport dimensions
        int windowWidth  = mProjectRuntime->getWindowComponent()->getWidth();
        int windowHeight = mProjectRuntime->getWindowComponent()->getHeight();

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

        freeGeometryBuffers();
        if (!setupGeometryBuffers())
        {
            LOG_ERROR("GraphicsComponent: Unable to create geometry buffers");
            return false;
        }

        freeShadowBuffers();
        if (!setupShadowBuffers())
        {
            LOG_ERROR("GraphicsComponent: Unable to create shadow pass buffers");
            return false;
        }

        freeLightPassQuad();
        if (!setupLightPassQuad())
        {
            LOG_ERROR("GraphicsComponent: Unable to create screen quad");
            return false;
        }

        LOG_DEBUG("GraphicsComponent: Initialisation Done.");

        return true;
    }

    bool GraphicsComponent::handleResize()
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (mProjectRuntime->getWindowComponent()->getWindowSizeChangedFlag())
        {
            LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
            // Define the viewport dimensions
            int windowWidth  = mProjectRuntime->getWindowComponent()->getWidth();
            int windowHeight = mProjectRuntime->getWindowComponent()->getHeight();

            if (windowWidth == 0 || windowHeight == 0)
            {
                LOG_ERROR("GraphicsComponent: Cannot setup buffers with 0,0 viewport");
                return false;
            }

            glViewport(0, 0, windowWidth, windowHeight);
            GLCheckError();
            freeGeometryBuffers();
            setupGeometryBuffers();

            LOG_DEBUG("GraphicsComponent: Window dimensions changed: width: {}, height: {}",windowWidth, windowHeight);
            mProjectRuntime->getWindowComponent()->setWindowSizeChangedFlag(false);
        }
        return true;
    }

    // Geometry Pass ============================================================

    void GraphicsComponent::renderGeometryPass(SceneRuntime* sr)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        if (sr == nullptr)
        {
            LOG_TRACE("GraphicsComponent: SceneRuntime == nullptr");
            return;
        }

        auto shaderCache = mProjectRuntime->getShaderCache();

        int width = mProjectRuntime->getWindowComponent()->getWidth();
        int height = mProjectRuntime->getWindowComponent()->getHeight();

        checkFrameBufferDimensions();

        LOG_DEBUG("GraphicsComponent: ==> Running Geometry Render Pass");
        // Setup
        glViewport(0, 0,width ,height);
        GLCheckError();
        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER,mGeometryPassFB);
        GLCheckError();
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();
        glDisable(GL_BLEND);
        GLCheckError();
        glEnable(GL_DEPTH_TEST);
        GLCheckError();
        glDepthFunc(GL_LESS);
        GLCheckError();
        glEnable(GL_CULL_FACE);
        GLCheckError();
        glCullFace(GL_BACK);
        GLCheckError();

        LOG_DEBUG("GraphicsComponent: Geometry Pass post setup");

        // Clear the colorbuffer
        if (sr != nullptr)
        {
            LOG_DEBUG("GraphicsComponent: SR is valid, clear color specific");
            auto clearColour = sr->getClearColour();
            glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), 0.0f);
            GLCheckError();
        }
        else
        {
            LOG_DEBUG("GraphicsComponent: SR is NULL, clear color black");
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            GLCheckError();
            return;
        }

        auto camera = sr->getCamera();

        LOG_DEBUG("GraphicsComponent: Got camera, iterating shaders...");

        for (auto& runtime : *shaderCache->getRuntimeVectorHandle())
        {
            auto shader = static_cast<ShaderRuntime*>(runtime.get());

            LOG_DEBUG("GraphicsComponent: rending with shader {}",shader->getNameAndUuidString());

            if (shader->countMaterials() == 0)
            {
                LOG_DEBUG("GraphicsComponent: shader {} has no materials",shader->getNameAndUuidString());
                continue;
            }

            if(shader->use())
            {
                LOG_ERROR("GraphicsComponent: shader {} all good, rendering geometry pass",shader->getNameAndUuidString());
                shader->setViewMatrix(camera->getViewMatrix());
                shader->setProjectionMatrix(camera->getProjectionMatrix());
                shader->setViewerPosition(camera->getTranslation());
                shader->drawGeometryPass(camera);
            }
            else
            {
                LOG_ERROR("GraphicsComponent: shader {} use() call failed",shader->getNameAndUuidString());
            }
        }
    }

    void
    GraphicsComponent::freeGeometryBuffers
    ()
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (mGeometryPassFB > 0)
        {
            glDeleteFramebuffers(1, &mGeometryPassFB);
            GLCheckError();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        ShaderRuntime::CurrentTexture0 = 0;

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderRuntime::CurrentTexture1 = 0;

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderRuntime::CurrentTexture2 =0;

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderRuntime::CurrentTexture3 = 0;

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderRuntime::CurrentTexture4 = 0;

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderRuntime::CurrentTexture5 = 0;

        // - position color buffer
        if (mGeometryPassPositionBuffer > 0)
        {
            glDeleteTextures(1, &mGeometryPassPositionBuffer);
            GLCheckError();
        }

        // - normal color buffer
        if (mGeometryPassNormalBuffer > 0)
        {
            glDeleteTextures(1, &mGeometryPassNormalBuffer);
            GLCheckError();
        }

        // - color + specular color buffer
        if (mGeometryPassAlbedoBuffer > 0)
        {
            glDeleteTextures(1, &mGeometryPassAlbedoBuffer);
            GLCheckError();
        }

        // Depth Buffer
        if (mGeometryPassDepthBuffer > 0)
        {
            glDeleteRenderbuffers(1,&mGeometryPassDepthBuffer);
            GLCheckError();
        }

        // Ignore Buffer
        if (mGeometryPassIgnoreBuffer > 0)
        {
            glDeleteTextures(1,&mGeometryPassIgnoreBuffer);
            GLCheckError();
        }
    }

    bool
    GraphicsComponent::setupGeometryBuffers
    ()
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        auto width = mProjectRuntime->getWindowComponent()->getWidth();
        auto height = mProjectRuntime->getWindowComponent()->getHeight();

        checkFrameBufferDimensions();

        LOG_DEBUG("GraphicsComponent: Setting up Geometry Buffers with dimensions {}x{}",width,height);

        glGenFramebuffers(1,&mGeometryPassFB);
        GLCheckError();

        if (mGeometryPassFB == 0)
        {
            LOG_ERROR("GraphicsComponent: Unable to create Geometry Framebuffer");
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, mGeometryPassFB);
        GLCheckError();

        // 0 - position buffer
        glGenTextures(1, &mGeometryPassPositionBuffer);
        GLCheckError();
        glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
        GLCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GLCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGeometryPassPositionBuffer, 0);
        GLCheckError();

        // 1 - normal buffer
        glGenTextures(1, &mGeometryPassNormalBuffer);
        GLCheckError();
        glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
        GLCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GLCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGeometryPassNormalBuffer, 0);
        GLCheckError();

        // 2 - Albedo+specular buffer
        glGenTextures(1, &mGeometryPassAlbedoBuffer);
        GLCheckError();
        glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
        GLCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GLCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGeometryPassAlbedoBuffer, 0);
        GLCheckError();

        // 4 - Ignore Lighting Buffer
        glGenTextures(1, &mGeometryPassIgnoreBuffer);
        GLCheckError();
        glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
        GLCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width,height, 0, GL_RGB, GL_FLOAT, 0);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GLCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mGeometryPassIgnoreBuffer, 0);
        GLCheckError();

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[4] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3
        };
        glDrawBuffers(4, attachments);
        GLCheckError();

        // then also add render buffer object as depth buffer and check for completeness.
        glGenRenderbuffers(1, &mGeometryPassDepthBuffer);
        GLCheckError();
        glBindRenderbuffer(GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        GLCheckError();
#if defined (GL_ES_VERSION_3_0)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        GLCheckError();
#else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        GLCheckError();
#endif
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        GLCheckError();

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("GraphicsComponent: Deferred Rendering Framebuffer not complete!");
            return false;
        }
        else
        {
            LOG_DEBUG("GraphicsComponent: Deferred Rending Buffer is complete!");
        }
        return true;
    }

    // Lighting Pass ============================================================

    void GraphicsComponent::freeLightPassQuad()
    {
        if (mLightPassQuadVBO > 0) glDeleteBuffers(1,&mLightPassQuadVBO);
        if (mLightPassQuadVAO > 0) glDeleteBuffers(1,&mLightPassQuadVAO);
    }

    bool
    GraphicsComponent::setupLightPassQuad
    ()
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &mLightPassQuadVAO);
        GLCheckError();
        glGenBuffers(1, &mLightPassQuadVBO);
        GLCheckError();
        glBindVertexArray(mLightPassQuadVAO);
        GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, mLightPassQuadVBO);
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
    GraphicsComponent::renderLightingPass
    (SceneRuntime* sr)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        if (sr == nullptr) {
            LOG_TRACE("GraphicsComponent: SceneRuntime == nullptr");
            return;
        }

        ShaderRuntime* lightPassShader = sr->getLightingPassShader();
        if (lightPassShader == nullptr || !lightPassShader->getLoaded()) return;

        // Get window dimensions
        auto width = mProjectRuntime->getWindowComponent()->getWidth();
        auto height = mProjectRuntime->getWindowComponent()->getHeight();

        LOG_DEBUG("==> Running Lighting Render Pass");

        // Bind default framebuffer
        mProjectRuntime->getWindowComponent()->bindFrameBuffer();

        // Setup viewport
        glViewport(0, 0, width, height);
        GLCheckError();

        // Clear Buffer
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        GLCheckError();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        // Activate light pass shader
        if (lightPassShader->use())
        {   // Setup source textures

            // Position Texture
            glActiveTexture(GL_TEXTURE0);
            GLCheckError();
            glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
            GLCheckError();
            ShaderRuntime::CurrentTexture0 = mGeometryPassPositionBuffer;

            // Normal Texture
            glActiveTexture(GL_TEXTURE1);
            GLCheckError();
            glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
            GLCheckError();
            ShaderRuntime::CurrentTexture1 = mGeometryPassNormalBuffer;

            // Albedo Texture
            glActiveTexture(GL_TEXTURE2);
            GLCheckError();
            glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
            GLCheckError();
            ShaderRuntime::CurrentTexture2 = mGeometryPassAlbedoBuffer;

            // Depth Texture
            glActiveTexture(GL_TEXTURE3);
            GLCheckError();
            glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);
            GLCheckError();
            ShaderRuntime::CurrentTexture3 = mShadowPassDepthBuffer;

            // Ignore Texture
            glActiveTexture(GL_TEXTURE4);
            GLCheckError();
            glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
            GLCheckError();
            ShaderRuntime::CurrentTexture4 = mGeometryPassIgnoreBuffer;

            // Setup Uniforms
            GLuint pos    = 0;
            GLuint norm   = 1;
            GLuint alb    = 2;
            GLuint shadow = 3;
            GLuint ignore = 4;
            lightPassShader->addUniform(INT1,"gPosition"  ,1, &pos);
            lightPassShader->addUniform(INT1,"gNormal"    ,1, &norm);
            lightPassShader->addUniform(INT1,"gAlbedoSpec",1, &alb);
            lightPassShader->addUniform(INT1,"gShadow"    ,1, &shadow);
            lightPassShader->addUniform(INT1,"gIgnore"    ,1, &ignore);

            // Shadow Space Matrix
            auto shadowMtx = lightPassShader->getUniformLocation("shadowSpaceMatrix");
            glUniformMatrix4fv(shadowMtx,1,GL_FALSE,glm::value_ptr(mShadowMatrix));
            GLCheckError();

            // Viewer Position
            lightPassShader->setViewerPosition(sr->getCamera()->getTranslation());

            // Light Queue
            lightPassShader->bindLightQueue(mLightQueue);

            // Synchronize Uniforms
            lightPassShader->syncUniforms();

            // Copy content of geometry's depth buffer to the target
            // framebuffer's depth buffer
#if defined (GL_ES_VERSION_3_0)
            GLuint read_fb = mGeometryPassFB;
            GLuint draw_fb = mProjectRuntime->getWindowComponent()->getDepthBuffer();
            glBindFramebuffer(GL_READ_FRAMEBUFFER, read_fb);
            GLenum fb_status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
            if(fb_status != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_ERROR("GraphicsComponent: READ_FRAMEBUFFER incomplete");
                assert(false);
            }
            GLCheckError();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_fb);
            fb_status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
            if(fb_status != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_ERROR("GraphicsComponent: DRAW_FRAMEBUFFER incomplete");
                assert(false);
            }
            GLCheckError();

            LOG_TRACE("GraphicsComponent: Blit from SourceFB:{} to DestinationFB:{} size {}x{}",
                      read_fb, draw_fb, width,height);

            glBlitFramebuffer(
                        /* Source */ 0, 0, width, height,
                        /* Dest   */ 0, 0, width, height,
                        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            GLCheckError();
#else
            // Setup the Read & Draw Framebuffers
            GLuint read_fb = mGeometryPassFB;
            GLuint draw_fb = mProjectRuntime->getWindowComponent()->getDepthBuffer();
            glBindFramebuffer(GL_READ_FRAMEBUFFER, read_fb);
            GLCheckError();

            GLenum fb_status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
            if(fb_status != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_ERROR("GraphicsComponent: READ_FRAMEBUFFER incomplete");
                assert(false);
            }
            GLCheckError();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_fb);
            fb_status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
            if(fb_status != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_ERROR("GraphicsComponent: DRAW_FRAMEBUFFER incomplete");
                assert(false);
            }
            GLCheckError();

            LOG_TRACE("GraphicsComponent: Blit from SourceFB:{} to DestinationFB:{} size {}x{}",
                      read_fb, draw_fb, width,height);

            glBlitFramebuffer(
                        /* Source */ 0, 0, width, height,
                        /* Dest   */ 0, 0, width, height,
                        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            GLCheckError();
#endif
            mProjectRuntime->getWindowComponent()->bindFrameBuffer();
            // Bind & draw fullscreen quad
            lightPassShader->bindVertexArray(mLightPassQuadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            GLCheckError();
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
    (SceneRuntime* sr)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        if (sr == nullptr) {
            LOG_TRACE("GraphicsComponent: SceneRuntime == nullptr");
            return;
        }

        ShaderRuntime* shadowPassShader = sr->getShadowPassShader();
        auto shaderCache = mProjectRuntime->getShaderCache();

        if (mShadowLight == nullptr || shadowPassShader == nullptr || !shadowPassShader->getLoaded())
        {
            LOG_ERROR("GraphicsComponent: Cannot render shadow pass");
            if (mShadowLight == nullptr) LOG_ERROR("GraphicsComponent: shadow pass Light is null");

            if (shadowPassShader == nullptr) LOG_ERROR("GraphicsComponent: shadow pass shader is null");
            else if (shadowPassShader->getLoaded() || shadowPassShader->getLoadError()) LOG_ERROR("GraphicsComponent: shadpw pass shader load error");

            return;
        }

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

        mat4 lightMat = mShadowLight->getTransform().getMatrix();
        mat4 lightView = glm::lookAt(
                    vec3(lightMat[3]), // Light Pos
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
            auto lsUniform = shadowPassShader->getUniformLocation("lightSpaceMatrix");
            glUniformMatrix4fv(lsUniform,1,GL_FALSE,glm::value_ptr(lightMat));
            for (auto& runtime : *shaderCache->getRuntimeVectorHandle())
            {
                auto s = static_cast<ShaderRuntime*>(runtime.get());
                s->drawShadowPass(shadowPassShader);
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

    void GraphicsComponent::renderFonts(SceneRuntime* sceneRuntime)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        LOG_DEBUG("==> Running Font Pass");

        if (sceneRuntime == nullptr) {
            LOG_TRACE("GraphicsComponent: SceneRuntime == nullptr");
            return;
        }

        ShaderRuntime* fontShader = sceneRuntime->getFontShader();
        if (fontShader == nullptr) return;

        mProjectRuntime->getWindowComponent()->bindFrameBuffer();

        glViewport(0,0,
                   mProjectRuntime->getWindowComponent()->getWidth(),
                   mProjectRuntime->getWindowComponent()->getHeight());

        glEnable(GL_BLEND);
        GLCheckError();
        glDisable(GL_DEPTH_TEST);
        GLCheckError();
        /*
        glDepthFunc(GL_LESS);
        GLCheckError();
        glDisable(GL_CULL_FACE);
        GLCheckError();
        */

        auto fontCache = sceneRuntime->getProjectRuntime()->getFontCache();

        if (fontCache->runtimeCount() == 0) return;


        if (fontShader == nullptr || !fontShader->getLoaded())
        {
            LOG_ERROR("GraphicsComponent: Font shader not found");
            return;
        }

        // Activate the Font Shader
        fontShader->use();
        mat4 fontProjection = ortho(
                    0.f, (float)mProjectRuntime->getWindowComponent()->getWidth(),
                    0.f, (float)mProjectRuntime->getWindowComponent()->getHeight(),
                    -1.f, 1.f);
        fontShader->setFontProjectionUniform(fontProjection);

        // Iterate through FontRuntimes
        for (auto& saRuntime : *fontCache->getRuntimeVectorHandle())
        {
            FontRuntime* fontRuntime  = static_cast<FontRuntime*>(saRuntime.get());
            // Iterate through FontTextInstances
            for (EntityRuntime* entity : *fontRuntime->getInstanceVector())
            {
                fontShader->setFontPositionUniform(entity->getTransform().getTranslation().toGLM());
                GLCheckError();
                fontShader->setFontColorUniform(entity->getFontColor());
                GLCheckError();
                fontRuntime->draw(entity);
            }
        }
    }

    // Sprite ==================================================================

    void GraphicsComponent::renderSprites(SceneRuntime* sceneRuntime)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        LOG_DEBUG("==> Running Sprite Pass");

        if (sceneRuntime == nullptr) {
            LOG_TRACE("GraphicsComponent: SceneRuntime == nullptr");
            return;
        }

        ShaderRuntime* shader = sceneRuntime->getSpriteShader();
        if (shader == nullptr) return;

        mProjectRuntime->getWindowComponent()->bindFrameBuffer();

        glViewport(0,0,
                   mProjectRuntime->getWindowComponent()->getWidth(),
                   mProjectRuntime->getWindowComponent()->getHeight());

        glEnable(GL_BLEND);
        GLCheckError();

        glDisable(GL_DEPTH_TEST);
        GLCheckError();
        /*
        glDepthFunc(GL_LESS);
        GLCheckError();
        glDisable(GL_CULL_FACE);
        GLCheckError();
        */

        auto textureCache = sceneRuntime->getProjectRuntime()->getTextureCache();

        if (textureCache->runtimeCount() == 0) return;


        if (shader == nullptr)
        {
            LOG_ERROR("GraphicsComponent: Sprite shader not found");
            return;
            //assert(false);
        }

        if (!shader->getLoaded()) return;


        // Activate the Font Shader
        shader->use();
        mat4 projection = ortho(
                    0.f, (float)mProjectRuntime->getWindowComponent()->getWidth(),
                    0.f, (float)mProjectRuntime->getWindowComponent()->getHeight(),
                    -1.f, 1.f);
        shader->setSpriteProjectionUniform(projection);

        // Iterate through Textureuntimes
        for (auto& textureRuntime : *textureCache->getRuntimeVectorHandle())
        {
            // None
            if (textureRuntime->getInstanceVector()->empty())
            {
                continue;
            }

            // Iterate through SpriteRuntimes
            for (EntityRuntime* entityRuntime : *textureRuntime->getInstanceVector())
            {
                //shader->setSpritePositionUniform(entityRuntime->getTransform().getTranslation().toGLM());
                //GLCheckError();
                //spriteRuntime->draw();
            }
        }
    }

    // Accessors ================================================================

    void GraphicsComponent::addToLightQueue(EntityRuntime* runt)
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        auto light = runt->getLightRuntime();

        if (std::find(mLightQueue.begin(), mLightQueue.end(), light) == mLightQueue.end())
        {
            if (light->getType() == LightType::LT_DIRECTIONAL)
            {
                mShadowLight = runt;
            }
            mLightQueue.push_back(light);
        }
    }

    void GraphicsComponent::clearLightQueue()
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        mLightQueue.clear();
    }

    GLuint GraphicsComponent::getGeometryPassPositionBuffer()
    const
    {
        return mGeometryPassPositionBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassAlbedoBuffer()
    const
    {
        return mGeometryPassAlbedoBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassNormalBuffer()
    const
    {
        return mGeometryPassNormalBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassDepthBuffer()
    const
    {
        return mGeometryPassDepthBuffer;
    }

    GLuint GraphicsComponent::getShadowPassDepthBuffer()
    const
    {
        return mShadowPassDepthBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassIgnoreBuffer()
    const
    {
        return mGeometryPassIgnoreBuffer;
    }

    void GraphicsComponent::checkFrameBufferDimensions()
    const
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        int width = mProjectRuntime->getWindowComponent()->getWidth();
        int height = mProjectRuntime->getWindowComponent()->getHeight();
        assert(mMaxFrameBufferSize > 0);
        assert(width > 0 && width < mMaxFrameBufferSize && height > 0 && height < mMaxFrameBufferSize);
    }

    void
    GraphicsComponent::logShaders
    ()
    {
        LOG_DEBUG("GraphicsComponent: Contents of shader cache");
        auto shaderCache = mProjectRuntime->getShaderCache();
        for (auto& runtime : *shaderCache->getRuntimeVectorHandle())
        {
            auto shader = static_cast<ShaderRuntime*>(runtime.get());
            LOG_DEBUG("GraphicsComponent: {}",shader->getNameAndUuidString());
            shader->logMaterials();
        }
    }

    void GraphicsComponent::pushTasks()
    {
        // Materials
        auto materialCache = mProjectRuntime->getMaterialCache();
        for (auto& material : *materialCache->getRuntimeVectorHandle())
        {
            material->pushNextTask();
        }

        // Models
        auto modelCache = mProjectRuntime->getModelCache();
        for (auto& model : *modelCache->getRuntimeVectorHandle())
        {
            model->pushNextTask();
        }

        // Shaders
        auto shaderCache = mProjectRuntime->getShaderCache();
        for (auto& shader : *shaderCache->getRuntimeVectorHandle())
        {
            shader->pushNextTask();
        }

        // Textures
        auto textureCache = mProjectRuntime->getTextureCache();
        for (auto& texture : *textureCache->getRuntimeVectorHandle())
        {
            texture->pushNextTask();
        }

        // Fonts
        auto fontCache = mProjectRuntime->getFontCache();
        for (auto& font : *fontCache->getRuntimeVectorHandle())
        {
            font->pushNextTask();
        }

        // This

        mTaskQueue.pushTask(mHandleResizeTask);

        if (mSetupBuffersTask->hasState(TASK_STATE_QUEUED))
        {
            mTaskQueue.pushTask(mSetupBuffersTask);
        }

        if (mSetupBuffersTask->hasState(TASK_STATE_COMPLETED))
        {
            mTaskQueue.pushTask(mRenderTask);
        }
    }

    GraphicsTaskQueue*
    GraphicsComponent::getTaskQueue()
    {
        return &mTaskQueue;
    }

    GraphicsDestructionTaskQueue*
    GraphicsComponent::getDestructionTaskQueue()
    {
        return &mDestructionTaskQueue;
    }
}
