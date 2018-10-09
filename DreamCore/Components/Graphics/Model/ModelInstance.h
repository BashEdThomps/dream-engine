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
struct aiNode;

using nlohmann::json;
using glm::mat4;
using Assimp::Importer;


namespace Dream
{
    class Texture;
    class MaterialCache;
    class ShaderCache;
    class ShaderInstance;
    class ModelMesh;
    class Vertex;
    class Texture;
    class Material;

    class ModelInstance : public IAssetInstance
    {
    public:
        ModelInstance(
            ShaderCache*,
            MaterialCache*,
            IAssetDefinition*,
            SceneObjectRuntime*
        );

        ~ModelInstance() override;
        bool load(string) override;
        void draw(
            const shared_ptr<ShaderInstance>&,
            vec3 transorm,
            vec3 camPos,
            float maxDistance,
            bool alwaysDraw = false
        );
        void loadExtraAttributes(json) override;
        BoundingBox getBoundingBox();
        void setModelMatrix(mat4);
        mat4 getModelMatrix();

        void addInstance(SceneObjectRuntime*);
        void removeInstance(SceneObjectRuntime*);

    private:
        // Variables
        MaterialCache* mMaterialCache;
        ShaderCache* mShaderCache;
        map<string,ShaderInstance*> mMaterialShaderMap;

        vector<shared_ptr<ModelMesh>> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;

        // Methods
        void loadModel(string);
        shared_ptr<Importer> loadImporter(string path);
        void loadShaders();
        shared_ptr<Texture> loadMaterialTexture(aiMaterial*, aiTextureType);

        void updateBoundingBox(BoundingBox& box, aiMesh* mesh);

        void processNode(aiNode*, const aiScene*);
        shared_ptr<ModelMesh> processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        void processTextureData(aiMesh* mesh, const aiScene* scene, shared_ptr<Material> material);
    }; // End of AssimpModelInstance

} // End of Dream
