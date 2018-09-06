#include "MaterialMeshGlob.h"

#include "../Shader/ShaderInstance.h"
#include "AssimpMesh.h"
#include "AssimpMaterial.h"

namespace Dream
{
    MaterialMeshGlob::MaterialMeshGlob
    (ShaderInstance* shader, AssimpMaterial* material)
        : DreamObject("MaterialMeshGlob"),
          mShader(shader),
          mMaterial(material)
    {
        auto log = getLog();
        log->trace("Constructing");
    }

    MaterialMeshGlob::~MaterialMeshGlob()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    void MaterialMeshGlob::buildGeometry()
    {
        mVertices.clear();
        mIndices.clear();
        /*

        for (AssimpMesh* mesh : mMaterial->mUsedBy)
        {
            auto meshVerts = mesh->getVertices();
            auto meshIndices = mesh->getIndices();

            mVertices.insert(end(mVertices),begin(meshVerts),end(meshVerts));
            mIndices.insert(end(mIndices),begin(meshIndices),end(meshIndices));
        }

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
        glBindVertexArray(0);
        */
    }

    void
    MaterialMeshGlob::draw
    ()
    {
        // Bind Shader Values
        bindTextures();
        bindDiffuse();
        bindSpecular();
        bindOpacity();

        // Sync Uniforms
        mShader->syncUniforms();
        checkGLError();

        // Draw mesh
        mShader->bindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        mShader->unbindVertexArray();
    }

    void
    MaterialMeshGlob::bindTextures
    ()
    {
        auto log = getLog();
        bindTexture(mMaterial->mDiffuseTexture.get());
        bindTexture(mMaterial->mSpecularTexture.get());
        bindTexture(mMaterial->mNormalTexture.get());
        glActiveTexture(GL_TEXTURE0);
    }

    void
    MaterialMeshGlob::bindTexture
    (Texture* t)
    {
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

        log->info(
              "Binding Material {} with GL Texture {} to unit {}",
              name,
              t->id,
              nextTexture
         );

        glActiveTexture(nextTexture);
        glBindTexture(GL_TEXTURE_2D, t->id);
    }

    void
    MaterialMeshGlob::unbindTextures
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
    MaterialMeshGlob::bindDiffuse
    ()
    {
        auto log = getLog();
        aiColor4D diff = mMaterial->mColorDiffuse;
        auto diffuse = vec4(diff.r, diff.g, diff.b, diff.a);
        log->trace("Material Diffuse for {}: ({},{},{})",mMaterial->mName.C_Str(),diff.r, diff.g, diff.b, diff.a);
        mShader->addUniform(FLOAT4,"materialDiffuseColour",1,&diffuse);
    }

    void
    MaterialMeshGlob::bindSpecular
    ()
    {
        auto log = getLog();
        aiColor4D spec = mMaterial->mColorSpecular;
        auto specular = vec4(spec.r, spec.g, spec.b, spec.a);
        log->trace(
            "Material Specular ({},{},{}, {}) strength {}",
            spec.r, spec.g, spec.b, spec.a,
            mMaterial->mShininessStrength
        );
        mShader->addUniform(FLOAT4,"materialSpecularColour",1,&specular);
        mShader->addUniform(FLOAT1,"materialShininess",1,&mMaterial->mShininessStrength);
    }

    void
    MaterialMeshGlob::bindAmbient
    ()
    {
        auto log = getLog();
        aiColor4D amb = mMaterial->mColorAmbient;
        log->trace(
            "Material Ambient for ({},{},{}, {})",
            amb.r, amb.g, amb.b, amb.a
        );
        auto ambient = vec4(amb.r, amb.g, amb.b, amb.a);
        mShader->addUniform(FLOAT4,"materialAmbientColour",1,&ambient);
    }

    void
    MaterialMeshGlob::bindOpacity
    ()
    {
        auto log = getLog();
        log->trace("Material Opacity {}", mMaterial->mOpacity);
        mShader->addUniform(FLOAT1,"materialOpacity",1,&mMaterial->mOpacity);
    }
}

