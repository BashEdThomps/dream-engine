/*
* OGLVideo
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

#include "OGLVideo.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	// Global Camera
	Camera OGLVideo::sCamera = Camera();
	
	// Global Event Handlers
	void onWindowSizeChangedEvent(GLFWwindow *window, int width, int height) {
		std::cout << "OGLVideo: Window Resized " << width << "," << height << std::endl;
	}
	
	void onWindowCloseEvent(GLFWwindow *window) {
		std::cout << "OGLVideo: Window Close Event." << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	void onFramebufferSizeEvent(GLFWwindow *window, int width, int height ) {
		std::cout << "OGLVideo: Framebuffer Resized " << width << "," << height << std::endl;
		glViewport(0, 0, width, height);
	}
	
	OGLVideo::OGLVideo(void) : VideoPluginInterface() {}
	
	OGLVideo::~OGLVideo(void) {
		if (mWindow) {
  		glfwDestroyWindow(mWindow);
  	}
		
		glfwTerminate();
	}

	bool OGLVideo::init(void) {
		std::cout << "OGLVideo: Initialising..." << std::endl;
		
		if (!glfwInit()) {
			std::cerr << "OGLVideo: GLFW failed to initialise." << std::endl;
			return false;
		}
	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mScreenName.c_str(), NULL, NULL);
		if (!mWindow) {
			std::cerr << "OGLVideo: Fatal, Unable to create Window" << std::endl;
			glfwTerminate();
			return false;
		}
		
		setupWindowEventHandlers();
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);
		#ifdef VERBOSE
			checkGLError(-10);
		#endif
		
		std::cout << "OGLVideo: Initialised GLFW" << std::endl;
		// Initialize GLEW to setup the OpenGL Function pointers
		glewExperimental = GL_TRUE;
		int glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			std::cerr << "OGLVideo: GLEW failed to initialise." << std::endl;
			return false;
		}
		
		#ifdef VERBOSE
			checkGLError(-20);
		#endif
		
		std::cout << "OGLVideo: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		// Define the viewport dimensions
		glViewport(0, 0, mScreenWidth*2, mScreenHeight*2);
		#ifdef VERBOSE
			checkGLError(50);
		#endif
		
		// Setup some OpenGL options
		glEnable(GL_DEPTH_TEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		#ifdef VERBOSE
			checkGLError(51);
		#endif
		
		std::cout << "OGLVideo: Initialised GLEW." << std::endl;
		std::cout << "OGLVideo: Initialisation Done." << std::endl;
		return true;
	}
	
	void OGLVideo::setCursorEnabled(bool cursorEnabled) {
		glfwSetInputMode(mWindow, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
	
	void OGLVideo::closeWindow() {
		mWindowShouldClose = true;
		glfwSetWindowShouldClose(mWindow,GL_TRUE);
	}
	
	void OGLVideo::setupWindowEventHandlers() {
		glfwSetWindowSizeCallback(mWindow, onWindowSizeChangedEvent);
		glfwSetWindowCloseCallback(mWindow, onWindowCloseEvent);
		glfwSetFramebufferSizeCallback(mWindow, onFramebufferSizeEvent);
	}
	
	void OGLVideo::update(Dream::Scene::Scene* scene) {
		//std::cout << "OGLVideo: Update" << std::endl;
		std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		if (!glfwWindowShouldClose(mWindow)) {
			// Clear the colorbuffer
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			#ifdef VERBOSE
				checkGLError(541);
			#endif
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			#ifdef VERBOSE
				checkGLError(542);
			#endif
			//scene->generateScenegraphVector();
			std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
			for (std::vector<Dream::Scene::SceneObject*>::iterator it = scenegraph.begin(); it!=scenegraph.end(); it++) {
				Dream::Scene::SceneObject *object = (*it);
				if (object->hasModelAssetInstance()) {
					if (object->hasShaderAssetInstance()){
    				drawSceneObject(object);
						#ifdef VERBOSE
							checkGLError(555);
						#endif
					} else {
						std::cerr << "OGLVideo: Object " << object->getUUID() << " has no ShaderInstance assigned." << std::endl;
					}
				}
			}
    	glfwSwapBuffers(mWindow);
			#ifdef VERBOSE
				checkGLError(99);
			#endif
		} else {
			mWindowShouldClose = true;
		}
	}
	
	void OGLVideo::drawSceneObject(Dream::Scene::SceneObject* sceneObject) {
		Dream::Asset::Instances::Model::Assimp::AssimpModelInstance* model;
		model = dynamic_cast<Dream::Asset::Instances::Model::Assimp::AssimpModelInstance*>(sceneObject->getModelAssetInstance());
		Dream::Asset::Instances::Shader::ShaderInstance* shader;
		shader = dynamic_cast<Dream::Asset::Instances::Shader::ShaderInstance*>(sceneObject->getShaderAssetInstance());
		shader->use();
		#ifdef VERBOSE
			checkGLError(1201);
		#endif
		// Transformation matrices
		glm::mat4 projection = glm::perspective(sCamera.getZoom(), (float)mScreenWidth/(float)mScreenHeight, mMinimumDraw,mMaximumDraw);
		glm::mat4 view = sCamera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		#ifdef VERBOSE
			checkGLError(1202);
		#endif
		// Draw the loaded model
		glm::mat4 modelMatrix;
		std::vector<float> translation = sceneObject->getTranslation();
		std::vector<float> rotation    = sceneObject->getRotation();
		std::vector<float> scale       = sceneObject->getScale();
		#ifdef VERBOSE
  		std::cout << "OGLVideo: Drawing Scene Object" << std::endl;
  		std::cout	<< "\tT("<<translation[0]<<","<<translation[1]<<","<<translation[2]<<")"<<std::endl;
  		std::cout	<< "\tR("<<rotation[0]<<","<<rotation[1]<<","<<rotation[2]<<")"<<std::endl;
  		std::cout << "\tS("<<scale[0]<<","<<scale[1]<<","<<scale[2]<<")"<<std::endl;;
			checkGLError(1203);
		#endif
		// Translate
		modelMatrix = glm::translate(modelMatrix, glm::vec3( translation[0], translation[1], translation[2] ));
	  // Rotate
		modelMatrix = glm::rotate(modelMatrix, rotation[0], glm::vec3(1,0,0));
		modelMatrix = glm::rotate(modelMatrix, rotation[1], glm::vec3(0,1,0));
		modelMatrix = glm::rotate(modelMatrix, rotation[2], glm::vec3(0,0,1));
		// Scale
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale[0], scale[1], scale[2]));
		#ifdef VERBOSE
			checkGLError(1204);
		#endif
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		#ifdef VERBOSE
			checkGLError(1205);
		#endif
		model->draw(shader);
		#ifdef VERBOSE
			checkGLError(1206);
		#endif
		glUseProgram(0);
		#ifdef VERBOSE
			checkGLError(1207);
		#endif
	}
	
	bool OGLVideo::checkGLError(int errorIndex) {
		GLenum errorCode = 0;
		bool wasError = false;
		do {
			errorCode = glGetError();
			if (errorCode!=0) {
  			std::cerr << "OGLVideo: Error Check " << errorIndex << ": " << std::endl;
				switch (errorCode) {
    			case GL_NO_ERROR:
						std::cerr << "\tGL_NO_ERROR" << std::endl;
						break;
    			case GL_INVALID_ENUM:
						std::cerr << "\tGL_INVALID_ENUM" << std::endl;
						break;
    			case GL_INVALID_VALUE:
						std::cerr << "\tGL_INVALID_VALUE" << std::endl;
						break;
    			case GL_INVALID_OPERATION:
						std::cerr << "\tGL_INVALID_OPERATION" << std::endl;
						break;
    			case GL_INVALID_FRAMEBUFFER_OPERATION:
						std::cerr << "\tGL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
						break;
    			case GL_OUT_OF_MEMORY:
						std::cerr << "\tGL_OUT_OF_MEMORY" << std::endl;
						break;
    		}
				std::cerr << "\tName: " << glewGetErrorString(errorCode) << std::endl;
				std::cerr << "\tCode: " << errorCode << std::endl;
  			wasError = true;
			}
		} while(errorCode != 0);
		return wasError;
	}
	
	GLFWwindow* OGLVideo::getWindow() {
		return mWindow;
	}
	
	void OGLVideo::setDefaultCameraTranslation(std::vector<float> translation) {
		sCamera.setTranslation(translation);
	}
	
	void OGLVideo::setDefaultCameraRotation(std::vector<float> rotation) {
		sCamera.setRotation(rotation);
	}
	
	void OGLVideo::setCameraMovementSpeed (float speed) {
		sCamera.setMovementSpeed(speed);
	}
		
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
