/*
* Dream::Asset::Model::ModelInstance
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

#include "ModelInstance.h"

#include <limits>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"
#include <SOIL/SOIL.h>

#include "ModelMesh.h"
#include "../Texture/Texture.h"
#include "../Material/MaterialCache.h"

#include "../BoundingBox.h"
#include "../Shader/ShaderInstance.h"
#include "../Shader/ShaderCache.h"
#include "../Shader/ShaderDefinition.h"
#include "../../../Common/Constants.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "ModelDefinition.h"

#include "../Material/Material.h"
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::numeric_limits;
using ::Assimp::Importer;

namespace Dream
{
    ModelInstance::ModelInstance
    (
        ShaderCache* shaderCache,
        MaterialCache* texCache,
        IAssetDefinition* definition,
        SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mMaterialCache(texCache),
          mShaderCache(shaderCache)
    {
        setLogClassName("ModelInstance");
        auto log = getLog();
        log->trace( "Constructing {}", definition->getNameAndUuidString() );
        mBoundingBox.init();
        return;
    }

    ModelInstance::~ModelInstance
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object");
        mMeshes.clear();
        return;
    }

    bool
    ModelInstance::load
    (string projectPath)
    {
        auto log = getLog();
        string path = projectPath + mDefinition->getAssetPath();
        log->info( "Loading Model - {}" , path);

        auto model = loadImporter(path);

        if (model == nullptr)
        {
            log->error("Could not get model importer, load failed");
            return false;
        }

        const aiScene* scene = model->GetScene();
        //scene->mRootNode->mTransformation = aiMatrix4x4();

        if(scene == nullptr)
        {
            log->error("Could not get assimp scene from model. Loading failed");
            return false;
        }

        mDirectory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
        mLoaded = true;
        return mLoaded;
    }

    void
    ModelInstance::draw
    (
        const shared_ptr<ShaderInstance>& shader,
        vec3 transform,
        vec3 camPos,
        float maxDistance,
        bool alwaysDraw
    ) {
        auto log = getLog();
        for(auto mesh : mMeshes)
        {
            vec3 center = transform + mesh->getBoundingBox().getCenter();
            float distance = glm::distance(center,camPos);
            if (!alwaysDraw && distance > maxDistance)
            {
                log->debug("Mesh exceeds max distance, skipping");
                continue;
            }
            mesh->draw(shader);
        }
    }

    void
    ModelInstance::processNode
    (aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            mBoundingBox.updateFromMesh(mesh);
            auto aMesh = processMesh(mesh, scene);
            if (aMesh != nullptr)
            {
                mMeshes.push_back(aMesh);
            }
        }
        // Then do the same for each of its children
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    vector<Vertex>
    ModelInstance::processVertexData
    (aiMesh* mesh)
    {

        vector<Vertex>  vertices;
        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // Process vertex positions, normals and texture coordinates
            // TODO - Optimise with memcpy if speed is REALLLLY an issue here,
            // otherwise... nah. Profile it first.

            if (mesh->mVertices)
            {
                vertex.Position.x = mesh->mVertices[i].x;
                vertex.Position.y = mesh->mVertices[i].y;
                vertex.Position.z = mesh->mVertices[i].z;
            }
            else
            {
                vertex.Position = glm::vec3(0.0f);
            }

            if (mesh->mNormals)
            {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }

            if (mesh->mTangents)
            {
                vertex.Tangent.x = mesh->mTangents[i].x;
                vertex.Tangent.y = mesh->mTangents[i].y;
                vertex.Tangent.z = mesh->mTangents[i].z;
            }
            else
            {
                vertex.Tangent = glm::vec3(0.0f);
            }

            if (mesh->mBitangents)
            {
                vertex.Bitangent.x = mesh->mBitangents[i].x;
                vertex.Bitangent.y = mesh->mBitangents[i].y;
                vertex.Bitangent.z = mesh->mBitangents[i].z;
            }
            else
            {
                vertex.Bitangent = glm::vec3(0.0f);
            }

            if(mesh->mTextureCoords[0])
            {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
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
    ModelInstance::processIndexData
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

    void
    ModelInstance::processTextureData
    (aiMesh* mesh,const aiScene* scene, shared_ptr<Material> aMaterial)
    {
        auto log = getLog();
        // Process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString name;
        aiGetMaterialString(material,AI_MATKEY_NAME,&name);

        log->debug("Loading Textures {} for {}",name.C_Str(), getNameAndUuidString());

        // Diffuse Textures
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            log->debug("Loading Diffuse Texture {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mDiffuseTexture = loadMaterialTexture(material, aiTextureType_DIFFUSE);
        }

        // Specular Textures
        if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            log->debug("Loading Specular {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mSpecularTexture = loadMaterialTexture(material, aiTextureType_SPECULAR);
        }

        // Normal Textures
        if(material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            log->debug("Loading Normal {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mNormalTexture = loadMaterialTexture(material, aiTextureType_NORMALS);
        }

        // Displacement Texture
        if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
        {
            log->debug("Loading Displacement {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mDisplacementTexture = loadMaterialTexture(material, aiTextureType_DISPLACEMENT);
        }
    }

    shared_ptr<ModelMesh>
    ModelInstance::processMesh
    (aiMesh* mesh, const aiScene* scene)
    {
        auto log = getLog();
        vector<Vertex>  vertices = processVertexData(mesh);
        vector<GLuint>  indices = processIndexData(mesh);

        // Load any new materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString name;
        aiGetMaterialString(material, AI_MATKEY_NAME, &name);

        if(mMaterialCache != nullptr)
        {
            shared_ptr<Material> aMaterial = mMaterialCache->getMaterialByName(name);

            if (aMaterial == nullptr)
            {
                log->debug("Creating Material {}", name.C_Str());
                aMaterial = mMaterialCache->newMaterial(material);
                processTextureData(mesh,scene, aMaterial);

                // Get the shader for the material
                auto modelDef = dynamic_cast<ModelDefinition*>(mDefinition);
                auto shaderUuid = modelDef->getShaderForMaterial(aMaterial->mName.C_Str());

                log->debug("Getting shader for uuid {}",shaderUuid);
                auto shaderDef = mSceneObjectRuntime->getAssetDefinitionByUuid(shaderUuid);

                if (shaderDef == nullptr)
                {
                    log->error("Could not find shader definition for {}",shaderUuid);
                    return nullptr;
                }

                auto shader = mShaderCache->getShaderFromCache
                (
                    mSceneObjectRuntime->getProjectPath(),
                    dynamic_cast<ShaderDefinition*>(shaderDef),
                    mSceneObjectRuntime

                );

                // Assign the material the specified shader
                if (shader != nullptr)
                {
                    shader->addMaterial(aMaterial);
                }
                else
                {
                    log->critical("Could not register mesh, Shader was not found in caceh");
                    return nullptr;
                }
            }
            else
            {
                log->debug("Found existing material");
            }

            log->debug( "Using Material {}" , aMaterial->mName.C_Str());

            BoundingBox box;
            box.updateFromMesh(mesh);
            auto aMesh = make_shared<ModelMesh>
            (
                this,
                string(mesh->mName.C_Str()),
                vertices,
                indices,
                aMaterial
            );

            aMesh->setBoundingBox(box);

            // Assign mesh to material
            aMaterial->addMesh(aMesh);
            aMaterial->debug();


            return aMesh;
        }
        else
        {
            log->critical("Material Cache is nullptr");
        }
        return nullptr;
    }

    shared_ptr<Texture>
    ModelInstance::loadMaterialTexture
    (aiMaterial* mat, aiTextureType type)
    {
        aiString str;
        mat->GetTexture(type, 0, &str);
        if (mMaterialCache)
        {
            return mMaterialCache->loadTextureFromFile(str.C_Str(), mDirectory.c_str(), type);
        }
        return nullptr;
    }

    void
    ModelInstance::loadExtraAttributes
    (json)
    {}

    BoundingBox
    ModelInstance::getBoundingBox
    ()
    {
        return mBoundingBox;
    }

    void
    ModelInstance::setModelMatrix
    (glm::mat4 modelMatrix)
    {
        mModelMatrix = modelMatrix;
    }

    glm::mat4
    ModelInstance::getModelMatrix
    ()
    {
        return mModelMatrix;
    }

    void
    ModelInstance::addInstance
    (SceneObjectRuntime* inst)
    {
        for (auto mesh : mMeshes)
        {
            mesh->addInstance(inst);
        }
    }

    void
    ModelInstance::removeInstance
    (SceneObjectRuntime* inst)
    {
        for (auto mesh : mMeshes)
        {
            mesh->removeInstance(inst);
        }
    }

    shared_ptr<Importer>
    ModelInstance::loadImporter
    (string path)
    {
        auto log = getLog();

        log->debug("Loading {} from disk",  path);

        auto importer = make_shared<Importer>();
        importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        const aiScene* scene = importer->GetScene();
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            log->error( "Error {}" ,importer->GetErrorString() );
            return nullptr;
        }

        return importer;
    }

} // End of Dream
