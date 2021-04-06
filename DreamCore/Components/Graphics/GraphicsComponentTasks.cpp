#include "GraphicsComponentTasks.h"

#include "GraphicsComponent.h"
#include "Project/ProjectRuntime.h"

#include <sstream>
using std::stringstream;

namespace octronic::dream
{
  // GraphicsComponentTask ===================================================

  GraphicsTask::GraphicsTask
  (ProjectRuntime& pr, const string& taskName)
    : Task(pr, taskName)
  {

  }

  // GraphicsComponentDestructionTask ========================================

  GraphicsDestructionTask::GraphicsDestructionTask
  (ProjectRuntime& pr, const string& taskName)
    : DestructionTask(pr, taskName)
  {

  }

  // SetupBuffersTask ========================================================

  SetupBuffersTask::SetupBuffersTask
  (ProjectRuntime& pr)
    : GraphicsTask(pr, "SetupBuffersTask")
  {
  }

  void SetupBuffersTask::execute()
  {
    auto& graphicsComponent = getProjectRuntime().getGraphicsComponent();
    if (graphicsComponent.setupBuffers())
    {
      setState(TASK_STATE_COMPLETED);
    }
    else
    {
      setState(TASK_STATE_DEFERRED);
    }
  }

  // ResizeTask ========================================================

  ResizeTask::ResizeTask
  (ProjectRuntime& pr)
    : GraphicsTask(pr, "ResizeTask")
  {
  }

  void ResizeTask::execute()
  {
    auto& graphicsComponent = getProjectRuntime().getGraphicsComponent();
    if (graphicsComponent.handleResize())
    {
      setState(TASK_STATE_COMPLETED);
    }
    else
    {
      setState(TASK_STATE_FAILED);
    }
  }

  // RenderTask ==============================================================

  RenderTask::RenderTask
  (ProjectRuntime& pr)
    : GraphicsTask(pr, "RenderTask")
  {
  }

  void RenderTask::execute()
  {
    auto& graphicsComponent = getProjectRuntime().getGraphicsComponent();
    auto srOpt = getProjectRuntime().getActiveSceneRuntime();
    if (srOpt)
    {
      auto& sr = srOpt.value();
      graphicsComponent.clearBuffers(sr);
      graphicsComponent.renderModels(sr);
      graphicsComponent.renderEnvironment(sr);
      //graphicsComponent->renderShadowPass(sr);
      //graphicsComponent->renderSprites(sr);
      //graphicsComponent->renderFonts(sr);
    }
    setState(TASK_STATE_COMPLETED);
  }
}
