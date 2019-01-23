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

    ModelMesh::ModelMesh
    (ModelRuntime* parent, string name, vector<Vertex> vertices,
     vector<GLuint> indices,MaterialRuntime* material)
        : DreamObject("ModelMesh"),
        mParent(parent),
        mMaterial(material),
        mName(name),
        mVertices(vertices),
        mIndices(indices)
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
        /*
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mIBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertices.size() * sizeof(Vertex)), &mVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(mIndices.size() * sizeof(GLuint)),&mIndices[0], GL_STATIC_DRAW);
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    static_cast<GLvoid*>(nullptr)
                    );
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                    1, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    (GLvoid*)offsetof(Vertex, Normal)
                    );
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
                    2, 2, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    (GLvoid*)offsetof(Vertex, TexCoords)
                    );
        // Vertex Tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
                    3, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    (GLvoid*)offsetof(Vertex, Tangent)
                    );
        // Vertex Bitangents
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(
                    4, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    (GLvoid*)offsetof(Vertex, Bitangent)
                    );
        glBindVertexArray(0);
        */
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
            auto sor = mRuntimes.at(i);
            if(camera->visibleInFrustum(sor))
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
        auto size = static_cast<GLsizei>(mRuntimesInFrustum.size());
        RuntimesDrawn += size;
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
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
        auto size = static_cast<GLsizei>(inFrustumOnly ? mRuntimesInFrustum.size() : mRuntimes.size());
        ShadowRuntimesDrawn += size;
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
        ShadowDrawCalls++;
    }

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::RuntimesDrawn = 0;
    long ModelMesh::ShadowDrawCalls = 0;
    long ModelMesh::ShadowRuntimesDrawn = 0;
}
