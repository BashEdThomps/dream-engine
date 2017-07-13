#include "AssimpMesh.h"
#include "AssimpModelInstance.h"

namespace Dream
{

    AssimpMesh::AssimpMesh
    (
            AssimpModelInstance* parent,
            vector<Vertex> vertices,
            vector<GLuint> indices,
            vector<Texture> textures,
            aiColor3D diffuse,
            aiColor3D specular
    ) : mParentHandle(parent),
        mVertices(vertices),
        mIndices(indices),
        mTextures(textures),
        mDiffuseColour(diffuse),
        mSpecularColour(specular)

    {
        if (Constants::DEBUG)
        {
            cout << "AssimpMesh: Constructing Mesh for "
                 << parent->getName()
                 << endl;
        }
                init();
    }

    AssimpMesh::~AssimpMesh
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssimpMesh: Destroying Mesh for "
                 << mParentHandle->getNameAndUuidString()
                 << endl;
        }
    }

    void
    AssimpMesh::bindTextures
    (ShaderInstance*)
    {
        GLuint diffuseNr  = 1;
        GLuint specularNr = 1;
        GLuint normalNr   = 1;

        size_t numTextures = mTextures.size();

        for(GLuint i = 0; i < numTextures; i++)
        {

            // Activate proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);

            // Retrieve texture number (the N in diffuse_textureN)
            stringstream materialStr;
            string name = mTextures[i].type;
            materialStr << name << "_";
            GLuint idx = 0;

            if(name == "texture_diffuse")
            {
                idx = diffuseNr++;
                materialStr << idx; // Transfer GLuint to stream
            }
            else if(name == "texture_specular")
            {
                idx = specularNr++;
                materialStr << idx; // Transfer GLuint to stream
            }
            else if (name == "texture_normals")
            {
                idx = normalNr++;
                materialStr << idx;
            }

            if (Constants::DEBUG)
            {
                cout << "AssimpMesh: Binding Material " << materialStr.str()
                     << " at position " << mTextures[i].id
                     << " for " << mParentHandle->getNameAndUuidString()
                     << endl;
            }

            glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    void
    AssimpMesh::bindDiffuse
    (ShaderInstance *shaderHandle)
    {
        shaderHandle->setDiffuseColour(vec3(mDiffuseColour.r,mDiffuseColour.g,mDiffuseColour.b));
    }

    void
    AssimpMesh::bindSpecular
    (ShaderInstance *shaderHandle)
    {
        shaderHandle->setSpecularColour(vec3(mDiffuseColour.r,mDiffuseColour.g,mDiffuseColour.b));
    }

    void
    AssimpMesh::draw
    (ShaderInstance* shader)
    {
        bindTextures(shader);
        bindDiffuse(shader);
        bindSpecular(shader);

        // Draw mesh
        shader->bindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, 0);
        shader->unbindVertexArray();
    }

    void
    AssimpMesh::init
    ()
    {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mEBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertices.size() * sizeof(Vertex)), &mVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(mIndices.size() * sizeof(GLuint)),&mIndices[0], GL_STATIC_DRAW);
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(Vertex)),
                    static_cast<GLvoid*>(0)
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
        glBindVertexArray(0);
    }

} // End of Dream
