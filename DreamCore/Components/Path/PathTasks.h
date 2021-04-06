#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
  class PathRuntime;
  class PathUpdateTask : public Task
  {
  public:
    PathUpdateTask(ProjectRuntime& pr, PathRuntime& rt);
    void execute();
  protected:
    PathRuntime& getPathRuntime() const;
  private:
    reference_wrapper<PathRuntime> mPathRuntime;
  };
}
