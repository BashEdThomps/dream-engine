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

namespace Dream {

      vector<Texture> AssimpModelInstance::sTextureCache = vector<Texture>();

      GLint TextureFromFile(const char* path, string directory) {
        //Generate texture ID and load texture data
        string filename = string(path);
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

      AssimpModelInstance::AssimpModelInstance(
          AssetDefinition* definition,
          Transform3D* transform
      ) : AssetInstance(definition,transform) {
        return;
      }

      AssimpModelInstance::~AssimpModelInstance() {
        return;
      }

      bool AssimpModelInstance::load(string projectPath) {
        string path = projectPath+mDefinition->getAssetPath();
        cout << "\tLoading Model:   " << path << endl;
        ::Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
          cout << "AssimpModelInstance: Error " << import.GetErrorString() << endl;
          return false;
        }
        mDirectory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
        return true;
      }

      void AssimpModelInstance::draw(ShaderInstance* shader) {
        GLuint nMeshes = mMeshes.size();
        for(GLuint i = 0; i < nMeshes; i++ ) {
          mMeshes[i].draw(shader);
        }
      }

      void AssimpModelInstance::processNode(aiNode* node, const aiScene* scene) {
        // Process all the node's meshes (if any)
        for(GLuint i = 0; i < node->mNumMeshes; i++) {
          aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
          mMeshes.push_back(processMesh(mesh, scene));
        }
        // Then do the same for each of its children
        for(GLuint i = 0; i < node->mNumChildren; i++) {
          processNode(node->mChildren[i], scene);
        }
      }

      AssimpMesh AssimpModelInstance::processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex>  vertices;
        vector<GLuint>  indices;
        vector<Texture> textures;

        for(GLuint i = 0; i < mesh->mNumVertices; i++) {
          Vertex vertex;
          // Process vertex positions, normals and texture coordinates
          glm::vec3 vector;

          vector.x = mesh->mVertices[i].x;
          vector.y = mesh->mVertices[i].y;
          vector.z = mesh->mVertices[i].z;
          vertex.Position = vector;

          if (mesh->mNormals) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
          }

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
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return AssimpMesh(vertices, indices, textures);
      }

      vector<Texture> AssimpModelInstance::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
        vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
          aiString str;
          mat->GetTexture(type, i, &str);
          GLboolean skip = false;
          for(GLuint j = 0; j < sTextureCache.size(); j++) {
            if(sTextureCache[j].path == str) {
              textures.push_back(sTextureCache[j]);
              skip = true;
              break;
            }
          }
          if(!skip) {   // If texture hasn't been loaded already, load it
            cout << "\tLoading Texture: " << mDirectory << "/" << str.C_Str() <<  endl;
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), mDirectory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            sTextureCache.push_back(texture);  // Add to loaded textures
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
            cerr << "AssimpModelInstance: Error Check " << errorIndex << ": " << endl;
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

      void AssimpModelInstance::loadExtraAttributes(nlohmann::json jsonData) {}

} // End of Dream
