#include "GraphicsComponentTasks.h"

#include "GraphicsComponent.h"
#include "Project/ProjectRuntime.h"

#include <sstream>
using std::stringstream;

namespace octronic::dream
{
  // GraphicsComponentTask ===================================================

  GraphicsTask::GraphicsTask
  (const weak_ptr<ProjectRuntime>& pr, const string& taskName)
    : Task(pr, taskName)
  {

  }

  // GraphicsComponentDestructionTask ========================================

  GraphicsDestructionTask::GraphicsDestructionTask
  (const weak_ptr<ProjectRuntime>& pr, const string& taskName)
    : DestructionTask(pr, taskName)
  {

  }

  // SetupBuffersTask ========================================================

  SetupBuffersTask::SetupBuffersTask
  (const weak_ptr<ProjectRuntime>& pr)
    : GraphicsTask(pr, "SetupBuffersTask")
  {
  }

  void SetupBuffersTask::execute()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto graphicsComponent = prLock->getGraphicsComponent().lock())
      {
        if (graphicsComponent->setupBuffers())
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_DEFERRED);
        }
      }
    }
  }

  // ResizeTask ========================================================

  ResizeTask::ResizeTask
  (const weak_ptr<ProjectRuntime>& pr)
    : GraphicsTask(pr, "ResizeTask")
  {
  }

  void ResizeTask::execute()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto graphicsComponent = prLock->getGraphicsComponent().lock())
      {
        if (graphicsComponent->handleResize())
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_FAILED);
        }
      }
    }
  }

  // RenderTask ==============================================================

  RenderTask::RenderTask
  (const weak_ptr<ProjectRuntime>& pr)
    : GraphicsTask(pr, "RenderTask")
  {
  }

  void RenderTask::execute()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto graphicsComponent = prLock->getGraphicsComponent().lock())
      {
        if (auto sr = prLock->getActiveSceneRuntime().lock())
        {
          graphicsComponent->clearBuffers(sr);
          graphicsComponent->renderModels(sr);
          graphicsComponent->renderEnvironment(sr);
          //graphicsComponent->renderShadowPass(sr);
          //graphicsComponent->renderSprites(sr);
          //graphicsComponent->renderFonts(sr);

        }
        setState(TASK_STATE_COMPLETED);
      }
    }
  }
}
