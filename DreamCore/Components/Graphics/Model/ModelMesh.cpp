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
    BoundingBox ModelMesh::getBoundingBox() const
    {
        return mBoundingBox;
    }

    ModelMesh::ModelMesh
    (ModelRuntime* parent, string name, vector<Vertex> vertices,
     vector<GLuint> indices, MaterialRuntime* material, const BoundingBox bb)
        : DreamObject("ModelMesh"),
        mParent(parent),mMaterial(material),mName(name),
        mVertices(vertices),mIndices(indices),mBoundingBox(bb)
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Constructing Mesh for {}", parent->getName());
#endif
        init();
    }

    ModelMesh::~ModelMesh
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destroying Mesh for {}",mParent->getNameAndUuidString());
#endif
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
        mParent->getProjectRuntime()->getGraphicsComponent()->pushTask(new ModelInitMeshTask(this));
    }

#ifdef DREAM_LOG
    void
    ModelMesh::logRuntimes
    ()
    {
       auto log = getLog();
       for (auto Runtime : mRuntimes)
       {
           log->debug("\t\t\tRuntime for {}", Runtime->getNameAndUuidString());
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
       auto log = getLog();
       log->debug("Removing Runtime of mesh for {}", runt->getNameAndUuidString());
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
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        mRuntimesInFrustum.clear();
        for (size_t i=0; i<mRuntimes.size();i++)
        {
            // TODO -- Per mesh Culling
            auto sor = mRuntimes.at(i);
            if(camera->visibleInFrustum(mBoundingBox, sor->getTransform().getMatrix()))
            {
                mRuntimesInFrustum.push_back(sor);
            }
        }
        if (mRuntimesInFrustum.empty())
        {
#ifdef DREAM_LOG
            log->debug("(Geometry) No Runtimes of {} in Frustum", getName());
#endif
            return;
        }
#ifdef DREAM_LOG
        log->trace("(Geometry) Drawing {} Runtimes of mesh {} for Geometry pass", mRuntimes.size(), getName());
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
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        if (mRuntimes.empty())
        {
#ifdef DREAM_LOG
            log->debug("(Shadow) No Runtimes of {} in Frustum", getName());
#endif
            return;
        }
#ifdef DREAM_LOG
        log->trace("(Shadow) Drawing {} Runtimes of mesh {}", mRuntimes.size(), getName());
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

    void ModelMesh::setVAO(const GLuint& vAO)
    {
        mVAO = vAO;
    }

    GLuint ModelMesh::getVBO() const
    {
        return mVBO;
    }

    void ModelMesh::setVBO(const GLuint& vBO)
    {
        mVBO = vBO;
    }

    GLuint ModelMesh::getIBO() const
    {
        return mIBO;
    }

    void ModelMesh::setIBO(const GLuint& iBO)
    {
        mIBO = iBO;
    }

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::MeshesDrawn = 0;
    long ModelMesh::TrianglesDrawn = 0;
    long ModelMesh::ShadowDrawCalls = 0;
    long ModelMesh::ShadowMeshesDrawn = 0;
    long ModelMesh::ShadowTrianglesDrawn = 0;
}
