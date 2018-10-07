#include "AssimpMesh.h"
#include "AssimpModelInstance.h"

namespace Dream
{

    AssimpMesh::AssimpMesh
    (
        AssimpModelInstance* parent,
        string name,
        vector<Vertex> vertices,
        vector<GLuint> indices,
        shared_ptr<AssimpMaterial> material
    ) : DreamObject("AssimpMesh"),
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

    AssimpMesh::~AssimpMesh
    ()
    {
        auto log = getLog();
        log->trace("Destroying Mesh for {}",mParent->getNameAndUuidString());
    }

    string AssimpMesh::getName() const
    {
        return mName;
    }

    void AssimpMesh::setName(const string& name)
    {
        mName = name;
    }

    BoundingBox AssimpMesh::getBoundingBox() const
    {
        return mBoundingBox;
    }

    void AssimpMesh::setBoundingBox(const BoundingBox& boundingBox)
    {
        mBoundingBox = boundingBox;
    }

    vector<Vertex> AssimpMesh::getVertices() const
    {
        return mVertices;
    }

    vector<GLuint> AssimpMesh::getIndices() const
    {
        return mIndices;
    }

    void
    AssimpMesh::draw
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
    AssimpMesh::init
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

    const shared_ptr<AssimpMaterial>&
    AssimpMesh::getMaterial
    ()
    {
       return mMaterial;
    }
} // End of Dream
