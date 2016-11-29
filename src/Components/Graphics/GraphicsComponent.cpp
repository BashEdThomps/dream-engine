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

namespace Dream {

  // Global Event Handlers

  GraphicsComponent::GraphicsComponent(Camera* camera) : ComponentInterface () {
    setWindowWidth(Graphics_INTERFACE_DEFAULT_SCREEN_WIDTH);
    setWindowHeight(Graphics_INTERFACE_DEFAULT_SCREEN_HEIGHT);
    mWindowShouldClose = false;
    mCamera = camera;
  }

  bool GraphicsComponent::createSDLWindow() {
    mWindow = SDL_CreateWindow(
          mScreenName.c_str(),
          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
          mWindowWidth, mWindowHeight,
          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
          );

    if (mWindow == nullptr){
      cout << "GraphicsComopnent: SDL_CreateWindow Error = " << SDL_GetError() << endl;
      SDL_Quit();
      return false;
    }
    return true;
  }

  bool GraphicsComponent::createSDLRenderer() {
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == nullptr){
      SDL_DestroyWindow(mWindow);
      cout << "GraphicsComponent: SDL_CreateRenderer Error = " << SDL_GetError() << endl;
      SDL_Quit();
      return false;
    }
    return true;
  }

  GraphicsComponent::~GraphicsComponent(void) {

    if (mRenderer) {
      SDL_DestroyRenderer(mRenderer);
    }

    if (mWindow) {
      SDL_DestroyWindow(mWindow);
    }

    SDL_Quit();
  }

  bool GraphicsComponent::init(void) {
    cout << "GraphicsComponent: Initialising..." << endl;

    if (!createSDLWindow()) {
      cerr << "GraphicsComponent: Unable to create SDL Window" << endl;
      return false;
    }

    if (!createSDLRenderer()) {
      cerr << "GraphicsComponent: Unable to create SDL Renderer" << endl;
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
      cerr << "GraphicsComponent: OpenGL context could not be created! - "
           << SDL_GetError()
           << endl;
      return false;
    }

    cout << "GraphicsComponent: Initialised SDL" << endl;

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
      cerr << "GraphicsComponent: GLEW failed to initialise." << endl;
      return false;
    }

    cout << "GraphicsComponent: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    // Define the viewport dimensions
    glViewport(0, 0, mWindowWidth, mWindowHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    cout << "GraphicsComponent: Initialisation Done." << endl;
    return true;
  }

  void GraphicsComponent::setCursorEnabled(bool cursorEnabled) {}

  void GraphicsComponent::closeWindow() {
    mWindowShouldClose = true;
  }

  void GraphicsComponent::setupWindowEventHandlers() {}

  void GraphicsComponent::update(Scene* scene) {
    SDL_PollEvent(&mEvent);

    switch(mEvent.type) {
      case SDL_QUIT:
        mWindowShouldClose = true;
        break;
    }

    vector<SceneObject*> scenegraph = scene->getScenegraphVector();
    if (!mWindowShouldClose) {

      // Clear the colorbuffer
      //SDL_RenderClear(mRenderer);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      vector<SceneObject*> scenegraph = scene->getScenegraphVector();
      for (vector<SceneObject*>::iterator it = scenegraph.begin();
           it!=scenegraph.end();
           it++ ) {
        SceneObject *object = (*it);
        // Models
        if (object->hasModelInstance()) {
          if (object->hasShaderInstance()){
            drawModel(object);
          } else {
            cerr << "GraphicsComponent: Object " << object->getUUID()
                 << " has model, but no shader assigned." << endl;
          }
        }
        // Sprites
        if (object->hasSpriteInstance()) {
          drawSprite(object);
        }
      }
      //SDL_RenderPresent(mRenderer);
      SDL_GL_SwapWindow(mWindow);
      SDL_Delay(100);
    }
  }

  void GraphicsComponent::drawSprite(SceneObject* sceneObject) {
    SpriteInstance *sprite = sceneObject->getSpriteInstance();
    SDL_RenderCopy(mRenderer, sprite->getTexture(), nullptr, sprite->getDestination());
  }

  void GraphicsComponent::drawModel(SceneObject* sceneObject) {
    AssimpModelInstance* model;
    model = dynamic_cast<AssimpModelInstance*>(sceneObject->getModelInstance());
    ShaderInstance* shader;
    shader = dynamic_cast<ShaderInstance*>(sceneObject->getShaderInstance());
    shader->use();

    // Transformation matrices
    glm::mat4 projection = glm::perspective(
          mCamera->getZoom(),
          static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight),
          mMinimumDraw,
          mMaximumDraw
    );

    vector<vector<float>> view = mCamera->getViewMatrix();

    glm::mat4 viewMat4 = glm::mat4(
        view[0][0], view[0][1], view[0][2], view[0][3],
        view[1][0], view[1][1], view[1][2], view[1][3],
        view[2][0], view[2][1], view[2][2], view[2][3],
        view[3][0], view[3][1], view[3][2], view[3][3]
    );

    glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(viewMat4));

    // Draw the loaded model
    glm::mat4 modelMatrix;
    vector<float> translation = sceneObject->getTranslation();
    vector<float> rotation    = sceneObject->getRotation();
    vector<float> scale       = sceneObject->getScale();

    // Translate
    modelMatrix = glm::translate(modelMatrix, glm::vec3( translation[0], translation[1], translation[2] ));

    // Rotate
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation[0]), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation[1]), glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation[2]), glm::vec3(0,0,1));

    // Scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale[0], scale[1], scale[2]));
    glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    model->draw(shader);
    glUseProgram(0);
  }

  bool GraphicsComponent::checkGLError(int errorIndex) {
    GLenum errorCode = 0;
    bool wasError = false;
    do {
      errorCode = glGetError();
      if (errorCode!=0) {
        cerr << "GraphicsComponent: Error Check " << errorIndex << ": " << endl;
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

  int  GraphicsComponent::getWindowWidth() {
    return mWindowWidth;
  }

  void GraphicsComponent::setWindowHeight(int height) {
    mWindowHeight = height;
  }

  int  GraphicsComponent::getWindowHeight() {
    return mWindowHeight;
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

  SDL_Renderer* GraphicsComponent::getRenderer() {
    return mRenderer;
  }

} // End of Dream
