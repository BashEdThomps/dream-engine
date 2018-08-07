#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "Texture.h"

#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"
#include "../../../Common/Constants.h"
#include "../../../Common/ILoggable.h"


namespace Dream
{
    class AssimpModelInstance;
    class AssimpMesh : public ILoggable
    {
    private:
        AssimpModelInstance* mParentHandle;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mEBO;


        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        vector<Texture> mTextures;
        vector<GLuint>  mTexturesInUse;

        aiColor3D mDiffuseColour;
        aiColor3D mSpecularColour;


        void bindTextures(ShaderInstance*);
        void unbindTextures();
        void bindDiffuse(ShaderInstance*);
        void bindSpecular(ShaderInstance*);

    public:
        AssimpMesh
        (
                AssimpModelInstance* parent,
                string name,
                vector<Vertex> vertexArray,
                vector<GLuint> indexArray,
                vector<Texture> textureArray,
                aiColor3D diffuse = aiColor3D(0.0f),
                aiColor3D specular = aiColor3D(0.0f)
        );
        ~AssimpMesh();
        void draw(ShaderInstance*);
        void init();
        string getName() const;
        void setName(const string& name);
    };
}
