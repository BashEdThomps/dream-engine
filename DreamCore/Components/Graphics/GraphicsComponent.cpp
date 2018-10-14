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

#define GLM_FORCE_RADIANS
#include "GraphicsComponent.h"

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

#include "../../Common/Constants.h"

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
    (
            Camera* camera,
            IWindowComponent* windowComponent
            )
        : IComponent(),
          mCamera(camera),
          mMinimumDraw(1.0f),
          mMaximumDraw(3000.0f),
          mMeshCullDistance(2500.0f),
          mWindowComponent(windowComponent),
          mShaderCacheHandle(nullptr)
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
        clearModelQueue();
        clearLightQueue();
    }

    bool
    GraphicsComponent::init
    (void)
    {
        auto log = getLog();
        log->debug("Initialising");
        log->debug("Initialising GLEW");

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

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        checkGLError();

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

        log->debug
        (
            "Window Dimensions Changed! {}x{}",
            windowWidth , windowHeight
        );

        glViewport(0, 0, windowWidth, windowHeight);

        checkGLError();

        // Ortho projection for 2D
        mOrthoProjection = ortho
        (
            0.0f, static_cast<float>(windowWidth), // Left, Right
            static_cast<float>(windowHeight), 0.0f // Botto, Top
        );

        checkGLError();

        // Perspective Projection Matrix
        mProjectionMatrix = perspective(
            Constants::CAMERA_ZOOM,
            static_cast<float>(windowWidth)/static_cast<float>(windowHeight),
            mMinimumDraw,
            mMaximumDraw
        );

        checkGLError();

        log->debug
        (
            "Window dimensions changed: width: {}, height: {} min draw: {}, max draw {}",
            mWindowComponent->getWidth(),
            mWindowComponent->getHeight(),
            mMinimumDraw,
            mMaximumDraw
        );
    }

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

    void
    GraphicsComponent::preModelRender
    ()
    {
        auto log = getLog();
        log->debug("Pre Render" );
        checkGLError();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Clear the colorbuffer
        if (mActiveSceneRuntime != nullptr)
        {
            auto clearColour = mActiveSceneRuntime->getClearColour();
            glClearColor
            (
                clearColour[Constants::RED_INDEX],
                clearColour[Constants::GREEN_INDEX],
                clearColour[Constants::BLUE_INDEX],
                clearColour[Constants::ALPHA_INDEX]
            );
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
        }

        checkGLError();
    }

    void
    GraphicsComponent::postModelRender
    ()
    {
        auto log = getLog();
        log->debug("Post Render" );
        checkGLError();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        checkGLError();
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
            // Clear existing Queues
            clearModelQueue();
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

                        // Models
                        /*
                        if (object->hasModelInstance())
                        {
                            if (object->hasShaderInstance())
                            {
                                addToModelQueue(object);
                            }
                            else
                            {
                                log->error("Object {} has model, but no shader assigned." , object->getUuid());
                            }
                        }
                        */

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
        endUpdate();
    }

    void
    GraphicsComponent::clearModelQueue
    ()
    {
        auto log = getLog();
        log->debug("Clear 3D Queue" );
        mModelQueue.clear();
    }


    /* INTERMISSION =========================================================
     *
     * We hope you're enjoying tonights show.
     *
     * Let's think about rendering pipeline optimisation...
     *
     * I want to order my rendering quque in such a way that it minimises shader
     * changes. Using one shader per ModelInstance is OK but I'm moving to a
     * 1-shader-per-material approach to allow ModelInstances to be rendered
     * with multiple shaders, depending on their material.
     *
     * Each ModelIinstance has a table of Materials present within the model and the
     * ShaderInstance that said material want's to be rendered with.
     *
     * Using this data I can find all of the meshes in the scene that want to
     * use any given shader. Then I can build a data structure that holds the
     * relationship of ShaderInstance to vector<Mesh>. These meshes can be
     * ordered by distance to implement draw distance limiting.
     *
     * e.g
     *
     * // Meshes in Frustum OrderedBy Distance Ascending
     *
     * vector<Mesh> mMeshes;
     *
     * for (Mesh m : mMeshes)
     * {
     *     if (mesh.radiusFromCamera() > RenderDistanceMax) break;
     *
     *     addToVBO(mMesh)
     * }
     *
     * setupShader(currentShader)
     * render(currentShader,VBO);
     *  setTextures()    \
     *  setUniforms()     \ etc...
     *  setVariables()   /
     *
     * For each ShaderInstance I can setup the shader and render the vector of
     * meshes (Possibly globbed into a single VBO) in order from furthest to
     * or vice cersa. without switching shaders.
     *
     * The goal is to have each shader used only once per frame.
     *
     * The initial map can be built at load-time rather than once per frame,
     * then meshes ordered by distance once per frame as this is dynamic, but
     * mesh's material/shader will never change.
     *
     * =======================================================================
     */


    void
    GraphicsComponent::addToModelQueue
    (SceneObjectRuntime* object)
    {
        auto log = getLog();
        log->debug("Adding {} to 3D Queue",object->getNameAndUuidString());
        mModelQueue.push_back(object);
    }


    void
    GraphicsComponent::debugOptimisedModelQueue
    ()
    {
        auto log = getLog();
        log->critical("Optimised Model Queue");
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

    void
    GraphicsComponent::drawModelQueue
    ()
    {
        auto log = getLog();
        preModelRender();
        log->debug("Draw 3D Queue" );

        /*
        for (SceneObjectRuntime* it : mModelQueue)
        {
            drawModel(it);
            checkGLError();
        }
        */

        if (mShaderCacheHandle != nullptr)
        {
            mShaderCacheHandle->draw
            (
                mViewMatrix,
                mProjectionMatrix,
                mCamera->getTranslation(),
                mLightQueue
            );

        }

        postModelRender();
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
    GraphicsComponent::drawModel
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        checkGLError();

        log->debug("Drawing Model " , sceneObject->getNameAndUuidString() );

        // Get Assets
        const shared_ptr<ModelInstance>& model = sceneObject->getModelInstance();
        const shared_ptr<ShaderInstance>& shader = sceneObject->getShaderInstance();
        shader->use();

        // Set Point Light Values
        log->debug("The scene has {} lights", mLightQueue.size());
        shader->bindLightQueue(mLightQueue);

        vec3 cameraTranslation = mCamera->getTranslation();
        shader->addUniform(FLOAT3,"viewPos",1,&cameraTranslation);
        checkGLError();

        // Pass view/projection transform to shader
        shader->setProjectionMatrix(mProjectionMatrix);
        checkGLError();

        shader->setViewMatrix(mViewMatrix);
        checkGLError();

        // calculate the model matrix
        mat4 modelMatrix = sceneObject->getTransform()->asMat4();
        vec3 objTranslation = sceneObject->getTranslation();
        /*
        mat4 modelMatrix = mat4(1.0f);
        // Get raw data
        quat objOrientation = sceneObject->getTransform()->getOrientation();
        vec3 objScale       = sceneObject->getScale();
        // Translate
        modelMatrix = translate(modelMatrix,objTranslation);
        // Rotate
        mat4 rotMat = mat4_cast(objOrientation);
        modelMatrix = modelMatrix * rotMat;
        // Scale
        modelMatrix = scale(modelMatrix, objScale);
        model->setModelMatrix(modelMatrix);
        */

        // Pass model matrix to shader
        shader->setModelMatrix(modelMatrix);
        checkGLError();

        // Draw using shader
        bool always = sceneObject->getSceneObjectDefinition()->getAlwaysDraw();
        model->draw(shader, objTranslation, mCamera->getTranslation(), mMeshCullDistance, always);
        checkGLError();
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

} // End of Dream
