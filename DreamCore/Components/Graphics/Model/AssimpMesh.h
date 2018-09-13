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
#include "../../../Common/DreamObject.h"


namespace Dream
{
    class AssimpModelInstance;

    class AssimpMesh : public DreamObject
    {
    private:
        weak_ptr<AssimpModelInstance> mParent;
        weak_ptr<AssimpMaterial> mMaterial;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;

        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        BoundingBox mBoundingBox;

        void bindTextures(weak_ptr<ShaderInstance>);
        void bindTexture(weak_ptr<Texture> material);
        void unbindTextures();
        void bindDiffuse(weak_ptr<ShaderInstance>);
        void bindSpecular(weak_ptr<ShaderInstance>);
        void bindAmbient(weak_ptr<ShaderInstance> shader);
        void bindOpacity(weak_ptr<ShaderInstance> shader);

    public:
        AssimpMesh
        (
            weak_ptr<AssimpModelInstance> parent,
            string name,
            vector<Vertex> vertexArray,
            vector<GLuint> indexArray,
            weak_ptr<AssimpMaterial> material
        );

        ~AssimpMesh();
        void draw(weak_ptr<ShaderInstance>);
        void init();

        string getName() const;
        void setName(const string& name);
        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
    };
}
