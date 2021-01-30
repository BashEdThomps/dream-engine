#include "ModelTasks.h"

#include "ModelMesh.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    ModelInitMeshTask::ModelInitMeshTask(ModelMesh* mesh)
        : GraphicsComponentTask("ModelMeshInitTask"),
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

    ModelFreeMeshTask::ModelFreeMeshTask() :
        GraphicsComponentDestructionTask ("ModelMeshFreeTask")
    {
    }

    void ModelFreeMeshTask::setBuffers(GLuint vao, GLuint vbo, GLuint ibo)
    {
        mVAO = vao;
        mVBO = vbo;
        mIBO = ibo;
    }

    void ModelFreeMeshTask::execute()
    {
        LOG_TRACE("ModelTasks: Executing on thread {}",mThreadId);

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
