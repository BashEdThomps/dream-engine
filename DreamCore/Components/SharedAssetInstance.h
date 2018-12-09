#pragma once
#include "AssetInstance.h"

namespace Dream
{
    class ProjectRuntime;
    class SharedAssetInstance : public AssetInstance
    {
    public:
        SharedAssetInstance(AssetDefinition* def, ProjectRuntime* runtime);
        ~SharedAssetInstance() override;

        string getAssetFilePath(string fmt = "") override;
        string getAssetDirectoryPath() override;

    protected:
        ProjectRuntime* mProjectRuntime;

    };
}
