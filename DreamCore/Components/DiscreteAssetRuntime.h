#pragma once

#include "AssetRuntime.h"

namespace octronic::dream
{
  class EntityRuntime;

  /**
     * @brief A DiscreteAssetRuntime is a Runtime that is unique to a single
     * EntityRuntime.
     *
     * DiscreteAssetRuntimes should be stored by their owners as
     * DiscreteAssetRuntime*
     */
  class DiscreteAssetRuntime : public AssetRuntime
  {
  public:
    /**
         * @param def Definition from which to create Runtime.
         * @param runtime Owner of this AssetRuntime.
         */
    DiscreteAssetRuntime(ProjectRuntime& prt, AssetDefinition& def, EntityRuntime& runtime);

    // Default Move
    DiscreteAssetRuntime(DiscreteAssetRuntime&&) = default;
    DiscreteAssetRuntime& operator=(DiscreteAssetRuntime&&) = default;

  protected:
    EntityRuntime& getEntityRuntime() const;

  private:
    reference_wrapper<EntityRuntime> mEntityRuntime;
  };
}
