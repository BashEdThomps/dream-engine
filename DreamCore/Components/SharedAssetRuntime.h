#pragma once

#include "AssetRuntime.h"
#include <vector>

using std::vector;

namespace octronic::dream
{
    class ProjectRuntime;
    class EntityRuntime;

    /**
     * @brief A SharedAssetRuntime is shared by several EntityRuntimes. Entities
     * using this Runtime are stored in the mInstances vector.
     *
     * SharedAssetRuntimes should be owned ONLY by their respective Cache object.
     * The Cache stores them  and observers will store references.
     */
    class SharedAssetRuntime : public AssetRuntime
    {
    public:
        SharedAssetRuntime(ProjectRuntime& prt,AssetDefinition& def);
        SharedAssetRuntime(SharedAssetRuntime&&) = default;
        SharedAssetRuntime& operator=(SharedAssetRuntime&&) = default;

        void addInstance(EntityRuntime& er);
        void removeInstance(EntityRuntime& er);
        void removeInstanceByUuid(UuidType spriteUuid);
        vector<reference_wrapper<EntityRuntime>> getInstanceVector() const;

        bool getReloadFlag() const;
        void setReloadFlag(bool reloadFlag);

    protected:
        vector<reference_wrapper<EntityRuntime>> mInstances;
        bool mReloadFlag;
    };
}
