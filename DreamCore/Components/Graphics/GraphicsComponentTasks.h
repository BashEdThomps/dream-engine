#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
  // Construction Task =======================================================

  class GraphicsTask : public Task
  {
  public:
    GraphicsTask(const weak_ptr<ProjectRuntime>& pr, const string& taskName);
  };

  // Destruction Task ========================================================

  class GraphicsDestructionTask : public DestructionTask
  {
  public:
    GraphicsDestructionTask(const weak_ptr<ProjectRuntime>& pr, const string& taskName);
  };


  // SetupBuffersTask ========================================================

  class SetupBuffersTask : public GraphicsTask
  {
  public:
    SetupBuffersTask(const weak_ptr<ProjectRuntime>& pr);
    void execute() override;
  };

  // ResizeTask ========================================================

  class ResizeTask : public GraphicsTask
  {
  public:
    ResizeTask(const weak_ptr<ProjectRuntime>& pr);
    void execute() override;
  };

  // RenderTask ========================================================

  class RenderTask : public GraphicsTask
  {
  public:
    RenderTask(const weak_ptr<ProjectRuntime>& pr);
    void execute() override;
  };
}
