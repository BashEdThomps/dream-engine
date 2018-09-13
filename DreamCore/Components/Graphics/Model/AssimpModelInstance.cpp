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
#include "MaterialCache.h"

#include "../BoundingBox.h"
#include "../Shader/ShaderInstance.h"
#include "../../../Common/Constants.h"
#include "ModelDefinition.h"

#include "AssimpCache.h"
#include "AssimpMaterial.h"
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::numeric_limits;

namespace Dream
{
    AssimpModelInstance::AssimpModelInstance
    (
        weak_ptr<AssimpCache> modelCache,
        weak_ptr<MaterialCache> texCache,
        weak_ptr<IAssetDefinition> definition,
        weak_ptr<SceneObjectRuntime> transform)
        : IAssetInstance(definition,transform),
          mModelCache(modelCache),
          mMaterialCache(texCache)
    {
        setLogClassName("AssimpModelInstance");
        auto log = getLog();
        log->trace( "Constructing {}", definition->getNameAndUuidString() );
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
        auto log = getLog();
        log->trace( "Destroying Object");
        return;
    }

    bool
    AssimpModelInstance::load
    (string projectPath)
    {
        auto log = getLog();
        string path = projectPath + mDefinition->getAssetPath();
        log->info( "Loading Model - {}" , path);

        auto modelCache = mModelCache.lock();
        if (modelCache == nullptr)
        {
            return false;
        }

        auto model = modelCache->getModelFromCache(path);

        if (model == nullptr)
        {
            return false;
        }

        const aiScene* scene = model->GetScene();

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
    (weak_ptr<ShaderInstance> shader, vec3 transform, vec3 camPos, float maxDistance, bool alwaysDraw)
    {
        auto log = getLog();
        for(shared_ptr<AssimpMesh> mesh : mMeshes)
        {
            vec3 center = transform + mesh->getBoundingBox().getCenter();
            float distance = glm::distance(center,camPos);
            if (!alwaysDraw && distance > maxDistance)
            {
                log->info("Mesh exceeds max distance, skipping");
                continue;
            }
            mesh->draw(shader);
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
            updateBoundingBox(mBoundingBox, mesh);
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

            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

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

    void
    AssimpModelInstance::processTextureData
    (aiMesh* mesh,const aiScene* scene, AssimpMaterial* aMaterial)
    {
        auto log = getLog();
        // Process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString name;
        aiGetMaterialString(material,AI_MATKEY_NAME,&name);

        log->info("Loading Textures {} for {}",name.C_Str(), getNameAndUuidString());

        // Diffuse Textures
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            log->info("Loading Diffuse Texture {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mDiffuseTexture = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        }

        // Specular Textures
        if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            log->info("Loading Specular {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mSpecularTexture = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        }

        // Normal Textures
        if(material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            log->info("Loading Normal {} for {}",name.C_Str(), getNameAndUuidString());
            aMaterial->mNormalTexture = loadMaterialTexture(material, aiTextureType_NORMALS, "texture_normals");
        }
    }

    shared_ptr<AssimpMesh>
    AssimpModelInstance::processMesh
    (aiMesh* mesh, const aiScene* scene)
    {
        auto log = getLog();
        vector<Vertex>  vertices = processVertexData(mesh);
        vector<GLuint>  indices = processIndexData(mesh);

        shared_ptr<AssimpMesh> aMesh;

        // Material info Colours
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString name;
        aiGetMaterialString(material, AI_MATKEY_NAME, &name);

        auto matCache = mMaterialCache.lock();
        if(matCache != nullptr)
        {
            shared_ptr<AssimpMaterial> aMaterial = matCache->getMaterialByName(name);

            if (aMaterial == nullptr)
            {
                log->info("Creating Material {}", name.C_Str());
                aMaterial = make_shared<AssimpMaterial>();
                aMaterial->mName = name;
                aiGetMaterialInteger(material, AI_MATKEY_TWOSIDED,           &aMaterial->mTwoSided);
                aiGetMaterialInteger(material, AI_MATKEY_SHADING_MODEL,      &aMaterial->mShadingModel);
                aiGetMaterialInteger(material, AI_MATKEY_ENABLE_WIREFRAME,   &aMaterial->mEnableWireframe);
                aiGetMaterialInteger(material, AI_MATKEY_BLEND_FUNC,         &aMaterial->mBlendFunc);
                aiGetMaterialFloat(material,   AI_MATKEY_OPACITY,            &aMaterial->mOpacity);
                aiGetMaterialFloat(material,   AI_MATKEY_BUMPSCALING,        &aMaterial->mBumpScaling);
                aiGetMaterialFloat(material,   AI_MATKEY_SHININESS,          &aMaterial->mHardness);
                aiGetMaterialFloat(material,   AI_MATKEY_REFLECTIVITY,       &aMaterial->mReflectivity);
                aiGetMaterialFloat(material,   AI_MATKEY_SHININESS_STRENGTH, &aMaterial->mShininessStrength);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_DIFFUSE,      &aMaterial->mColorDiffuse);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_AMBIENT,      &aMaterial->mColorAmbient);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_SPECULAR,     &aMaterial->mColorSpecular);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_EMISSIVE,     &aMaterial->mColorEmissive);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_TRANSPARENT,  &aMaterial->mColorTransparent);
                aiGetMaterialColor(material,   AI_MATKEY_COLOR_REFLECTIVE,   &aMaterial->mColorReflective);
                matCache->addMaterialToCache(aMaterial);
                processTextureData(mesh,scene, aMaterial.get());
            }

            log->info( "Using Material {}" , aMaterial->mName.C_Str());
            aMaterial->debug();

            BoundingBox box;
            updateBoundingBox(box, mesh);
            aMesh = make_shared<AssimpMesh>(
                dynamic_pointer_cast<AssimpModelInstance>(shared_from_this()),
                string(mesh->mName.C_Str()),
                vertices,
                indices,
                aMaterial
            );
            aMesh->setBoundingBox(box);
        }
        return aMesh;
    }

    shared_ptr<Texture>
    AssimpModelInstance::loadMaterialTexture
    (aiMaterial* mat, aiTextureType type, string typeName)
    {
        aiString str;
        mat->GetTexture(type, 0, &str);
        auto matCache = mMaterialCache.lock();
        if (matCache)
        {
            return matCache->loadTextureFromFile(str.C_Str(), mDirectory.c_str(),typeName.c_str());
        }
        return nullptr;
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
    (BoundingBox& box, aiMesh* mesh)
    {
        auto log = getLog();
        log->info( "Updating bounding box for {}", getNameAndUuidString() );

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (box.maximum.x < vertex.x)
            {
                box.maximum.x = vertex.x;
            }

            if (box.maximum.y < vertex.y)
            {
                box.maximum.y = vertex.y;
            }

            if (box.maximum.z < vertex.z)
            {
                box.maximum.z = vertex.z;
            }

            // Maximum
            if (box.minimum.x > vertex.x)
            {
                box.minimum.x = vertex.x;
            }

            if (box.minimum.y > vertex.y)
            {
                box.minimum.y = vertex.y;
            }

            if (box.minimum.z > vertex.z)
            {
                box.minimum.z = vertex.z;
            }
        }

        float maxBound;
        maxBound = (
                    box.maximum.x > box.maximum.y ?
                        box.maximum.x :
                        box.maximum.y
                        );

        maxBound = (
                    maxBound > box.maximum.z ?
                        maxBound :
                        box.maximum.z
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
