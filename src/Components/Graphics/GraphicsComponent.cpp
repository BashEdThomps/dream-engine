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

#include <SDL2/SDL_ttf.h>
#include "GraphicsComponent.h"
#include <glm/gtc/quaternion.hpp>

namespace Dream {

    GraphicsComponent::GraphicsComponent(Camera* camera) : ComponentInterface () {
        setWindowWidth(Graphics_INTERFACE_DEFAULT_SCREEN_WIDTH);
        setWindowHeight(Graphics_INTERFACE_DEFAULT_SCREEN_HEIGHT);
        mWindowShouldClose = false;
        mCamera = camera;
        mClearColour = {1.0f,1.0f,1.0f,1.0f};
        mAmbientLightColour = {1.0f,1.0f,1.0f,1.0f};
    }

    void GraphicsComponent::setClearColour(vector<float> clearColour) {
        mClearColour[CLEAR_RED] = clearColour[CLEAR_RED];
        mClearColour[CLEAR_GREEN] = clearColour[CLEAR_GREEN];
        mClearColour[CLEAR_BLUE] = clearColour[CLEAR_BLUE];
        mClearColour[CLEAR_ALPHA] = clearColour[CLEAR_ALPHA];
    }

    void GraphicsComponent::setAmbientLightColour(vector<float> clearColour) {
        mAmbientLightColour[CLEAR_RED]   = clearColour[CLEAR_RED];
        mAmbientLightColour[CLEAR_GREEN] = clearColour[CLEAR_GREEN];
        mAmbientLightColour[CLEAR_BLUE]  = clearColour[CLEAR_BLUE];
        mAmbientLightColour[CLEAR_ALPHA] = clearColour[CLEAR_ALPHA];
    }

    bool GraphicsComponent::createSDLWindow() {
        mWindow = SDL_CreateWindow(
                      mScreenName.c_str(),
                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      mWindowWidth, mWindowHeight,
                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                      );

        if (mWindow == nullptr) {
            if (DEBUG) {
                cout << "GraphicsComopnent: SDL_CreateWindow Error = " << SDL_GetError() << endl;
            }
            SDL_Quit();
            return false;
        }
        SDL_SetRelativeMouseMode(SDL_TRUE);
        return true;
    }

    GraphicsComponent::~GraphicsComponent(void) {

        if (DEBUG) {
            cout << "GraphicsComponent: Destroying Object" << endl;
        }

        TTF_Quit();
        if (mWindow != nullptr) {
            SDL_DestroyWindow(mWindow);
        }
        SDL_Quit();
    }

    bool GraphicsComponent::init(void) {
        if (DEBUG) {
            cout << "GraphicsComponent: Initialising..." << endl;
        }

        if (!createSDLWindow()) {
            cerr << "GraphicsComponent: Unable to create SDL Window" << endl;
            return false;
        }


        //Initialize SDL_ttf
        if(TTF_Init() == -1) {
            cerr << "GraphicsComponent: Unable to init SDL TTF" << endl;
            return false;
        }

        //Use OpenGL 3.2 core
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        //Create context
        mContext = SDL_GL_CreateContext(mWindow);
        if(mContext == nullptr) {
            cerr << "GraphicsComponent: OpenGL context could not be created! "
                 << "SDL Says: " << SDL_GetError() << endl;
            return false;
        }
        if (DEBUG) {
            cout << "GraphicsComponent: Initialised SDL" << endl;
        }

        // Initialize GLEW to setup the OpenGL Function pointers
        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();
        if (glewInitResult != GLEW_OK) {
            cerr << "GraphicsComponent: GLEW failed to initialise." << endl;
            return false;
        }
        if (DEBUG) {
            cout << "GraphicsComponent: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        }
        // Define the viewport dimensions
        glViewport(0, 0, mWindowWidth, mWindowHeight);
        // Ortho projection for 2D
        mOrthoProjection = glm::ortho(
                               0.0f,
                               static_cast<float>(mWindowWidth),
                               static_cast<float>(mWindowHeight),
                               0.0f,
                               -1.0f, 1.0f
                               );
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        create2DVertexObjects();
        if (DEBUG) {
            cout << "GraphicsComponent: Initialisation Done." << endl;
        }
        return true;
    }

    void GraphicsComponent::create2DVertexObjects() {
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

    void GraphicsComponent::closeWindow() {
        mWindowShouldClose = true;
    }

    void GraphicsComponent::update(Scene* scene) {
        if (VERBOSE) {
            cout << "GraphicsComponrnt: Update Called" << endl;
        }

        mEvents.clear();
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                mWindowShouldClose = true;
            }
            SDL_Event e;
            memcpy(&e,&event,sizeof(SDL_Event));
            mEvents.push_back(e);
        }
        if (VERBOSE) {
            cout << "GraphicsComponent " << mEvents.size() << " Events" << endl;
        }

        if (!mWindowShouldClose) {
            // Clear existing Queues
            clear2DQueue();
            clear3DQueue();
            clearLightQueue();
            // Clear the colorbuffer
            glClearColor(
                        mClearColour[CLEAR_RED],
                        mClearColour[CLEAR_GREEN],
                        mClearColour[CLEAR_BLUE],
                        mClearColour[CLEAR_ALPHA]
                        );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            vector<SceneObject*> scenegraph = scene->getScenegraphVector();
            for (vector<SceneObject*>::iterator it = scenegraph.begin(); it!=scenegraph.end(); it++ ) {
                SceneObject *object = (*it);
                // Models
                if (object->hasModelInstance()) {
                    if (object->hasShaderInstance()){
                        addTo3DQueue(object);
                    } else {
                        cerr << "GraphicsComponent: Object " << object->getUuid()
                             << " has model, but no shader assigned." << endl;
                    }
                }
                // Sprites
                if (object->hasSpriteInstance()) {
                    if (object->hasShaderInstance()){
                        addTo2DQueue(object);
                    } else {
                        cerr << "GraphicsComponent: Object " << object->getUuid()
                             << " has sprite, but no shader assigned." << endl;
                    }
                }
                // Fonts
                if (object->hasFontInstance()) {
                    if (object->hasShaderInstance()){
                        addTo2DQueue(object);
                    } else {
                        cerr << "GraphicsComponent: Object " << object->getUuid()
                             << " has font, but no shader assigned." << endl;
                    }
                }
                // Lights
                if (object->hasLightInstance()) {
                   addToLightQueue(object->getLightInstance());
                }
            }
        }
    }

