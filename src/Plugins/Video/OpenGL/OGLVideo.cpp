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
	
	OGLVideo::OGLVideo(void) : VideoPluginInterface() {}
	
	OGLVideo::~OGLVideo(void) {
		if (mWindow) glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	bool OGLVideo::init(void) {
		std::cout << "OGLVideo: Initialising...";
		
		if (!glfwInit()) {
			return false;
		}
		
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mScreenName.c_str(), NULL, NULL);
		if (!mWindow) {
			glfwTerminate();
			return false;
		}
		
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);
		
		std::cout << "done." << std::endl;
		return true;
	}

	void OGLVideo::update(Dream::Scene::Scene* scene) {
		std::vector<Dream::Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		if (!glfwWindowShouldClose(mWindow)) {
			float ratio;
			int width, height;
			glfwGetFramebufferSize(mWindow, &width, &height);
			ratio = width / (float) height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
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
	
	void OGLVideo::drawObjModel(
		Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* model,
		Dream::Asset::Instances::Shader::ShaderInstance* shader) {
		
		std::vector<tinyobj::shape_t>    shapes    = model->getShapesVector();
  	std::vector<tinyobj::material_t> materials = model->getMaterialsVector();
							
		GLenum errorCode = 0;
		GLuint vertexBuffer, indexBuffer, vertexArrayObject;
		
		// Copy data to GPU
		
		// Vertex
		size_t vertexBufferSize = 0;
		for (size_t i = 0; i < shapes.size(); i++) {
			vertexBufferSize += sizeof(float) * shapes[i].mesh.positions.size();
		}
		
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
		
		vertexBufferSize = 0;
		for (size_t i = 0; i < shapes.size(); i++) {
			glBufferSubData(
				GL_ARRAY_BUFFER,
				vertexBufferSize,
				sizeof(float) * shapes[i].mesh.positions.size(),
				&shapes[i].mesh.positions[0]
			);
			vertexBufferSize += sizeof(float) * shapes[i].mesh.positions.size();
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Index
		size_t indexBufferSize = 0;
		for (size_t i = 0; i < shapes.size(); i++) {
			indexBufferSize += sizeof(unsigned int)* shapes[i].mesh.indices.size();
		}
		
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);
		
		indexBufferSize = 0;
		for (size_t i = 0; i < shapes.size(); i++) {
			glBufferSubData(
				GL_ELEMENT_ARRAY_BUFFER,
				indexBufferSize,
				sizeof(unsigned int) * shapes[i].mesh.indices.size(),
				&shapes[i].mesh.indices[0]
			);
			indexBufferSize += sizeof(unsigned int) * shapes[i].mesh.indices.size();
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		// draw multiple objects with one draw call
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBindVertexArray(0);
		
		uniformMvp = glGetUniformLocation(shaderProgram, "MVP");
		
		errorCode = glGetError();
		if (errorCode != 0) {
			std::cerr << "Error data: " << glewGetErrorString(errorCode) << ", code " <<  errorCode << std::endl;
		}
		
		// Use our shader
		glUseProgram(shader->getShaderProgram());
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, glm::value_ptr(cam.calculateMVP()));
		
		glBindVertexArray(vertexArrayObject);
		glEnableVertexAttribArray(0);
		
		size_t vertexBufferSize = 0;
		size_t indexBufferSize  = 0;
		
		for (size_t i = 0; i < shapes.size(); i++) {
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)vertex_buffer_size);
			glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, (void*)indexBufferSize);
			
			vertexBufferSize += sizeof (float) * shapes[i].mesh.positions.size();
			indexBufferSize  += sizeof (unsigned int) * shapes[i].mesh.indices.size();
			
			if (errorCode != 0) {
				std::cerr << "Error rendering shape[" << i << "].name = " << shapes[i].name << "." << std::endl
				          << "Error name: " << glewGetErrorString(errorCode) << "." << std::endl
				          << "Error code code: " << errorCode << std::endl;
			}
		}
		
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		
		glUseProgram(0);
	}
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
