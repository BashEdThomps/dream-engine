#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
  // Construction Task =======================================================

  class GraphicsTask : public Task
  {
  public:
    GraphicsTask(ProjectRuntime& pr, const string& taskName);
  };

  // Destruction Task ========================================================

  class GraphicsDestructionTask : public DestructionTask
  {
  public:
    GraphicsDestructionTask(ProjectRuntime& pr, const string& taskName);
  };


  // SetupBuffersTask ========================================================

  class SetupBuffersTask : public GraphicsTask
  {
  public:
    SetupBuffersTask(ProjectRuntime& pr);
    void execute() override;
  };

  // ResizeTask ========================================================

  class ResizeTask : public GraphicsTask
  {
  public:
    ResizeTask(ProjectRuntime& pr);
    void execute() override;
  };

  // RenderTask ========================================================

  class RenderTask : public GraphicsTask
  {
  public:
    RenderTask(ProjectRuntime& pr);
    void execute() override;
  };
}
