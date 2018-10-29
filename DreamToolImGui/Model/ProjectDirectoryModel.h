#pragma once

#include <DreamCore.h>

using Dream::DreamObject;
using Dream::Project;
using Dream::AssetType;
using Dream::IAssetDefinition;

namespace DreamTool
{
    class ProjectDirectoryModel : public DreamObject
    {
    public:
        ProjectDirectoryModel(Project* project);
        ~ProjectDirectoryModel() override;

        bool baseDirectoryExists();
        bool createBaseDirectory();
        bool createAssetDirectory(string, AssetType);
        bool createAllAssetDirectories(string);

        vector<char> readAssetData(IAssetDefinition*, string format = "");
        bool writeAssetData(IAssetDefinition*, vector<char>, string format = "");
        string getAssetAbsolutePath(IAssetDefinition*, string format = "");

    private:
        Project* mProject;
        const static size_t BufferSize;
    };
}
