#include "ModelRuntime.h"

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
using std::make_unique;

namespace octronic::dream
{
  ModelRuntime::ModelRuntime
  (ProjectRuntime& runtime,
   AssetDefinition& definition)
    : SharedAssetRuntime(runtime, definition),
      mGlobalInverseTransform(mat4(1.0f))
  {
    LOG_TRACE("ModelRuntime: Constructing {}", getDefinition().getNameAndUuidString());
  }

  bool
  ModelRuntime::loadFromDefinition
  ()
  {
    auto& projectDir = getProjectRuntime().getProjectDirectory();
    string path = projectDir.getAssetAbsolutePath(static_cast<ModelDefinition&>(getDefinition()));
    LOG_INFO( "ModelRuntime: Loading Model - {}" , path);

    mMaterialNames.clear();

    auto& sm = getProjectRuntime().getStorageManager();
    auto& modelFile = sm.openFile(path);

    if (modelFile.exists())
    {
      if (modelFile.readBinary())
      {
        auto importer = Importer();
        auto binData = modelFile.getBinaryData();
        importer.ReadFileFromMemory(&binData[0], binData.size(), aiProcess_Triangulate | aiProcess_FlipUVs);
        sm.closeFile(modelFile);

        const aiScene* scene = importer.GetScene();

        if(scene == nullptr)
        {
          LOG_ERROR("ModelRuntime: Could not get assimp scene from model. Loading failed");
          return false;
        }

        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
          LOG_ERROR( "ModelRuntime: Error {}" ,importer.GetErrorString() );
          throw std::exception();
        }

        mGlobalInverseTransform = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation.Inverse());
        mBoundingBox.setToLimits();
        processNode(scene->mRootNode, scene);
        mLoaded = true;
        return mLoaded;

      }
      LOG_ERROR("ModelRuntime: Error reading model file");
      sm.closeFile(modelFile);
    }
    return false;
  }

  void
  ModelRuntime::processNode
  (aiNode* node, const aiScene* scene)
  {
    // Process all the node's meshes (if any)
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

      processMesh(mesh, scene);
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
        vertex.Position = vec3(0.0f);
      }

      if (mesh->mNormals)
      {
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;
      }
      else
      {
        vertex.Normal = vec3(0.0f);
      }

      if(mesh->mTextureCoords[0])
      {
        vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
        vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
      }
      else
      {
        vertex.TexCoords = vec2(0.0f, 0.0f);
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

  void
  ModelRuntime::processMesh
  (aiMesh* mesh, const aiScene* scene)
  {
    vector<Vertex>  vertices = processVertexData(mesh);
    vector<GLuint>  indices = processIndexData(mesh);

    // Load any new materials
    aiMaterial* assimpMaterial = scene->mMaterials[mesh->mMaterialIndex];

    aiString name;
    aiGetMaterialString(assimpMaterial, AI_MATKEY_NAME, &name);
    mMaterialNames.push_back(string(name.C_Str()));

    auto& materialCache = getProjectRuntime().getMaterialCache();
    auto materialName = string(name.C_Str());
    auto& modelDef = static_cast<ModelDefinition&>(getDefinition());
    auto materialUuid = modelDef.getDreamMaterialForModelMaterial(materialName);

    auto& pDef = static_cast<ProjectDefinition&>(getProjectRuntime().getDefinition());
    auto matDefOpt = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_MATERIAL,materialUuid);

    optional<reference_wrapper<MaterialRuntime>> mtlRuntime = std::nullopt;

    if (matDefOpt)
    {
      auto& matDef = static_cast<MaterialDefinition&>(matDefOpt.value().get());
      LOG_DEBUG( "ModelRuntime: Using Material {}" , matDef.getName());
      mtlRuntime.emplace(materialCache.getRuntime(matDef));
    }

    BoundingBox bb = generateBoundingBox(mesh);
    mBoundingBox.integrate(bb);

    auto& aMesh = mMeshes.emplace_back(make_unique<ModelMesh>(*this, string(mesh->mName.C_Str()), vertices, indices, mtlRuntime, bb));
    aMesh->initTasks();
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

  vector<reference_wrapper<ModelMesh>>
  ModelRuntime::getMeshes
  ()
  const
  {
    vector<reference_wrapper<ModelMesh>> ret;
    for (auto& mesh : mMeshes)
    {
      ret.push_back(*mesh);
    }
    return  ret;
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

  mat4
  ModelRuntime::getGlobalInverseTransform
  () const
  {
    return mGlobalInverseTransform;
  }

  void
  ModelRuntime::setGlobalInverseTransform
  (const mat4& globalInverseTransform)
  {
    mGlobalInverseTransform = globalInverseTransform;
  }

  mat4 ModelRuntime::aiMatrix4x4ToGlm
  (const aiMatrix4x4& from)
  const
  {
    mat4 to;
    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;
    return to;
  }

  void ModelRuntime::pushTasks()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();
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
        taskQueue.pushTask(mLoadFromDefinitionTask);
      }
    }
    else if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED))
    {
      for (auto& mesh : mMeshes)
      {
        mesh->pushTasks();
      }
    }
  }
}
