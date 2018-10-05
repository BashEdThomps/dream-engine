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
    AssimpMesh::bindTextures
    (const shared_ptr<ShaderInstance>&)
    {
        auto log = getLog();
        bindTexture(mMaterial->mDiffuseTexture);
        bindTexture(mMaterial->mSpecularTexture);
        bindTexture(mMaterial->mNormalTexture);
        glActiveTexture(GL_TEXTURE0);
    }

    void AssimpMesh::bindTexture(const shared_ptr<Texture>& t)
    {
        if (t == nullptr)
        {
            return;
        }

        auto log = getLog();
        string name = t->type;
        GLuint nextTexture = 0;

        if (t->id == 0)
        {
            return;
        }

        if(name == "texture_diffuse")
        {
            // Activate proper texture unit before binding
            log->critical("Binding diffuse texture");
            nextTexture = GL_TEXTURE0;
        }
        else if(name == "texture_specular")
        {
            // Activate proper texture unit before binding
            log->critical("Binding specular texture");
            nextTexture = GL_TEXTURE1;
        }
        else if (name == "texture_normals")
        {
            // Activate proper texture unit before binding
            log->critical("Binding normals texture");
            nextTexture = GL_TEXTURE2;
        }

        log->debug(
              "Binding Material {} with GL Texture {} to unit {} for {} in {}",
              name,
              t->id,
              nextTexture,
              getName(),
              mParent->getNameAndUuidString()
         );

        glActiveTexture(nextTexture);
        glBindTexture(GL_TEXTURE_2D, t->id);
    }

    void
    AssimpMesh::unbindTextures
    ()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

    }

    void
    AssimpMesh::bindDiffuse
    (const shared_ptr<ShaderInstance>& shader)
    {
        auto log = getLog();
        aiColor4D diff = mMaterial->mColorDiffuse;
        auto diffuse = vec4(diff.r, diff.g, diff.b, diff.a);
        log->trace("Material Diffuse for {}: ({},{},{},{})",getName(),diff.r, diff.g, diff.b, diff.a);
        shader->addUniform(FLOAT4,"materialDiffuseColour",1,&diffuse);
    }

    void
    AssimpMesh::bindSpecular
    (const shared_ptr<ShaderInstance>& shader)
    {
        auto log = getLog();
        aiColor4D spec = mMaterial->mColorSpecular;
        auto specular = vec4(spec.r, spec.g, spec.b, spec.a);
        log->trace(
            "Material Specular for {}: ({},{},{},{}) strength {}",
            getName(),
            spec.r, spec.g, spec.b, spec.a,
            mMaterial->mShininessStrength
        );
        shader->addUniform(FLOAT4,"materialSpecularColour",1,&specular);
        shader->addUniform(FLOAT1,"materialShininess",1,&mMaterial->mShininessStrength);
    }

    void
    AssimpMesh::bindAmbient
    (const shared_ptr<ShaderInstance>& shader)
    {
        auto log = getLog();
        aiColor4D amb = mMaterial->mColorAmbient;
        log->trace(
            "Material Ambient for {}: ({},{},{},{})",
            getName(),
            amb.r, amb.g, amb.b, amb.a
        );
        auto ambient = vec4(amb.r, amb.g, amb.b, amb.a);
        shader->addUniform(FLOAT4,"materialAmbientColour",1,&ambient);
    }

    void
    AssimpMesh::bindOpacity
    (const shared_ptr<ShaderInstance>& shader)
    {
        shader->addUniform(FLOAT1,"materialOpacity",1,&mMaterial->mOpacity);
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
} // End of Dream
