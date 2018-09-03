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
        shared_ptr<AssimpModelInstance> mParent;
        shared_ptr<AssimpMaterial> mMaterial;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;

        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        BoundingBox mBoundingBox;

        void bindTextures(shared_ptr<ShaderInstance>);
        void bindTexture(shared_ptr<Texture> material);
        void unbindTextures();
        void bindDiffuse(shared_ptr<ShaderInstance>);
        void bindSpecular(shared_ptr<ShaderInstance>);
        void bindAmbient(shared_ptr<ShaderInstance> shader);
        void bindOpacity(shared_ptr<ShaderInstance> shader);

    public:
        AssimpMesh
        (
            shared_ptr<AssimpModelInstance> parent,
            string name,
            vector<Vertex> vertexArray,
            vector<GLuint> indexArray,
            shared_ptr<AssimpMaterial> material
        );

        ~AssimpMesh();
        void draw(shared_ptr<ShaderInstance>);
        void init();

        string getName() const;
        void setName(const string& name);
        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
    };
}
