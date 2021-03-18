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
     */
    class SharedAssetRuntime : public AssetRuntime
    {
    public:
        SharedAssetRuntime(
                const weak_ptr<ProjectRuntime>& prt,
                const weak_ptr<AssetDefinition>& def);

        ~SharedAssetRuntime() override;

        void addInstance(const weak_ptr<EntityRuntime>& er);
        void removeInstance(const weak_ptr<EntityRuntime>& er);
        void removeInstanceByUuid(UuidType spriteUuid);
        vector<weak_ptr<EntityRuntime>> getInstanceVector() const;

        bool getReloadFlag() const;
        void setReloadFlag(bool reloadFlag);

    protected:
        vector<weak_ptr<EntityRuntime>> mInstances;
        bool mReloadFlag;
    };
}
