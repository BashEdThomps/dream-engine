#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class ModelMesh;
    class MaterialRuntime;

    class ModelInitMeshTask : public GraphicsTask
    {
        ModelMesh* mMesh;
    public:
        ModelInitMeshTask(ProjectRuntime* pr, ModelMesh* mesh);
        void execute() override;
    };

    class ModelFreeMeshTask : public GraphicsDestructionTask
    {
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
    public:
       ModelFreeMeshTask(ProjectRuntime* pr);
       void setBuffers(GLuint vao, GLuint vbo, GLuint ibo);
       void execute() override;
    };
}
