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
#include <glm/matrix.hpp>

#include "../../SharedAssetRuntime.h"
#include "../../../Scene/SceneObject/BoundingBox.h"


struct aiScene;
struct aiMaterial;
struct aiNode;
struct aiMesh;
using namespace glm;
using namespace Assimp;


namespace Dream
{
    class Texture;
    class MaterialCache;
    class ShaderCache;
    class ShaderRuntime;
    class ModelMesh;
    struct Vertex;
    class Texture;
    class MaterialRuntime;
    class SceneObjectRuntime;

    class ModelRuntime
        : public SharedAssetRuntime
    {
    public:
        ModelRuntime(
            ShaderCache*,
            MaterialCache*,
            AssetDefinition*,
            ProjectRuntime*
        );

        ~ModelRuntime() override;
        bool useDefinition() override;

        BoundingBox& getBoundingBox();

        void setModelMatrix(mat4);
        mat4 getModelMatrix();

        void addInstance(SceneObjectRuntime*);
        void removeInstance(SceneObjectRuntime*);

        vector<string> getMaterialNames();
        vector<ModelMesh*> getMeshes() const;

    private:
        // Variables
        MaterialCache* mMaterialCache;
        ShaderCache* mShaderCache;
        vector<ModelMesh*> mMeshes;
        string mDirectory;
        BoundingBox mBoundingBox;
        mat4 mModelMatrix;
        vector<string> mMaterialNames;

        // Methods
        void updateBoundingBox(aiMesh* mesh);
        void loadModel(string);
        shared_ptr<Importer> loadImporter(string path);
        void processNode(aiNode*, const aiScene*);
        ModelMesh* processMesh(aiMesh*, const aiScene*);
        vector<Vertex> processVertexData(aiMesh* mesh);
        vector<GLuint> processIndexData(aiMesh* mesh);
    };

}
