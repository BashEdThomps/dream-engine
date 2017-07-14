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

#include <limits>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"
#include <SOIL.h>

#include "AssimpMesh.h"
#include "Texture.h"
#include "TextureCache.h"

#include "../BoundingBox.h"
#include "../Shader/ShaderInstance.h"
#include "../../../Common/Constants.h"
#include "ModelDefinition.h"

#include "AssimpCache.h"

using std::numeric_limits;

namespace Dream
{
    AssimpModelInstance::AssimpModelInstance
    (AssimpCache* modelCache, TextureCache* texCache, IAssetDefinition* definition, SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mModelCacheHandle(modelCache),
          mTextureCacheHandle(texCache)
    {
        if (Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Constructing "
                 << definition->getNameAndUuidString() << endl;
        }
        initBoundingBox();
        return;
    }

    void
    AssimpModelInstance::initBoundingBox
    ()
    {
        mBoundingBox.maximum = vec3(numeric_limits<float>::min());
        mBoundingBox.minimum = vec3(numeric_limits<float>::max());
    }

    AssimpModelInstance::~AssimpModelInstance
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Destroying Object" << endl;
        }
        return;
    }

    bool
    AssimpModelInstance::load
    (string projectPath)
    {
        string path = projectPath + mDefinitionHandle->getAssetPath();
        if (Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Loading Model - " << path << endl;
        }
        const aiScene* scene = mModelCacheHandle->getModelFromCache(path)->GetScene();
        if(scene == nullptr)
        {
            return false;
        }
        mDirectory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
        mLoaded = true;
        return mLoaded;
    }

    void
    AssimpModelInstance::draw
    (ShaderInstance* shader)
    {
        for(AssimpMesh mesh : mMeshes)
        {
            mesh.draw(shader);
        }
    }

    void
    AssimpModelInstance::processNode
    (aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            updateBoundingBox(mesh);
            mMeshes.push_back(processMesh(mesh, scene));
        }
        // Then do the same for each of its children
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    vector<Vertex>
    AssimpModelInstance::processVertexData
    (aiMesh* mesh)
    {

        vector<Vertex>  vertices;
        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // Process vertex positions, normals and texture coordinates
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->mNormals)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if(mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        return vertices;

    }

    vector<GLuint>
    AssimpModelInstance::processIndexData
    (aiMesh* mesh)
    {

        vector<GLuint> indices;
        // Process indices
        for(GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        return indices;
    }

    vector<Texture>
    AssimpModelInstance::processTextureData
    (aiMesh* mesh,const aiScene* scene)
    {
        vector<Texture> textures;
        // Process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Diffuse Textures
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        if (diffuseMaps.size() > 0 && Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Inserting " << diffuseMaps.size() << " diffuse textures" << endl;

        }
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Specular Textures
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        if (specularMaps.size() > 0 && Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Inserting " << specularMaps.size() << " specular textures" << endl;

        }
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // Normal Textures
        vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normals");
        if (normalMaps.size() > 0 && Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Inserting " << normalMaps.size() << " normal textures" << endl;

        }
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        return textures;

    }

    AssimpMesh
    AssimpModelInstance::processMesh
    (aiMesh* mesh, const aiScene* scene)
    {
        vector<Vertex>  vertices = processVertexData(mesh);
        vector<GLuint>  indices = processIndexData(mesh);
        vector<Texture> textures = processTextureData(mesh,scene);

        // Colours

        aiColor3D diffuse (0.f,0.f,0.f);
        aiColor3D specular(0.f,0.f,0.f);
        aiString materialName;

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        material->Get(AI_MATKEY_COLOR_DIFFUSE,diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR,specular);
        material->Get(AI_MATKEY_NAME,materialName);

        if (Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Using Material " << materialName.C_Str() << endl;
        }

        return AssimpMesh(this, vertices, indices, textures, diffuse, specular);
    }

    vector<Texture>
    AssimpModelInstance::loadMaterialTextures
    (aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture tex = mTextureCacheHandle->loadTextureFromFile(str.C_Str(), mDirectory.c_str(),typeName.c_str());
            textures.push_back(tex);
        }
        return textures;
    }

    void
    AssimpModelInstance::loadExtraAttributes
    (json)
    {

    }

    BoundingBox
    AssimpModelInstance::getBoundingBox
    ()
    {
        return mBoundingBox;
    }

    void
    AssimpModelInstance::updateBoundingBox
    (aiMesh* mesh)
    {
        if (Constants::DEBUG)
        {
            cout << "AssimpModelInstance: Updating bounding box for "
                 << getNameAndUuidString() << endl;
        }

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (mBoundingBox.maximum.x < vertex.x)
            {
                mBoundingBox.maximum.x = vertex.x;
            }

            if (mBoundingBox.maximum.y < vertex.y)
            {
                mBoundingBox.maximum.y = vertex.y;
            }

            if (mBoundingBox.maximum.z < vertex.z)
            {
                mBoundingBox.maximum.z = vertex.z;
            }

            // Maximum
            if (mBoundingBox.minimum.x > vertex.x)
            {
                mBoundingBox.minimum.x = vertex.x;
            }

            if (mBoundingBox.minimum.y > vertex.y)
            {
                mBoundingBox.minimum.y = vertex.y;
            }

            if (mBoundingBox.minimum.z > vertex.z)
            {
                mBoundingBox.minimum.z = vertex.z;
            }
        }

        float maxBound;
        maxBound = (
            mBoundingBox.maximum.x > mBoundingBox.maximum.y ?
            mBoundingBox.maximum.x :
            mBoundingBox.maximum.y
        );

        maxBound = (
            maxBound > mBoundingBox.maximum.z ?
            maxBound :
            mBoundingBox.maximum.z
        );

        mBoundingBox.maxDimension = maxBound;
    }

    void
    AssimpModelInstance::setModelMatrix
    (glm::mat4 modelMatrix)
    {
        mModelMatrix = modelMatrix;
    }

    glm::mat4
    AssimpModelInstance::getModelMatrix
    ()
    {
        return mModelMatrix;
    }

} // End of Dream
