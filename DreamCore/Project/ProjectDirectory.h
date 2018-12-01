#pragma once

#include "../Common/DreamObject.h"

namespace Dream
{
    class Project;
    class IAssetDefinition;

    class ProjectDirectory : public DreamObject
    {
    public:
        ProjectDirectory(Project* project);
        ~ProjectDirectory() override;

        bool baseDirectoryExists();
        bool createBaseDirectory();
        bool createAssetDirectory(string, AssetType);
        bool createAllAssetDirectories(string);

        vector<char> readAssetData(IAssetDefinition*, string format = "");
        bool writeAssetData(IAssetDefinition*, vector<char>, string format = "");
        string getAssetAbsolutePath(IAssetDefinition*, string format = "");
        string getAssetDirectoryPath(IAssetDefinition*);
        bool removeAssetDirectory(IAssetDefinition*);
        bool saveProject();
        string getProjectFilePath();
        bool assetTypeDirectoryExists(AssetType type);
        string getAssetTypeDirectory(AssetType type);
        bool createAssetTypeDirectory(AssetType type);
        void cleanupAssetsDirectory();

    private:
        Project* mProject;
        const static size_t BufferSize;
    };
}
