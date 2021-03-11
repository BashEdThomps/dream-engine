/*
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

#include "ModelRuntime.h"

#include "ModelMesh.h"
#include "ModelDefinition.h"
#include "Components/Cache.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Storage/StorageManager.h"
#include "Storage/File.h"

#include <limits>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

using Assimp::Importer;
using std::pair;
using std::make_shared;

namespace octronic::dream
{
    ModelRuntime::ModelRuntime
    (ProjectRuntime* runtime, AssetDefinition* definition)
        : SharedAssetRuntime(runtime, definition),
          mGlobalInverseTransform(mat4(1.0f))
    {
        LOG_TRACE( "ModelRuntime: Constructing {}", definition->getNameAndUuidString() );
    }

    ModelRuntime::~ModelRuntime
    ()
    {
        LOG_TRACE( "ModelRuntime: Destroying Object");
        mMeshes.clear();
    }

    bool
    ModelRuntime::loadFromDefinition
    ()
    {
        string path = getAssetFilePath();
        LOG_INFO( "ModelRuntime: Loading Model - {}" , path);

        mMaterialNames.clear();

        auto model = loadImporter(path);

        if (model == nullptr)
        {
            LOG_ERROR("ModelRuntime: Could not get model importer, load failed");
            return false;
        }

        const aiScene* scene = model->GetScene();

        if(scene == nullptr)
        {
            LOG_ERROR("ModelRuntime: Could not get assimp scene from model. Loading failed");
            return false;
        }

        mGlobalInverseTransform = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation.Inverse());
        mBoundingBox.setToLimits();
        processNode(scene->mRootNode, scene);
        mLoaded = true;
        return mLoaded;

    }

    void
    ModelRuntime::processNode
    (aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        aiMatrix4x4 rootTx = node->mTransformation;
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

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
    ModelRuntime::processVertexData
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

            if(mesh->mTextureCoords[0])
            {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            if (mesh->mNormals)
            {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }
            else
            {
                vertex.Normal = glm::vec3(0.0f);
            }

            vertices.push_back(vertex);
        }
        return vertices;
    }

    vector<GLuint>
    ModelRuntime::processIndexData
    (aiMesh* mesh)
    {
        vector<GLuint> indices;
        // Process indices
        for(unsigned int face_index = 0; face_index < mesh->mNumFaces; face_index++)
        {
            aiFace face = mesh->mFaces[face_index];
            for(unsigned int indices_index = 0; indices_index < face.mNumIndices; indices_index++)
            {
                indices.push_back(face.mIndices[indices_index]);
            }
        }
        return indices;

    }

    shared_ptr<ModelMesh>
    ModelRuntime::processMesh
    (aiMesh* mesh, const aiScene* scene)
    {
        vector<Vertex>  vertices = processVertexData(mesh);
        vector<GLuint>  indices = processIndexData(mesh);

        // Load any new materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString name;
        aiGetMaterialString(material, AI_MATKEY_NAME, &name);
        mMaterialNames.push_back(string(name.C_Str()));

        auto materialCache = mProjectRuntimeHandle->getMaterialCache();
        if(materialCache != nullptr)
        {
            auto materialName = string(name.C_Str());
            auto modelDef = static_cast<ModelDefinition*>(mDefinitionHandle);
            auto materialUuid = modelDef->getDreamMaterialForModelMaterial(materialName);
            auto material = materialCache->getRuntimeHandle(materialUuid);
            if (material == nullptr)
            {
                LOG_ERROR( "ModelRuntime: No material for mesh {} in model {}. Cannot create mesh with null material.",
                            mesh->mName.C_Str(), getNameAndUuidString());
                return nullptr;
            }
            LOG_DEBUG( "ModelRuntime: Using Material {}" , material->getName());
            BoundingBox bb = generateBoundingBox(mesh);
            mBoundingBox.integrate(bb);

            auto aMesh = make_shared<ModelMesh>(this, string(mesh->mName.C_Str()),
                                                vertices, indices, material, bb);
            material->addMesh(aMesh.get());
            material->debug();
            return aMesh;
        }
        else
        {
            LOG_CRITICAL("ModelRuntime: Material Cache is nullptr");
        }

        return nullptr;
    }

    BoundingBox
    ModelRuntime::getBoundingBox
    ()
    const
    {
        return mBoundingBox;
    }

    void
    ModelRuntime::setBoundingBox
    (const BoundingBox& bb)
    {
        mBoundingBox = bb;
    }

    vector<string>
    ModelRuntime::getMaterialNames
    ()
    const
    {
        return mMaterialNames;
    }

    vector<shared_ptr<ModelMesh>>*
    ModelRuntime::getMeshes
    ()
    {
        return &mMeshes;
    }

    shared_ptr<Importer>
    ModelRuntime::loadImporter
    (string path)
    {
        LOG_DEBUG("ModelRuntime: Loading {} from disk",  path);

        StorageManager* fm = mProjectRuntimeHandle->getStorageManager();
        File* modelFile = fm->openFile(path);

        if (modelFile == nullptr)
        {
            LOG_ERROR("ModelRuntime: Could not open Model file");
            return nullptr;
        }

        if (!modelFile->exists())
        {
            LOG_ERROR("ModelRuntime: Model file does not exist");
            return nullptr;
        }

        if (!modelFile->readBinary())
        {
            LOG_ERROR("ModelRuntime: Error reading model file");
            fm->closeFile(modelFile);
            modelFile = nullptr;
            return nullptr;
        }

        auto importer = make_shared<Importer>();
        importer->ReadFileFromMemory(modelFile->getBinaryData(),
    	    modelFile->getBinaryDataSize(),
    	    aiProcess_Triangulate | aiProcess_FlipUVs);

        fm->closeFile(modelFile);
        modelFile = nullptr;

        const aiScene* scene = importer->GetScene();
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR( "ModelRuntime: Error {}" ,importer->GetErrorString() );
            return nullptr;
        }

        return importer;
    }

    BoundingBox
    ModelRuntime::generateBoundingBox
    (aiMesh* mesh)
    const
    {
        LOG_DEBUG( "ModelRuntime: Updating bounding box");

        BoundingBox bb;
        vec3 min, max;
		min = bb.getMinimum();
		max = bb.getMaximum();

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (max.x < vertex.x) max.x = vertex.x;
            if (max.y < vertex.y) max.y = vertex.y;
            if (max.z < vertex.z) max.z = vertex.z;
            // Maximum
            if (min.x > vertex.x) min.x = vertex.x;
            if (min.y > vertex.y) min.y = vertex.y;
            if (min.z > vertex.z) min.z = vertex.z;
        }

        float maxBound;
        maxBound = (max.x > max.y ? max.x : max.y);
        maxBound = (maxBound > max.z ? maxBound : max.z);
        bb.setMinimum(min);
        bb.setMaximum(max);
        bb.setMaxDimension(maxBound);
        return bb;
    }

    mat4 ModelRuntime::getGlobalInverseTransform() const
    {
        return mGlobalInverseTransform;
    }

    void ModelRuntime::setGlobalInverseTransform(const mat4& globalInverseTransform)
    {
        mGlobalInverseTransform = globalInverseTransform;
    }

    glm::mat4 ModelRuntime::aiMatrix4x4ToGlm
    (const aiMatrix4x4& from)
    const
    {
        glm::mat4 to;
        to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
        to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
        to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
        to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;
        return to;
    }

    void ModelRuntime::pushTasks()
    {
        auto projectTaskQueue = mProjectRuntimeHandle->getTaskQueue();

        if (mReloadFlag)
        {
            mGlobalInverseTransform = mat4(1.f);
            mMeshes.clear();
            mMaterialNames.clear();
            mLoaded = false;
            mLoadError = false;
            mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);
            mReloadFlag = false;
        }
		else if (!mLoaded && !mLoadError)
        {
            if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
            {
               projectTaskQueue->pushTask(mLoadFromDefinitionTask);
            }
        }
        else if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED))
        {
            for (auto mesh : mMeshes)
            {
                mesh->pushTasks();
            }
        }
    }
}
