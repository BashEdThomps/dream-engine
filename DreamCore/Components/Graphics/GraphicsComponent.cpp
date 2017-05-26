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
#include <glm/gtc/quaternion.hpp>
#include <glm/matrix.hpp>

namespace Dream
{

    GraphicsComponent::GraphicsComponent
    (Camera* camera, IWindowComponent* windowComponent)
        : IComponent()
    {
        mWindowComponent = windowComponent;
        mCamera = camera;
        mClearColour = {1.0f,1.0f,1.0f,1.0f};
        mAmbientLightColour = {1.0f,1.0f,1.0f,1.0f};
    }


    void
    GraphicsComponent::setClearColour
    (vector<float> clearColour)
    {
        mClearColour[CLEAR_RED] = clearColour[CLEAR_RED];
        mClearColour[CLEAR_GREEN] = clearColour[CLEAR_GREEN];
        mClearColour[CLEAR_BLUE] = clearColour[CLEAR_BLUE];
        mClearColour[CLEAR_ALPHA] = clearColour[CLEAR_ALPHA];
    }


    void
    GraphicsComponent::setAmbientLightColour
    (vector<float> clearColour)
    {
        mAmbientLightColour[CLEAR_RED]   = clearColour[CLEAR_RED];
        mAmbientLightColour[CLEAR_GREEN] = clearColour[CLEAR_GREEN];
        mAmbientLightColour[CLEAR_BLUE]  = clearColour[CLEAR_BLUE];
        mAmbientLightColour[CLEAR_ALPHA] = clearColour[CLEAR_ALPHA];
    }


    GraphicsComponent::~GraphicsComponent
    (void)
    {
        if (DEBUG)
        {
            cout << "GraphicsComponent: Destroying Object" << endl;
        }
    }


