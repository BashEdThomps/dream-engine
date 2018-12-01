/*
* GraphicsComponent
*
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

#ifdef WIN32
    #include <windows.h>
#endif

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glu.h>
#endif


#define GLM_FORCE_RADIANS

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Camera.h"
#include "Light/LightInstance.h"
#include "Model/ModelInstance.h"
#include "Model/ModelMesh.h"
#include "Material/MaterialCache.h"
#include "Shader/ShaderInstance.h"
#include "Shader/ShaderCache.h"
#include "../Transform.h"
#include "../Time.h"
#include "../Window/IWindowComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectDefinition.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Utilities/Math.h"

using namespace glm;

namespace Dream
{
    GraphicsComponent::GraphicsComponent
    (IWindowComponent* windowComponent)
        : IComponent(),
          mProjectionMatrix(mat4(1.0f)),
          mMinimumDraw(1.0f),
          mMaximumDraw(3000.0f),
          mMeshCullDistance(2500.0f),
          mWindowComponent(windowComponent),
          mShaderCacheHandle(nullptr),

          mGeometryPassFB(0),
          mGeometryPassPositionBuffer(0),
          mGeometryPassAlbedoBuffer(0),
          mGeometryPassNormalBuffer(0),
          mGeometryPassDepthBuffer(0),

          mShadowPassShader(nullptr),
          mShadowPassFB(0),
          mShadowPassDepthBuffer(0),
          mShadowMatrix(mat4(1.0f)),

          mLightingPassShader(nullptr),
          mScreenQuadVAO(0),
          mScreenQuadVBO(0)
    {
        setLogClassName("GraphicsComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    GraphicsComponent::~GraphicsComponent
    (void)
    {
        auto log = getLog();
        log->trace("Destroying Object");
        clearLightQueue();
        freeGeometryBuffers();
        freeShadowBuffers();
    }

    // Init/Setup ===============================================================

    bool
    GraphicsComponent::init
    ()
    {
        auto log = getLog();
        log->debug("Initialising");

        onWindowDimensionsChanged();
        checkGLError();

        if (!setupGeometryBuffers())
        {
            log->error("Unable to create geometry buffers");
            return false;
        }

        if (!setupShadowBuffers())
        {
            log->error("Unable to create shadow pass buffers");
            return false;
        }

        if (!setupScreenQuad())
        {
            log->error("Unable to create screen quad");
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        log->debug("Initialisation Done.");
        return true;
    }

    void
    GraphicsComponent::onWindowDimensionsChanged
    ()
    {
        auto log = getLog();

        // Define the viewport dimensions
        int windowWidth  = mWindowComponent->getWidth();
        int windowHeight = mWindowComponent->getHeight();

        if (windowWidth == 0 || windowHeight == 0)
        {
            log->debug("It's a bit rude to use 0,0 viewport and divide by 0...");
            windowWidth = 1;
            windowHeight = 1;
        }

        glViewport(0, 0, windowWidth, windowHeight);
        checkGLError();

        // Perspective Projection Matrix
        mProjectionMatrix = perspective
        (
            Constants::CAMERA_ZOOM,
            static_cast<float>(windowWidth)/static_cast<float>(windowHeight),
            mMinimumDraw,
            mMaximumDraw
        );
        checkGLError();

        freeGeometryBuffers();
        setupGeometryBuffers();

        log->debug
        (
            "Window dimensions changed: width: {}, height: {} min draw: {}, max draw {}",
            windowWidth, windowHeight,
            mMinimumDraw,
            mMaximumDraw
        );
    }

    void
    GraphicsComponent::updateComponent
    (SceneRuntime* sr)
    {
        beginUpdate();
        auto log = getLog();
        log->debug("GraphicsComponrnt: updateComponent() Called" );
        setMeshCullDistance(sr->getMeshCullDistance());
        setMinimumDraw(sr->getMinDrawDistance());
        setMaximumDraw(sr->getMaxDrawDistance());
        sr->getCamera()->setProjectionMatrix(mProjectionMatrix);
        sr->getCamera()->updateCameraVectors();
        mLightingPassShader = sr->getLightingPassShader();
        mShadowPassShader = sr->getShadowPassShader();
        updateLightQueue(sr);
        endUpdate();
    }

    void
    GraphicsComponent::handleResize
    ()
    {
        if (mWindowComponent->sizeHasChanged())
        {
            onWindowDimensionsChanged();
        }
    }


    // Geometry Pass ============================================================

    void
    GraphicsComponent::renderGeometryPass
    (SceneRuntime* sr)
    {
        auto log = getLog();
        log->debug
        (
           "\n\n"
           "==> Running Geometry Render Pass"
           "\n"
        );
        // Setup
        glViewport(0, 0, mWindowComponent->getWidth(), mWindowComponent->getHeight());
        glBindFramebuffer(GL_FRAMEBUFFER,mGeometryPassFB);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Clear the colorbuffer
        if (sr != nullptr)
        {
            auto clearColour = sr->getClearColour();
            glClearColor
            (
                clearColour[Constants::RED_INDEX],
                clearColour[Constants::GREEN_INDEX],
                clearColour[Constants::BLUE_INDEX],
                1.0f
            );
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            return;
        }

        if (mShaderCacheHandle != nullptr)
        {
            mShaderCacheHandle->drawGeometryPass
            (
                sr->getCamera(),
                mProjectionMatrix
            );
        }

    }

    void
    GraphicsComponent::freeGeometryBuffers
    ()
    {
        mWindowComponent->bindDefaultFrameBuffer();

        if (mGeometryPassFB != 0)
        {
            glDeleteFramebuffers(1, &mGeometryPassFB);
            checkGLError();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        ShaderInstance::CurrentTexture0 = 0;

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderInstance::CurrentTexture1 = 0;

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderInstance::CurrentTexture2 =0;

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
        ShaderInstance::CurrentTexture3 = 0;

        // - position color buffer
        if (mGeometryPassPositionBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassPositionBuffer);
            checkGLError();
        }

        // - normal color buffer
        if (mGeometryPassNormalBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassNormalBuffer);
            checkGLError();
        }

        // - color + specular color buffer
        if (mGeometryPassAlbedoBuffer != 0)
        {
            glDeleteTextures(1, &mGeometryPassAlbedoBuffer);
            checkGLError();
        }

        // Depth Buffer
        if (mGeometryPassDepthBuffer != 0)
        {
            glDeleteRenderbuffers(1,&mGeometryPassDepthBuffer);
            checkGLError();
        }

         // Depth Buffer
        if (mGeometryPassIgnoreBuffer != 0)
        {
            glDeleteTextures(1,&mGeometryPassIgnoreBuffer);
            checkGLError();
        }
    }

    bool
    GraphicsComponent::setupGeometryBuffers
    ()
    {
        auto log = getLog();
        auto width = mWindowComponent->getWidth();
        auto height = mWindowComponent->getHeight();

        if (width == 0 || height == 0)
        {
            log->debug("It's a bit rude to allocate empty buffers...");
            width = 1;
            height = 1;
        }

        log->debug("Setting up Geometry Buffers with dimensions {}x{}",width,height);

        glGenFramebuffers(1,&mGeometryPassFB);
        checkGLError();

        if (mGeometryPassFB == 0)
        {
            log->error("Unable to create Geometry Framebuffer");
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, mGeometryPassFB);
        checkGLError();

        // - position color buffer
        glGenTextures(1, &mGeometryPassPositionBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGeometryPassPositionBuffer, 0);
        checkGLError();

        // - normal color buffer
        glGenTextures(1, &mGeometryPassNormalBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGeometryPassNormalBuffer, 0);
        checkGLError();

        // - color + specular color buffer
        glGenTextures(1, &mGeometryPassAlbedoBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGeometryPassAlbedoBuffer, 0);
        checkGLError();

        // - Ignore Lighting Buffer
        glGenTextures(1, &mGeometryPassIgnoreBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width,height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mGeometryPassIgnoreBuffer, 0);
        checkGLError();

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(4, attachments);
        checkGLError();

        // then also add render buffer object as depth buffer and check for completeness.
        glGenRenderbuffers(1, &mGeometryPassDepthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        checkGLError();

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            log->error("Deferred Rendering Framebuffer not complete!");
            return false;
        }
        else
        {
            log->debug("Deferred Rending Buffer is complete!");
        }
        mWindowComponent->bindDefaultFrameBuffer();
        checkGLError();
        return true;
    }

    // Lighting Pass ============================================================

    bool
    GraphicsComponent::setupScreenQuad
    ()
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &mScreenQuadVAO);
        glGenBuffers(1, &mScreenQuadVBO);
        glBindVertexArray(mScreenQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mScreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        return true;
    }

    void
    GraphicsComponent::renderLightingPass
    (SceneRuntime* sr)
    {
        auto log = getLog();

        log->debug
        (
           "\n\n"
           "==> Running Lighting Render Pass"
           "\n"
        );

        glViewport(0, 0, mWindowComponent->getWidth(), mWindowComponent->getHeight());
        mWindowComponent->bindDefaultFrameBuffer();
        // Clear Buffer
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mLightingPassShader == nullptr)
        {
            log->error("Lighting Shader is nullptr");
            return;
        }

        mLightingPassShader->use();

        // Setup source textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
        ShaderInstance::CurrentTexture0 = mGeometryPassPositionBuffer;

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
        ShaderInstance::CurrentTexture1 = mGeometryPassNormalBuffer;

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
        ShaderInstance::CurrentTexture2 = mGeometryPassAlbedoBuffer;

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);
        ShaderInstance::CurrentTexture3 = mShadowPassDepthBuffer;

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
        ShaderInstance::CurrentTexture4 = mGeometryPassIgnoreBuffer;

        GLuint pos, norm, alb, shadow, ignore;
        pos = 0;
        norm = 1;
        alb  = 2;
        shadow = 3;
        ignore = 4;
        mLightingPassShader->addUniform(INT1,"gPosition"  ,1, &pos);
        mLightingPassShader->addUniform(INT1,"gNormal"    ,1, &norm);
        mLightingPassShader->addUniform(INT1,"gAlbedoSpec",1, &alb);
        mLightingPassShader->addUniform(INT1,"gShadow",1, &shadow);
        mLightingPassShader->addUniform(INT1,"gIgnore",1,&ignore);

        auto shadowMtx = mLightingPassShader->getUniformLocation("shadowSpaceMatrix");
        glUniformMatrix4fv(shadowMtx,1,GL_FALSE,glm::value_ptr(mShadowMatrix));

        mLightingPassShader->setViewerPosition(sr->getCamera()->getTranslation());
        mLightingPassShader->bindLightQueue(mLightQueue);
        mLightingPassShader->syncUniforms();

        mLightingPassShader->bindVertexArray(mScreenQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Copy content of geometry's depth buffer to default framebuffer's depth buffer
        auto width = mWindowComponent->getWidth();
        auto height = mWindowComponent->getHeight();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mGeometryPassDepthBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void
    GraphicsComponent::updateLightQueue
    (SceneRuntime* sr)
    {
        auto log = getLog();
        log->debug("Updating Light Queue");
        // Clear existing Queues
        clearLightQueue();

        sr->getRootSceneObjectRuntime()->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* object)
                {

                    if (object->getHidden())
                    {
                        return nullptr;
                    }

                    // Lights
                    if (object->hasLightInstance())
                    {
                        LightInstance* light = object->getLightInstance();
                        log->debug("Adding light instance to queue {}",light->getNameAndUuidString());
                        addToLightQueue(light);
                        if (light->getType() == LightType::LT_DIRECTIONAL)
                        {
                           mShadowLight = object;
                        }
                    }

                    return nullptr;
                }
            )
        );
    }

    // Shadow Pass ==============================================================

    bool
    GraphicsComponent::setupShadowBuffers
    ()
    {
       auto log = getLog();
       log->info("Setting up ShadowPass FrameBuffer");
       glGenFramebuffers(1,&mShadowPassFB);
       checkGLError();

       if (mShadowPassFB == 0)
       {
          log->error("Unable to create shadow pass FB");
          return false;
       }

       glGenTextures(1, &mShadowPassDepthBuffer);
       checkGLError();

       if (mShadowPassDepthBuffer == 0)
       {
           log->error("Unable to create shadow pass depth buffer");
           return false;
       }

       glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);
       checkGLError();
       glTexImage2D
       (
            GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0,
            GL_DEPTH_COMPONENT,GL_FLOAT,nullptr
       );

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
       glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
       checkGLError();

       glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowPassDepthBuffer, 0);
       glDrawBuffer(GL_NONE);
       glReadBuffer(GL_NONE);
       glBindFramebuffer(GL_FRAMEBUFFER, 0);
       checkGLError();

       return true;
    }

    void
    GraphicsComponent::renderShadowPass
    (SceneRuntime* sr)
    {
        auto log = getLog();
        if (mShadowLight == nullptr || mShadowPassShader == nullptr)
        {
            log->error
            (
                "Cannot render shadow pass Light: {}, Shader: {]",
                mShadowLight != nullptr,
                mShadowPassShader != nullptr
            );
            return;
        }

        log->debug
        (
           "\n\n"
           "==> Running Shadow Render Pass"
           "\n"
        );

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Setup Uniforms
        static float near_plane = 1.0f;
        static float far_plane = 1000.0f;
        static float sz = 100.0f;
        glm::mat4 lightProjection = glm::ortho(-sz, sz, -sz, sz, near_plane, far_plane);
        mat4 lightMat = mShadowLight->getTransform().getMatrix();
        mat4 lightView = glm::lookAt
        (
            vec3(lightMat[3]), // Light Pos
            vec3(glm::translate(lightMat,vec3(0,0,-100))[3]),//vec3(0.0f),  // Centre
            vec3(0.0f,1.0f,0.0f) // Up
        );
        mShadowMatrix = lightProjection*lightView;
        glDisable( GL_CULL_FACE );
        if (mShaderCacheHandle != nullptr)
        {
            mShaderCacheHandle->drawShadowPass
            (
                mShadowMatrix,
                mShadowPassShader
            );
        }
        glEnable( GL_CULL_FACE );
    }

    void
    GraphicsComponent::freeShadowBuffers
    ()
    {
        mWindowComponent->bindDefaultFrameBuffer();

        if (mShadowPassFB != 0)
        {
            glDeleteFramebuffers(1, &mShadowPassFB);
            checkGLError();
        }

        if (mShadowPassDepthBuffer != 0)
        {
            glDeleteTextures(1, &mShadowPassDepthBuffer);
            checkGLError();
        }
    }

    // Accessors ================================================================

    float
    GraphicsComponent::getMeshCullDistance
    ()
    const
    {
        return mMeshCullDistance;
    }

    void
    GraphicsComponent::setMeshCullDistance
    (float meshCullDistance)
    {
        mMeshCullDistance = meshCullDistance;
    }

    mat4
    GraphicsComponent::getProjectionMatrix
    ()
    {
        return mProjectionMatrix;
    }

    void
    GraphicsComponent::addToLightQueue
    (LightInstance* lightInstance)
    {
        mLightQueue.push_back(lightInstance);
    }

    void
    GraphicsComponent::clearLightQueue
    ()
    {
        mLightQueue.clear();
    }

    void
    GraphicsComponent::setMinimumDraw
    (float minimumDraw)
    {
        mMinimumDraw = minimumDraw;
    }

    void
    GraphicsComponent::setMaximumDraw
    (float maximumDraw)
    {
        mMaximumDraw = maximumDraw;
    }

    void
    GraphicsComponent::setShaderCache
    (ShaderCache* cache)
    {
        mShaderCacheHandle = cache;
    }

    ShaderInstance*
    GraphicsComponent::getLightingShader
    ()
    const
    {
        return mLightingPassShader;
    }

    void
    GraphicsComponent::setLightingShader
    (ShaderInstance* lightingShader)
    {
        mLightingPassShader = lightingShader;
    }

    float GraphicsComponent::getMinimumDraw() const
    {
        return mMinimumDraw;
    }

    float GraphicsComponent::getMaximumDraw() const
    {
        return mMaximumDraw;
    }

    ShaderInstance* GraphicsComponent::getShadowPassShader() const
    {
        return mShadowPassShader;
    }

    void GraphicsComponent::setShadowPassShader(ShaderInstance* shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    GLuint GraphicsComponent::getGeometryPassPositionBuffer() const
    {
        return mGeometryPassPositionBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassAlbedoBuffer() const
    {
        return mGeometryPassAlbedoBuffer;
    }

    void GraphicsComponent::setGeometryPassAlbedoBuffer(const GLuint& geometryPassAlbedoBuffer)
    {
        mGeometryPassAlbedoBuffer = geometryPassAlbedoBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassNormalBuffer() const
    {
        return mGeometryPassNormalBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassDepthBuffer() const
    {
        return mGeometryPassDepthBuffer;
    }

    GLuint GraphicsComponent::getShadowPassDepthBuffer() const
    {
        return mShadowPassDepthBuffer;
    }

    GLuint GraphicsComponent::getGeometryPassIgnoreBuffer() const
    {
        return mGeometryPassIgnoreBuffer;
    }

} // End of Dream
