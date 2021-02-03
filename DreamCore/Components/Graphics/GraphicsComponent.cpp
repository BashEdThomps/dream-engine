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
#include "Common/GLHeader.h"
#include "Common/Logger.h"
#include "Common/Math.h"
#include "Components/Transform.h"
#include "Components/Time.h"
#include "Components/Window/WindowComponent.h"
#include "Light/LightRuntime.h"
#include "Model/ModelRuntime.h"
#include "Model/ModelMesh.h"
#include "Material/MaterialCache.h"
#include "Shader/ShaderRuntime.h"
#include "Shader/ShaderCache.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityDefinition.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Font/FontCache.h"
#include "Font/FontRuntime.h"

#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::ortho;
using std::unique_lock;

namespace octronic::dream
{
    GraphicsComponent::GraphicsComponent(ProjectRuntime* pr, WindowComponent* windowComponent)
        : Component("GraphicsComponent",pr),
          mWindowComponent(windowComponent),
          mShaderCache(nullptr),
          mMaxFrameBufferSize(0),
          // Geometry Pass Vars
          mGeometryPassFB(0),
          mGeometryPassPositionBuffer(0),
          mGeometryPassAlbedoBuffer(0),
          mGeometryPassNormalBuffer(0),
          mGeometryPassDepthBuffer(0),
          mGeometryPassIgnoreBuffer(0),
          // Shadow Pass Vars
          mShadowPassShader(nullptr),
          mShadowPassFB(0),
          mShadowPassDepthBuffer(0),
          mShadowMatrix(mat4(1.0f)),
          // Light Pass Vars
          mLightingPassShader(nullptr),
          mScreenQuadVAO(0),
          mScreenQuadVBO(0),
          // Font
          mFontShader(nullptr)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: Constructing");
        GLCheckError();
    }

    GraphicsComponent::~GraphicsComponent()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: Destroying Object");
        clearLightQueue();
        freeGeometryBuffers();
        freeShadowBuffers();
    }

    // Init/Setup ===============================================================

    bool GraphicsComponent::init()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG("GraphicsComponent: Initialising");

        // Define the viewport dimensions
        int windowWidth  = mWindowComponent->getWidth();
        int windowHeight = mWindowComponent->getHeight();

        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&mMaxFrameBufferSize);

        LOG_DEBUG("GraphicsComponent: Max FB size = {}x{}",mMaxFrameBufferSize,mMaxFrameBufferSize);

        if (windowWidth > mMaxFrameBufferSize || windowHeight > mMaxFrameBufferSize)
        {
            LOG_ERROR(
                        "GraphicsComponent: Requested framebuffer larger than max supported size {}x{} > {}x{}",
                        windowWidth, windowHeight, mMaxFrameBufferSize, mMaxFrameBufferSize);
            return false;
        }

        if (windowWidth == 0 || windowHeight == 0)
        {
            LOG_DEBUG("GraphicsComponent: Window size not available");
            return false;
        }

        if (!setupGeometryBuffers())
        {
            LOG_ERROR("GraphicsComponent: Unable to create geometry buffers");
            return false;
        }

        if (!setupShadowBuffers())
        {
            LOG_ERROR("GraphicsComponent: Unable to create shadow pass buffers");
            return false;
        }

        if (!setupScreenQuad())
        {
            LOG_ERROR("GraphicsComponent: Unable to create screen quad");
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        // TODO Multisample kills line & tool rendering for some reason
        //glEnable(GL_MULTISAMPLE);
        LOG_DEBUG("GraphicsComponent: Initialisation Done.");
        return true;
    }

    void GraphicsComponent::onWindowDimensionsChanged()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        // Define the viewport dimensions
        int windowWidth  = mWindowComponent->getWidth();
        int windowHeight = mWindowComponent->getHeight();

        if (windowWidth == 0 || windowHeight == 0)
        {
            LOG_ERROR("GraphicsComponent: Cannot setup buffers with 0,0 viewport");
            return;
        }

        glViewport(0, 0, windowWidth, windowHeight);
        GLCheckError();
        freeGeometryBuffers();
        setupGeometryBuffers();

        LOG_DEBUG("GraphicsComponent: Window dimensions changed: width: {}, height: {}",windowWidth, windowHeight);
        mWindowComponent->setWindowSizeChangedFlag(false);
    }

    void GraphicsComponent::handleResize()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (mWindowComponent->getWindowSizeChangedFlag())
        {
            onWindowDimensionsChanged();
        }
    }


    // Geometry Pass ============================================================

    void GraphicsComponent::renderGeometryPass(SceneRuntime* sr)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (!mEnabled)
        {
            LOG_CRITICAL("GraphicsComponent: Component Disabled");
            return;
        }

        int width = mWindowComponent->getWidth();
        int height = mWindowComponent->getHeight();

        checkFrameBufferDimensions();

        LOG_DEBUG("\n\n==> Running Geometry Render Pass\n");
        // Setup
        glViewport(0, 0,width ,height);
        GLCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER,mGeometryPassFB);
        GLCheckError();
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

        // Clear the colorbuffer
        if (sr != nullptr)
        {
            auto clearColour = sr->getClearColour();
            glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), 1.0f);
            GLCheckError();
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            GLCheckError();
            return;
        }

        if (mShaderCache != nullptr)
        {
            mShaderCache->drawGeometryPass(sr->getCamera());
        }
    }

    void
    GraphicsComponent::freeGeometryBuffers
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (mGeometryPassFB != 0)
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
        if (mGeometryPassPositionBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassPositionBuffer);
            GLCheckError();
        }

        // - normal color buffer
        if (mGeometryPassNormalBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassNormalBuffer);
            GLCheckError();
        }

        // - color + specular color buffer
        if (mGeometryPassAlbedoBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassAlbedoBuffer);
            GLCheckError();
        }

        // Depth Buffer
        if (mGeometryPassDepthBuffer != 0)
        {
            glDeleteRenderbuffers(1,&mGeometryPassDepthBuffer);
            GLCheckError();
        }

        // Ignore Buffer
        if (mGeometryPassIgnoreBuffer != 0)
        {
            glDeleteTextures(1,&mGeometryPassIgnoreBuffer);
            GLCheckError();
        }
    }

    bool
    GraphicsComponent::setupGeometryBuffers
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        auto width = mWindowComponent->getWidth();
        auto height = mWindowComponent->getHeight();

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

    bool
    GraphicsComponent::setupScreenQuad
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &mScreenQuadVAO);
        GLCheckError();
        glGenBuffers(1, &mScreenQuadVBO);
        GLCheckError();
        glBindVertexArray(mScreenQuadVAO);
        GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, mScreenQuadVBO);
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);

        // Graphics is disabled
        if (!mEnabled)
        {
            LOG_CRITICAL("GraphicsComponent: Component Disabled");
            return;
        }

        // Get window dimensions
        auto width = mWindowComponent->getWidth();
        auto height = mWindowComponent->getHeight();

        LOG_DEBUG("\n\n==> Running Lighting Render Pass\n");

        // Setup viewport
        glViewport(0, 0, width, height);
        GLCheckError();

        // Bind default framebuffer
        mWindowComponent->bindFrameBuffer();

        // Clear Buffer
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        GLCheckError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

        // Get lighting pass shader
        mLightingPassShader = sr->getLightingPassShader();

        // Check shader exists
        if (mLightingPassShader == nullptr)
        {
            LOG_ERROR("GraphicsComponent: Lighting Shader is nullptr");
            return;
        }

        // Activate light pass shader
        if (mLightingPassShader->use())
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
            mLightingPassShader->addUniform(INT1,"gPosition"  ,1, &pos);
            mLightingPassShader->addUniform(INT1,"gNormal"    ,1, &norm);
            mLightingPassShader->addUniform(INT1,"gAlbedoSpec",1, &alb);
            mLightingPassShader->addUniform(INT1,"gShadow"    ,1, &shadow);
            mLightingPassShader->addUniform(INT1,"gIgnore"    ,1, &ignore);

            // Shadow Space Matrix
            auto shadowMtx = mLightingPassShader->getUniformLocation("shadowSpaceMatrix");
            glUniformMatrix4fv(shadowMtx,1,GL_FALSE,glm::value_ptr(mShadowMatrix));
            GLCheckError();

            // Viewer Position
            mLightingPassShader->setViewerPosition(sr->getCamera()->getTranslation());

            // Light Queue
            mLightingPassShader->bindLightQueue(mLightQueue);

            // Synchronize Uniforms
            mLightingPassShader->syncUniforms();

            // Bind & draw fullscreen quad
            mLightingPassShader->bindVertexArray(mScreenQuadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            GLCheckError();

            // Copy content of geometry's depth buffer to the target
            // framebuffer's depth buffer


#if defined (GL_ES_VERSION_3_0)
            GLuint read_fb = mGeometryPassFB;
            GLuint draw_fb = mWindowComponent->getDepthBuffer();
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
                        GL_DEPTH_BUFFER_BIT, GL_NEAREST
                        );
            GLCheckError();
