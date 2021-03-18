#include "ShaderRuntime.h"
#include "ShaderTasks.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace octronic::dream
{
  ShaderCompileFragmentTask::ShaderCompileFragmentTask
  (const weak_ptr<ProjectRuntime>& prt,
   const weak_ptr<ShaderRuntime>& rt)
    : GraphicsTask(prt,"ShaderCompileFragmentTask"),
      mShaderRuntime(rt)
  {
  }

  void
  ShaderCompileFragmentTask::execute
  ()
  {
    if (auto shLock = mShaderRuntime.lock())
    {
      LOG_TRACE("ShaderCompileFragmentTask: {} Executing on Graphics thread",shLock->getName());
      if (!shLock->performFragmentCompilation())
      {
        setState(TASK_STATE_DEFERRED);
      }
      else
      {
        setState(TASK_STATE_COMPLETED);
      }
    }
  }

  ShaderCompileVertexTask::ShaderCompileVertexTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<ShaderRuntime>& rt)
    : GraphicsTask(pr, "ShaderCompileVertexTask"),
      mShaderRuntime(rt)
  {
  }

  void
  ShaderCompileVertexTask::execute
  ()
  {
    if (auto shLock = mShaderRuntime.lock())
    {
      LOG_TRACE("ShaderCompileVertexTask: {} Executing on Graphics thread",shLock->getName());
      if (!shLock->performVertexCompilation())
      {
        setState(TASK_STATE_DEFERRED);
      }
      else
      {
        setState(TASK_STATE_COMPLETED);
      }
    }
  }

  ShaderLinkTask::ShaderLinkTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<ShaderRuntime>& rt)
    : GraphicsTask(pr, "ShaderLinkTask"),
      mShaderRuntime(rt)
  {
  }

  void
  ShaderLinkTask::execute
  ()
  {
    if (auto shLock = mShaderRuntime.lock())
    {
      LOG_TRACE("ShaderLinkTask: Linking Shader [{}] on Graphics thread",shLock->getName());

      if (!shLock->performLinking())
      {
        setState(TASK_STATE_DEFERRED);
      }
      else
      {
        setState(TASK_STATE_COMPLETED);
      }
    }
  }

  ShaderFreeTask::ShaderFreeTask
  (const weak_ptr<ProjectRuntime>& pr)
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
