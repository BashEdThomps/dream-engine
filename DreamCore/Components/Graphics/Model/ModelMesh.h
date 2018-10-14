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

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "../Texture/Texture.h"

#include "../Material/Material.h"
#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"
#include "../BoundingBox.h"
#include "../../../Common/Constants.h"
#include "../../../Common/DreamObject.h"


namespace Dream
{
    class ModelInstance;

    class ModelMesh : public DreamObject
    {
    private:
        ModelInstance* mParent;
        shared_ptr<Material> mMaterial;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;

        vector<Vertex>  mVertices;
        vector<GLuint> mIndices;
        BoundingBox mBoundingBox;
        vector<SceneObjectRuntime*> mInstances;

    public:
        ModelMesh
        (
            ModelInstance* parent,
            string name,
            vector<Vertex> vertexArray,
            vector<GLuint> indexArray,
            shared_ptr<Material> material
        );

        ~ModelMesh();
        void draw(const shared_ptr<ShaderInstance>&);
        void init();
        void logInstances();
        void addInstance(SceneObjectRuntime* runt);
        void removeInstance(SceneObjectRuntime* runt);

        const shared_ptr<Material>& getMaterial();

        string getName() const;
        void setName(const string& name);
        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
        GLuint getVAO() const;
        void drawInstances(ShaderInstance* shader);
    };
}
