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


#include "ModelMesh.h"

#include "ModelRuntime.h"
#include "ModelTasks.h"
#include "Common/Logger.h"
#include "Components/Graphics/CameraRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  ModelMesh::ModelMesh
  (const weak_ptr<ModelRuntime>& parent,
   const string& name,
   const vector<Vertex>& vertices,
   const vector<GLuint>& indices,
   const weak_ptr<MaterialRuntime>& material,
   const BoundingBox& bb)
    : mParent(parent),
      mMaterial(material),
      mName(name),
      mVAO(0),
      mVBO(0),
      mIBO(0),
      mLoaded(false),
      mVertices(vertices),
      mIndices(indices),
      mVerticesCount(vertices.size()),
      mIndicesCount(indices.size()),
      mBoundingBox(bb)
    {
    if (auto parentLock = mParent.lock())
    {
      LOG_TRACE("ModelMesh: Constructing Mesh for {}", parentLock->getName());
    }
  }

  void
  ModelMesh::initTasks
  ()
  {
    if (auto parentLock = mParent.lock())
    {
      mInitMeshTask = make_shared<ModelInitMeshTask>(parentLock->getProjectRuntime(), shared_from_this());
      mFreeMeshTask = make_shared<ModelFreeMeshTask>(parentLock->getProjectRuntime());
    }
  }

  ModelMesh::~ModelMesh
  ()
  {
    if (auto parentLock = mParent.lock())
    {
      LOG_TRACE("ModelMesh: Destroying Mesh for {}",parentLock->getNameAndUuidString());

      if (!mMaterial.expired())
      {
        mMaterial.lock()->removeMesh(shared_from_this());
      }

      mFreeMeshTask->setBuffers(mVAO,mVBO,mIBO);

      if (auto prLock = parentLock->getProjectRuntime().lock())
      {
        if (auto gfxComp = prLock->getGraphicsComponent().lock())
        {
          if (auto gfxDestQueue = gfxComp->getDestructionTaskQueue().lock())
          {
            gfxDestQueue->pushTask(mFreeMeshTask);
          }
        }
      }
    }
  }

  string
  ModelMesh::getName
  () const
  {
    return mName;
  }

  void
  ModelMesh::setName
  (const string& name)
  {
    mName = name;
  }

  vector<Vertex>
  ModelMesh::getVertices
  () const
  {
    return mVertices;
  }

  vector<GLuint>
  ModelMesh::getIndices
  () const
  {
    return mIndices;
  }

  void
  ModelMesh::logRuntimes
  ()
  {
    if (auto parentLock = mParent.lock())
    {
      for (auto runtime : parentLock->getInstanceVector())
      {
        if (auto runtimeLock = runtime.lock())
        {
          LOG_DEBUG("\t\t\tRuntime for {}", runtimeLock->getNameAndUuidString());
        }
      }
    }
  }

  weak_ptr<MaterialRuntime>
  ModelMesh::getMaterial
  ()
  const
  {
    return mMaterial;
  }

  GLuint
  ModelMesh::getVAO
  ()
  const
  {
    return mVAO;
  }

  void
  ModelMesh::drawModelRuntimes
  (const weak_ptr<CameraRuntime>& camera,
   const weak_ptr<ShaderRuntime>& shader)
  {
    if (auto parentLock = mParent.lock())
    {
      if (auto cameraLock = camera.lock())
      {
        // TODO - Move this to grpahics component
        mRuntimesInFrustum.clear();
        auto runtimes = parentLock->getInstanceVector();

        for (auto entity : runtimes)
        {
          if (auto entityLock = entity.lock())
          {
            // TODO -- Per mesh Culling
            if(cameraLock->visibleInFrustum(mBoundingBox, entityLock->getTransform().getMatrix()))
            {
              mRuntimesInFrustum.push_back(entity);
            }
          }
        }

        if (mRuntimesInFrustum.empty())
        {
          LOG_DEBUG("ModelMesh: (Geometry) No Runtimes of {} in Frustum", getName());
          return;
        }

        if (auto shaderLock = shader.lock())
        {
          LOG_TRACE("ModelMesh: (Geometry) Drawing {} Runtimes of mesh {} for Geometry pass",
                    runtimes.size(),
                    getName());
          shaderLock->bindVertexArray(mVAO);
          shaderLock->bindRuntimes(mRuntimesInFrustum);
          shaderLock->syncUniforms();
          GLCheckError();

          size_t size = mRuntimesInFrustum.size();
          if (size == 0)
          {
            LOG_TRACE("ModelMesh: No runtimes to draw");
            return;
          }
          if (size > ShaderRuntime::MAX_RUNTIMES)
          {
            LOG_TRACE("ModelMesh: (Geometry) Limiting to {}", ShaderRuntime::MAX_RUNTIMES);
            size = ShaderRuntime::MAX_RUNTIMES;
          }

          size_t indices = mIndicesCount;
          if (indices == 0) return;

          size_t tris = indices/3;
          MeshesDrawn += size;
          TrianglesDrawn += tris*size;
          glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr,size);
          //renderDebugSphere(shader);
          GLCheckError();
          DrawCalls++;
        }
      }
    }

  }

  void
  ModelMesh::drawShadowPassRuntimes
  (const weak_ptr<ShaderRuntime>& shader, bool inFrustumOnly)
  {
    if (auto parentLock = mParent.lock())
    {

      auto runtimes = parentLock->getInstanceVector();

      if (runtimes.empty())
      {
        LOG_DEBUG("ModelMesh: (Shadow) No Runtimes of {} in Frustum", getName());
        return;
      }

      LOG_TRACE("ModelMesh: (Shadow) Drawing {} Runtimes of mesh {}", runtimes.size(), getName());

      if (auto shaderLock = shader.lock())
      {

        shaderLock->bindVertexArray(mVAO);
        shaderLock->bindRuntimes(inFrustumOnly ? mRuntimesInFrustum : runtimes);
        shaderLock->syncUniforms();

        size_t size = (inFrustumOnly ? mRuntimesInFrustum.size() : runtimes.size());

        if (size > ShaderRuntime::MAX_RUNTIMES)
        {
          size = ShaderRuntime::MAX_RUNTIMES;
          LOG_TRACE("ModelMesh: (Shadow) Limiting to {}", ShaderRuntime::MAX_RUNTIMES);
        }

        size_t indices = mIndicesCount;
        size_t tris = indices/3;
        ShadowMeshesDrawn += size;
        ShadowTrianglesDrawn += tris*size;
        glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr,size);
        ShadowDrawCalls++;
      }
    }
  }

  void
  ModelMesh::setVAO
  (const GLuint& vAO)
  {
    mVAO = vAO;
  }

  GLuint
  ModelMesh::getVBO
  () const
  {
    return mVBO;
  }

  void
  ModelMesh::setVBO
  (const GLuint& vBO)
  {
    mVBO = vBO;
  }

  GLuint
  ModelMesh::getIBO
  () const
  {
    return mIBO;
  }

  void
  ModelMesh::setIBO
  (const GLuint& iBO)
  {
    mIBO = iBO;
  }

  BoundingBox
  ModelMesh::getBoundingBox
  ()
  const
  {
    return mBoundingBox;
  }

  void
  ModelMesh::clearVertices
  ()
  {
    mVertices.clear();
  }

  void
  ModelMesh::clearIndices
  ()
  {
    mIndices.clear();
  }

  size_t
  ModelMesh::getIndicesCount
  ()
  const
  {
    return mIndicesCount;
  }

  size_t
  ModelMesh::getVerticesCount
  ()
  const
  {
    return mVerticesCount;
  }

  bool
  ModelMesh::loadIntoGL()
  {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mIBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(getVertices().size() * sizeof(Vertex)), &getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(getIndices().size() * sizeof(GLuint)),&getIndices()[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(Vertex)), static_cast<GLvoid*>(nullptr));
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, TexCoords));
    // Vertex Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Tangent));
    // Vertex Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);

    clearVertices();
    clearIndices();

    mLoaded = true;

    return true;
  }

  // renders (and builds at first invocation) a sphere
  // -------------------------------------------------
  void
  ModelMesh::renderDebugSphere
  (const weak_ptr<ShaderRuntime>& shader)
  {
    static unsigned int sphereVAO = 0;
    static unsigned int indexCount;

    if (sphereVAO == 0)
    {
      glGenVertexArrays(1, &sphereVAO);

      unsigned int vbo, ebo;
      glGenBuffers(1, &vbo);
      glGenBuffers(1, &ebo);

      std::vector<glm::vec3> positions;
      std::vector<glm::vec2> uv;
      std::vector<glm::vec3> normals;
      std::vector<unsigned int> indices;

      const unsigned int X_SEGMENTS = 64;
      const unsigned int Y_SEGMENTS = 64;
      const float PI = 3.14159265359;
      for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
      {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
          float xSegment = (float)x / (float)X_SEGMENTS;
          float ySegment = (float)y / (float)Y_SEGMENTS;
          float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
          float yPos = std::cos(ySegment * PI);
          float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

          positions.push_back(glm::vec3(xPos, yPos, zPos));
          uv.push_back(glm::vec2(xSegment, ySegment));
          normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
      }

      bool oddRow = false;
      for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
      {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
          for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
          {
            indices.push_back(y       * (X_SEGMENTS + 1) + x);
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
          }
        }
        else
        {
          for (int x = X_SEGMENTS; x >= 0; --x)
          {
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            indices.push_back(y       * (X_SEGMENTS + 1) + x);
          }
        }
        oddRow = !oddRow;
      }
      indexCount = indices.size();

      std::vector<float> data;
      for (unsigned int i = 0; i < positions.size(); ++i)
      {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
          data.push_back(uv[i].x);
          data.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
          data.push_back(normals[i].x);
          data.push_back(normals[i].y);
          data.push_back(normals[i].z);
        }
      }
      glBindVertexArray(sphereVAO);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
      float stride = (3 + 2 + 3) * sizeof(float);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    size_t size = mRuntimesInFrustum.size();
    if (size > ShaderRuntime::MAX_RUNTIMES)
    {
      LOG_TRACE("ModelMesh: (Geometry) Limiting to {}", ShaderRuntime::MAX_RUNTIMES);
      size = ShaderRuntime::MAX_RUNTIMES;
    }


    if (auto sLock = shader.lock())
    {
    	sLock->syncUniforms();
    }
    glBindVertexArray(sphereVAO);
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, nullptr,size);
    //glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    GLCheckError();
  }

  bool ModelMesh::getLoaded() const
  {
    return mLoaded;
  }


  void ModelMesh::pushTasks()
  {
    if (auto parentLock = mParent.lock())
    {
      if (auto prLock = parentLock->getProjectRuntime().lock())
      {
        if (auto gfxComp = prLock->getGraphicsComponent().lock())
        {
          if (auto gfxQueue = gfxComp->getTaskQueue().lock())
          {
            if (mInitMeshTask->hasState(TASK_STATE_QUEUED))
            {
              gfxQueue->pushTask(mInitMeshTask);
            }
          }
        }
      }
    }
  }

  // Statics =================================================================

  long ModelMesh::DrawCalls = 0;
  long ModelMesh::MeshesDrawn = 0;
  long ModelMesh::TrianglesDrawn = 0;
  long ModelMesh::ShadowDrawCalls = 0;
  long ModelMesh::ShadowMeshesDrawn = 0;
  long ModelMesh::ShadowTrianglesDrawn = 0;


}
