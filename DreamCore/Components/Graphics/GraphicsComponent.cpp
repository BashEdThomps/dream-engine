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

#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

using namespace glm;

namespace Dream
{
    GraphicsComponent::GraphicsComponent
    (ProjectRuntime* pr, WindowComponent* windowComponent)
        : Component(pr),
          mWindowComponent(windowComponent),
          mShaderCache(nullptr),

          mGeometryPassFB(0),
          mGeometryPassPositionBuffer(0),
          mGeometryPassAlbedoBuffer(0),
          mGeometryPassNormalBuffer(0),
          mGeometryPassDepthBuffer(0),
          mGeometryPassIgnoreBuffer(0),

          mShadowPassShader(nullptr),
          mShadowPassFB(0),
          mShadowPassDepthBuffer(0),
          mShadowMatrix(mat4(1.0f)),

          mLightingPassShader(nullptr),
          mScreenQuadVAO(0),
          mScreenQuadVBO(0)
    {
        LOG_TRACE("Constructing");
        GLCheckError();
    }

    GraphicsComponent::~GraphicsComponent
    (void)
    {
        LOG_TRACE("Destroying Object");
        clearLightQueue();
        freeGeometryBuffers();
        freeShadowBuffers();
    }

    // Init/Setup ===============================================================

    bool
    GraphicsComponent::init
    ()
    {
        LOG_DEBUG("Initialising");

        onWindowDimensionsChanged();
        GLCheckError();

        if (!setupGeometryBuffers())
        {
            LOG_ERROR("Unable to create geometry buffers");
            return false;
        }

        if (!setupShadowBuffers())
        {
            LOG_ERROR("Unable to create shadow pass buffers");
            return false;
        }

        if (!setupScreenQuad())
        {
            LOG_ERROR("Unable to create screen quad");
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        // TODO Multisample kills line & tool rendering for some reason
        //glEnable(GL_MULTISAMPLE);
        LOG_DEBUG("Initialisation Done.");
        return true;
    }

    void
    GraphicsComponent::onWindowDimensionsChanged
    ()
    {
        // Define the viewport dimensions
        int windowWidth  = mWindowComponent->getWidth();
        int windowHeight = mWindowComponent->getHeight();

        if (windowWidth == 0 || windowHeight == 0)
        {
            LOG_DEBUG("It's a bit rude to use 0,0 viewport and divide by 0...");
            windowWidth = 1;
            windowHeight = 1;
        }

        glViewport(0, 0, windowWidth, windowHeight);
        GLCheckError();
        freeGeometryBuffers();
        setupGeometryBuffers();

        LOG_DEBUG("Window dimensions changed: width: {}, height: {}",windowWidth, windowHeight);
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
        if (!mEnabled)
        {

            LOG_CRITICAL("Component Disabled");
            return;
        }

        LOG_DEBUG("\n\n==> Running Geometry Render Pass\n");
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
            glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), 1.0f);
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
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
        auto width = mWindowComponent->getWidth();
        auto height = mWindowComponent->getHeight();

        if (width == 0 || height == 0)
        {
            LOG_DEBUG("It's a bit rude to allocate empty buffers...");
            width = 1;
            height = 1;
        }

        LOG_DEBUG("Setting up Geometry Buffers with dimensions {}x{}",width,height);

        glGenFramebuffers(1,&mGeometryPassFB);
        GLCheckError();

        if (mGeometryPassFB == 0)
        {
            LOG_ERROR("Unable to create Geometry Framebuffer");
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, mGeometryPassFB);
        GLCheckError();

        // 0 - position buffer
        glGenTextures(1, &mGeometryPassPositionBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGeometryPassPositionBuffer, 0);
        GLCheckError();

        // 1 - normal buffer
        glGenTextures(1, &mGeometryPassNormalBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGeometryPassNormalBuffer, 0);
        GLCheckError();

        // 2 - Albedo+specular buffer
        glGenTextures(1, &mGeometryPassAlbedoBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGeometryPassAlbedoBuffer, 0);
        GLCheckError();

