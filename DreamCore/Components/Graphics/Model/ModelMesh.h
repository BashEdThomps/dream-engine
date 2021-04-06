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

#include "ModelBone.h"
#include "ModelTasks.h"

#include "Common/GLHeader.h"

#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Vertex.h"
#include "Entity/BoundingBox.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

using std::vector;
using std::shared_ptr;
using std::string;

namespace octronic::dream
{
  class ModelRuntime;
  class MaterialRuntime;
  class TextureRuntime;
  class ShaderRuntime;
  class EntityRuntime;
  class CameraRuntime;

  class ModelMesh
  {
  public: // static
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

  public:
    ModelMesh(ModelRuntime& parent, const string &name,
              const vector<Vertex>& vertexArray, const vector<GLuint>& indexArray,
              MaterialRuntime& material, const BoundingBox& bb);

    ~ModelMesh();

    void initTasks();

    void logRuntimes();
    void addRuntime(EntityRuntime& runt);
    void removeRuntime(EntityRuntime& runt);

    optional<reference_wrapper<MaterialRuntime>> getMaterial();

    string getName() const;
    void setName(const string& name);

    vector<Vertex> getVertices() const;
    vector<GLuint> getIndices() const;

    void drawModelRuntimes(CameraRuntime& camera, ShaderRuntime& shader);
    void drawShadowPassRuntimes(ShaderRuntime& shader, bool inFrustumOnly = false);

    GLuint getVAO() const;
    void setVAO(const GLuint& vAO);

    GLuint getVBO() const;
    void setVBO(const GLuint& vBO);

    GLuint getIBO() const;
    void setIBO(const GLuint& iBO);

    BoundingBox getBoundingBox() const;
    void clearVertices();
    void clearIndices();
    size_t getIndicesCount() const;
    size_t getVerticesCount() const;
    bool loadIntoGL();
    void pushTasks();

    bool getLoaded() const;

    ModelRuntime& getParent();

  private:
    void renderDebugSphere(ShaderRuntime& shader);
  private:
    reference_wrapper<ModelRuntime> mParent;
    optional<reference_wrapper<MaterialRuntime>> mMaterial;
    string mName;
    GLuint mVAO;
    GLuint mVBO;
    GLuint mIBO;
    vector<Vertex> mVertices;
    vector<GLuint> mIndices;
    vector<reference_wrapper<EntityRuntime>> mRuntimesInFrustum;
    size_t mVerticesCount;
    size_t mIndicesCount;
    BoundingBox mBoundingBox;
    bool mLoaded;
    // Mesh Tasks
    shared_ptr<ModelInitMeshTask> mInitMeshTask;
    shared_ptr<ModelFreeMeshTask> mFreeMeshTask;
  };
}
