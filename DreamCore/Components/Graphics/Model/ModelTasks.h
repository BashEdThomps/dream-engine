#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTask.h"

namespace Dream
{
    class ModelMesh;

    class ModelInitMeshTask : public GraphicsComponentTask
    {
        ModelMesh* mMesh;
    public:
        ModelInitMeshTask(ModelMesh* mesh);
        void execute() override;
    };

    class ModelFreeMeshTask : public GraphicsComponentDestructionTask
    {
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
    public:
       ModelFreeMeshTask();
       void setBuffers(GLuint vao, GLuint vbo, GLuint ibo);
       void execute() override;
    };
}
