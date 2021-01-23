#pragma once
#include "AssetRuntime.h"

// TODO - Add Used-by vector for common user management?

namespace octronic::dream
{
    class ProjectRuntime;
    class SharedAssetRuntime : public AssetRuntime
    {
    public:
        SharedAssetRuntime(AssetDefinition* def, ProjectRuntime* runtime);
        ~SharedAssetRuntime() override;

        string getAssetFilePath(string fmt = "") override;
        string getAssetDirectoryPath() override;

        ProjectRuntime* getProjectRuntime() const;

    protected:
        ProjectRuntime* mProjectRuntime;
    };
}
