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

namespace Dream {
namespace Plugins {
namespace Video {
namespace OpenGL {
	
	OGLVideo::OGLVideo(void) : VideoPluginInterface() {
		mCamera = new Camera();
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
			std::cerr << "\tOGLVideo: GLFW failed to initialise." << std::endl;
			return false;
		}
		
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mScreenName.c_str(), NULL, NULL);
		if (!mWindow) {
			glfwTerminate();
			return false;
		}
		
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);
		checkGLError(-10);
		
		std::cout << "\tOGLVideo: Initialised GLFW" << std::endl;
		
		int glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			std::cerr << "\tOGLVideo: GLEW failed to initialise." << std::endl;
			return false;
		}
		checkGLError(-20);
		
		int width, height;
		glfwGetFramebufferSize(mWindow, &width, &height);
		checkGLError(50);
		
		mScreenRatio = width / (float) height;
		glViewport(0, 0, width, height);
		checkGLError(51);
		
		//glEnable(GL_DEPTH_TEST);
		//checkGLError(101);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		checkGLError(52);
		
		//glClearDepth(1.0f);
		//checkGLError(53);
		
		std::cout << "\tOGLVideo: Initialised GLEW." << std::endl;
		std::cout << "OGLVideo: Initialisation Done." << std::endl;
		
