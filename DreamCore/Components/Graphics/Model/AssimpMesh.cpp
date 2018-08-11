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
        vector<Texture> textures,
        AssimpMaterial material
    ) : ILoggable("AssimpMesh"),
        mParentHandle(parent),
        mMaterial(material),
        mName(name),
        mVertices(vertices),
        mIndices(indices),
        mTextures(textures)

    {
        auto log = getLog();
        log->trace("Constructing Mesh for {}", parent->getName());
        init();
    }

    AssimpMesh::~AssimpMesh
    ()
    {
        auto log = getLog();
        log->trace("Destroying Mesh for {}",mParentHandle->getNameAndUuidString());
    }

    string AssimpMesh::getName() const
    {
        return mName;
    }

    void AssimpMesh::setName(const string& name)
    {
        mName = name;
    }

    void
    AssimpMesh::bindTextures
    (ShaderInstance*)
    {
        auto log = getLog();
        GLuint diffuseNr  = 1;
        GLuint specularNr = 1;
        GLuint normalNr   = 1;

        size_t numTextures = mTextures.size();

        for(GLuint i = 0; i < numTextures; i++)
        {
            // Activate proper texture unit before binding
            GLuint nextTexture = GL_TEXTURE0 + i;
            glActiveTexture(nextTexture);
            mTexturesInUse.push_back(nextTexture);
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

            log->info(
                  "Binding Material {} with GL Texture {} to unit {} for {} in {}",
                  materialStr.str(),
                  mTextures[i].id,
                  nextTexture,
                  getName(),
                  mParentHandle->getNameAndUuidString()
             );

            glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    void
    AssimpMesh::unbindTextures
    ()
    {
        for (GLuint texture : mTexturesInUse)
        {
            glActiveTexture(texture);
            glBindTexture(GL_TEXTURE_2D,0);
        }
        mTexturesInUse.clear();
    }

    void
    AssimpMesh::bindDiffuse
    (ShaderInstance *shaderHandle)
    {
        aiColor3D diff = mMaterial.mColorDiffuse;
        auto diffuse = vec3(diff.r, diff.g, diff.b);
        shaderHandle->addUniform(ShaderUniform(FLOAT3,"materialDiffuseColour",1,glm::value_ptr(diffuse)));
        shaderHandle->addUniform(ShaderUniform(FLOAT1,"materialDiffuseStrength",1,&mMaterial.mRefracti));
    }

    void
    AssimpMesh::bindSpecular
    (ShaderInstance *shaderHandle)
    {
        aiColor3D spec = mMaterial.mColorSpecular;
        auto specular = vec3(spec.r, spec.g, spec.b);
        shaderHandle->addUniform(ShaderUniform(FLOAT3,"materialSpecularColour",1,glm::value_ptr(specular)));
        shaderHandle->addUniform(ShaderUniform(FLOAT1,"materialSpecularStrength",1,&mMaterial.mShininess));
    }

    void
    AssimpMesh::bindAmbient
    (ShaderInstance *shaderHandle)
    {
        aiColor3D amb = mMaterial.mColorAmbient;
        auto ambient = vec3(amb.r, amb.g, amb.b);
        shaderHandle->addUniform(ShaderUniform(FLOAT3,"materialAmbientColour",1,glm::value_ptr(ambient)));
        shaderHandle->addUniform(ShaderUniform(FLOAT1,"materialAmbientStrength",1,&mMaterial.mOpacity));
    }

    void
    AssimpMesh::draw
    (ShaderInstance* shader)
    {
        // Bind Shader Values
        bindTextures(shader);
        bindDiffuse(shader);
        bindSpecular(shader);

        // Sync Uniforms
        shader->syncUniforms();
        Constants::checkGLError("After sync uniforms");

        // Draw mesh
        shader->bindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        shader->unbindVertexArray();
        //unbindTextures();
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
    }

} // End of Dream