    bool
    GraphicsComponent::init
    (void)
    {
        if (DEBUG)
        {
            cout << "GraphicsComponent: Initialising..." << endl;
        }

        // Initialize GLEW to setup the OpenGL Function pointers
        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();
        if (glewInitResult != GLEW_OK)
        {
            cerr << "GraphicsComponent: GLEW failed to initialise." << endl;
            return false;
        }
        if (DEBUG)
        {
            cout << "GraphicsComponent: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        }
        // Define the viewport dimensions
        int windowWidth  = mWindowComponent->getWidth();
        int windowHeight = mWindowComponent->getHeight();
        glViewport(0, 0, windowWidth, windowHeight);
        // Ortho projection for 2D
        mOrthoProjection = glm::ortho
                (
                    0.0f,
                    static_cast<float>(windowWidth),
                    static_cast<float>(windowHeight),
                    0.0f,
                    -1.0f, 1.0f
                    );
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        create2DVertexObjects();
        createFontVertexObjects();
        if (DEBUG)
        {
            cout << "GraphicsComponent: Initialisation Done." << endl;
        }
        return true;
    }


    void GraphicsComponent::create2DVertexObjects
    ()
    {
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
    (Scene* scene)
    {
        if (VERBOSE)
        {
            cout << "GraphicsComponrnt: updateComponent(Scene*) Called" << endl;
        }

        if (!mWindowComponent->shouldClose())
        {
            // Clear existing Queues
            clear2DQueue();
            clear3DQueue();
            clearLightQueue();
            // Clear the colorbuffer
            glClearColor
                    (
                        mClearColour[CLEAR_RED],
                        mClearColour[CLEAR_GREEN],
                        mClearColour[CLEAR_BLUE],
                        mClearColour[CLEAR_ALPHA]
                        );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            vector<SceneObject*> scenegraph = scene->getScenegraphVector();
            for (vector<SceneObject*>::
                 iterator it = scenegraph.begin(); it!=scenegraph.end(); it++ )
            {
                SceneObject *object = (*it);

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
            }
        }
    }


    void GraphicsComponent::
    clear2DQueue()
    {
        m2DQueue.clear();
    }


    void GraphicsComponent::
    addTo2DQueue(SceneObject* object)
    {
        m2DQueue.push_back(object);
    }


    void GraphicsComponent::
    draw2DQueue()
    {
        for (vector<SceneObject*>::
             iterator it = m2DQueue.begin(); it!=m2DQueue.end(); it++ )
        {
            SceneObject* sceneObj = *it;
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


    void GraphicsComponent::
    clear3DQueue()
    {
        m3DQueue.clear();
    }


    void GraphicsComponent::
    addTo3DQueue(SceneObject* object)
    {
        m3DQueue.push_back(object);
    }


    void GraphicsComponent::
    draw3DQueue()
    {
        // Transformation matrices
        if (VERBOSE)
        {
            cout << "GraphicsComponent: Drawing 3D Queue" << endl
                 << "\tWindowWidth:" << mWindowComponent->getWidth() << endl
                 << "\tWindowHeight:" << mWindowComponent->getHeight() << endl
                 << "\tMinDraw: " << mMinimumDraw << endl
                 << "\tMaxDraw: " << mMaximumDraw << endl;
        }
        mProjectionMatrix = glm::perspective(
            mCamera->getZoom(),
            static_cast<float>(mWindowComponent->getWidth()) /
            static_cast<float>(mWindowComponent->getHeight()),
            mMinimumDraw,
            mMaximumDraw
        );
        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        for (vector<SceneObject*>::
             iterator it = m3DQueue.begin(); it!=m3DQueue.end(); it++ )
        {
            drawModel(*it);
        }
    }


    glm::mat4 GraphicsComponent::
    getViewMatrix()
    {
        return mViewMatrix;
    }


    glm::mat4 GraphicsComponent::
    getProjectionMatrix()
    {
        return mProjectionMatrix;
    }

    /*
     * Using
     *    https://learnopengl.com/#!In-Practice/2D-Game/Rendering-Sprites
     */

    void GraphicsComponent::
    drawSprite(SceneObject* sceneObject)
    {
        // Get Assets
        SpriteInstance* sprite = sceneObject->getSpriteInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        // Get arguments
        glm::vec2 size = glm::vec2(sprite->getWidth(),sprite->getHeight());
        GLfloat rotate = sceneObject->getTransform()->getRotationZ();
        GLfloat scale = sceneObject->getTransform()->getScaleZ();
        glm::vec3 color = glm::vec3(1.0f);
        // Setup Shader
        shader->use();
        float tX = sprite->getTransform()->getTranslationX();
        float tY = sprite->getTransform()->getTranslationY();
        glm::vec2 position = glm::vec2(tX,tY);
        // Offset origin to middle of sprite
        glm::
                mat4 model;
        model = glm::
                translate(model, glm::
                          vec3(position, 0.0f));
        model = glm::
                translate(model, glm::
                          vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::
                rotate(model, rotate, glm::
                       vec3(0.0f, 0.0f, 1.0f));
        model = glm::
                translate(model, glm::
                          vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::
                scale(model, glm::
                      vec3(size.x*scale,size.y*scale, 1.0f));
        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, glm::
                           value_ptr(model)
                           );
        glUniform3fv(glGetUniformLocation(
                         shader->getShaderProgram(), "spriteColor"),
                     1, glm::
                     value_ptr(color)
                     );
        glUniform1i(glGetUniformLocation(shader->getShaderProgram(),"image"),0);
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, glm::
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
    (SceneObject* sceneObject)
    {
        // Get Assets
        FontInstance* font = sceneObject->getFontInstance();
        float tX = font->getTransform()->getTranslationX();
        float tY = font->getTransform()->getTranslationY();

        // Setup Shader
        ShaderInstance* shader = sceneObject->getShaderInstance();
        glm::vec2 size = glm::vec2(font->getWidth(),font->getHeight());
        GLfloat rotate = sceneObject->getTransform()->getRotationZ();
        GLfloat scale = sceneObject->getTransform()->getScaleZ();

        shader->use();

        glm::vec2 position = glm::vec2(tX,tY);
        // Offset origin to middle of sprite
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size.x*scale,size.y*scale, 1.0f));

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
                           1, GL_FALSE, glm::value_ptr(model)
                           );

        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, glm::value_ptr(mOrthoProjection)
                           );

        // Iterate through all characters
        string text = font->getText();
        map<GLchar,Character> charMap = font->getCharacterMap();
        for (string::const_iterator c = text.begin(); c != text.end(); c++)
        {
            Character ch = charMap[*c];

            GLfloat xpos = tX + ch.Bearing.x * scale;
            GLfloat ypos = tY - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
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
            tX += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);






    }


    void GraphicsComponent::
    drawModel(SceneObject* sceneObject)
    {
        // Get Assets
        AssimpModelInstance* model = sceneObject->getModelInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        shader->use();
        // Set Ambient Light Values
        GLint uAmbientStrength = glGetUniformLocation(shader->getShaderProgram(),"ambientStrength");
        GLint uAmbientColor    = glGetUniformLocation(shader->getShaderProgram(),"ambientColor");
        if (uAmbientColor > 0 && uAmbientStrength > 0)
        {
            glm::
                    vec3 ambientColor = glm::
                    vec3(mAmbientLightColour[0],mAmbientLightColour[1],mAmbientLightColour[2]);
            glUniform1f(uAmbientStrength,mAmbientLightColour[3]);
            glUniform3fv(uAmbientColor,1,glm::
                         value_ptr(ambientColor));
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

            if (uLightPos > 0 && uLightColor > 0)
            {
                glm::
                        vec3 lightPos = (*lights)->getTransform()->getTranslation();
                glm::
                        vec3 lightColor = (*lights)->getColor();
                glUniform3fv(uLightPos  ,1, glm::
                             value_ptr(lightPos));
                glUniform3fv(uLightColor,1, glm::
                             value_ptr(lightColor));
            }
            else
            {
                if (VERBOSE)
                {
                    cout << "GraphicsComponent: cannot find uniform for " << uPosStr.str() << endl;
                    cout << "GraphicsComponent: cannot find uniform for " << uColorStr.str() << endl;
                }
            }
        }

        // Pass view/projection transform to shader
        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "projection"), 1, GL_FALSE, glm::
                           value_ptr(mProjectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "view"), 1, GL_FALSE, glm::
                           value_ptr(mViewMatrix));
        // calculate the model matrix
        glm::
                mat4 modelMatrix;
        // Get raw data
        glm::
                vec3 translation = sceneObject->getTranslation();
        glm::
                quat rot = sceneObject->getTransform()->getOrientation();
        glm::
                vec3 scale = sceneObject->getScale();
        // Translate
        modelMatrix = glm::
                translate(modelMatrix,translation);
        // Rotate
        glm::
                mat4 rotMat = glm::
                mat4_cast(rot);
        modelMatrix = modelMatrix * rotMat;
        // Scale
        modelMatrix = glm::
                scale(modelMatrix, scale);
        // Pass model matrix to shader
        glUniformMatrix4fv(
                    glGetUniformLocation(shader->getShaderProgram(), "model"),
                    1, GL_FALSE, glm::
                    value_ptr(modelMatrix)
                    );
        // Draw using shader
        model->draw(shader);
        // Unbind shader
        glBindVertexArray(0);
        glUseProgram(0);
    }


