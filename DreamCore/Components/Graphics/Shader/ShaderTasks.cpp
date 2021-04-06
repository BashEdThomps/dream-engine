#include "ShaderRuntime.h"
#include "ShaderTasks.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace octronic::dream
{
  // ShaderRuntimeTask =========================================================
  ShaderRuntimeTask::ShaderRuntimeTask
  (ProjectRuntime& pr, ShaderRuntime& sr, const string& name)
    : GraphicsTask(pr,name),
      mShaderRuntime(sr)
  {

  }

  ShaderRuntime&
  ShaderRuntimeTask::getShaderRuntime
  ()
  const
  {
    return mShaderRuntime.get();
  }

  // ShaderCompileFragmentTask =================================================

  ShaderCompileFragmentTask::ShaderCompileFragmentTask
  (ProjectRuntime& prt, ShaderRuntime& rt)
    : ShaderRuntimeTask(prt,rt,"ShaderCompileFragmentTask")
  {
  }

  void
  ShaderCompileFragmentTask::execute
  ()
  {
    LOG_TRACE("ShaderCompileFragmentTask: {} Executing on Graphics thread",getShaderRuntime().getName());
    if (!getShaderRuntime().performFragmentCompilation())
    {
      setState(TASK_STATE_DEFERRED);
    }
    else
    {
      setState(TASK_STATE_COMPLETED);
    }
  }

  // ShaderCompileVertexTask ===================================================

  ShaderCompileVertexTask::ShaderCompileVertexTask
  (ProjectRuntime& pr, ShaderRuntime& rt)
    : ShaderRuntimeTask(pr,rt, "ShaderCompileVertexTask")
  {
  }

  void
  ShaderCompileVertexTask::execute
  ()
  {
    LOG_TRACE("ShaderCompileVertexTask: {} Executing on Graphics thread",getShaderRuntime().getName());
    if (!getShaderRuntime().performVertexCompilation())
    {
      setState(TASK_STATE_DEFERRED);
    }
    else
    {
      setState(TASK_STATE_COMPLETED);
    }
  }

  // ShaderLinkTask ============================================================

  ShaderLinkTask::ShaderLinkTask
  (ProjectRuntime& pr,ShaderRuntime& rt)
    : ShaderRuntimeTask(pr, rt,"ShaderLinkTask")
  {
  }

  void
  ShaderLinkTask::execute
  ()
  {
    LOG_TRACE("ShaderLinkTask: Linking Shader [{}] on Graphics thread",getShaderRuntime().getName());

    if (!getShaderRuntime().performLinking())
    {
      setState(TASK_STATE_DEFERRED);
    }
    else
    {
      setState(TASK_STATE_COMPLETED);
    }
  }

  // ShaderFreeTask ============================================================

  ShaderFreeTask::ShaderFreeTask
  (ProjectRuntime& pr)
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
