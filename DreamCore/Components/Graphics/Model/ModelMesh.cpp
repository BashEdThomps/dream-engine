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
#include "../Camera.h"
#include "../Shader/ShaderRuntime.h"
#include "../GraphicsComponent.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../Project/ProjectRuntime.h"

namespace Dream
{


    ModelMesh::ModelMesh
    (ModelRuntime* parent, const string& name, const vector<Vertex>& vertices,
     const vector<GLuint>& indices, MaterialRuntime* material, const BoundingBox& bb)
        : DreamObject("ModelMesh"),
          mParent(parent),
          mMaterial(material),
          mName(name),
          mVAO(0),
          mVBO(0),
          mIBO(0),
          mVertices(vertices),
          mIndices(indices),
          mBoundingBox(bb),
          mInitMeshTask(nullptr)
    {
        #ifdef DREAM_LOG
        getLog()->trace("Constructing Mesh for {}", parent->getName());
        #endif
        init();
    }

    ModelMesh::~ModelMesh
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destroying Mesh for {}",mParent->getNameAndUuidString());
        #endif
        if (mInitMeshTask)
        {
            mInitMeshTask->setExpired(true);
        }
        mParent
            ->getProjectRuntime()
            ->getGraphicsComponent()
            ->pushTask(new ModelFreeMeshTask(mVAO,mVBO,mIBO));
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
        mInitMeshTask = new ModelInitMeshTask(this);
        mParent->getProjectRuntime()->getGraphicsComponent()->pushTask(mInitMeshTask);
    }

    #ifdef DREAM_LOG
    void
    ModelMesh::logRuntimes
    ()
    {
       for (auto runtime : mRuntimes)
       {
           getLog()->debug("\t\t\tRuntime for {}", runtime->getNameAndUuidString());
       }
    }
    #endif

    void
    ModelMesh::addRuntime
    (SceneObjectRuntime* runt)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Adding Runtime of mesh for {}", runt->getNameAndUuidString());
        #endif
        mRuntimes.push_back(runt);
    }

    void
    ModelMesh::removeRuntime
    (SceneObjectRuntime* runt)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Removing Runtime of mesh for {}", runt->getNameAndUuidString());
        #endif
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
            #ifdef DREAM_LOG
            getLog()->debug("(Geometry) No Runtimes of {} in Frustum", getName());
            #endif
            return;
        }
        #ifdef DREAM_LOG
        getLog()->trace("(Geometry) Drawing {} Runtimes of mesh {} for Geometry pass", mRuntimes.size(), getName());
        #endif
        shader->bindVertexArray(mVAO);
        shader->bindRuntimes(mRuntimesInFrustum);
        GLsizei size = mRuntimesInFrustum.size();
        GLsizei indices = mIndices.size();
        GLsizei tris = indices/3;
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
            #ifdef DREAM_LOG
            getLog()->debug("(Shadow) No Runtimes of {} in Frustum", getName());
            #endif
            return;
        }
        #ifdef DREAM_LOG
        getLog()->trace("(Shadow) Drawing {} Runtimes of mesh {}", mRuntimes.size(), getName());
        #endif
        shader->bindVertexArray(mVAO);
        shader->bindRuntimes(inFrustumOnly ? mRuntimesInFrustum : mRuntimes);
        GLsizei size = (inFrustumOnly ? mRuntimesInFrustum.size() : mRuntimes.size());
        GLsizei indices = mIndices.size();
        GLsizei tris = indices/3;
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
    ModelMesh::clearInitMeshTask
    ()
    {
       mInitMeshTask = nullptr;
    }

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::MeshesDrawn = 0;
    long ModelMesh::TrianglesDrawn = 0;
    long ModelMesh::ShadowDrawCalls = 0;
    long ModelMesh::ShadowMeshesDrawn = 0;
    long ModelMesh::ShadowTrianglesDrawn = 0;
}