    bool GraphicsComponent::
    checkGLError(string marker)
    {
        GLenum errorCode = 0;
        bool wasError = false;
        do
        {
            errorCode = glGetError();
            if (errorCode!=0)
            {
                cerr << "GraphicsComponent: Error Check " << marker << ": " << endl;
                switch (errorCode)
                {
                    case GL_NO_ERROR:
                        cerr << "\tGL_NO_ERROR" << endl;
                        break;
                    case GL_INVALID_ENUM:
                        cerr << "\tGL_INVALID_ENUM" << endl;
                        break;
                    case GL_INVALID_VALUE:
                        cerr << "\tGL_INVALID_VALUE" << endl;
                        break;
                    case GL_INVALID_OPERATION:
                        cerr << "\tGL_INVALID_OPERATION" << endl;
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        cerr << "\tGL_INVALID_FRAMEBUFFER_OPERATION" << endl;
                        break;
                    case GL_OUT_OF_MEMORY:
                        cerr << "\tGL_OUT_OF_MEMORY" << endl;
                        break;
                }
                cerr << "\tName: " << glewGetErrorString(errorCode) << endl;
                cerr << "\tCode: " << errorCode << endl;
                wasError = true;
            }
        }
        while(errorCode != 0);
        return wasError;
    }


    void GraphicsComponent::
    addToLightQueue(LightInstance* lightInstance)
    {
        mLightQueue.push_back(lightInstance);
    }


    void GraphicsComponent::
    clearLightQueue()
    {
        mLightQueue.clear();
    }


    void GraphicsComponent::
    setGameController(GameController* gameController)
    {
        mGameController = gameController;
    }


    GameController* GraphicsComponent::
    getGameController()
    {
        return mGameController;
    }

} // End of Dream