        // 4 - Ignore Lighting Buffer
        glGenTextures(1, &mGeometryPassIgnoreBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width,height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
        glBindRenderbuffer(GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mGeometryPassDepthBuffer);
        GLCheckError();

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("Deferred Rendering Framebuffer not complete!");
            return false;
        }
        else
        {
            LOG_DEBUG("Deferred Rending Buffer is complete!");
        }
        mWindowComponent->bindDefaultFrameBuffer();
        GLCheckError();
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
        if (!mEnabled)
        {
            LOG_CRITICAL("Component Disabled");
            return;
        }
        mLightingPassShader = sr->getLightingPassShader();


        LOG_DEBUG
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
            LOG_ERROR("Lighting Shader is nullptr");
            return;
        }

        if (mLightingPassShader->use())
        {

            // Setup source textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mGeometryPassPositionBuffer);
            ShaderRuntime::CurrentTexture0 = mGeometryPassPositionBuffer;

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
            ShaderRuntime::CurrentTexture1 = mGeometryPassNormalBuffer;

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
            ShaderRuntime::CurrentTexture2 = mGeometryPassAlbedoBuffer;

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);
            ShaderRuntime::CurrentTexture3 = mShadowPassDepthBuffer;

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, mGeometryPassIgnoreBuffer);
            ShaderRuntime::CurrentTexture4 = mGeometryPassIgnoreBuffer;

            GLuint pos, norm, alb, shadow, ignore, depth;
            pos = 0;
            norm = 1;
            alb  = 2;
            shadow = 3;
            ignore = 4;
            mLightingPassShader->addUniform(INT1,"gPosition"  ,1, &pos);
            mLightingPassShader->addUniform(INT1,"gNormal"    ,1, &norm);
            mLightingPassShader->addUniform(INT1,"gAlbedoSpec",1, &alb);
            mLightingPassShader->addUniform(INT1,"gShadow"    ,1, &shadow);
            mLightingPassShader->addUniform(INT1,"gIgnore"    ,1, &ignore);

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
            mWindowComponent->bindDefaultFrameBuffer();
        }
        clearLightQueue();
    }

    // Shadow Pass ==============================================================

    bool
    GraphicsComponent::setupShadowBuffers
    ()
    {
       LOG_INFO("Setting up ShadowPass FrameBuffer");
       glGenFramebuffers(1,&mShadowPassFB);
       GLCheckError();

       if (mShadowPassFB == 0)
       {
          LOG_ERROR("Unable to create shadow pass FB");
          return false;
       }

       glGenTextures(1, &mShadowPassDepthBuffer);
       GLCheckError();

       if (mShadowPassDepthBuffer == 0)
       {

           LOG_ERROR("Unable to create shadow pass depth buffer");
           return false;
       }

       glBindTexture(GL_TEXTURE_2D, mShadowPassDepthBuffer);

       GLCheckError();
       glTexImage2D
       (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            SHADOW_SIZE, SHADOW_SIZE, 0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
       glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
       GLCheckError();

       glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowPassDepthBuffer, 0);
       glDrawBuffer(GL_NONE);
       glReadBuffer(GL_NONE);
       glBindFramebuffer(GL_FRAMEBUFFER, 0);
       GLCheckError();

       return true;
    }

    void
    GraphicsComponent::renderShadowPass
    (SceneRuntime* sr)
    {
        if (!mEnabled)
        {
            LOG_CRITICAL("Component Disabled");
            return;
        }

        mShadowPassShader = sr->getShadowPassShader();

        if (mShadowLight == nullptr || mShadowPassShader == nullptr)
        {
            LOG_ERROR
            (
                "Cannot render shadow pass Light: {}, Shader: {]",
                mShadowLight != nullptr,
                mShadowPassShader != nullptr
            );
            return;
        }


        LOG_DEBUG("\n\n==> Running Shadow Render Pass\n");

        glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
        glBindFramebuffer(GL_FRAMEBUFFER, mShadowPassFB);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Setup Uniforms
        static float near_plane = 1.0f;
        static float far_plane = 300.0f;
        static float sz = 100.0f;

        static glm::mat4 lightProjection = glm::ortho(-sz, sz, -sz, sz, near_plane, far_plane);

        mat4 lightMat = mShadowLight->getTransform().getMatrix();
        mat4 lightView = glm::lookAt
        (
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
        if (mShaderCache != nullptr)
        {
            mShaderCache->drawShadowPass(mShadowMatrix, mShadowPassShader);
        }
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    void
    GraphicsComponent::freeShadowBuffers
    ()
    {
        //mWindowComponent->bindDefaultFrameBuffer();
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

    // Accessors ================================================================

    void
    GraphicsComponent::addToLightQueue
    (EntityRuntime* runt)
    {
        auto light = runt->getLightRuntime();
        if (light->getType() == LightType::LT_DIRECTIONAL)
        {
           mShadowLight = runt;
        }
        mLightQueue.push_back(light);
    }

    void
    GraphicsComponent::clearLightQueue
    ()
    {
        static mutex m;
        m.lock();
        mLightQueue.clear();
        m.unlock();
    }

    void
    GraphicsComponent::setShaderCache
    (ShaderCache* cache)
    {
        mShaderCache = cache;
    }

    ShaderRuntime*
    GraphicsComponent::getLightingShader
    ()
    const
    {
        return mLightingPassShader;
    }

    void
    GraphicsComponent::setLightingShader
    (ShaderRuntime* lightingShader)
    {
        mLightingPassShader = lightingShader;
    }

    ShaderRuntime*
    GraphicsComponent::getShadowPassShader
    () const
    {
        return mShadowPassShader;
    }

    void
    GraphicsComponent::setShadowPassShader
    (ShaderRuntime* shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    GLuint
    GraphicsComponent::getGeometryPassPositionBuffer
    () const
    {
        return mGeometryPassPositionBuffer;
    }

    GLuint
    GraphicsComponent::getGeometryPassAlbedoBuffer
    () const
    {
        return mGeometryPassAlbedoBuffer;
    }

    GLuint
    GraphicsComponent::getGeometryPassNormalBuffer
    () const
    {
        return mGeometryPassNormalBuffer;
    }

    GLuint
    GraphicsComponent::getGeometryPassDepthBuffer
    () const
    {
        return mGeometryPassDepthBuffer;
    }

    GLuint
    GraphicsComponent::getShadowPassDepthBuffer
    () const
    {
        return mShadowPassDepthBuffer;
    }

    GLuint
    GraphicsComponent::getGeometryPassIgnoreBuffer
    () const
    {
        return mGeometryPassIgnoreBuffer;
    }

    void
    GraphicsComponent::pushTask
    (GraphicsComponentTask* t)
    {
       mTaskQueue.push_back(t);
    }
    void
    GraphicsComponent::pushDestructionTask
    (const shared_ptr<GraphicsComponentDestructionTask>& t)
    {
        mDestructionTaskQueue.push_back(t);
    }

    void
    GraphicsComponent::executeTaskQueue
    ()
    {
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
                if (t->isWaitingForDependencies())
                {
                    t->incrementDeferralCount();
                }
                else
                {
                    t->setState(TaskState::ACTIVE);
                    t->execute();
                }

                if (t->getState() == TaskState::COMPLETED)
                {
                   completed.push_back(t);
                }
            }

            for (auto t : completed)
            {
                auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                if (itr != mTaskQueue.end())
                {
                    t->notifyDependents();
                    mTaskQueue.erase(itr);
                }
            }
            completed.clear();
        }
        mTaskQueue.clear();
    }

    void
    GraphicsComponent::executeDestructionTaskQueue
    ()
    {
        for (auto itr = mDestructionTaskQueue.begin(); itr != mDestructionTaskQueue.end(); itr++)
        {
            const auto& t = (*itr);
            t->setState(TaskState::ACTIVE);
            t->execute();
        }
        mDestructionTaskQueue.clear();
    }

    const vector<GraphicsComponentTask*>&
    GraphicsComponent::getDebugTaskQueue
    ()
    {
        return mDebugTaskQueue;
    }
}
