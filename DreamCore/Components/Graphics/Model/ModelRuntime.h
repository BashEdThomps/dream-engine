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

#include "Common/GLHeader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <vector>
#include <string>
#include <map>

#include "glm/matrix.hpp"

#include "Components/SharedAssetRuntime.h"
#include "Scene/Entity/BoundingBox.h"
#include "ModelBone.h"
#include "ModelAnimation.h"

struct aiScene;
struct aiMaterial;
struct aiNode;
struct aiMesh;
using namespace glm;
using namespace Assimp;

namespace octronic::dream
{
    class Texture;
    class MaterialCache;
    class ShaderCache;
    class ShaderRuntime;
    class ModelMesh;
    struct Vertex;
    class Texture;
    class MaterialRuntime;
    class EntityRuntime;

    class ModelRuntime : public SharedAssetRuntime
    {
    public:
        ModelRuntime(ShaderCache*,MaterialCache*,AssetDefinition*,ProjectRuntime*);

        ~ModelRuntime() override;
        bool useDefinition() override;

        BoundingBox& getBoundingBox();

        void addRuntime(EntityRuntime*);
        void removeRuntime(EntityRuntime*);

        vector<string> getMaterialNames();
        vector<ModelMesh*> getMeshes() const;

        map<string, Bone>& getBones();
        map<string, ModelAnimation>& getAnimations();

        mat4 getGlobalInverseTransform() const;
        void setGlobalInverseTransform(const mat4& globalInverseTransform);

    private:
        // Variables
        MaterialCache* mMaterialCache;
        ShaderCache* mShaderCache;
        vector<ModelMesh*> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;
        vector<string> mMaterialNames;
        map<string,Bone> mBones;
        map<string,ModelAnimation> mAnimations;
        mat4 mGlobalInverseTransform;

        // Methods
        BoundingBox updateBoundingBox(aiMesh* mesh);
        void loadModel(string);
        shared_ptr<Importer> loadImporter(string path);
        void processNode(aiNode*, const aiScene*);
        ModelMesh* processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        void processBoneData(aiMesh* mesh);
        void processAnimationData(aiNode* mesh);

        glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
    };
}
