#pragma once

#include "Runtime.h"
#include <memory>

using std::shared_ptr;

namespace octronic::dream
{
  class ProjectRuntime;

  class DeferredLoadRuntime : public Runtime
  {
  public:
    DeferredLoadRuntime(ProjectRuntime& pr, Definition& def);

    DeferredLoadRuntime(DeferredLoadRuntime&&) = default;
    DeferredLoadRuntime& operator=(DeferredLoadRuntime&&) = default;

    shared_ptr<RuntimeLoadFromDefinitionTask> getLoadFromDefinitionTask() const;
    bool getLoaded() const;
    void setLoaded(bool);
    bool getLoadError() const;
    void setLoadError(bool);
    ProjectRuntime& getProjectRuntime() const;

  protected:
    /**
         * @brief Flag set when the runtime data has been loaded
         */
    bool mLoaded;
    bool mLoadError;
    /**
          * @brief Task to inflate the Runtime from it's definition
          */
    shared_ptr<RuntimeLoadFromDefinitionTask> mLoadFromDefinitionTask;
  private:
    reference_wrapper<ProjectRuntime> mProjectRuntime;
  };
}
