#pragma once

#include "../../../Common/DreamObject.h"
#include <vector>
#include "Texture.h"
#include "../Vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

using std::vector;

namespace Dream
{
    class ShaderInstance;
    class AssimpMaterial;

    class MaterialMeshGlob : public DreamObject
    {
    public:
        MaterialMeshGlob(ShaderInstance*, AssimpMaterial*);
        ~MaterialMeshGlob() override;

        ShaderInstance* mShader;
        AssimpMaterial* mMaterial;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mEBO;

        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;

        void buildGeometry();
        void bindTextures();
        void bindTexture(Texture* t);
        void bindDiffuse();
        void bindSpecular();
        void bindOpacity();
        void bindAmbient();
        void draw();
        void unbindTextures();
    };
}
