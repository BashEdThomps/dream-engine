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
#include <glm/glm.hpp>

#include "../Material/MaterialRuntime.h"
#include "../Shader/ShaderRuntime.h"
#include "../Vertex.h"
#include "../Texture/TextureRuntime.h"
#include "../../../Common/DreamObject.h"
#include "../Light/LightRuntime.h"
#include "ModelBone.h"

namespace Dream
{
    class ModelRuntime;
    class MaterialRuntime;
    class TextureRuntime;
    class ShaderRuntime;
    class SceneObjectRuntime;
    class Camera;

    class ModelMesh : public DreamObject
    {
    private:
        ModelRuntime* mParent;
        MaterialRuntime* mMaterial;
        string mName;
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
        vector<Vertex> mVertices;
        vector<GLuint> mIndices;
        vector<SceneObjectRuntime*> mRuntimes;
        vector<SceneObjectRuntime*> mRuntimesInFrustum;

    public:
        static long DrawCalls;
        static long RuntimesDrawn;
        static long ShadowDrawCalls;
        static long ShadowRuntimesDrawn;

        ModelMesh
        (
            ModelRuntime* parent, string name, vector<Vertex> vertexArray,
            vector<GLuint> indexArray, MaterialRuntime* material
        );

        ~ModelMesh();

        void init();
        void logRuntimes();
        void addRuntime(SceneObjectRuntime* runt);
        void removeRuntime(SceneObjectRuntime* runt);
        MaterialRuntime* getMaterial();
        string getName() const;
        void setName(const string& name);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
        GLuint getVAO() const;
        void drawGeometryPassRuntimes(Camera* camera, ShaderRuntime* shader);
        void drawShadowPassRuntimes(ShaderRuntime* shader, bool inFrustumOnly = false);
    };
}
