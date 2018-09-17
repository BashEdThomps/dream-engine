/*
* Dream::Components::Graphics::AssimpModelInstance
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

#pragma once

#include <vector>
#include <string>
#include <map>

#ifdef __APPLE__
#include <nlohmann/json.hpp>
#else
#include <json.hpp>
#endif

#include <assimp/Importer.hpp>
#include <assimp/material.h>

#include <glm/matrix.hpp>
#include <GL/glew.h>

#include "../../IAssetInstance.h"
#include "../BoundingBox.h"

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

using nlohmann::json;
using glm::mat4;
using Assimp::Importer;

namespace Dream
{
    class Texture;
    class MaterialCache;
    class ShaderInstance;
    class AssimpMesh;
    class AssimpCache;
    class Vertex;
    class Texture;
    class AssimpMaterial;

    class AssimpModelInstance : public IAssetInstance
    {
    public:
        AssimpModelInstance(
            AssimpCache*,
            MaterialCache*,
            IAssetDefinition*,
            SceneObjectRuntime*
        );
        ~AssimpModelInstance() override;
        bool load(string) override;
        void draw(
            ShaderInstance*,
            vec3 transorm,
            vec3 camPos,
            float maxDistance,
            bool alwaysDraw = false
        );
        void loadExtraAttributes(json) override;
        BoundingBox getBoundingBox();
        void setModelMatrix(mat4);
        mat4 getModelMatrix();

    private:
        // Variables
        AssimpCache* mModelCache;
        MaterialCache* mMaterialCache;

        vector<AssimpMesh*> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;

        // Methods
        void loadModel(string);
        void loadShaders();
        Texture* loadMaterialTexture(aiMaterial*, aiTextureType, string);

        void updateBoundingBox(BoundingBox& box, aiMesh* mesh);
        void initBoundingBox();

        void processNode(aiNode*, const aiScene*);
        AssimpMesh* processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        void processTextureData(aiMesh* mesh, const aiScene* scene, AssimpMaterial* material);
        map<string,ShaderInstance*> mMaterialShaderMap;
    }; // End of AssimpModelInstance

} // End of Dream
