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
				Dream::Asset::AssetInstance* modelAsset = sceneObject->getModelAssetInstance();
				
				if (modelAsset) {
					try {
						Dream::Asset::Instances::Model::WaveFront::ObjModelInstance* objModel = NULL;
    				objModel = dynamic_cast<Dream::Asset::Instances::Model::WaveFront::ObjModelInstance*>(modelAsset);
    						
    				if (objModel != NULL) {
    					std::vector<tinyobj::shape_t>    shapes    = objModel->getShapesVector();
    					std::vector<tinyobj::material_t> materials = objModel->getMaterialsVector();
							
							GLenum errorCode = 0;
							GLuint vertex_buffer;
							// Copy data to GPU
							
							// Vertex
							size_t vertex_buffer_size = 0;
							for (size_t i = 0; i < shapes.size(); i++) {
								vertex_buffer_size += sizeof(float)* shapes[i].mesh.positions.size();
							}
							
							glGenBuffers(1, &vertex_buffer);
							glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
							glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, NULL, GL_STATIC_DRAW);
							vertex_buffer_size = 0;
							for (size_t i = 0; i < shapes.size(); i++) {
								glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_size, sizeof(float)* shapes[i].mesh.positions.size(), &shapes[i].mesh.positions[0]);
								vertex_buffer_size += sizeof(float)* shapes[i].mesh.positions.size();
							}
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							
							// Index
							GLuint index_buffer;
							
							size_t index_buffer_size = 0;
							for (size_t i = 0; i < shapes.size(); i++) {
								index_buffer_size += sizeof(unsigned int)* shapes[i].mesh.indices.size();
							}
							
							glGenBuffers(1, &index_buffer);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
							glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, NULL, GL_STATIC_DRAW);
							index_buffer_size = 0;
							for (size_t i = 0; i < shapes.size(); i++) {
								glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, sizeof(unsigned int)* shapes[i].mesh.indices.size(), &shapes[i].mesh.indices[0]);
								index_buffer_size += sizeof(unsigned int)* shapes[i].mesh.indices.size();
							}
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							
							// draw multiple objects with one draw call
							GLuint vertex_array_object;
							
							glGenVertexArrays(1, &vertex_array_object);
							glBindVertexArray(vertex_array_object);
							glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
							glBindVertexArray(0);
							
							uniform_mvp = glGetUniformLocation(shader_program, "MVP");
							
							errorCode = glGetError();
							if (errorCode != 0)
							{
								fprintf(stderr, "Error data: %s, code %d\n", glewGetErrorString(errorCode), errorCode);
							}
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
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
