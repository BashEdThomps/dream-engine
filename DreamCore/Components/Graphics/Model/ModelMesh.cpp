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
#include "Components/Graphics/Camera.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;


namespace octronic::dream
{
    ModelMesh::ModelMesh
    (ModelRuntime* parent, const string& name, const vector<Vertex>& vertices,
     const vector<GLuint>& indices, MaterialRuntime* material, const BoundingBox& bb)
        : mParent(parent),
          mMaterial(material),
          mName(name),
          mVAO(0),
          mVBO(0),
          mIBO(0),
          mVertices(vertices),
          mIndices(indices),
          mVerticesCount(vertices.size()),
          mIndicesCount(indices.size()),
          mBoundingBox(bb),
          mInitMeshTask(make_shared<ModelInitMeshTask>(parent->getProjectRuntimeHandle(), this)),
          mFreeMeshTask(make_shared<ModelFreeMeshTask>(parent->getProjectRuntimeHandle()))
    {
        LOG_TRACE("ModelMesh: Constructing Mesh for {}", parent->getName());
    }

    ModelMesh::~ModelMesh
    ()
    {
        LOG_TRACE("ModelMesh: Destroying Mesh for {}",mParent->getNameAndUuidString());
        mFreeMeshTask->clearState();
        mFreeMeshTask->setBuffers(mVAO,mVBO,mIBO);
        auto gfxDestructionQueue = mParent->getProjectRuntimeHandle()->getGraphicsComponent()->getDestructionTaskQueue();
        gfxDestructionQueue->pushTask(mFreeMeshTask);
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

    const vector<Vertex> &
    ModelMesh::getVertices
    () const
    {
        return mVertices;
    }

    const vector<GLuint>&
    ModelMesh::getIndices
    () const
    {
        return mIndices;
    }

    void
    ModelMesh::logRuntimes
    ()
    {
        for (auto runtime : *mParent->getInstanceVector())
        {
            LOG_DEBUG("\t\t\tRuntime for {}", runtime->getNameAndUuidString());
        }
    }

    MaterialRuntime*
    ModelMesh::getMaterial
    ()
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
    ModelMesh::drawGeometryPassRuntimes
    (Camera* camera, ShaderRuntime* shader)
    {
        mRuntimesInFrustum.clear();
        auto runtimes = mParent->getInstanceVector();

        for (auto entity : *runtimes)
        {
            // TODO -- Per mesh Culling
            if(camera->visibleInFrustum(mBoundingBox, entity->getTransform().getMatrix()))
            {
                mRuntimesInFrustum.push_back(entity);
            }
        }

        if (mRuntimesInFrustum.empty())
        {
            LOG_DEBUG("ModelMesh: (Geometry) No Runtimes of {} in Frustum", getName());
            return;
        }

        LOG_TRACE("ModelMesh: (Geometry) Drawing {} Runtimes of mesh {} for Geometry pass", runtimes->size(), getName());
        shader->bindVertexArray(mVAO);
        shader->bindRuntimes(mRuntimesInFrustum);

        size_t size = mRuntimesInFrustum.size();
        if (size > ShaderRuntime::MAX_RUNTIMES)
        {
            LOG_TRACE("ModelMesh: (Geometry) Limiting to {}", ShaderRuntime::MAX_RUNTIMES);
            size = ShaderRuntime::MAX_RUNTIMES;
        }

        size_t indices = mIndicesCount;
        size_t tris = indices/3;
        MeshesDrawn += size;
        TrianglesDrawn += tris*size;
        glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr,size);
        DrawCalls++;

    }

    void
    ModelMesh::drawShadowPassRuntimes
    (ShaderRuntime* shader, bool inFrustumOnly)
    {
        auto runtimes = mParent->getInstanceVector();

        if (runtimes->empty())
        {
            LOG_DEBUG("ModelMesh: (Shadow) No Runtimes of {} in Frustum", getName());
            return;
        }

        LOG_TRACE("ModelMesh: (Shadow) Drawing {} Runtimes of mesh {}", runtimes->size(), getName());

        shader->bindVertexArray(mVAO);
        shader->bindRuntimes(inFrustumOnly ? mRuntimesInFrustum : *runtimes);

        size_t size = (inFrustumOnly ? mRuntimesInFrustum.size() : runtimes->size());
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
    () const
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
    ModelMesh::getIndicesCount()
    {
        return mIndicesCount;
    }

    size_t
    ModelMesh::getVerticesCount()
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

        return true;
    }


    void ModelMesh::pushNextTask()
    {
        auto gfxTaskQueue = mParent->getProjectRuntimeHandle()->getGraphicsComponent()->getTaskQueue();
        if (mInitMeshTask->hasState(TASK_STATE_QUEUED))
        {
            gfxTaskQueue->pushTask(mInitMeshTask);
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
