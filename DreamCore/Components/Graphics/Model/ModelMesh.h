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

#include <iostream>
#include <sstream>
#include <vector>

#include "../BoundingBox.h"
#include "../Material/MaterialInstance.h"
#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"
#include "../Texture/TextureInstance.h"
#include "../../../Common/DreamObject.h"
#include <glm/glm.hpp>


namespace Dream
{
    class ModelInstance;
	class MaterialInstance;
	class TextureInstance;
	class ShaderInstance;
	class SceneObjectRuntime;

    class ModelMesh : public DreamObject
    {
    private:
        ModelInstance* mParent;
        MaterialInstance* mMaterial;
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
            MaterialInstance* material
        );

        ~ModelMesh();
        void init();
        void logInstances();
        void addInstance(SceneObjectRuntime* runt);
        void removeInstance(SceneObjectRuntime* runt);

        MaterialInstance* getMaterial();

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
