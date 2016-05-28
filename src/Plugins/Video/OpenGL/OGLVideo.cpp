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
	
	OGLVideo::OGLVideo(void) : VideoPluginInterface() {
		mCamera = new Camera();
		mDeltaTime = 0.0f;
		mLastFrame = 0.0f;
	}
	
	OGLVideo::~OGLVideo(void) {
		if (mCamera) {
			delete mCamera;
		}
		
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
		//glfwWind§owHint(GLFW_RESIZABLE, GL_FALSE);
		
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mScreenName.c_str(), NULL, NULL);
		if (!mWindow) {
			std::cerr << "OGLVideo: Fatal, Unable to create Window" << std::endl;
			glfwTerminate();
			return false;
		}
		
		setupWindowEventHandlers();
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);
		checkGLError(-10);
		
		std::cout << "OGLVideo: Initialised GLFW" << std::endl;
		// Initialize GLEW to setup the OpenGL Function pointers
		glewExperimental = GL_TRUE;
		int glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			std::cerr << "OGLVideo: GLEW failed to initialise." << std::endl;
			return false;
		}
		checkGLError(-20);
		
		std::cout << "OGLVideo: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		// Define the viewport dimensions
		glViewport(0, 0, mScreenWidth, mScreenHeight);
		checkGLError(50);
		
		// Setup some OpenGL options
		glEnable(GL_DEPTH_TEST);
		checkGLError(51);
		
		std::cout << "OGLVideo: Initialised GLEW." << std::endl;
		std::cout << "OGLVideo: Initialisation Done." << std::endl;
		return true;
	}
	
	void OGLVideo::setupWindowEventHandlers() {
		glfwSetWindowSizeCallback  (mWindow, onWindowSizeChangedEvent);
		glfwSetWindowCloseCallback (mWindow, onWindowCloseEvent);
		glfwSetFramebufferSizeCallback(mWindow, onFramebufferSizeEvent);
	}
	
	void OGLVideo::update(Dream::Scene::Scene* scene) {
		//std::cout << "OGLVideo: Update" << std::endl;
		std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		if (!glfwWindowShouldClose(mWindow)) {
			// Set frame time
			GLfloat currentFrame = glfwGetTime();
			mDeltaTime = currentFrame - mLastFrame;
			mLastFrame = currentFrame;
			
			// Clear the colorbuffer
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			checkGLError(541);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			checkGLError(542);
			
			
			std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
			for (std::vector<Dream::Scene::SceneObject*>::iterator it = scenegraph.begin(); it!=scenegraph.end(); it++) {
				Dream::Scene::SceneObject *object = (*it);
				if (object->hasModelAssetInstance()) {
					if (object->hasShaderAssetInstance()){
    				drawSceneObject(object);
    				checkGLError(555);
					} else {
						std::cerr << "OGLVideo: Object " << object->getUUID() << " has no ShaderInstance assigned." << std::endl;
					}
				}
			}
			
    	glfwSwapBuffers(mWindow);
			checkGLError(99);
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
		checkGLError(1201);
		// Transformation matrices
		glm::mat4 projection = glm::perspective(mCamera->mZoom, (float)mScreenWidth/(float)mScreenHeight, 0.1f, 100.0f);
		glm::mat4 view = mCamera->getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		checkGLError(1202);
		// Draw the loaded model
		glm::mat4 modelMatrix;
		float *translation = sceneObject->getTranslation();
		//float *rotation    = sceneObject->getRotation();
		//float *scale       = sceneObject->getScale();
		checkGLError(1203);
		modelMatrix = glm::translate (modelMatrix, glm::vec3( translation[0], translation[1], translation[2] ));
		//modelMatrix = glm::rotate    (modelMatrix, glm::vec3( rotation[0],    rotation[1],    rotation[2]    ));
		//model = glm::scale     (model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit toor our scene, so scale it down
		checkGLError(1204);
		glUniformMatrix4fv(glGetUniformLocation(shader->getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		checkGLError(1205);
		model->draw(shader);
		checkGLError(1206);
		glUseProgram(0);
		checkGLError(1207);
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
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
