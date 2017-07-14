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
#include <json.hpp>

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
    class TextureCache;
    class ShaderInstance;
    class AssimpMesh;
    class AssimpCache;
    class Vertex;
    class Texture;

    class AssimpModelInstance : public IAssetInstance
    {
    private:
        // Variables
        AssimpCache* mModelCacheHandle;
        TextureCache* mTextureCacheHandle;

        vector<AssimpMesh> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;

        // Methods
        void loadModel(string);
        vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
        void updateBoundingBox(aiMesh* mesh);
        void initBoundingBox();

        void processNode(aiNode*, const aiScene*);
        AssimpMesh processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
        vector<Texture> processTextureData(aiMesh* mesh, const aiScene* scene);
    public:
        AssimpModelInstance(AssimpCache*, TextureCache*,  IAssetDefinition*,SceneObjectRuntime*);
        ~AssimpModelInstance();
        bool load(string);
        void draw(ShaderInstance*);
        void loadExtraAttributes(json);
        BoundingBox getBoundingBox();
        void setModelMatrix(mat4);
        mat4 getModelMatrix();

    }; // End of AssimpModelInstance

} // End of Dream
