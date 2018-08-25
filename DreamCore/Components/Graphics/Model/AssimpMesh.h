#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "Texture.h"

#include "AssimpMaterial.h"
#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"
#include "../BoundingBox.h"
#include "../../../Common/Constants.h"
#include "../../../Common/ILoggable.h"


namespace Dream
{
    class AssimpModelInstance;

    class AssimpMesh : public ILoggable
    {
    private:
        AssimpModelInstance* mParentHandle;
        AssimpMaterial* mMaterialHandle;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;

        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        BoundingBox mBoundingBox;

        void bindTextures(ShaderInstance*);
        void bindTexture(Texture* material);
        void unbindTextures();
        void bindDiffuse(ShaderInstance*);
        void bindSpecular(ShaderInstance*);
        void bindAmbient(ShaderInstance* shaderHandle);
        void bindOpacity(ShaderInstance* shaderHandle);

    public:
        AssimpMesh
        (
            AssimpModelInstance* parent,
            string name,
            vector<Vertex> vertexArray,
            vector<GLuint> indexArray,
            AssimpMaterial* material
        );

        ~AssimpMesh();
        void draw(ShaderInstance*);
        void init();

        string getName() const;
        void setName(const string& name);
        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
    };
}
