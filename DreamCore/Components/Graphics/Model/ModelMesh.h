#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "Material/Texture.h"

#include "Material/Material.h"
#include "../Shader/ShaderInstance.h"
#include "../Vertex.h"
#include "../BoundingBox.h"
#include "../../../Common/Constants.h"
#include "../../../Common/DreamObject.h"


namespace Dream
{
    class ModelInstance;

    class ModelMesh : public DreamObject
    {
    private:
        ModelInstance* mParent;
        shared_ptr<Material> mMaterial;
        string  mName;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;

        vector<Vertex>  mVertices;
        vector<GLuint> mIndices;
        BoundingBox mBoundingBox;
        vector<SceneObjectRuntime*> mInstances;

    public:
        ModelMesh
        (
            ModelInstance* parent,
            string name,
            vector<Vertex> vertexArray,
            vector<GLuint> indexArray,
            shared_ptr<Material> material
        );

        ~ModelMesh();
        void draw(const shared_ptr<ShaderInstance>&);
        void init();
        void logInstances();
        void addInstance(SceneObjectRuntime* runt);
        void removeInstance(SceneObjectRuntime* runt);

        const shared_ptr<Material>& getMaterial();

        string getName() const;
        void setName(const string& name);
        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);
        vector<Vertex> getVertices() const;
        vector<GLuint> getIndices() const;
        GLuint getVAO() const;
        void drawInstances(ShaderInstance* shader);
    };
}
