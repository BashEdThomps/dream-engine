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
#include "../Shader/ShaderRuntime.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Camera.h"

namespace Dream
{

    ModelMesh::ModelMesh
    (ModelRuntime* parent,
        string name,
        vector<Vertex> vertices,
        vector<GLuint> indices,
        MaterialRuntime* material
    ) : DreamObject("ModelMesh"),
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
    ModelMesh::init
    ()
    {
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
    }

#ifdef DREAM_LOG
    void
    ModelMesh::logInstances
    ()
    {
       auto log = getLog();
       for (auto instance : mInstances)
       {
           log->debug("\t\t\tInstance for {}", instance->getNameAndUuidString());
       }
    }
#endif

    void
    ModelMesh::addInstance
    (SceneObjectRuntime* runt)
    {
#ifdef DREAM_LOG
        getLog()->debug("Adding instance of mesh for {}", runt->getNameAndUuidString());
#endif
        mInstances.push_back(runt);
    }

    void
    ModelMesh::removeInstance
    (SceneObjectRuntime* runt)
    {
#ifdef DREAM_LOG
       auto log = getLog();
       log->debug("Removing instance of mesh for {}", runt->getNameAndUuidString());
#endif
       auto itr = find (mInstances.begin(), mInstances.end(), runt);
       if (itr != mInstances.end())
       {
           mInstances.erase(itr);
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
    ModelMesh::drawGeometryPassInstances
    (Camera* camera, ShaderRuntime* shader)
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        mInstancesInFrustum.clear();
        for (size_t i=0; i<mInstances.size();i++)
        {
            auto sor = mInstances.at(i);
            if(camera->visibleInFrustum(sor))
            {
                mInstancesInFrustum.push_back(sor);
            }
        }
        if (mInstancesInFrustum.empty())
        {
#ifdef DREAM_LOG
            log->debug("(Geometry) No instances of {} in Frustum", getName());
#endif
            return;
        }
#ifdef DREAM_LOG
        log->trace("(Geometry) Drawing {} instances of mesh {} for Geometry pass", mInstances.size(), getName());
#endif
        shader->bindVertexArray(mVAO);
        shader->bindInstances(mInstancesInFrustum);
        auto size = static_cast<GLsizei>(mInstancesInFrustum.size());
        InstancesDrawn += size;
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
        DrawCalls++;
    }

    void
    ModelMesh::drawShadowPassInstances
    (ShaderRuntime* shader)
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        if (mInstances.empty())
        {
#ifdef DREAM_LOG
            log->debug("(Shadow) No instances of {} in Frustum", getName());
#endif
            return;
        }
#ifdef DREAM_LOG
        log->trace("(Shadow) Drawing {} instances of mesh {}", mInstances.size(), getName());
#endif
        shader->bindVertexArray(mVAO);
        shader->bindInstances(mInstances);
        auto size = static_cast<GLsizei>(mInstances.size());
        ShadowInstancesDrawn += size;
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
        ShadowDrawCalls++;
    }

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::InstancesDrawn = 0;
    long ModelMesh::ShadowDrawCalls = 0;
    long ModelMesh::ShadowInstancesDrawn = 0;
}
