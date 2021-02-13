#include "ShaderRuntime.h"
#include "ShaderTasks.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace octronic::dream
{
    ShaderCompileFragmentTask::ShaderCompileFragmentTask
    (ProjectRuntime* prt, ShaderRuntime* rt)
        : GraphicsTask(prt,"ShaderCompileFragmentTask"),
          mShaderRuntime(rt)
    {
    }

    void ShaderCompileFragmentTask::execute()
    {
        LOG_TRACE("ShaderCompileFragmentTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        if (!mShaderRuntime->performFragmentCompilation())
        {
            setState(TASK_STATE_FAILED);
        }
        else
        {
            setState(TASK_STATE_COMPLETED);
        }
    }

    ShaderCompileVertexTask::ShaderCompileVertexTask
    (ProjectRuntime* pr, ShaderRuntime* rt)
        : GraphicsTask(pr, "ShaderCompileVertexTask"), mShaderRuntime(rt)
    {
    }

    void ShaderCompileVertexTask::execute()
    {
        LOG_TRACE("ShaderCompileVertexTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        if (!mShaderRuntime->performVertexCompilation())
        {
            setState(TASK_STATE_FAILED);
        }
        else
        {
            setState(TASK_STATE_COMPLETED);
        }
    }

    ShaderLinkTask::ShaderLinkTask
    (ProjectRuntime* pr, ShaderRuntime* rt)
        : GraphicsTask(pr, "ShaderLinkTask"), mShaderRuntime(rt)
    {
    }

    void ShaderLinkTask::execute()
    {
        LOG_TRACE("ShaderLinkTask: Linking Shader [{}] on Graphics thread",mShaderRuntime->getName());

        if (!mShaderRuntime->performLinking())
        {
            setState(TASK_STATE_FAILED);
        }
        else
        {
            setState(TASK_STATE_COMPLETED);
        }
    }

    ShaderFreeTask::ShaderFreeTask(ProjectRuntime* pr)
        : GraphicsDestructionTask(pr, "ShaderFreeTask"),
          mShaderProgram(0)
    {
    }

    void ShaderFreeTask::setShaderProgram(GLuint rt)
    {
        mShaderProgram = rt;
    }

    void ShaderFreeTask::execute()
    {
        LOG_TRACE("ShaderFreeTask: ShaderProgram:{} Executing on Graphics thread",mShaderProgram);
        glDeleteProgram(mShaderProgram);
        setState(TASK_STATE_COMPLETED);
    }
}
