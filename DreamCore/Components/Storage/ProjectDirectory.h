/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <vector>

#include "Common/Uuid.h"
#include "Common/Constants.h" // AssetType

using std::string;
using std::vector;

namespace octronic::dream
{
    class Project;
    class File;
    class AssetDefinition;
    class StorageManager;
    class Directory;

    /**
     * @brief The ProjectDirectory class is an interface to a Project's physical
     * location on disk. All IO should be done through the ProjectDirectory, to
     * facilitate cross-platform abstractions.
     */
    class ProjectDirectory
    {
    public:
        ProjectDirectory(StorageManager* fm);
        ProjectDirectory(StorageManager* fm, Project* project);
        ~ProjectDirectory();

        bool baseDirectoryExists() const;
        bool createBaseDirectory() const;
        bool createAllAssetDirectories() const;

        File* readAssetData(AssetDefinition*, const string &format = "") const;
        bool writeAssetData(AssetDefinition*, uint8_t* data, size_t data_sz, const string &format = "") const;

        string getAssetAbsolutePath(UuidType) const;
        string getAssetAbsolutePath(AssetDefinition*) const;
        string getAssetAbsolutePath(AssetDefinition*, const string &format) const;
        string getAssetDirectoryPath(AssetDefinition*) const;
        string getAssetTypeDirectory(AssetType type, const string &base = "") const;

        bool removeAssetDirectory(AssetDefinition*) const;

        string getProjectFilePath() const;
        bool assetTypeDirectoryExists(AssetType type) const;

        bool createAssetTypeDirectory(AssetType type) const;
        vector<UuidType> cleanupAssetsDirectory() const;

        Project* newProject(Directory* projectDir);
        Project* openFromFile(File* file);
        Project* openFromDirectory(Directory* directory);
        bool saveProject() const;
        void closeProject();

    protected:
        string findProjectFileInDirectory(Directory* dir) const;
        bool directoryContainsProject(Directory* dir) const;
        bool findAssetDirectories(Directory* dir) const;

    private:
        StorageManager* mStorageManager;
        Project* mProject;
        string mPath;
    };
}