#else
            GLuint read_fb = mGeometryPassFB;
            GLuint draw_fb = mWindowComponent->getDepthBuffer();
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
                        GL_DEPTH_BUFFER_BIT, GL_NEAREST
                        );
            GLCheckError();
#endif
        }
        clearLightQueue();
    }

    // Shadow Pass ==============================================================

    bool
    GraphicsComponent::setupShadowBuffers
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCheckError();

        return true;
    }

    void
    GraphicsComponent::renderShadowPass
    (SceneRuntime* sr)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        if (!mEnabled)
        {
            LOG_CRITICAL("GraphicsComponent: Component Disabled");
            return;
        }

        mShadowPassShader = sr->getShadowPassShader();

        if (mShadowLight == nullptr || mShadowPassShader == nullptr)
        {
            LOG_ERROR(
                        "GraphicsComponent: Cannot render shadow pass Light: {}, Shader: {]",
                        mShadowLight != nullptr,
                        mShadowPassShader != nullptr
                    );
            return;
        }


        LOG_DEBUG("\n\n==> Running Shadow Render Pass\n");

        glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
        GLCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
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
        //glCullFace(GL_FRONT);
        glDisable(GL_CULL_FACE);
        GLCheckError();
        if (mShaderCache != nullptr)
        {
            mShaderCache->drawShadowPass(mShadowMatrix, mShadowPassShader);
        }
        glEnable(GL_CULL_FACE);
        GLCheckError();
        glCullFace(GL_BACK);
        GLCheckError();
    }

    void GraphicsComponent::freeShadowBuffers()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);


        LOG_DEBUG("\n\n==> Running Font Pass\n");

        mWindowComponent->bindFrameBuffer();

        glViewport(0,0,mWindowComponent->getWidth(), mWindowComponent->getHeight());

        glEnable(GL_BLEND);
        GLCheckError();

        glEnable(GL_DEPTH_TEST);
        GLCheckError();
        glDepthFunc(GL_LESS);
        GLCheckError();
        glDisable(GL_CULL_FACE);
        GLCheckError();


        FontCache* fontCache = sceneRuntime->getProjectRuntime()->getFontCache();

        if (fontCache->runtimeCount() == 0) return;

        ShaderRuntime* fontShader = sceneRuntime->getFontShader();

        if (fontShader == nullptr)
        {
            LOG_ERROR("GraphicsComponent: Font shader not found");
            assert(false);
        }

        // Activate the Font Shader
        fontShader->use();
        mat4 fontProjection = ortho(
                    0.f, (float)mWindowComponent->getWidth(),
                    0.f, (float)mWindowComponent->getHeight(),
                    -1.f, 1.f);
        fontShader->setFontProjection(fontProjection);

        // Iterate through FontRuntimes
        for (SharedAssetRuntime* saRuntime : fontCache->getRuntimeVector())
        {
            FontRuntime* fontRuntime  = static_cast<FontRuntime*>(saRuntime);
            // Iterate through FontTextInstances
            for (EntityRuntime* entity : fontRuntime->getInstanceVector())
            {
                fontShader->setFontPositionUniform(entity->getTransform().getTranslation().toGLM());
                GLCheckError();
                fontShader->setFontColorUniform(entity->getFontColor());
                GLCheckError();
                fontRuntime->draw(entity);
            }
        }
    }

    // Accessors ================================================================

    void GraphicsComponent::addToLightQueue(EntityRuntime* runt)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        auto light = runt->getLightRuntime();
        if (light->getType() == LightType::LT_DIRECTIONAL)
        {
            mShadowLight = runt;
        }
        mLightQueue.push_back(light);
    }

    void GraphicsComponent::clearLightQueue()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        static mutex m;
        m.lock();
        mLightQueue.clear();
        m.unlock();
    }

    void GraphicsComponent::setShaderCache(ShaderCache* cache)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mShaderCache = cache;
    }

    ShaderRuntime* GraphicsComponent::getLightingShader()
    const
    {
        return mLightingPassShader;
    }

    void GraphicsComponent::setLightingShader(ShaderRuntime* lightingShader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mLightingPassShader = lightingShader;
    }

    ShaderRuntime* GraphicsComponent::getShadowPassShader()
    const
    {
        return mShadowPassShader;
    }

    void GraphicsComponent::setShadowPassShader(ShaderRuntime* shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    ShaderRuntime* GraphicsComponent::getFontShader()
    const
    {
        return mFontShader;
    }

    void GraphicsComponent::setFontShader(ShaderRuntime* fontShader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mFontShader = fontShader;
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

    void GraphicsComponent::pushTask(GraphicsComponentTask* t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        t->setState(TaskState::TASK_STATE_QUEUED);
        mTaskQueue.push_back(t);
    }

    void GraphicsComponent::pushDestructionTask
    (const shared_ptr<GraphicsComponentDestructionTask>& t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        t->setState(TaskState::TASK_STATE_QUEUED);
        mDestructionTaskQueue.push_back(t);
    }

    void GraphicsComponent::executeTaskQueue()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        vector<GraphicsComponentTask*> completed;
        while (!mTaskQueue.empty())
        {
            for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); itr++)
            {
                // Put in debug task queue
                auto t = (*itr);
                if (find(mDebugTaskQueue.begin(),mDebugTaskQueue.end(), t) == mDebugTaskQueue.end())
                {
                    mDebugTaskQueue.push_back(t);
                }

                // Check if ready to execute
                if (!t->isWaitingForDependencies())
                {
                    t->setState(TaskState::TASK_STATE_ACTIVE);
                    t->execute();
                }
                // Still Waiting
                else
                {
                    t->incrementDeferralCount();
                }

                // Task has completed
                if (t->getState() == TaskState::TASK_STATE_COMPLETED)
                {
                    t->notifyTasksWaitingForMe();
                    completed.push_back(t);
                }
            }

            for (auto t : completed)
            {
                auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                if (itr != mTaskQueue.end())
                {
                    mTaskQueue.erase(itr);
                }
            }
            completed.clear();
        }
        mTaskQueue.clear();
    }

    void GraphicsComponent::executeDestructionTaskQueue()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        for (auto itr = mDestructionTaskQueue.begin(); itr != mDestructionTaskQueue.end(); itr++)
        {
            const auto& t = (*itr);
            t->setState(TaskState::TASK_STATE_ACTIVE);
            t->execute();
            t->notifyTasksWaitingForMe();
        }
        mDestructionTaskQueue.clear();
    }

    const vector<GraphicsComponentTask*>& GraphicsComponent::getDebugTaskQueue()
    {
        return mDebugTaskQueue;
    }

    void GraphicsComponent::checkFrameBufferDimensions()
    const
    {
        LOG_TRACE("GraphicsComponent: {}",__FUNCTION__);
        int width = mWindowComponent->getWidth();
        int height = mWindowComponent->getHeight();

        assert(mMaxFrameBufferSize > 0);

        assert(width > 0 && width < mMaxFrameBufferSize &&
               height > 0 && height < mMaxFrameBufferSize);
    }
}
