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
          mLoaded(false),
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
        if (mMaterial != nullptr)
        {
            mMaterial->removeMesh(this);
        }
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
    ModelMesh::drawModelRuntimes
    (CameraRuntime* camera, ShaderRuntime* shader)
    {
        // TODO - Move this to grpahics component
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
        shader->syncUniforms();
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
		shader->syncUniforms();

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
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertices.size() * sizeof(Vertex)), &mVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(mIndices.size() * sizeof(GLuint)),&mIndices[0], GL_STATIC_DRAW);

        /*
        	glVertexAttribPointer
			index, size, type, is_normalized, stride, offset
        */
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              static_cast<GLint>(sizeof(Vertex)),
                              (GLvoid*)offsetof(Vertex,Position));

        // Vertex Texture Coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              static_cast<GLint>(sizeof(Vertex)),
                              (GLvoid*)offsetof(Vertex, TexCoords));
        // Vertex Normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                              static_cast<GLint>(sizeof(Vertex)),
                              (GLvoid*)offsetof(Vertex, Normal));

        glBindVertexArray(0);

        clearVertices();
        clearIndices();

        mLoaded = true;

        return true;
    }

    // renders (and builds at first invocation) a sphere
    // -------------------------------------------------
    void ModelMesh::renderDebugSphere(ShaderRuntime* shader)
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

        shader->syncUniforms();
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
