#pragma once
#include "AssetRuntime.h"

namespace Dream
{
    class ProjectRuntime;
    class SharedAssetRuntime : public AssetRuntime
    {
    public:
        SharedAssetRuntime(AssetDefinition* def, ProjectRuntime* runtime);
        ~SharedAssetRuntime() override;

        string getAssetFilePath(string fmt = "") override;
        string getAssetDirectoryPath() override;

    protected:
        ProjectRuntime* mProjectRuntime;

    };
}
