/*
* Dream::Asset::Model::AssimpModelInstance
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

#include "AssimpModelInstance.h"

namespace Dream      {
namespace Asset      {
namespace Instances  {
namespace Model      {
namespace Assimp     {

	GLint TextureFromFile(const char* path, std::string directory) {
		//Generate texture ID and load texture data
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width,height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		// Parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
	
	AssimpModelInstance::AssimpModelInstance(Dream::Asset::AssetDefinition* definition) : Dream::Asset::AssetInstance(definition) {
			
	}

	AssimpModelInstance::~AssimpModelInstance() {
	
	}
	
	bool AssimpModelInstance::load(std::string projectPath) {
		std::string path = projectPath+mDefinition->getAssetPath();
		std::cout << "AssimpModelInstance: Loading Model from " << path << std::endl;
		::Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			std::cout << "AssimpModelInstance: Error " << import.GetErrorString() << std::endl;
			return false;
		}
		mDirectory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
		return true;
	}
	
	void AssimpModelInstance::draw(Dream::Asset::Instances::Shader::ShaderInstance* shader) {
		GLuint nMeshes = mMeshes.size();
		for(GLuint i = 0; i < nMeshes; i++ ) {
			mMeshes[i].draw(shader);
		}
	}

	void AssimpModelInstance::processNode(aiNode* node, const aiScene* scene) {
		// Process all the node's meshes (if any)
		for(GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			mMeshes.push_back(this->processMesh(mesh, scene));
		}
		// Then do the same for each of its children
		for(GLuint i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}
	
	Mesh AssimpModelInstance::processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex>  vertices;
		std::vector<GLuint>  indices;
		std::vector<Texture> textures;
		
		for(GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			// Process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			
			if(mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
				
			} else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			
			vertices.push_back(vertex);
		}
		
		// Process indices
		for(GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for(GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
		// Process material
		//if(mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//}
		
		return Mesh(vertices, indices, textures);
	}
	
	std::vector<Texture> AssimpModelInstance::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Texture> textures;
		for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;
			for(GLuint j = 0; j < mTexturesLoaded.size(); j++) {
				if(mTexturesLoaded[j].path == str) {
					textures.push_back(mTexturesLoaded[j]);
					skip = true;
					break;
				}
			}
			if(!skip) {   // If texture hasn't been loaded already, load it
				std::cout << "AssimpModelInstance: Loading Texture: " << str.C_Str() << " From: " << mDirectory << std::endl;
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), mDirectory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				mTexturesLoaded.push_back(texture);  // Add to loaded textures
			}
		}
		return textures;
	}
	
	bool AssimpModelInstance::checkGLError(int errorIndex) {
		GLenum errorCode = 0;
		bool wasError = false;
		do {
			errorCode = glGetError();
			if (errorCode!=0) {
				std::cerr << "AssimpModelInstance: Error Check " << errorIndex << ": " << std::endl;
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
	
} // End of Assimp
} // End of Model
} // End of Instance
} // End of Asset
} // End of DreamFileFormats
