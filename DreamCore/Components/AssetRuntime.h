#pragma once

#include "Base/DeferredLoadRuntime.h"

namespace octronic::dream
{
  class AssetDefinition;
  class ProjectRuntime;

  /**
   * @brief Holds runtime data for an Asset.
   */
  class AssetRuntime : public DeferredLoadRuntime
  {
  public:
    /**
     * @brief Default Constructor
     * @param asset AssetDefinition to construct runtime from
     */
    AssetRuntime(ProjectRuntime& pr, AssetDefinition& assetDef);
    AssetRuntime(AssetRuntime&&) = default;
    AssetRuntime& operator=(AssetRuntime&&) = default;

    virtual ~AssetRuntime();

    virtual void pushTasks() = 0;
  };
}
