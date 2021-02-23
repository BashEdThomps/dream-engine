#include "ModelTasks.h"

#include "ModelMesh.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    ModelInitMeshTask::ModelInitMeshTask(ProjectRuntime* pr, ModelMesh* mesh)
        : GraphicsTask(pr,"ModelMeshInitTask"),
          mMesh(mesh)
    {
    }

    void ModelInitMeshTask::execute()
    {
        LOG_TRACE("ModelInitMeshTask: for mesh: {}, Executing on Graphics thread",mMesh->getName());
        if (mMesh->loadIntoGL())
        {
           setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
           setState(TaskState::TASK_STATE_FAILED);
        }
    }

    ModelFreeMeshTask::ModelFreeMeshTask(ProjectRuntime* pr) :
        GraphicsDestructionTask (pr, "ModelMeshFreeTask")
    {
    }

    void ModelFreeMeshTask::setBuffers(GLuint vao, GLuint vbo, GLuint ibo)
    {
        mVAO = vao;
        mVBO = vbo;
        mIBO = ibo;
    }

    void ModelFreeMeshTask::setMaterialRuntime(MaterialRuntime* m)
    {
       mMaterial = m;
    }

    void ModelFreeMeshTask::setMeshRemovedPointer(ModelMesh* mesh)
    {
        mMeshRemoved = mesh;
    }

    void ModelFreeMeshTask::execute()
    {
        LOG_TRACE("ModelTasks: Executing {}",getID());

        if (mMaterial != nullptr)
        {
            mMaterial->removeMesh(mMeshRemoved);
        }

        if (mVAO > 0)
        {
            glDeleteVertexArrays(1,&mVAO);
        }

        if (mVBO > 0)
        {
            glDeleteBuffers(1,&mVBO);
        }

        if (mIBO > 0)
        {
            glDeleteBuffers(1,&mIBO);
        }
        setState(TaskState::TASK_STATE_COMPLETED);
    }
}
