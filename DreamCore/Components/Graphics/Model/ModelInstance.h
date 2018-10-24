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

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <vector>
#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include "../BoundingBox.h"
#include "../../IAssetInstance.h"
#include <glm/matrix.hpp>


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
    struct Vertex;
    class Texture;
    class MaterialInstance;

    class ModelInstance : public IAssetInstance
    {
    public:
        ModelInstance(
            ShaderCache*,
            MaterialCache*,
            IAssetDefinition*
        );

        ~ModelInstance() override;
        bool load(string) override;
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

        vector<ModelMesh*> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;

        // Methods
        void loadModel(string);
        shared_ptr<Importer> loadImporter(string path);

        void updateBoundingBox(BoundingBox& box, aiMesh* mesh);

        void processNode(aiNode*, const aiScene*);
        ModelMesh* processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
    }; // End of AssimpModelInstance

} // End of Dream
