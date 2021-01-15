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
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;

namespace Dream
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
          mInitMeshTask(this),
          mFreeMeshTask(nullptr)
    {
        LOG_TRACE("ModelMesh: Constructing Mesh for {}", parent->getName());
        init();
    }

    ModelMesh::~ModelMesh
    ()
    {
        LOG_TRACE("ModelMesh: Destroying Mesh for {}",mParent->getNameAndUuidString());
        mFreeMeshTask = make_shared<ModelFreeMeshTask>();
        mFreeMeshTask->clearState();
        mFreeMeshTask->setState(TaskState::QUEUED);
        mFreeMeshTask->setBuffers(mVAO,mVBO,mIBO);
        mParent->getProjectRuntime()->getGraphicsComponent()->pushDestructionTask(mFreeMeshTask);
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
    ModelMesh::init
    ()
    {
        mInitMeshTask.clearState();
        mInitMeshTask.setState(TaskState::QUEUED);
        mParent->getProjectRuntime()->getGraphicsComponent()->pushTask(&mInitMeshTask);
    }

    void
    ModelMesh::logRuntimes
    ()
    {
       for (auto runtime : mRuntimes)
       {
           LOG_DEBUG("\t\t\tRuntime for {}", runtime->getNameAndUuidString());
       }
    }

    void
    ModelMesh::addRuntime
    (EntityRuntime* runt)
    {
        LOG_DEBUG("ModelMesh: Adding Runtime of mesh for {}", runt->getNameAndUuidString());
        mRuntimes.push_back(runt);
    }

    void
    ModelMesh::removeRuntime
    (EntityRuntime* runt)
    {
        LOG_DEBUG("ModelMesh: Removing Runtime of mesh for {}", runt->getNameAndUuidString());
        auto itr = find (mRuntimes.begin(), mRuntimes.end(), runt);
        if (itr != mRuntimes.end())
        {
           mRuntimes.erase(itr);
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
        for (auto* sor : mRuntimes)
        {
            // TODO -- Per mesh Culling
            if(camera->visibleInFrustum(mBoundingBox, sor->getTransform().getMatrix()))
            {
                mRuntimesInFrustum.push_back(sor);
            }
        }
        if (mRuntimesInFrustum.empty())
        {
            LOG_DEBUG("ModelMesh: (Geometry) No Runtimes of {} in Frustum", getName());
            return;
        }
        LOG_TRACE("ModelMesh: (Geometry) Drawing {} Runtimes of mesh {} for Geometry pass", mRuntimes.size(), getName());
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
        if (mRuntimes.empty())
        {
            LOG_DEBUG("ModelMesh: (Shadow) No Runtimes of {} in Frustum", getName());
            return;
        }
        LOG_TRACE("ModelMesh: (Shadow) Drawing {} Runtimes of mesh {}", mRuntimes.size(), getName());
        shader->bindVertexArray(mVAO);
        shader->bindRuntimes(inFrustumOnly ? mRuntimesInFrustum : mRuntimes);
        size_t size = (inFrustumOnly ? mRuntimesInFrustum.size() : mRuntimes.size());
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

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::MeshesDrawn = 0;
    long ModelMesh::TrianglesDrawn = 0;
    long ModelMesh::ShadowDrawCalls = 0;
    long ModelMesh::ShadowMeshesDrawn = 0;
    long ModelMesh::ShadowTrianglesDrawn = 0;
}
