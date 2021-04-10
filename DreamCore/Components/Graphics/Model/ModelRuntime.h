#pragma once

#include "ModelMesh.h"

#include "Common/GLHeader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <vector>
#include <string>
#include <map>

#include "glm/matrix.hpp"

#include "Components/SharedAssetRuntime.h"
#include "Entity/BoundingBox.h"
#include "ModelBone.h"
#include "ModelAnimation.h"

struct aiScene;
struct aiMaterial;
struct aiNode;
struct aiMesh;
using Assimp::Importer;
using std::shared_ptr;
using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace octronic::dream
{
    struct Vertex;
    class ShaderRuntime;
    class MaterialRuntime;
    class EntityRuntime;

    class ModelRuntime : public SharedAssetRuntime
    {
    public:
        ModelRuntime(ProjectRuntime&,AssetDefinition&);
        ModelRuntime(ModelRuntime&& other) = default;
        ModelRuntime& operator=(ModelRuntime&& other) = default;

        bool loadFromDefinition() override;

        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& bb);

        vector<string> getMaterialNames() const;
        vector<reference_wrapper<ModelMesh>> getMeshes() const;

        mat4 getGlobalInverseTransform() const;
        void setGlobalInverseTransform(const mat4& globalInverseTransform);

        void pushTasks() override;

    private: // Methods
        BoundingBox generateBoundingBox(aiMesh* mesh) const;
        void loadModel(string);
        void processNode(aiNode*, const aiScene*);
        void processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) const;

    private:
        BoundingBox mBoundingBox;
        mat4 mGlobalInverseTransform;
        vector<unique_ptr<ModelMesh>> mMeshes;
        vector<string> mMaterialNames;
    };
}
