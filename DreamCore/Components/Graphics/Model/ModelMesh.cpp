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

#include "ModelInstance.h"
#include "../Shader/ShaderInstance.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Camera.h"

namespace Dream
{

    ModelMesh::ModelMesh
    (ModelInstance* parent,
        string name,
        vector<Vertex> vertices,
        vector<GLuint> indices,
        MaterialInstance* material
    ) : DreamObject("ModelMesh"),
        mParent(parent),
        mMaterial(material),
        mName(name),
        mVertices(vertices),
        mIndices(indices)
    {
        auto log = getLog();
        log->trace("Constructing Mesh for {}", parent->getName());
        init();
    }

    ModelMesh::~ModelMesh
    ()
    {
        auto log = getLog();
        log->trace("Destroying Mesh for {}",mParent->getNameAndUuidString());
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

    void
    ModelMesh::addInstance
    (SceneObjectRuntime* runt)
    {
        auto log = getLog();
        log->debug("Adding instance of mesh for {}", runt->getNameAndUuidString());
        mInstances.push_back(runt);
    }

    void
    ModelMesh::removeInstance
    (SceneObjectRuntime* runt)
    {
       auto log = getLog();
       log->debug("Removing instance of mesh for {}", runt->getNameAndUuidString());
       auto itr = find (mInstances.begin(), mInstances.end(), runt);
       if (itr != mInstances.end())
       {
           mInstances.erase(itr);
       }
    }

    MaterialInstance*
    ModelMesh::getMaterial
    ()
    {
       return mMaterial;
    }



    GLuint ModelMesh::getVAO() const
    {
        return mVAO;
    }

    void
    ModelMesh::drawInstances
    (Camera* camera, ShaderInstance* shader)
    {
        auto log = getLog();
        vector<SceneObjectRuntime*> inFrustum;
        for (int i=0; i<mInstances.size();i++)
        {
            auto sor = mInstances.at(i);
            if(camera->inFrustum(sor))
            {
                inFrustum.push_back(sor);
            }
        }
        if (inFrustum.empty())
        {
            log->error("No instances of {} in Frustum", getName());
            return;
        }
        log->error("Drawing {} instances of mesh {}", mInstances.size(), getName());
        shader->bindVertexArray(mVAO);
        shader->bindInstances(inFrustum);
        shader->syncUniforms();
        auto size = static_cast<GLsizei>(inFrustum.size());
        InstancesDrawn += size;
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
        DrawCalls++;
    }

    long ModelMesh::DrawCalls = 0;
    long ModelMesh::InstancesDrawn = 0;

} // End of Dream
