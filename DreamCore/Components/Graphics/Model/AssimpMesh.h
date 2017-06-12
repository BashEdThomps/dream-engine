#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "../../../Constants.h"
#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"

#include "Texture.h"

namespace Dream
{
    class AssimpModelInstance;
    class AssimpMesh
    {
    private:
        GLuint mVAO;
        GLuint mVBO;
        GLuint mEBO;
        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        vector<Texture> mTextures;
        AssimpModelInstance* mParentHandle;
    public:
        AssimpMesh
        (
                AssimpModelInstance*,
                vector<Vertex>,
                vector<GLuint>,
                vector<Texture>
                );
        ~AssimpMesh();
        void draw(ShaderInstance*);
        void init();
    };
}
