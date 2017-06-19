#include "AssimpMesh.h"
#include "AssimpModelInstance.h"

namespace Dream
{

    AssimpMesh::AssimpMesh
    (
            AssimpModelInstance* parent,
            vector<Vertex> vertices,
            vector<GLuint> indices,
            vector<Texture> textures
    )
    {
        if (Constants::DEBUG)
        {
            cout << "AssimpMesh: Constructing Mesh for "
                 << parent->getName()
                 << endl;
        }
        mParentHandle = parent;
        mVertices = vertices;
        mIndices  = indices;
        mTextures = textures;
        init();
    }

    AssimpMesh::~AssimpMesh
    ()
    {
        /*
        if (Constants::DEBUG)
        {
            cout << "AssimpMesh: Destroying Mesh for "
                 << mParentHandle->getName()
                 << endl;
        }
        */
    }

    void
    AssimpMesh::draw
    (ShaderInstance* shader)
    {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        size_t numTextures = mTextures.size();
        for(GLuint i = 0; i < numTextures; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            stringstream materialStr;
            string number;
            string name = mTextures[i].type;
            materialStr << "material." << name;

            if(name == "texture_diffuse")
            {
                materialStr << diffuseNr++; // Transfer GLuint to stream
            }
            else if(name == "texture_specular")
            {
                materialStr << specularNr++; // Transfer GLuint to stream
            }
            materialStr << number;
            glUniform1f(glGetUniformLocation(shader->getShaderProgram(), materialStr.str().c_str()), i);
            glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // Draw mesh
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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