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

#include "../Transform3D.h"

#include "../Window/IWindowComponent.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectDefinition.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

#include "../../Utilities/Math.h"
#include "../Time.h"


using glm::vec3;
using glm::mat4;
using glm::rotate;
using glm::translate;
using glm::scale;

namespace Dream
{


    GraphicsComponent::GraphicsComponent
    (Camera* camera, IWindowComponent* windowComponent)
        : IComponent(),
          mCamera(camera),
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
          mScreenQuadVAO(0),
          mScreenQuadVBO(0),
          mLightingShader(nullptr)
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
    }

    // Init/Setup ===============================================================

    bool
    GraphicsComponent::init
    (void)
    {
        auto log = getLog();
        log->debug("Initialising");

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            log->error("GLEW failed to initialise");
            return false;
        }

        checkGLError();

        log->debug(
            "OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );

        onWindowDimensionsChanged();
        checkGLError();

        if (!setupGeometryBuffers())
        {
            log->error("Unable to create geometry buffers");
            return false;
        }

        if (!setupScreenQuad())
        {
            log->error("Unable to create screen quad");
        }

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glDisable(GL_BLEND);

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
        mProjectionMatrix = perspective(
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
    ()
    {
        beginUpdate();
        auto log = getLog();
        log->debug("GraphicsComponrnt: updateComponent() Called" );

        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        if (!mWindowComponent->shouldClose())
        {
            updateLightQueue();
        }
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
    ()
    {
        auto log = getLog();
        log->debug("Running Geometry Render Pass");

        // Setup
        glBindFramebuffer(GL_FRAMEBUFFER,mGeometryPassFB);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Clear the colorbuffer
        if (mActiveSceneRuntime != nullptr)
        {
            auto clearColour = mActiveSceneRuntime->getClearColour();
            glClearColor
            (
                clearColour[Constants::RED_INDEX],
                clearColour[Constants::GREEN_INDEX],
                clearColour[Constants::BLUE_INDEX],
                1.0f//clearColour[Constants::ALPHA_INDEX]
            );
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            return;
        }

        if (mShaderCacheHandle != nullptr)
        {
            mShaderCacheHandle->draw
            (
                mViewMatrix,
                mProjectionMatrix,
                mCamera->getTranslation()
            );
        }
    }

    // Lighting Pass ============================================================

    void
    GraphicsComponent::freeGeometryBuffers
    ()
    {
        //mWindowComponent->bindDefaultFrameBuffer();

        if (mGeometryPassFB != 0)
        {
            glDeleteFramebuffers(1, &mGeometryPassFB);
        }

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGeometryPassPositionBuffer, 0);
        checkGLError();

        // - normal color buffer
        glGenTextures(1, &mGeometryPassNormalBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassNormalBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGeometryPassNormalBuffer, 0);
        checkGLError();

        // - color + specular color buffer
        glGenTextures(1, &mGeometryPassAlbedoBuffer);
        glBindTexture(GL_TEXTURE_2D, mGeometryPassAlbedoBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGeometryPassAlbedoBuffer, 0);
        checkGLError();

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);
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
    ()
    {
        // Clear Buffer
        mWindowComponent->bindDefaultFrameBuffer();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glDisable(GL_DEPTH);
        //glDisable(GL_BLEND);

        mLightingShader->use();

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

        GLuint pos, norm, alb;
        pos  = 0;
        norm = 1;
        alb  = 2;
        mLightingShader->addUniform(INT1,"gPosition"  ,1, &pos);
        mLightingShader->addUniform(INT1,"gNormal"    ,1, &norm);
        mLightingShader->addUniform(INT1,"gAlbedoSpec",1, &alb);

        mLightingShader->setViewerPosition(mCamera->getTranslation());
        mLightingShader->bindLightQueue(mLightQueue);
        mLightingShader->syncUniforms();

        mLightingShader->bindVertexArray(mScreenQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void
    GraphicsComponent::updateLightQueue
    ()
    {
        auto log = getLog();
        log->debug("Updating Light Queue");
        // Clear existing Queues
        clearLightQueue();

        mActiveSceneRuntime->getRootSceneObjectRuntime()->applyToAll
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
                    }

                    return nullptr;
                }
            )
        );
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
    GraphicsComponent::getViewMatrix
    ()
    {
        return mViewMatrix;
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

    Camera*
    GraphicsComponent::getCamera
    ()
    {
        return mCamera;
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
        return mLightingShader;
    }

    void
    GraphicsComponent::setLightingShader
    (ShaderInstance* lightingShader)
    {
        mLightingShader = lightingShader;
    }

} // End of Dream
