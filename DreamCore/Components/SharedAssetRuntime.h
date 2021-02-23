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
        SharedAssetRuntime(ProjectRuntime* prt, AssetDefinition* def);
        ~SharedAssetRuntime() override;

        string getAssetFilePath(const string& fmt = "") override;
        string getAssetDirectoryPath() override;

        void addInstance(EntityRuntime* er);
        void removeInstance(EntityRuntime* er);
        void removeInstanceByUuid(UuidType spriteUuid);
        vector<EntityRuntime*>* getInstanceVector();

        bool getReloadFlag() const;
        void setReloadFlag(bool reloadFlag);

    protected:
        vector<EntityRuntime*> mInstances;
        bool mReloadFlag;
    };
}