		return true;
	}
	
	void OGLVideo::setupView(Dream::Scene::Scene* scene) {
		glClear(GL_COLOR_BUFFER_BIT);
		checkGLError(54);
		
		glMatrixMode(GL_PROJECTION);
		checkGLError(55);
		
		glLoadIdentity();
		checkGLError(56);
		
		glOrtho(-mScreenRatio, mScreenRatio, -1.f, 1.f, 1.f, -1.f);
		checkGLError(57);
		
		glMatrixMode(GL_MODELVIEW);
		checkGLError(58);
		
		glLoadIdentity();
		checkGLError(59);
	}

	void OGLVideo::update(Dream::Scene::Scene* scene) {
		std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		
		if (!glfwWindowShouldClose(mWindow)) {
			GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
			if (fbStatus == GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "OGLVideo: FrameBuffer Status: GL_FRAMEBUFFER_COMPLETE" << std::endl;
			} else {
				std::cerr << "OGLVideo: glCheckFramebufferStatus error: " << fbStatus << std::endl;
				return;
			}
			
			setupView(scene);
			
			for (std::vector<Dream::Scene::SceneObject*>::iterator it = scenegraph.begin(); it!=scenegraph.end(); it++) {
				Dream::Scene::SceneObject* sceneObject = (*it);
				Dream::Asset::AssetInstance *modelAsset  = sceneObject->getModelAssetInstance();
				Dream::Asset::AssetInstance *shaderAsset = sceneObject->getShaderAssetInstance();
				
				if (modelAsset && shaderAsset) {
					try {
						Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* objModel = NULL;
						Dream::Asset::Instances::Shader::ShaderInstance* shader;
						
    				objModel = dynamic_cast<Dream::Asset::Instances::Model::WaveFront::ObjModelInstance*>(modelAsset);
						shader = dynamic_cast<Dream::Asset::Instances::Shader::ShaderInstance*>(shaderAsset);
						
    				if (objModel && shader) {
							drawObjModel(objModel,shader);
    				}
					} catch (std::exception exception) {
						std::cerr << "OGLVideo: Unable to cast model to ObjModelInstance, " << exception.what() << std::endl;
					}
				}
			}
			
			glEnd();
    	glfwSwapBuffers(mWindow);
		}
	}
	
	bool OGLVideo::initDrawData() {
		std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		for (std::vector<Dream::Scene::SceneObject*>::iterator it = scenegraph.begin(); it!=scenegraph.end(); it++) {
			Dream::Scene::SceneObject* sceneObject = (*it);
			Dream::Asset::AssetInstance *modelAsset  = sceneObject->getModelAssetInstance();
			Dream::Asset::AssetInstance *shaderAsset = sceneObject->getShaderAssetInstance();
				
			if (modelAsset && shaderAsset) {
				try {
					Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* objModel = NULL;
					Dream::Asset::Instances::Shader::ShaderInstance* shader;
    			objModel = dynamic_cast<Dream::Asset::Instances::Model::WaveFront::ObjModelInstance*>(modelAsset);
					shader = dynamic_cast<Dream::Asset::Instances::Shader::ShaderInstance*>(shaderAsset);
    			if (objModel && shader) {
						if (!initDrawDataForObjModel(objModel,shader)) {
							return false;
						}
    			}
				} catch (std::exception exception) {
					std::cerr << "OGLVideo: initDrawData, Unable to cast model to ObjModelInstance, " << exception.what() << std::endl;
					return false;
				}
			}
		}
		return true;
	}
	
	void OGLVideo::initDrawDataForObjModel(
	  Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* model,
	  Dream::Asset::Instances::Shader::ShaderInstance* shader) {
		
	}
	
	void OGLVideo::drawObjModel(
		Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* model,
		Dream::Asset::Instances::Shader::ShaderInstance* shader) {
		
		std::vector<tinyobj::shape_t>    shapes    = model->getShapesVector();
  	std::vector<tinyobj::material_t> materials = model->getMaterialsVector();
							
		GLuint vertexBuffer       = 0;
		GLuint indexBuffer        = 0;
		GLuint vertexArrayObject  = 0;
		size_t vertexBufferSize   = 0;
		size_t vertexBufferOffset = 0;
		size_t indexBufferSize    = 0;
		size_t indexBufferOffset  = 0;
		
		// Copy Vertex Data
		for (size_t i = 0; i < shapes.size(); i++) {
			vertexBufferSize += sizeof(float) * shapes[i].mesh.positions.size();
		}
		
		std::cout << "OGLVideo: VertexBufferSize: " << vertexBufferSize << std::endl;
		
		glGenBuffers(1, &vertexBuffer);
		checkGLError(-3);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		checkGLError(-2);
		
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
		checkGLError(-4);
		
		for (size_t i = 0; i < shapes.size(); i++) {
			size_t nPositions = sizeof(float) * shapes[i].mesh.positions.size();
			glBufferSubData(GL_ARRAY_BUFFER,vertexBufferOffset,nPositions,&shapes[i].mesh.positions[0]);
			vertexBufferOffset += nPositions;
			checkGLError(-5);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		checkGLError(-1);
		
		// Copy Index Data
		for (size_t i = 0; i < shapes.size(); i++) {
			indexBufferSize += sizeof(unsigned int)* shapes[i].mesh.indices.size();
		}
		
		std::cout << "OGLVideo: IndexBufferSize: " << indexBufferSize << std::endl;
		
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);
		
		for (size_t i = 0; i < shapes.size(); i++) {
			size_t nPositions = sizeof(unsigned int) * shapes[i].mesh.indices.size();
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,indexBufferOffset,nPositions,&shapes[i].mesh.indices[0]);
			indexBufferOffset += nPositions;
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		checkGLError(0);
		
		// draw multiple objects with one draw call
		glGenVertexArraysAPPLE(1, &vertexArrayObject);
		checkGLError(20);
		glBindVertexArrayAPPLE(vertexArrayObject);
		checkGLError(21);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		checkGLError(22);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		checkGLError(23);
		glBindVertexArrayAPPLE(0);
		checkGLError(24);
		
		//GLint uniformMvp = glGetUniformLocation(shader->getShaderProgram(), "MVP");
		
	  checkGLError(1);
		
		// Use our shader
		if (!shader->areShadersCompiled()) {
  		shader->compileShaderProgram();
		}
		
		shader->showStatus();
		checkGLError(4);
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		//glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, glm::value_ptr(mCamera->calculateModelViewProjection()));
		glBindVertexArrayAPPLE(vertexArrayObject);
		
		checkGLError(2);
		
		glEnableVertexAttribArray(0);
		
		shader->linkShader();
		shader->showStatus();
		checkGLError(41);
		
		if (shader->isShaderLinked()) {
			shader->useShader();
		}
		checkGLError(5);
		
	  vertexBufferOffset = 0;
	  indexBufferOffset  = 0;
		for (size_t i = 0; i < shapes.size(); i++) {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)vertexBufferOffset);
			glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, (void*)indexBufferOffset);
			vertexBufferOffset += sizeof (float) * shapes[i].mesh.positions.size();
			indexBufferOffset  += sizeof (unsigned int) * shapes[i].mesh.indices.size();
			checkGLError(3);
		}
		
		// 1
		glUseProgram(0);
		checkGLError(16);
		// 2
		glDisableVertexAttribArray(0);
		checkGLError(9);
		// 3
		shader->detatchShader();
		checkGLError(31);
		//4
		glDeleteBuffers(1, &vertexBuffer);
		checkGLError(6);
		glDeleteBuffers(1, &indexBuffer);
		checkGLError(7);
		// 5
		glDeleteVertexArraysAPPLE(1, &vertexArrayObject);
		checkGLError(8);
		
	  // 6?
		//glBindVertexArrayAPPLE(0);
		//checkGLError(15);
		
		std::cout << "OGLVideo: *** DrawObjModel Success ***" << std::endl;
	}
	
	bool OGLVideo::checkGLError(int errorIndex) {
		GLenum errorCode = 0;
		bool wasError = false;
		do {
			errorCode = glGetError();
			if (errorCode!=0) {
  			std::cerr << "OGLVideo: Error Check " << errorIndex << ": " << std::endl;
				switch (errorCode)
        {
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
		
		/*if (!wasError) {
			std::cout<< "\tNo Error at " << errorIndex << std::endl;
		}*/
		return wasError;
	}
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
