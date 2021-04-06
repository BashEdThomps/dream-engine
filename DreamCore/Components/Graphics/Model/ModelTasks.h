#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
  class ModelMesh;
  class MaterialRuntime;

  // ModelInitMeshTask =========================================================

  class ModelInitMeshTask : public GraphicsTask
  {
  public:
    ModelInitMeshTask(ProjectRuntime& pr, ModelMesh& mesh);
    void execute() override;
  private:
    reference_wrapper<ModelMesh> mMesh;
    ModelMesh& getMesh() const;
  };

  // ModelFreeMeshTask =========================================================

  class ModelFreeMeshTask : public GraphicsDestructionTask
  {

  public:
    ModelFreeMeshTask(ProjectRuntime& pr);
    void setBuffers(GLuint vao, GLuint vbo, GLuint ibo);
    void execute() override;
  private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mIBO;
  };
}
