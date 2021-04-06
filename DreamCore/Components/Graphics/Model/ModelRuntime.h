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
        vector<ModelMesh>& getMeshes();

        mat4 getGlobalInverseTransform() const;
        void setGlobalInverseTransform(const mat4& globalInverseTransform);

        void pushTasks() override;

    private: // Methods
        BoundingBox generateBoundingBox(aiMesh* mesh) const;
        void loadModel(string);
        void processNode(aiNode*, const aiScene*);
        ModelMesh processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) const;

    private:
        BoundingBox mBoundingBox;
        mat4 mGlobalInverseTransform;
        vector<ModelMesh> mMeshes;
        vector<string> mMaterialNames;
    };
}
