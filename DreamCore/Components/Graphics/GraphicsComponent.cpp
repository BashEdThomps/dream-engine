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

#include "Font/FontInstance.h"
#include "Font/FontCharacter.h"

#include "Light/LightInstance.h"

#include "Model/AssimpModelInstance.h"
#include "Model/AssimpMesh.h"
#include "Model/MaterialCache.h"

#include "Shader/ShaderInstance.h"
#include "Shader/ShaderCache.h"

#include "Sprite/SpriteInstance.h"

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
    void GraphicsComponent::setMinimumDraw(float minimumDraw)
    {
        mMinimumDraw = minimumDraw;
    }

    void GraphicsComponent::setMaximumDraw(float maximumDraw)
    {
        mMaximumDraw = maximumDraw;
    }

    GraphicsComponent::GraphicsComponent
    (
        shared_ptr<Camera> camera,
        shared_ptr<IWindowComponent> windowComponent
    )
        : IComponent(),
          mCamera(camera),
          mMinimumDraw(1.0f),
          mMaximumDraw(3000.0f),
          mMeshCullDistance(2500.0f),
          mWindowComponent(windowComponent),
          mModelQueueType(LINEAR)


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
        clearSpriteQueue();
        clearFontQueue();
        clearModelQueue();
        clearLightQueue();
    }

    bool
    GraphicsComponent::init
    (void)
    {
        auto log = getLog();
        log->info("Initialising");
        log->info("Initialising GLEW");

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            log->error("GLEW failed to initialise");
            return false;
        }

        checkGLError();

        log->info(
                    "OpenGL Version {}, Shader Version {}",
                    glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION)
                    );

        onWindowDimensionsChanged();
        checkGLError();

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        checkGLError();

        create2DVertexObjects();
        checkGLError();

        createFontVertexObjects();
        checkGLError();

        log->info("Initialisation Done.");
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

        {
            log->info
                    (
                        "Window Dimensions Changed! {}x{}",
                        windowWidth , windowHeight
                        );
        }

        glViewport(0, 0, windowWidth, windowHeight);

        checkGLError();

        // Ortho projection for 2D
        mOrthoProjection = ortho
        (
            0.0f, static_cast<float>(windowWidth),
            static_cast<float>(windowHeight), 0.0f
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

    float GraphicsComponent::getMeshCullDistance() const
    {
        return mMeshCullDistance;
    }

    void GraphicsComponent::setMeshCullDistance(float meshCullDistance)
    {
        mMeshCullDistance = meshCullDistance;
    }

    void
    GraphicsComponent::preModelRender
    ()
    {
        auto log = getLog();
        log->info("Pre Render" );
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
        log->info("Post Render" );
        checkGLError();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        checkGLError();
    }

    void
    GraphicsComponent::preFontRender
    ()
    {
        auto log = getLog();
        log->info("Pre Render" );
        checkGLError();

        // Clear the colorbuffer
        if (mActiveSceneRuntime)
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
    GraphicsComponent::postFontRender
    ()
    {
        auto log = getLog();
        log->info("Post Render" );
        checkGLError();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        checkGLError();
    }



    void
    GraphicsComponent::create2DVertexObjects
    ()
    {
        auto log = getLog();
        log->info("Creating 2D VAO/VBO" );
        // Generate
        glGenVertexArrays(1, &mSpriteQuadVAO);
        glGenBuffers(1, &mSpriteVBO);
        // Setup
        glBindBuffer(GL_ARRAY_BUFFER, mSpriteVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mSpriteVertices), mSpriteVertices, GL_STATIC_DRAW);
        glBindVertexArray(mSpriteQuadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(mSpriteQuadVAO, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        // Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void
    GraphicsComponent::createFontVertexObjects
    ()
    {
        auto log = getLog();
        log->info("Creating Font VAO/VBO" );
        // Generate
        glGenVertexArrays(1, &mFontVAO);
        glGenBuffers(1, &mFontVBO);
    }

    void
    GraphicsComponent::updateComponent
    ()
    {
        beginUpdate();
        auto log = getLog();
        log->info("GraphicsComponrnt: updateComponent() Called" );


        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        if (!mWindowComponent->shouldClose())
        {
            // Clear existing Queues
            clearSpriteQueue();
            clearModelQueue();
            clearFontQueue();
            clearLightQueue();

            mActiveSceneRuntime->getRootSceneObjectRuntime()->applyToAll
                    (
                        function<shared_ptr<SceneObjectRuntime>(shared_ptr<SceneObjectRuntime>)>
                        (
                            [&](shared_ptr<SceneObjectRuntime> object)
                            {
                            // Models
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

                            // Sprites
                            if (object->hasSpriteInstance())
                            {
                                if (object->hasShaderInstance())
                                {
                                    addToSpriteQueue(object);
                                }
                                else
                                {
                                    log->error(
                                    "Object {} has sprite, but no shader assigned.",
                                    object->getUuid()
                                    );
                                }
                            }

                            // Fonts
                            if (object->hasFontInstance())
                            {
                                if (object->hasShaderInstance())
                                {
                                    addToFontQueue(object);
                                }
                                else
                                {
                                    log->error(
                                    "Object {} has font, but no shader assigned.",
                                    object->getUuid()
                                    );
                                }
                            }

                            // Lights
                            if (object->hasLightInstance())
                            {
                                shared_ptr<LightInstance> light = object->getLightInstance();
                                log->info("Adding light instance to queue {}",light->getNameAndUuidString());
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
    GraphicsComponent::clearSpriteQueue
    ()
    {
        auto log = getLog();
        log->info("Clear 2D Queue" );
        mSpriteQueue.clear();
    }

    void
    GraphicsComponent::addToSpriteQueue
    (shared_ptr<SceneObjectRuntime> object)
    {
        auto log = getLog();
        log->info("Adding {} to Sprite Queue",object->getNameAndUuidString());
        mSpriteQueue.push_back(object);
    }

    void
    GraphicsComponent::drawSpriteQueue
    ()
    {
        auto log = getLog();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        log->info("Draw 2D Queue" );
        for (shared_ptr<SceneObjectRuntime> sceneObj : mSpriteQueue)
        {
            drawSprite(sceneObj);
        }
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }

    void
    GraphicsComponent::clearModelQueue
    ()
    {
        auto log = getLog();
        log->info("Clear 3D Queue" );
        mModelQueue.clear();
    }


        /* INTERMISSION =========================================================
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
    (shared_ptr<SceneObjectRuntime> object)
    {
        auto log = getLog();
        log->info("Adding {} to 3D Queue",object->getNameAndUuidString());
        switch(mModelQueueType)
        {
            case LINEAR:
                mModelQueue.push_back(object);
                break;
            case OPTIMISED:
                break;
        }
    }


    void GraphicsComponent::debugOptimisedModelQueue()
    {
        auto log = getLog();
        log->critical("Optimised Model Queue");
    }

    void GraphicsComponent::handleResize()
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
        log->info("Draw 3D Queue" );
        for (shared_ptr<SceneObjectRuntime> it : mModelQueue)
        {
            drawModel(it);
            checkGLError();
        }
        postModelRender();
    }

    void
    GraphicsComponent::clearFontQueue
    ()
    {
        auto log = getLog();
        log->info("Clear Font Queue" );
        mFontQueue.clear();
    }

    void
    GraphicsComponent::addToFontQueue
    (shared_ptr<SceneObjectRuntime> object)
    {
        auto log = getLog();
        log->info("Adding {} to Font Queue",
                  object->getNameAndUuidString());
        mFontQueue.push_back(object);
    }

    void
    GraphicsComponent::drawFontQueue
    ()
    {
        auto log = getLog();
        preFontRender();
        log->info("Draw Font Queue" );
        for (shared_ptr<SceneObjectRuntime> it : mFontQueue)
        {
            drawFont(it);
            checkGLError();
        }
        postFontRender();
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
    GraphicsComponent::drawSprite
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        log->info("Drawing Sprite {}", sceneObject->getNameAndUuidString() );

        // Get Assets
        shared_ptr<SpriteInstance> sprite = sceneObject->getSpriteInstance();
        shared_ptr<ShaderInstance> shader = sceneObject->getShaderInstance();
        // Get arguments
        vec2 size = vec2(sprite->getWidth(),sprite->getHeight());

        auto transform = sceneObject->getTransform();

        GLfloat rotateValue = transform->getRotationZ();
        GLfloat scaleValue = transform->getScaleZ();

        vec3 color = vec3(1.0f);
        // Setup Shader
        shader->use();

        float tX = transform->getTranslationX();
        float tY = transform->getTranslationY();

        vec2 position = vec2(tX,tY);
        // Offset origin to middle of sprite
        mat4 model;
        model = translate(model, vec3(position, 0.0f));
        model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = rotate(model, rotateValue, vec3(0.0f, 0.0f, 1.0f));
        model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = scale(model, vec3(size.x*scaleValue,size.y*scaleValue, 1.0f));
        // Pass uniform arguments to shader
        glUniformMatrix4fv
                (
                    glGetUniformLocation(shader->getShaderProgram(), "model"),
                    1,
                    GL_FALSE,
                    value_ptr(model)
                    );
        glUniform3fv
                (
                    glGetUniformLocation(shader->getShaderProgram(), "spriteColor"),
                    1,
                    value_ptr(color)
                    );
        glUniform1i
                (
                    glGetUniformLocation(shader->getShaderProgram(),"image"),
                    0
                    );
        glUniformMatrix4fv
                (
                    glGetUniformLocation(shader->getShaderProgram(), "projection"),
                    1,
                    GL_FALSE,
                    value_ptr(mOrthoProjection)
                    );
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,sprite->getTexture());
        // Bind VAO
        glBindVertexArray(mSpriteQuadVAO);
        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Cleanup
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void
    GraphicsComponent::drawFont
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        log->info("Drawing Font {}", sceneObject->getNameAndUuidString() );

        // Get Assets
        shared_ptr<FontInstance> font = sceneObject->getFontInstance();
        shared_ptr<ShaderInstance> shader = sceneObject->getShaderInstance();
        shader->use();

        shader->setProjectionMatrix(mProjectionMatrix);
        checkGLError();

        shader->setViewMatrix(mViewMatrix);
        checkGLError();

        vec3 fontColour = font->getColourAsVec3();
        GLint fontColourLocation = glGetUniformLocation(shader->getShaderProgram(),"textColour");
        if (fontColourLocation != -1)
        {
            glUniform3fv(fontColourLocation,1,value_ptr(fontColour));
        }
        else
        {
            log->info("Font: Unable to set textColour uniform");
        }

        // calculate the model matrix
        vec3 fontTranslation = sceneObject->getTranslation();
        vec3 camTranslation = mCamera->getRelativeTranslation(fontTranslation.z);
        float advance = 0;
        //mat4 rotationMatrix = mCamera->getRelativeRotation(translation);

        // Iterate through all characters
        string text = font->getText();
        map<GLchar,FontCharacter> charMap = font->getCharMap();
        for (string::const_iterator c = text.begin(); c != text.end(); c++)
        {
            mat4 modelMatrix;

            if (sceneObject->followsCamera())
            {
                log->info("Font: Applying Camera Transform" );
                //modelMatrix *= rotationMatrix;
                modelMatrix = translate(modelMatrix,camTranslation);
            }

            modelMatrix = translate(modelMatrix,vec3(fontTranslation.x+advance,fontTranslation.y,fontTranslation.z));

            // Pass model matrix to shader
            shader->setModelMatrix(modelMatrix);
            checkGLError();

            float fontScalar = 10.0f;
            FontCharacter ch = charMap[*c];
            GLfloat xpos = ch.Bearing.x / fontScalar;
            GLfloat ypos = ch.Bearing.y / fontScalar;
            GLfloat width = ch.Size.x   / fontScalar;
            GLfloat height = ch.Size.y  / fontScalar;


            FontCharacterVertex vertices[6] =
            {
                FontCharacterVertex // Bottom Left
                (
                vec3(xpos, ypos - height, 0.0f),
                vec2(0.0f, 1.0f)
                ),
                FontCharacterVertex // Top Right
                (
                vec3(xpos + width, ypos, 0.0f),
                vec2(1.0f, 0.0f)
                ),
                FontCharacterVertex // Top Left
                (
                vec3(xpos, ypos, 0.0f),
                vec2(0.0f, 0.0f)
                ),
                FontCharacterVertex // Top Right
                (
                vec3(xpos + width, ypos, 0.0f),
                vec2(1.0f, 0.0f)
                ),
                FontCharacterVertex // Bottom Left
                (
                vec3(xpos, ypos - height, 0.0f),
                vec2(0.0f, 1.0f)
                ),
                FontCharacterVertex // Bottom Right
                (
                vec3(xpos + width, ypos - height, 0.0f),
                vec2(1.0f, 1.0f)
                )
            };

            log->info("Font texture is {}" , ch.TextureID );
            // Setup Texture
            glActiveTexture(GL_TEXTURE0);
            checkGLError();
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            checkGLError();
            // Setup VBO
            glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
            checkGLError();
            glBufferData(GL_ARRAY_BUFFER, sizeof(FontCharacterVertex)*6, &vertices[0], GL_STATIC_DRAW);
            checkGLError();
            // Setup VAO
            shader->bindVertexArray(mFontVAO);
            checkGLError();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                        0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(FontCharacterVertex),
                        static_cast<GLvoid*>(nullptr)
                        );
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                        1, 2, GL_FLOAT, GL_FALSE,
                        sizeof(FontCharacterVertex),
                        (GLvoid*)(offsetof(FontCharacterVertex, TextureCoordinates))
                        );

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            checkGLError();

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64)

            advance += (ch.Advance >> 6)/fontScalar; // * scale;
        }
        shader->unbindVertexArray();
        glBindTexture(GL_TEXTURE_2D, 0);
        //shader->unbind();
    }

    void
    GraphicsComponent::drawModel
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        checkGLError();

        log->info("Drawing Model " , sceneObject->getNameAndUuidString() );

        // Get Assets
        shared_ptr<AssimpModelInstance> model = sceneObject->getModelInstance();
        shared_ptr<ShaderInstance> shader = sceneObject->getShaderInstance();
        shader->use();

        // Set Point Light Values
        log->info("The scene has {} lights", mLightQueue.size());
        for (size_t i=0; i < mLightQueue.size(); i++)
        {
            auto light = mLightQueue.at(i);
            shader->bindLight(light);
            checkGLError();
        }

        vec3 cameraTranslation = mCamera->getTranslation();
        shader->addUniform(FLOAT3,"viewPos",1,&cameraTranslation);
        checkGLError();

        // Pass view/projection transform to shader
        shader->setProjectionMatrix(mProjectionMatrix);
        checkGLError();

        shader->setViewMatrix(mViewMatrix);
        checkGLError();

        // calculate the model matrix
        mat4 modelMatrix = mat4(1.0f);
        // Get raw data
        vec3 objTranslation = sceneObject->getTranslation();
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
    (shared_ptr<LightInstance> lightInstance)
    {
        mLightQueue.push_back(lightInstance);
    }

    void
    GraphicsComponent::clearLightQueue
    ()
    {
        mLightQueue.clear();
    }

    shared_ptr<Camera>
    GraphicsComponent::getCamera
    ()
    {
        return mCamera;
    }
} // End of Dream
