#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
  class DeferredLoadRuntime;
  class ProjectRuntime;
  class DeferredLoadRuntime;

  class RuntimeLoadFromDefinitionTask : public Task
  {
  public:
    RuntimeLoadFromDefinitionTask(ProjectRuntime& pr, DeferredLoadRuntime& runtime);
    void execute() override;
    string getNameAndIDString() const override;
    DeferredLoadRuntime& getRuntime() const;
  private:
    reference_wrapper<DeferredLoadRuntime> mRuntime;
  };
}
