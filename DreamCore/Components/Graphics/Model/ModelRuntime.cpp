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
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Material/MaterialCache.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

#include <limits>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <SOIL.h>

using ::Assimp::Importer;

namespace Dream
{
    ModelRuntime::ModelRuntime
    (
        ShaderCache* shaderCache,
        MaterialCache* texCache,
        AssetDefinition* definition,
        ProjectRuntime* runtime
    )
        : SharedAssetRuntime(definition,runtime),
          mMaterialCache(texCache),
          mShaderCache(shaderCache),
          mModelMatrix(mat4(1.0f)),
          mGlobalInverseTransform(mat4(1.0f))
    {
        LOG_TRACE( "Constructing {}", definition->getNameAndUuidString() );
    }

    ModelRuntime::~ModelRuntime
    ()
    {
        LOG_TRACE( "Destroying Object");
        for (auto mesh : mMeshes)
        {
            delete mesh;
        }
        mMeshes.clear();
    }

    bool
    ModelRuntime::useDefinition
    ()
    {
        string path = getAssetFilePath();
        LOG_INFO( "Loading Model - {}" , path);

        mMaterialNames.clear();

        auto model = loadImporter(path);

        if (model == nullptr)
        {
            LOG_ERROR("Could not get model importer, load failed");
            return false;
        }

        const aiScene* scene = model->GetScene();

        if(scene == nullptr)
        {
            LOG_ERROR("Could not get assimp scene from model. Loading failed");
            return false;
        }

        mGlobalInverseTransform = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation.Inverse());
        mDirectory = getAssetDirectoryPath();
        mBoundingBox.setToLimits();
        processNode(scene->mRootNode, scene);
        mLoaded = true;
        return mLoaded;
    }

    void
    ModelRuntime::processNode
    (aiNode* node, const aiScene* scene)
    {
        processAnimationData(node);
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
    ModelRuntime::processIndexData
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
    ModelRuntime::processBoneData
    (aiMesh* mesh)
    {
       for (unsigned int i = 0; i < mesh->mNumBones; i++)
       {
          aiBone* nextBone = mesh->mBones[i];
          string name(mesh->mBones[i]->mName.data);
          mBones.insert(pair<string,Bone>(name,Bone()));
       }
    }

    void
    ModelRuntime::processAnimationData
    (aiNode* node)
    {
    }

    ModelMesh*
    ModelRuntime::processMesh
    (aiMesh* mesh, const aiScene* scene)
    {
        processBoneData(mesh);
        vector<Vertex>  vertices = processVertexData(mesh);
        vector<GLuint>  indices = processIndexData(mesh);

        // Load any new materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString name;
        aiGetMaterialString(material, AI_MATKEY_NAME, &name);
        mMaterialNames.push_back(string(name.C_Str()));

        if(mMaterialCache != nullptr)
        {
            auto materialName = string(name.C_Str());
            auto modelDef = static_cast<ModelDefinition*>(mDefinition);
            auto materialUuid = modelDef->getDreamMaterialForModelMaterial(materialName);
            auto material = static_cast<MaterialRuntime*>(mMaterialCache->getRuntime(materialUuid));
            if (material == nullptr)
            {
                LOG_ERROR(
                    "No material for mesh {} in model {}."
                    " Cannot create mesh with null material.",
                    mesh->mName.C_Str(), getNameAndUuidString()
                );
                return nullptr;
            }
            LOG_DEBUG( "Using Material {}" , material->getName());
            BoundingBox bb = updateBoundingBox(mesh);
            mBoundingBox.integrate(bb);

            auto aMesh = new ModelMesh
            (this, string(mesh->mName.C_Str()), vertices, indices,
                material, bb);
            material->addMesh(aMesh);
            material->debug();
            return aMesh;
        }
        else
        {
            LOG_CRITICAL("Material Cache is nullptr");
        }

        return nullptr;
    }

    BoundingBox&
    ModelRuntime::getBoundingBox
    ()
    {
        return mBoundingBox;
    }

    void
    ModelRuntime::addRuntime
    (EntityRuntime* inst)
    {
        for (auto mesh : mMeshes)
        {
            mesh->addRuntime(inst);
        }
    }

    void
    ModelRuntime::removeRuntime
    (EntityRuntime* inst)
    {
        for (auto mesh : mMeshes)
        {
            mesh->removeRuntime(inst);
        }
    }

    vector<string>
    ModelRuntime::getMaterialNames
    ()
    {
       return mMaterialNames;
    }

    vector<ModelMesh*>
    ModelRuntime::getMeshes
    ()
    const
    {
        return mMeshes;
    }

    shared_ptr<Importer>
    ModelRuntime::loadImporter
    (string path)
    {
        LOG_DEBUG("Loading {} from disk",  path);

        auto importer = make_shared<Importer>();
        importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        const aiScene* scene = importer->GetScene();
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR( "Error {}" ,importer->GetErrorString() );
            return nullptr;
        }

        return importer;
    }

    BoundingBox
    ModelRuntime::updateBoundingBox
    (aiMesh* mesh)
    {
        LOG_DEBUG( "Updating bounding box");

        BoundingBox bb;

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (bb.maximum.x() < vertex.x)
            {
                bb.maximum.setX(vertex.x);
            }

            if (bb.maximum.y() > vertex.y)
            {
                bb.maximum.setY(vertex.y);
            }

            if (bb.maximum.z() < vertex.z)
            {
                bb.maximum.setZ(vertex.z);
            }

            // Maximum
            if (bb.minimum.x() > vertex.x)
            {
                bb.minimum.setX(vertex.x);
            }

            if (bb.minimum.y() > vertex.y)
            {
                bb.minimum.setY(vertex.y);
            }

            if (bb.minimum.z() > vertex.z)
            {
                bb.minimum.setZ(vertex.z);
            }
        }

        float maxBound;
        maxBound = (bb.maximum.x() > bb.maximum.y() ? bb.maximum.x() : bb.maximum.y());
        maxBound = (maxBound > bb.maximum.z() ? maxBound : bb.maximum.z());
        bb.maxDimension = maxBound;
        return bb;
    }

    map<string,Bone>&
    ModelRuntime::getBones
    ()
    {
       return mBones;
    }

    map<string,ModelAnimation>&
    ModelRuntime::getAnimations
    ()
    {
        return mAnimations;
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
    {
        glm::mat4 to;
        to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
        to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
        to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
        to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;
        return to;
    }

}
