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

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
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
#include "Model/TextureCache.h"

#include "Shader/ShaderInstance.h"
#include "Shader/ShaderCache.h"

#include "Sprite/SpriteInstance.h"

#include "../Transform3D.h"

#include "../Window/IWindowComponent.h"

#include "../../Common/Constants.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

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
          mWindowComponentHandle(windowComponent),
          mActiveSceneRuntimeHandle(nullptr)
    {

    }

    GraphicsComponent::~GraphicsComponent
    (void)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Destroying Object" << endl;
        }

        clear2DQueue();
        clear3DQueue();
        clearLightQueue();

        mActiveSceneRuntimeHandle = nullptr;
    }

    bool
    GraphicsComponent::init
    (void)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Initialising" << endl;
            cout << "GraphicsComponent: Initialising GLEW" << endl;
        }

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            cerr << "GraphicsComponent: GLEW failed to initialise." << endl;
            return false;
        }

        Constants::checkGLError("After GLEW init");

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: OpenGL Version " << glGetString(GL_VERSION) << endl;
            cout << "GraphicsComponent: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        }

        onWindowDimensionsChanged();
        Constants::checkGLError("After initial window dimensions changed");


        Constants::checkGLError("After enable depth");
        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        Constants::checkGLError("After perspective correction");

        create2DVertexObjects();
        Constants::checkGLError("After create 2D Vertex VBO/VAO");

        createFontVertexObjects();
        Constants::checkGLError("After create Font 2D VBO/VAO");

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Initialisation Done." << endl;
        }
        return true;
    }

    void
    GraphicsComponent::onWindowDimensionsChanged
    ()
    {

        // Define the viewport dimensions
        int windowWidth  = mWindowComponentHandle->getWidth();
        int windowHeight = mWindowComponentHandle->getHeight();

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponwnt: Window Dimensions Changed! "
                 << windowWidth << "," << windowHeight << endl;
        }

        glViewport(0, 0, windowWidth, windowHeight);

        Constants::checkGLError("After glViewport");

        // Ortho projection for 2D
        mOrthoProjection = ortho
                (
                    0.0f,
                    static_cast<float>(windowWidth),
                    static_cast<float>(windowHeight),
                    0.0f,
                    -1.0f, 1.0f
                    );

        Constants::checkGLError("After ortho");

        // Perspective Projection Matrix
        mProjectionMatrix = perspective(
                    mCamera->getZoom(),
                    static_cast<float>(windowWidth)/static_cast<float>(windowHeight),
                    mMinimumDraw,
                    mMaximumDraw
                    );

        Constants::checkGLError("After projection matrix");

        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponent: Window dimensions changed "
                 << " WindowWidth:" << mWindowComponentHandle->getWidth()
                 << " WindowHeight:" << mWindowComponentHandle->getHeight()
                 << " MinDraw: " << mMinimumDraw
                 << " MaxDraw: " << mMaximumDraw << endl;
        }
    }

    void
    GraphicsComponent::preRender
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Pre Render" << endl;
        }
        Constants::checkGLError("before pre render");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Clear the colorbuffer
        if (mActiveSceneRuntimeHandle)
        {
            glClearColor
            (
                mActiveSceneRuntimeHandle->getClearColour()[Constants::RED_INDEX],
                mActiveSceneRuntimeHandle->getClearColour()[Constants::GREEN_INDEX],
                mActiveSceneRuntimeHandle->getClearColour()[Constants::BLUE_INDEX],
                mActiveSceneRuntimeHandle->getClearColour()[Constants::ALPHA_INDEX]
            );
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
        }

        Constants::checkGLError("after pre render");
    }

    void
    GraphicsComponent::postRender
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Post Render" << endl;
        }
        Constants::checkGLError("before post render");
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        Constants::checkGLError("after post render");
    }

    void
    GraphicsComponent::create2DVertexObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Creating 2D VAO/VBO" << endl;
        }
        // Generate
        glGenVertexArrays(1, &mSpriteQuadVAO);
        glGenBuffers(1, &mSpriteVBO);
        // Setup
        glBindBuffer(GL_ARRAY_BUFFER, mSpriteVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mSpriteVertices), mSpriteVertices, GL_STATIC_DRAW);
        glBindVertexArray(mSpriteQuadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(mSpriteQuadVAO, 4, GL_FLOAT, GL_FALSE, 0, 0);
        // Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void
    GraphicsComponent::createFontVertexObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Creating Font VAO/VBO" << endl;
        }
        // Generate
        glGenVertexArrays(1, &mFontVAO);
        glGenBuffers(1, &mFontVBO);
        // Setup
        glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * 6, NULL, GL_STATIC_DRAW);
        glBindVertexArray(mFontVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);
        // Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void
    GraphicsComponent::updateComponent
    (SceneRuntime* scene)
    {
        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponrnt: updateComponent(Scene*) Called" << endl;
        }

        if (mWindowComponentHandle->sizeHasChanged())
        {
            onWindowDimensionsChanged();
        }

        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        if (!mWindowComponentHandle->shouldClose())
        {
            // Clear existing Queues
            clear2DQueue();
            clear3DQueue();
            clearLightQueue();

            scene->getRootSceneObjectRuntimeHandle()->applyToAll
            (
                function<void*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* object)
                    {
                        // Models
                        if (object->hasModelInstance())
                        {
                            if (object->hasShaderInstance())
                            {
                                addTo3DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object " << object->getUuid()
                                << " has model, but no shader assigned." << endl;
                            }
                        }

                        // Sprites
                        if (object->hasSpriteInstance())
                        {
                            if (object->hasShaderInstance())
                            {
                                addTo2DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object "
                                     << object->getUuid()
                                     << " has sprite, but no shader assigned."
                                     << endl;
                            }
                         }

                        // Fonts
                        if (object->hasFontInstance())
                        {
                            if (object->hasShaderInstance())
                            {
                                addTo3DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object "
                                     << object->getUuid()
                                     << " has font, but no shader assigned."
                                     << endl;
                            }
                        }

                        // Lights
                        if (object->hasLightInstance())
                        {
                            addToLightQueue(object->getLightInstance());
                        }

                        return nullptr;
                    }
                )
            );
        }
    }

    void
    GraphicsComponent::clear2DQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Clear 2D Queue" << endl;
        }
        m2DQueue.clear();
    }

    void
    GraphicsComponent::addTo2DQueue
    (SceneObjectRuntime* object)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Adding "
                 << object->getNameAndUuidString()
                 << " to 2D Queue" << endl;
        }
        m2DQueue.push_back(object);
    }

    void
    GraphicsComponent::draw2DQueue
    ()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Draw 2D Queue" << endl;
        }
        for (SceneObjectRuntime* sceneObj : m2DQueue)
        {
            if (sceneObj->hasSpriteInstance())
            {
                drawSprite(sceneObj);
            }
        }
        glDisable(GL_BLEND);
    }

    void
    GraphicsComponent::clear3DQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Clear 3D Queue" << endl;
        }
        m3DQueue.clear();
    }

    void
    GraphicsComponent::addTo3DQueue
    (SceneObjectRuntime* object)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Adding "
                 << object->getNameAndUuidString()
                 << " to 3D Queue" << endl;
        }
        m3DQueue.push_back(object);
    }

    void
    GraphicsComponent::draw3DQueue
    ()
    {
        preRender();

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Draw 3D Queue" << endl;
        }

        for (SceneObjectRuntime* it : m3DQueue)
        {
            if (it->hasModelInstance())
            {
                drawModel(it);
            }
            else if (it->hasFontInstance())
            {
                drawFont(it);
                Constants::checkGLError("GfxComponent: After Draw Font");
            }
        }

        postRender();
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
    (SceneObjectRuntime* sceneObject)
    {
        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponent: Drawing Sprite " << sceneObject->getNameAndUuidString() << endl;
        }

        // Get Assets
        SpriteInstance* sprite = sceneObject->getSpriteInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        // Get arguments
        vec2 size = vec2(sprite->getWidth(),sprite->getHeight());

        Transform3D transform = sceneObject->getTransform();

        GLfloat rotateValue = transform.getRotationZ();
        GLfloat scaleValue = transform.getScaleZ();

        vec3 color = vec3(1.0f);
        // Setup Shader
        shader->use();

        float tX = transform.getTranslationX();
        float tY = transform.getTranslationY();

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
    (SceneObjectRuntime* sceneObject)
    {
        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponent: Drawing Font " << sceneObject->getNameAndUuidString() << endl;
        }

        // Get Assets
        FontInstance* font = sceneObject->getFontInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        shader->use();

        shader->setProjectionMatrix(mProjectionMatrix);
        Constants::checkGLError("GfxComponent: Font: After set projection matrix");

        shader->setViewMatrix(mViewMatrix);
        Constants::checkGLError("GfxComponent: Font: After set view matrix");

        // calculate the model matrix
        mat4 modelMatrix;
        // Get raw data
        vec3 translation = sceneObject->getTranslation();
        quat rot = sceneObject->getTransform().getOrientation();
        vec3 scaleValue = sceneObject->getScale();
        // Translate
        modelMatrix = translate(modelMatrix,translation);
        // Rotate
        mat4 rotMat = mat4_cast(rot);
        modelMatrix = modelMatrix * rotMat;
        // Scale
        modelMatrix = scale(modelMatrix, scaleValue);

        // Pass model matrix to shader
        shader->setModelMatrix(modelMatrix);
        Constants::checkGLError("GfxComponent: Font: After set model matrix");

        // Activate corresponding render state
        glUniform3fv
        (
            glGetUniformLocation(shader->getShaderProgram(), "textColor"),
            1,
            value_ptr(vec3(font->getColour()[0],font->getColour()[1],font->getColour()[2]))
        );
        Constants::checkGLError("GfxComponent: Font: After textColor");

        glActiveTexture(GL_TEXTURE0);
        Constants::checkGLError("GfxComponent: Font: After ActivateTexture");

        shader->bindVertexArray(mFontVAO);
        Constants::checkGLError("GfxComponent: Font: After Bind VAO");

        // Iterate through all characters
        string text = font->getText();
        map<GLchar,FontCharacter> charMap = font->getCharMap();
        for (string::const_iterator c = text.begin(); c != text.end(); c++)
        {
            FontCharacter ch = charMap[*c];

            GLfloat xpos = translation.x + ch.Bearing.x;
            GLfloat ypos = translation.y - ch.Bearing.y;

            GLfloat w = ch.Size.x;
            GLfloat h = ch.Size.y;

            // Update VBO for each character
            GLfloat vertices[6][5] = {
                { xpos,     ypos + h, 0.0,    0.0, 1.0 },
                { xpos,     ypos,     0.0,    0.0, 0.0 },
                { xpos + w, ypos,     0.0,    1.0, 0.0 },

                { xpos,     ypos + h, 0.0,    0.0, 1.0 },
                { xpos + w, ypos,     0.0,    1.0, 0.0 },
                { xpos + w, ypos + h, 0.0,    1.0, 1.0 }
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            Constants::checkGLError("GfxComponent: Font: After ActivateTexture");

            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
            Constants::checkGLError("GfxComponent: Font: Bind Buffer");

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            Constants::checkGLError("GfxComponent: Font: After Buffer Sub Data");

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            Constants::checkGLError("GfxComponent: Font: After Draw Call");

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            translation.x += (ch.Advance >> 6);// * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            Constants::checkGLError("GfxComponent: Font: After unbind buffer");

            Constants::checkGLError("GfxComponent: Font: After set view");
        }
        shader->unbindVertexArray();
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->unbind();
    }

    void
    GraphicsComponent::drawModel
    (SceneObjectRuntime* sceneObject)
    {
        Constants::checkGLError("Before drawModel");

        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponent: Drawing Model " << sceneObject->getNameAndUuidString() << endl;
        }

        // Get Assets
        AssimpModelInstance* model = sceneObject->getModelInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        shader->use();

        // Set Ambient Light Values
        GLfloat ambientStrength = 1.0f;
        vec3 ambientColour(1.0f);

        if (mActiveSceneRuntimeHandle)
        {
            vector<float> ambient = mActiveSceneRuntimeHandle->getAmbientColour();
            ambientColour = vec3
            (
                ambient[Constants::RED_INDEX],
                ambient[Constants::GREEN_INDEX],
                ambient[Constants::BLUE_INDEX]
            );
            ambientStrength = ambient[Constants::ALPHA_INDEX];
        }

        shader->setAmbientLight(ambientColour,ambientStrength);
        Constants::checkGLError("After ambient uniforms");

        // Set Diffuse Light Values
        int i=1;
        for (auto light : mLightQueue)
        {
            vec3 lightPos = light->getSceneObjectRuntimeHandle()->getTransform().getTranslation();
            vec3 lightColour = light->getColor();
            shader->setPointLight(i,lightPos,lightColour);
            i++;
        }
        Constants::checkGLError("After light pos uniform");

        // Pass view/projection transform to shader

        shader->setProjectionMatrix(mProjectionMatrix);
        Constants::checkGLError("After set projection");

        shader->setViewMatrix(mViewMatrix);
        Constants::checkGLError("After set view");

        // Pass Viewer Position Uniform
        shader->setViewerPosition(mCamera->getTranslation());
        Constants::checkGLError("After set camPos uniform");

        // calculate the model matrix
        mat4 modelMatrix;
        // Get raw data
        vec3 translation = sceneObject->getTranslation();
        quat rot = sceneObject->getTransform().getOrientation();
        vec3 scaleValue = sceneObject->getScale();
        // Translate
        modelMatrix = translate(modelMatrix,translation);
        // Rotate
        mat4 rotMat = mat4_cast(rot);
        modelMatrix = modelMatrix * rotMat;
        // Scale
        modelMatrix = scale(modelMatrix, scaleValue);
        model->setModelMatrix(modelMatrix);

        // Pass model matrix to shader
        shader->setModelMatrix(modelMatrix);
        Constants::checkGLError("After set model");

        // Draw using shader
        model->draw(shader);
        Constants::checkGLError("After Draw");

        glBindVertexArray(0);

        shader->unbind();
        Constants::checkGLError("After unbind");
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
    GraphicsComponent::setActiveSceneRuntimeHandle
    (SceneRuntime* scene)
    {
        mActiveSceneRuntimeHandle = scene;
    }

} // End of Dream
