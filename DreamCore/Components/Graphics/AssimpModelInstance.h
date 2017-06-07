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

#ifndef ASSIMPMODELINSTANCE_H
#define ASSIMPMODELINSTANCE_H

#include "../../Constants.h"
#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL/SOIL.h>
#include "../../IAssetInstance.h"
#include "ShaderInstance.h"
#include <iostream>
#include <vector>
#include "AssimpMesh.h"
#include "Texture.h"
#include <map>

namespace Dream
{
    class AssimpModelInstance : public IAssetInstance
    {
    private:
        // Static
        static map<string,const aiScene*> AssimpModelCache;
        static const aiScene* getModelFromCache(string);
        static ::Assimp::Importer mImporter;
        // Variables
        vector<AssimpMesh> mMeshes;
        string mDirectory;
        // Methods
        void processNode(aiNode*, const aiScene*);
        AssimpMesh processMesh(aiMesh*, const aiScene*);
        void loadModel(string);
        vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
    public:
        AssimpModelInstance(AssetDefinition*,Transform3D*);
        ~AssimpModelInstance();
        bool load(string);
        void draw(ShaderInstance*);
        bool checkGLError(int);
        void loadExtraAttributes(nlohmann::json);
        static void cleanUpCache();
    }; // End of AssimpModelInstance
} // End of Dream

#endif // End of ASSIMPMODELINSTANCE_H
