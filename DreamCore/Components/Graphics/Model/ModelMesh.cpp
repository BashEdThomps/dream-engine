#include "ModelMesh.h"
#include "ModelInstance.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{

    GLuint ModelMesh::getVAO() const
    {
        return mVAO;
    }

    void
    ModelMesh::drawInstances
    (ShaderInstance* shader)
    {
        shader->bindVertexArray(mVAO);
        shader->bindInstances(mInstances);
        shader->syncUniforms();
        auto size = static_cast<GLsizei>(mInstances.size());
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr,size);
    }

    ModelMesh::ModelMesh
    (
        ModelInstance* parent,
        string name,
        vector<Vertex> vertices,
        vector<GLuint> indices,
        shared_ptr<Material> material
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

    BoundingBox
    ModelMesh::getBoundingBox
    () const
    {
        return mBoundingBox;
    }

    void
    ModelMesh::setBoundingBox
    (const BoundingBox& boundingBox)
    {
        mBoundingBox = boundingBox;
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
    ModelMesh::draw
    (const shared_ptr<ShaderInstance>& shader)
    {
        auto log = getLog();

        // Bind Material
        shader->bindMaterial(mMaterial);

        // Sync Uniforms
        shader->syncUniforms();
        checkGLError();

        // Bind VAO
        shader->bindVertexArray(mVAO);

        // Draw mesh
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr);

        log->debug("Completed a mesh draw");
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

    const shared_ptr<Material>&
    ModelMesh::getMaterial
    ()
    {
       return mMaterial;
    }

} // End of Dream
