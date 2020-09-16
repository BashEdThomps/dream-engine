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

#include "../../../Common/GLHeader.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../Material/MaterialRuntime.h"
#include "../Shader/ShaderRuntime.h"
#include "../Vertex.h"
#include "../Texture/TextureRuntime.h"
#include "../../../Common/DreamObject.h"
#include "../Light/LightRuntime.h"
#include "ModelBone.h"
#include "../../../Scene/SceneObject/BoundingBox.h"
#include "ModelTasks.h"

using std::vector;
using std::shared_ptr;
using std::string;

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
    public:
        static long DrawCalls;
        static long MeshesDrawn;
        static long TrianglesDrawn;
        static long ShadowDrawCalls;
        static long ShadowMeshesDrawn;
        static long ShadowTrianglesDrawn;

        static void ClearCounters()
        {
            DrawCalls = 0;
            MeshesDrawn = 0;
            TrianglesDrawn = 0;
            ShadowDrawCalls = 0;
            ShadowMeshesDrawn = 0;
            ShadowTrianglesDrawn = 0;
        }

        ModelMesh(ModelRuntime* parent, const string &name, const vector<Vertex> &vertexArray,
            const vector<GLuint> &indexArray, MaterialRuntime* material, const BoundingBox &bb);

        ~ModelMesh();

        void init();
        #ifdef DREAM_LOG
        void logRuntimes();
        #endif
        void addRuntime(SceneObjectRuntime* runt);
        void removeRuntime(SceneObjectRuntime* runt);

        MaterialRuntime* getMaterial();

        string getName() const;
        void setName(const string& name);

        const vector<Vertex>& getVertices() const;
        const vector<GLuint>& getIndices() const;
        GLuint getVAO() const;

        void drawGeometryPassRuntimes(Camera* camera, ShaderRuntime* shader);
        void drawShadowPassRuntimes(ShaderRuntime* shader, bool inFrustumOnly = false);

        void setVAO(const GLuint& vAO);
        GLuint getVBO() const;
        void setVBO(const GLuint& vBO);
        GLuint getIBO() const;
        void setIBO(const GLuint& iBO);
        BoundingBox getBoundingBox() const;
        void clearVertices();
        void clearIndices();
        size_t getIndicesCount();
        size_t getVerticesCount();

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
        size_t mVerticesCount;
        size_t mIndicesCount;
        BoundingBox mBoundingBox;
        ModelInitMeshTask mInitMeshTask;
        shared_ptr<ModelFreeMeshTask> mFreeMeshTask;
    };
}
