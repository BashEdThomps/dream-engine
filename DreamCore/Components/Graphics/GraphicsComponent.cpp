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

        glEnable(GL_DEPTH_TEST);

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
            cout << "GraphicsComponent: Window dimensions changed" << endl
                 << "\tWindowWidth:" << mWindowComponentHandle->getWidth() << endl
                 << "\tWindowHeight:" << mWindowComponentHandle->getHeight() << endl
                 << "\tMinDraw: " << mMinimumDraw << endl
                 << "\tMaxDraw: " << mMaximumDraw << endl;
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

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Clear the colorbuffer
        if (mActiveSceneRuntimeHandle)
        {
            glClearColor(
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
        glDisable(GL_CULL_FACE);
        glDisable (GL_BLEND);
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
        glGenVertexArrays(1, &mSpriteQuadVAO);
        glGenBuffers(1, &mSpriteVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mSpriteVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mSpriteVertices), mSpriteVertices, GL_STATIC_DRAW);
        glBindVertexArray(mSpriteQuadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void
    GraphicsComponent::createFontVertexObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Creating Font VAO/VBO" << endl;
        }
        glGenVertexArrays(1, &mFontVAO);
        glGenBuffers(1, &mFontVBO);
        glBindVertexArray(mFontVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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
                function < void*(SceneObjectRuntime*) >
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
                                cerr << "GraphicsComponent: Object " << object->getUuid()
                                     << " has sprite, but no shader assigned." << endl;
                            }
                        }

                        // Fonts
                        if (object->hasFontInstance())
                        {
                            if (object->hasShaderInstance())
                            {
                                addTo2DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object " << object->getUuid()
                                     << " has font, but no shader assigned." << endl;
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
            else if (sceneObj->hasFontInstance())
            {
                drawFont(sceneObj);
            }
        }
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
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Draw 3D Queue" << endl;
        }
        for (SceneObjectRuntime* it : m3DQueue)
        {
            drawModel(it);
        }
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
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, value_ptr(model)
                           );
        glUniform3fv(glGetUniformLocation(
                         shader->getShaderProgram(), "spriteColor"),
                     1, value_ptr(color)
                     );
        glUniform1i(glGetUniformLocation(shader->getShaderProgram(),"image"),0);
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, value_ptr(mOrthoProjection)
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
        // Get Assets
        FontInstance* font = sceneObject->getFontInstance();
        float tX = sceneObject->getTransform().getTranslationX();
        float tY = sceneObject->getTransform().getTranslationY();

        // Setup Shader
        ShaderInstance* shader = sceneObject->getShaderInstance();
        vec2 size = vec2(font->getWidth(),font->getHeight());
        GLfloat rotateValue = sceneObject->getTransform().getRotationZ();
        GLfloat scaleValue = sceneObject->getTransform().getScaleZ();

        shader->use();

        vec2 position = vec2(tX,tY);
        // Offset origin to middle of sprite
        mat4 model;
        model = translate(model, vec3(position, 0.0f));
        model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = rotate(model, rotateValue, vec3(0.0f, 0.0f, 1.0f));
        model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = scale(model, vec3(size.x*scaleValue,size.y*scaleValue, 1.0f));

        // Activate corresponding render state
        glUniform3f(
                    glGetUniformLocation(shader->getShaderProgram(), "textColor"),
                    font->getColour()[0], font->getColour()[1], font->getColour()[2]
        );

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mFontVAO);

        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, value_ptr(model)
                           );

        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, value_ptr(mOrthoProjection)
                           );

        // Iterate through all characters
        string text = font->getText();
        map<GLchar,FontCharacter> charMap = font->getCharMap();
        for (string::const_iterator c = text.begin(); c != text.end(); c++)
        {
            FontCharacter ch = charMap[*c];

            GLfloat xpos = tX + ch.Bearing.x;// * scale;
            //GLfloat ypos = tY - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat ypos = tY - (ch.Bearing.y);// * scale);

            GLfloat w = ch.Size.x;// * scale;
            GLfloat h = ch.Size.y;// * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos,     ypos,       0.0, 0.0 },
                { xpos + w, ypos,       1.0, 0.0 },

                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos + w, ypos,       1.0, 0.0 },
                { xpos + w, ypos + h,   1.0, 1.0 }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            tX += (ch.Advance >> 6);// * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

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
        GLint uAmbientStrength = glGetUniformLocation(shader->getShaderProgram(),"ambientStrength");
        GLint uAmbientColor    = glGetUniformLocation(shader->getShaderProgram(),"ambientColor");
        GLfloat strength = 1.0f;

        if (uAmbientColor > 0 && uAmbientStrength > 0)
        {
            vec3 ambientColour(1.0f);
            if (mActiveSceneRuntimeHandle)
            {
                vector<float> ambient = mActiveSceneRuntimeHandle->getAmbientColour();
                ambientColour = vec3(
                    ambient[Constants::RED_INDEX],
                    ambient[Constants::GREEN_INDEX],
                    ambient[Constants::BLUE_INDEX]
                );
                strength = ambient[Constants::ALPHA_INDEX];
            }

            glUniform1f(uAmbientStrength,strength);
            glUniform3fv(uAmbientColor,1,value_ptr(ambientColour));

        }
        else
        {
            if (Constants::VERBOSE)
            {
                cout << "GraphicsComponent: Warning, cound not find ambientColour/ambientStrength uniform locations"
                     << endl;
            }
        }

        // Set Diffuse Light Values
        vector<LightInstance*>::
                iterator lights;
        int i;
        for (i=1, lights = mLightQueue.begin(); lights != mLightQueue.end(); lights++, i++)
        {
            stringstream uPosStr;
            uPosStr << "diffusePos_" << i;
            stringstream uColorStr;
            uColorStr << "diffuseColor_" << i;

            GLint uLightPos   = glGetUniformLocation(shader->getShaderProgram(),uPosStr.str().c_str());
            GLint uLightColor = glGetUniformLocation(shader->getShaderProgram(),uColorStr.str().c_str());

            if (uLightPos > 0)
            {
                vec3 lightPos = (*lights)->getSceneObjectRuntimeHandle()->getTransform().getTranslation();
                glUniform3fv(uLightPos  ,1, value_ptr(lightPos));
            }
            else
            {
                if (Constants::VERBOSE)
                {
                    cout << "GraphicsComponent: cannot find uniform for " << uPosStr.str() << endl;
                }
            }

            if (uLightColor > 0)
            {
                vec3 lightColor = (*lights)->getColor();
                glUniform3fv(uLightColor,1, value_ptr(lightColor));
            }
            else
            {
                if (Constants::VERBOSE)
                {
                    cout << "GraphicsComponent: cannot find uniform for " << uColorStr.str() << endl;
                }
            }

        }

        Constants::checkGLError("After light pos uniform");

        // Pass view/projection transform to shader
        glUniformMatrix4fv
        (
            glGetUniformLocation(shader->getShaderProgram(), "projection"),
            1, GL_FALSE, value_ptr(mProjectionMatrix)
        );

        Constants::checkGLError("After set projection");

        glUniformMatrix4fv
        (
            glGetUniformLocation(shader->getShaderProgram(), "view"),
            1, GL_FALSE, value_ptr(mViewMatrix)
        );

        Constants::checkGLError("After set view");
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
        glUniformMatrix4fv(
                    glGetUniformLocation(shader->getShaderProgram(), "model"),
                    1, GL_FALSE, value_ptr(modelMatrix)
                    );

        Constants::checkGLError("After set model");
        // Draw using shader
        model->draw(shader);

        Constants::checkGLError("After Draw");
        // Unbind shader
        glBindVertexArray(0);
        glUseProgram(0);
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
    GraphicsComponent::cleanUp
    (SceneRuntime* scene)
    {
        clear2DQueue();
        clear3DQueue();
        clearLightQueue();
        // Clean up caches
        TextureCache::cleanUp();
        ShaderCache::cleanUp();
        AssimpModelInstance::cleanUpCache();
        mActiveSceneRuntimeHandle = nullptr;
    }

    void
    GraphicsComponent::setActiveSceneRuntimeHandle
    (SceneRuntime* scene)
    {
        mActiveSceneRuntimeHandle = scene;
    }

} // End of Dream
