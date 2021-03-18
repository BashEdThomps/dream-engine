#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class ModelMesh;
    class MaterialRuntime;

    class ModelInitMeshTask : public GraphicsTask
    {
        weak_ptr<ModelMesh> mMesh;
    public:
        ModelInitMeshTask(
            const weak_ptr<ProjectRuntime>& pr,
            const weak_ptr<ModelMesh>& mesh);
        void execute() override;
    };

    class ModelFreeMeshTask : public GraphicsDestructionTask
    {
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
    public:
       ModelFreeMeshTask(const weak_ptr<ProjectRuntime>& pr);
       void setBuffers(GLuint vao, GLuint vbo, GLuint ibo);
       void execute() override;
    };
}
