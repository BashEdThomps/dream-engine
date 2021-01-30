#include "ShaderRuntime.h"
#include "ShaderTasks.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace octronic::dream
{

    ShaderCompileFragmentTask::ShaderCompileFragmentTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderCompileFragmentTask"), mShaderRuntime(rt)
    {
    }

    void ShaderCompileFragmentTask::execute()
    {
        LOG_TRACE("ShaderCompileFragmentTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        if (!mShaderRuntime->tryLock())
        {
        	if (!mShaderRuntime->performFragmentCompilation())
            {
                setState(TASK_STATE_FAILED);
            }
            else
            {
				setState(TASK_STATE_COMPLETED);
            }
			mShaderRuntime->unlock();
        }
        else
        {
        	setState(TASK_STATE_FAILED);
        }
    }

    ShaderCompileVertexTask::ShaderCompileVertexTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderCompileVertexTask"), mShaderRuntime(rt)
    {
    }

    void ShaderCompileVertexTask::execute()
    {
        LOG_TRACE("ShaderCompileVertexTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        if (!mShaderRuntime->tryLock())
        {
            if (!mShaderRuntime->performVertexCompilation())
            {
                setState(TASK_STATE_FAILED);
            }
            else
            {
				setState(TASK_STATE_COMPLETED);
            }
			mShaderRuntime->unlock();
        }
        else
        {
        	setState(TASK_STATE_FAILED);
        }

    }

    ShaderLinkTask::ShaderLinkTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderLinkTask"), mShaderRuntime(rt)
    {
    }

    void ShaderLinkTask::execute()
    {
        LOG_TRACE("ShaderLinkTask: Linking Shader [{}] on Graphics thread",mShaderRuntime->getName());

        if (!mShaderRuntime->tryLock())
        {
            if (!mShaderRuntime->performLinking())
            {
                setState(TASK_STATE_FAILED);
            }
            else
            {
				setState(TASK_STATE_COMPLETED);
            }
			mShaderRuntime->unlock();
        }
        else
        {
        	setState(TASK_STATE_FAILED);
        }
    }

    ShaderFreeTask::ShaderFreeTask()
        : GraphicsComponentDestructionTask("ShaderFreeTask"),
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