    void GraphicsComponent::swapBuffers() {
        SDL_GL_SwapWindow(mWindow);
    }

    void GraphicsComponent::clear2DQueue() {
        m2DQueue.clear();
    }

    void GraphicsComponent::addTo2DQueue(SceneObject* object) {
        m2DQueue.push_back(object);
    }

    void GraphicsComponent::draw2DQueue() {
        for (vector<SceneObject*>::iterator it = m2DQueue.begin(); it!=m2DQueue.end(); it++ ) {
            SceneObject* sceneObj = *it;
            if (sceneObj->hasSpriteInstance()) {
                drawSprite(sceneObj);
            } else if (sceneObj->hasFontInstance()) {
                drawFont(sceneObj);
            }
        }
    }

    void GraphicsComponent::clear3DQueue() {
        m3DQueue.clear();
    }

    void GraphicsComponent::addTo3DQueue(SceneObject* object) {
        m3DQueue.push_back(object);
    }

    void GraphicsComponent::draw3DQueue() {
        // Transformation matrices
        mProjectionMatrix = glm::perspective(
                                mCamera->getZoom(),
                                static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight),
                                mMinimumDraw,
                                mMaximumDraw
                                );
        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        for (vector<SceneObject*>::iterator it = m3DQueue.begin(); it!=m3DQueue.end(); it++ ) {
            drawModel(*it);
        }
    }

    glm::mat4 GraphicsComponent::getViewMatrix() {
        return mViewMatrix;
    }

    glm::mat4 GraphicsComponent::getProjectionMatrix() {
        return mProjectionMatrix;
    }

    /*
     * Using
     *    https://learnopengl.com/#!In-Practice/2D-Game/Rendering-Sprites
     */
    void GraphicsComponent::drawSprite(SceneObject* sceneObject) {
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
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size.x*scale,size.y*scale, 1.0f));
        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, glm::value_ptr(model)
                           );
        glUniform3fv(glGetUniformLocation(
                         shader->getShaderProgram(), "spriteColor"),
                     1, glm::value_ptr(color)
                     );
        glUniform1i(glGetUniformLocation(shader->getShaderProgram(),"image"),0);
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, glm::value_ptr(mOrthoProjection)
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

    void GraphicsComponent::drawFont(SceneObject* sceneObject) {
        // Get Assets
        FontInstance* font = sceneObject->getFontInstance();
        if (font->hasChanged()) {
            font->renderToTexture();
        }
        ShaderInstance* shader = sceneObject->getShaderInstance();
        // Get arguments
        glm::vec2 size = glm::vec2(font->getWidth(),font->getHeight());
        GLfloat rotate = sceneObject->getTransform()->getRotationZ();
        GLfloat scale = sceneObject->getTransform()->getScaleZ();
        glm::vec3 color = glm::vec3(1.0f);
        // Configure VAO/VBO
        // Setup GL
        //createSpriteVertexObjects();
        // Setup Shader
        shader->use();
        float tX = font->getTransform()->getTranslationX();
        float tY = font->getTransform()->getTranslationY();
        glm::vec2 position = glm::vec2(tX,tY);
        // Offset origin to middle of sprite
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size.x*scale,size.y*scale, 1.0f));
        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, glm::value_ptr(model)
                           );
        glUniform3fv(glGetUniformLocation(
                         shader->getShaderProgram(), "spriteColor"),
                     1, glm::value_ptr(color)
                     );
        glUniform1i(glGetUniformLocation(shader->getShaderProgram(),"image"),0);
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, glm::value_ptr(mOrthoProjection)
                           );
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,font->getTexture());
        // Bind VAO
        glBindVertexArray(mSpriteQuadVAO);
        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Cleanup
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void GraphicsComponent::drawModel(SceneObject* sceneObject) {
        // Get Assets
        AssimpModelInstance* model = sceneObject->getModelInstance();
        ShaderInstance* shader = sceneObject->getShaderInstance();
        shader->use();
        // Set Ambient Light Values
        GLint uAmbientStrength = glGetUniformLocation(shader->getShaderProgram(),"ambientStrength");
        GLint uAmbientColor    = glGetUniformLocation(shader->getShaderProgram(),"ambientColor");
        if (uAmbientColor > 0 && uAmbientStrength > 0) {
            glm::vec3 ambientColor = glm::vec3(mAmbientLightColour[0],mAmbientLightColour[1],mAmbientLightColour[2]);
            glUniform1f(uAmbientStrength,mAmbientLightColour[3]);
            glUniform3fv(uAmbientColor,1,glm::value_ptr(ambientColor));
        }
        // Set Diffuse Light Values
        vector<LightInstance*>::iterator lights;
        int i;
        for (i=1, lights = mLightQueue.begin(); lights != mLightQueue.end(); lights++, i++) {
            stringstream uPosStr;
            uPosStr << "diffusePos_" << i;
            stringstream uColorStr;
            uColorStr << "diffuseColor_" << i;

            GLint uLightPos   = glGetUniformLocation(shader->getShaderProgram(),uPosStr.str().c_str());
            GLint uLightColor = glGetUniformLocation(shader->getShaderProgram(),uColorStr.str().c_str());

            if (uLightPos > 0 && uLightColor > 0) {
                glm::vec3 lightPos = (*lights)->getTransform()->getTranslation();
                glm::vec3 lightColor = (*lights)->getColor();
                glUniform3fv(uLightPos  ,1, glm::value_ptr(lightPos));
                glUniform3fv(uLightColor,1, glm::value_ptr(lightColor));
            } else {
                cerr << "GraphicsComponent: cannot find uniform for " << uPosStr.str() << endl;
                cerr << "GraphicsComponent: cannot find uniform for " << uColorStr.str() << endl;
            }
        }

        // Pass view/projection transform to shader
        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(mViewMatrix));

        // calculate the model matrix
        glm::mat4 modelMatrix;

        // Get raw data
        glm::vec3 translation = sceneObject->getTranslation();
        glm::quat rot = sceneObject->getTransform()->getOrientation();
        glm::vec3 scale = sceneObject->getScale();

        // Translate
        modelMatrix = glm::translate(modelMatrix,translation);

        // Rotate
        glm::mat4 rotMat = glm::mat4_cast(rot);
        modelMatrix = modelMatrix * rotMat;


        // Scale
        modelMatrix = glm::scale(modelMatrix, scale);

        // Pass model matrix to shader
        glUniformMatrix4fv(
            glGetUniformLocation(shader->getShaderProgram(), "model"),
            1, GL_FALSE, glm::value_ptr(modelMatrix)
        );

        // Draw using shader
        model->draw(shader);

        // Unbind shader
        glUseProgram(0);
    }

    bool GraphicsComponent::checkGLError(string marker) {
        GLenum errorCode = 0;
        bool wasError = false;
        do {
            errorCode = glGetError();
            if (errorCode!=0) {
                cerr << "GraphicsComponent: Error Check " << marker << ": " << endl;
                switch (errorCode) {
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
        } while(errorCode != 0);
        return wasError;
    }

    SDL_Window* GraphicsComponent::getWindow() {
        return mWindow;
    }

    void GraphicsComponent::setWindowWidth(int width) {
        mWindowWidth = width;
    }

    void GraphicsComponent::setWindowHeight(int height) {
        mWindowHeight = height;
    }

    void GraphicsComponent::setScreenName(string name) {
        mScreenName = name;
    }

    string GraphicsComponent::getScreenName() {
        return mScreenName;
    }

    bool GraphicsComponent::isWindowShouldCloseFlagSet() {
        return mWindowShouldClose;
    }

    vector<SDL_Event> GraphicsComponent::getSDL_Events() {
        return mEvents;
    }

    int GraphicsComponent::getWindowWidth() {
        updateWindowDimensions();
        return mWindowWidth;
    }

    int GraphicsComponent::getWindowHeight() {
        updateWindowDimensions();
        return mWindowHeight;
    }

    void GraphicsComponent::updateWindowDimensions() {
        if (mWindow != nullptr) {
            SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);
        }
    }

    void GraphicsComponent::setWindowShouldClose(bool close) {
        mWindowShouldClose = close;
    }

    void GraphicsComponent::addToLightQueue(LightInstance* lightInstance) {
       mLightQueue.push_back(lightInstance);
    }

    void GraphicsComponent::clearLightQueue() {
       mLightQueue.clear();
    }

} // End of Dream
